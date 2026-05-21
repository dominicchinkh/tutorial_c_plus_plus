#ifndef EXPRESSION_H
#define EXPRESSION_H

// For size_t
#include <cstddef>

// For uintptr_t
#include <cstdint>

// https://github.com/v8/v8/blob/8e2e0b438081242bb6cf8ff8e54019f109643d5b/include/v8-internal.h#L38
typedef uintptr_t Address;

// https://github.com/v8/v8/blob/8e2e0b438081242bb6cf8ff8e54019f109643d5b/src/zone/zone-segment.h#L22

// Segments represent chunks of memory: They have starting address
// (encoded in the this pointer) and a size in bytes. Segments are
// chained together forming a LIFO structure with the newest segment
// available as segment_head_. Segments are allocated using malloc()
// and de-allocated using free().

class Segment {
  public:
    Segment* next() const { return next_; }
    void set_next(Segment* const next) { next_ = next; }

    // {total_size} returns the allocated size including the bookkeeping bytes of
    // the {Segment}.
    size_t total_size() const { return size_; }

    // {capacity} returns the number of storage bytes in this {Segment}, i.e.
    // {end() - start()}.
    size_t capacity() const { return size_ - sizeof(Segment); }

    Address start() const { return address(sizeof(Segment)); }
    Address end() const { return address(size_); }

  private:
    explicit Segment(size_t size) : size_(size) {}

    // Computes the address of the nth byte in this segment.
    Address address(size_t n) const {
      return reinterpret_cast<Address>(this) + n;
    }

    Segment* next_ = nullptr;
    const size_t size_;
};

/*

// Forward declarations.
class AccountingAllocator;
class Zone;

class Segment {
 public:
  Zone* zone() const { return zone_; }
  void set_zone(Zone* const zone) { zone_ = zone; }

  // Zap the contents of the segment (but not the header).
  void ZapContents();
  // Zaps the header and makes the segment unusable this way.
  void ZapHeader();

 private:
  // Segments are only created by the AccountingAllocator.
  friend class AccountingAllocator;

  Zone* zone_ = nullptr;
};
*/

#endif
