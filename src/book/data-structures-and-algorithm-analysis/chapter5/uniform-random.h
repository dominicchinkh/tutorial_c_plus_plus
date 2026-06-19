#pragma once

#include <chrono>
#include <functional>
#include <random>

int64_t currentTimeSeconds()
{
    /*
     * std::chrono::high_resolution_clock::now(): Requests the current precise point in time
     * from the system's highest-resolution clock.
     * 
     * .time_since_epoch(): Converts that point in time into a duration by measuring the exact 
     * time elapsed between the clock's starting anchor point (the "epoch," which is Unix 
     * standard time: January 1, 1970) and right now.
     * 
     */
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();

    /*
     * std::chrono::duration_cast<std::chrono::seconds>(now): This converts the high-resolution
     * duration (e.g., nanoseconds) down into seconds. It cuts off any fractional remainder 
     * (truncates toward zero).
     *
     * .count(): Up until this point, the data is wrapped inside a type-safe C++ duration object.
     * Calling .count() extracts the raw numerical value from that object so it can be used as
     * a plain number.
     * 
     */
    return (std::chrono::duration_cast<std::chrono::seconds>(now)).count();
}

class UniformRandom
{
    public:
        UniformRandom(int64_t seed = currentTimeSeconds()): 
            generator(static_cast<uint64_t>(seed))
        {
        }

        int nextInt()
        {
            // The default bound is from 0 to std::numeric_limits<T>::max()
            static std::uniform_int_distribution<unsigned int> distribution;
            return distribution(generator);
        }

        int nextInt(int low, int high)
        {
            static std::uniform_int_distribution<int> distribution(low, high);
            return distribution(generator);
        }

        int nextInt(int high)
        {
            return nextInt(0, high - 1);
        }

        double nextDouble()
        {
            static std::uniform_real_distribution<double> distribution(0, 1);
            return distribution(generator);
        }

    private:
        /*
         * A highly regarded pseudo-random number generator (PRNG) introduced in C++11
         *
         * The name mt19937 stands for Mersenne Twister, and the number 19937 refers to the 
         * length of its period: it will generate a sequence of 2^19937 - 1 numbers before it 
         * ever repeats itself.
         * 
         */
        std::mt19937_64 generator;
};
