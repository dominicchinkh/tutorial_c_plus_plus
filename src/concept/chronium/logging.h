#pragma once

/*--------------------------------------------------------------------------------------------
 * This header file implements a highly efficient, Chromium-style stream-based logging system
 *
 * It exposes a simple LOG(severity) << ... macro interface while utilizing dynamic conditional guards underneath.
 * 
 * You stream data into LOG exactly like you would use std::cout. You do not need to append std::endl or \n; the framework
 * appends newlines automatically upon destruction.
 * 
 * Instead of writing data directly to the console on every step, it queues data into a stream wrapper. The real genius 
 * of this file lies in its lazy evaluation: if a specific log severity is muted by your system configuration, any code 
 * or functions inside that log line are completely ignored at runtime, saving valuable CPU cycles.
 * 
 * Global State Controls:
 *   g_min_log_level: An integer threshold. Any log with a severity lower than this value is ignored.
 *   g_logging_destination: A bitmask determining where logs are funneled.
 *   g_log_message_handler: A function pointer callback allowing you to intercept, custom-format, or filter logs before 
 *                          they propagate.
 * 
 * You can attach a callback function to intercept logs—perfect if you want to forward logs to an internal UI window, a 
 * custom database, or an analytics server.
 * 
 * bool MyCustomLogCallback(int severity, const char* file, int line, 
 *                          size_t message_start, const std::string& str) 
 * {
 *   std::cout << "[CUSTOM HOOK] Caught a severity " << severity << " log: " << str;
 *   return true; // Return true to consume it (prevents it from going to standard destinations)
 * }
 * 
 * int main(int argc, char *argv[]) 
 * {
 *   g_min_log_level = LOGGING_WARNING;
 *   g_log_message_handler = MyCustomLogCallback;
 * 
 *   LOG(ERROR) << "Processing request for user: " << endl;
 *   return 0;
 * }
 * 
 */

// For uint32_t
#include <cstdint>

// For size_t
#include <cstddef>

// For string
#include <string>

// For ostringstream
#include <sstream>

using LogSeverity = int;

constexpr LogSeverity LOGGING_VERBOSE        = -1;  // This is level 1 verbosity
constexpr LogSeverity LOGGING_INFO           = 0;
constexpr LogSeverity LOGGING_WARNING        = 1;
constexpr LogSeverity LOGGING_ERROR          = 2;
constexpr LogSeverity LOGGING_FATAL          = 3;
constexpr LogSeverity LOGGING_NUM_SEVERITIES = 4;

int g_min_log_level = 0;

// For LOGGING_ERROR and above, always print to stderr.
const int kAlwaysPrintErrorLevel = LOGGING_ERROR;

/*---------------------------------------
 * Specifies where logs will be written. 
 *
 * Multiple destinations can be specified with bitwise OR.
 *
 * Unless destination is LOG_NONE, all logs with severity ERROR and above will
 * be written to stderr in addition to the specified destination.
 * LOG_TO_FILE includes logging to externally-provided file handles.
 */
enum : uint32_t {
  LOG_NONE = 0,
  LOG_TO_FILE = 1 << 0,
  LOG_TO_SYSTEM_DEBUG_LOG = 1 << 1,
  LOG_TO_STDERR = 1 << 2,

  LOG_TO_ALL = LOG_TO_FILE | LOG_TO_SYSTEM_DEBUG_LOG | LOG_TO_STDERR,

  // On POSIX platforms, where it may not even be possible to locate the
  // executable on disk, use stderr.
  LOG_DEFAULT = LOG_TO_SYSTEM_DEBUG_LOG | LOG_TO_STDERR,
};

// Specifies the process' logging sink(s), represented as a combination of
// LoggingDestination values joined by bitwise OR.
uint32_t g_logging_destination = LOG_DEFAULT;

/*------------------------------------------------------------------------
 * Sets the Log Message Handler that gets passed every log message before
 * it's sent to other log destinations (if any).
 * 
 * Returns true to signal that it handled the message and the message
 * should not be sent to other log destinations.
 */
typedef bool (*LogMessageHandlerFunction)(int severity,
                                          const char* file,
                                          int line,
                                          size_t message_start,
                                          const std::string& str);

// A log message handler that gets notified of every log message we process.
LogMessageHandlerFunction g_log_message_handler = nullptr;

/*--------------------------------------------------------------
 * This class more or less represents a particular log message. 
 *
 * You create an instance of LogMessage and then stream stuff to it.
 * When you finish streaming to it, ~LogMessage is called and the
 * full message gets streamed to the appropriate destination.
 *
 * You shouldn't actually use LogMessage's constructor to log things,
 * though. You should use the LOG() macro (and variants thereof)
 * above.
 */
class LogMessage {
  public:
    LogMessage(const char* file, int line, LogSeverity severity): severity_(severity), file_(file), line_(line)
    {
      Init(file, line);
    }

    LogMessage(const LogMessage&) = delete;
    LogMessage& operator=(const LogMessage&) = delete;

    virtual ~LogMessage() {
      Flush();
    }

    std::ostream& stream() { return stream_; }
 
  protected:
    void Flush() 
    {
       std::string str_newline(stream_.str());

      // Give any log message handler first dibs on the message.
      if (g_log_message_handler &&
          g_log_message_handler(severity_, file_, line_, message_start_, str_newline)) {

        // The handler took care of it, no further processing.
        return;
      }

      std::cout << stream_.str() << std::endl;
    }

  private:
    void Init(const char* file, int line) 
    {
      stream_ << "[LEVEL" << severity_ << "] ";
      message_start_ = stream_.str().length();
    }

    const LogSeverity severity_;

    std::ostringstream stream_;

    size_t message_start_;  // Offset of the start of the message (past prefix
                            // info).

    // The file and line information passed in to the constructor.
    const char* const file_;
    const int line_;
};

class LogMessageFatal final : public LogMessage {
 public:
  // In C++, child classes do not inherit their parent's constructors by default.

  // Instead of forcing you to manually re-write all the constructor boilerplate for the child class (LogMessageFatal), 
  // the using keyword tells the compiler: "Automatically copy all the constructors from the parent class LogMessage 
  // and give them to LogMessageFatal too."
  using LogMessage::LogMessage;

  // The [[noreturn]] attribute is a directive to the compiler indicating that this function will never return control 
  // back to the code that called it.
  [[noreturn]] ~LogMessageFatal() override 
  {
    Flush();
    std::exit(1);
  }
};

/*-------------------------------------------------------------------
 * This class is used to explicitly ignore values in the conditional
 * logging macros.
 * 
 * This avoids compiler warnings like "value computed is not used" and 
 * "statement has no effect".
 */
class LogMessageVoidify {
 public:
  LogMessageVoidify() = default;

  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(std::ostream&) {}
};

/*-------------------------------------------------------------------
 * Helper macro which avoids evaluating the arguments to a stream if
 * the condition doesn't hold. Condition is evaluated once and only once.
 * 
 * In C++, a macro can expand into something that looks like dead or useless 
 * code to the compiler. If the macro simply expanded to:
 * 
 *   !(condition) ? (void)0 : (stream)
 * 
 * The compiler looks at the right-hand side of the : and sees a temporary object 
 * (LogMessage) being accessed without its value being assigned to anything.
 * 
 * The LogMessageVoidify class overloads the & operator (operator&) to take a reference 
 * to a standard output stream and return void.
 * 
 * When you write a statement like this in your code:
 * 
 *   LOG(INFO) << "Hello";
 * 
 * The preprocessor expands it using the macro definition. LOG_STREAM(INFO) returns an 
 * std::ostream&. The line transforms into:
 * 
 *   !LOG_IS_ON(INFO) ? 
 *       (void)0 : 
 *       LogMessageVoidify() & (LogMessage(__FILE__, __LINE__, 0).stream() << "Hello");
 * 
 * Because of operator precedence, the entire log-streaming expression on the right happens 
 * first, and its result (the stream) is passed directly to LogMessageVoidify::operator&.
 * 
 * Because you are explicitly passing the stream into an active operator function that 
 * evaluates to void, the compiler recognizes that the statement does have an intended effect. 
 * The warning is safely avoided
 * 
 */
#define LAZY_STREAM(stream, condition) !(condition) ? (void)0 : LogMessageVoidify() & (stream)

/*------------------------------------------------------------
 * A few definitions of macros that don't generate much code. 
 * 
 * These are used by LOG() and LOG_IF, etc. Since these are used all over our code, it's
 * better to have compact code for these operations.
 * 
 */
#define COMPACT_GOOGLE_LOG_EX_INFO(ClassName, ...)    ClassName(__FILE__, __LINE__, LOGGING_INFO,    ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_WARNING(ClassName, ...) ClassName(__FILE__, __LINE__, LOGGING_WARNING, ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_ERROR(ClassName, ...)   ClassName(__FILE__, __LINE__, LOGGING_ERROR,   ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_DFATAL(ClassName, ...)  ClassName(__FILE__, __LINE__, LOGGING_DFATAL,  ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_FATAL(ClassName, ...)   ClassName##Fatal(__FILE__, __LINE__, LOGGING_FATAL, ##__VA_ARGS__)

#define COMPACT_GOOGLE_LOG_INFO COMPACT_GOOGLE_LOG_EX_INFO(LogMessage)
#define COMPACT_GOOGLE_LOG_WARNING COMPACT_GOOGLE_LOG_EX_WARNING(LogMessage)
#define COMPACT_GOOGLE_LOG_ERROR COMPACT_GOOGLE_LOG_EX_ERROR(LogMessage)
#define COMPACT_GOOGLE_LOG_FATAL COMPACT_GOOGLE_LOG_EX_FATAL(LogMessage)
#define COMPACT_GOOGLE_LOG_DFATAL COMPACT_GOOGLE_LOG_EX_DFATAL(LogMessage)

/*
 * We use the preprocessor's merging operator, "##", so that, e.g., LOG(INFO) becomes the token COMPACT_GOOGLE_LOG_INFO.
 * 
 * When you stream something in C++, the << operator is actually a function call behind the scenes. However, not all <<
 * operators are defined the same way:
 * 
 * Member Operators: Basic types (like int, double, bool, or const void*) are built directly into the std::ostream class
 * itself as member functions.
 * 
 *   // How the compiler sees it:
 *   stream.operator<<(42);
 * 
 * Non-Member Operators: Complex types (like std::string or custom classes) are defined outside the stream class as free,
 * global functions. This is because standard library authors couldn't modify the core stream class to know about every
 * object created later.
 * 
 *   // How the compiler sees it:
 *   std::operator<<(stream, my_string);
 * 
 * An unnamed stream is an rvalue temporary - an object created on the fly that disappears at the end of the line.
 * 
 * If Chromium tried to let you stream directly to an unnamed `LogMessage` or an unnamed raw stream object like this:
 * 
 *   // Hypothetical design where LogMessage inherits directly from ostream:
 *   LogMessage(__FILE__, __LINE__, INFO) << "Hello";
 * 
 * The C++ compiler would look at the arguments. Because the stream object is a temporary, it can only match function 
 * signatures that accept temporary references (ostream&&).
 * 
 * For int (Member): The compiler allows calling member functions on temporaries. unnamed_stream.operator<<(42) works 
 * perfectly fine.
 * 
 * For std::string (Non-Member): Global operator<< functions are written to take a normal, modifiable reference 
 * (std::ostream&). C++ strictly forbids binding a temporary object to a non-const regular reference.
 * 
 * As a result, code like LogMessage(INFO) << std::string("test"); would cause a massive, cryptic compiler error because 
 * it couldn't find a matching global operator function.
 * 
 * To bypass this language restriction completely, Chromium doesn't make LogMessage a stream itself. Instead, LogMessage 
 * holds a normal, named std::ostringstream stream_ inside of it as a private member variable.
 * 
 * When the macro forces you to call .stream(), It fetches a regular, lvalue reference to that internal, named stream 
 * variable.
 * 
 *   // What your code actually becomes:
 *   (TemporaryLogMessage).stream() << "Hello";
 * 
 * Because .stream() returns a traditional std::ostream& (a reference to an object that has a fixed home inside the class, rather 
 * than a temporary object floating on the stack), the compiler is happy. It can now bind both member functions (int) and 
 * non-member functions (std::string) without throwing a single error.
 * 
 */
#define LOG_STREAM(severity) COMPACT_GOOGLE_LOG_##severity.stream()

// Used by LOG_IS_ON to lazy-evaluate stream arguments.
bool ShouldCreateLogMessage(int severity)
{
  if (severity < g_min_log_level) {
    return false;
  }

  // Return true here unless we know ~LogMessage won't do anything.
  return g_logging_destination != LOG_NONE || g_log_message_handler || severity >= kAlwaysPrintErrorLevel;
}

/* 
 * As special cases, we can assume that LOG_IS_ON(FATAL) always holds. Also, LOG_IS_ON(DFATAL) always holds in debug mode.
 * In particular, CHECK()s will always fire if they fail.
 * 
 * FATAL is always enabled and required to be resolved in compile time forLOG(FATAL) to be properly understood as 
 * [[noreturn]].
 * 
 */
#define LOG_IS_ON(severity) (LOGGING_##severity == LOGGING_FATAL || ShouldCreateLogMessage(LOGGING_##severity))

#define LOG(severity) LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity))
