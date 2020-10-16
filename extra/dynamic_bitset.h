
//
// Simple dynamic bitset implementation in C++11
//

/*
The MIT License (MIT)

Copyright (c) 2019 Syoyo Fujita.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#pragma once

#include <cassert>
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

//#include <iostream> // dbg

// TODO(syoyo): Consider endianness

///
/// @brief dynamically allocatable bitset
///
class dynamic_bitset {
 public:
  dynamic_bitset() = default;
  dynamic_bitset(dynamic_bitset &&) = default;
  dynamic_bitset(const dynamic_bitset &) = default;

  dynamic_bitset &operator=(const dynamic_bitset &) = default;

  ~dynamic_bitset() = default;

  ///
  /// @brief Construct dynamic_bitset with given number of bits.
  ///
  /// @param[in] nbits The number of bits to use.
  /// @param[in] value Initize bitfield with this value.
  ///
  explicit dynamic_bitset(size_t nbits, uint64_t value) {
    _num_bits = nbits;

    size_t num_bytes;
    if (nbits < 8) {
      num_bytes = 1;
    } else {
      num_bytes = 1 + (nbits - 1) / 8;
    }

    _data.resize(num_bytes);

    // init with zeros
    std::fill_n(_data.begin(), _data.size(), 0);

    // init with `value`.

    if (nbits < sizeof(uint64_t)) {

      assert(num_bytes < 3);

      uint64_t masked_value = value & ((1 << (nbits +1)) - 1);

      for (size_t i = 0; i < _data.size(); i++) {
        _data[i] = (masked_value >> (i * 8)) & 0xff;
      }

    } else {
      for (size_t i = 0; i < sizeof(uint64_t); i++) {
        _data[i] = (value >> (i * 8)) & 0xff;
      }
    }

  }

  ///
  /// Equivalent to std::bitset::any()
  ///
  bool any() const {
    for (size_t i = 0; i < _num_bits; i++) {
      if ((*this)[i]) {
        return true;
      }
    }

    return false;
  }

  ///
  /// Equivalent to std::bitset::all()
  ///
  bool all() const {
    for (size_t i = 0; i < _num_bits; i++) {
      if (false == (*this)[i]) {
        return false;
      }
    }

    return true;
  }

  ///
  /// Equivalent to std::bitset::none()
  ///
  bool none() const {
    for (size_t i = 0; i < _num_bits; i++) {
      if ((*this)[i]) {
        return false;
      }
    }

    return true;
  }

  ///
  /// Equivalent to std::bitset::flip()
  ///
  dynamic_bitset &flip() {
    for (size_t i = 0; i < _num_bits; i++) {
      set(i, (*this)[i] ? false : true);
    }

    return (*this);
  }

  ///
  /// Equivalent to std::bitset::flip(size_t pos)
  ///
  dynamic_bitset &flip(size_t pos) {
    set(pos, (*this)[pos] ? false : true);

    return (*this);
  }

  ///
  /// @brief Resize dynamic_bitset.
  ///
  /// @details Resize dynamic_bitset. Resize behavior is similar to std::vector::resize.
  ///
  /// @param[in] nbits The number of bits to use.
  ///
  void resize(size_t nbits) {

    _num_bits = nbits;

    size_t num_bytes;
    if (nbits < 8) {
      num_bytes = 1;
    } else {
      num_bytes = 1 + (nbits - 1) / 8;
    }

    _data.resize(num_bytes);
  }

  ///
  /// @return The number of bits that are set to `true`
  ///
  uint32_t count() const {

    uint32_t c = 0;

    for (size_t i = 0; i < _num_bits; i++) {
      c += (*this)[i] ? 1 : 0;
    }

    return c;
  }

  bool test(size_t pos) const {
    // TODO(syoyo): Do range check and throw when out-of-bounds access.
    return (*this)[pos];
  }

  void reset() {
    std::fill_n(_data.begin(), _data.size(), 0);
  }

  // Set all bitfield with `value`
  void setall(bool value) {
    for (size_t i = 0; i < _num_bits; i++) {
      set(i, value);
    }
  }

  void set(size_t pos, bool value = true) {
    size_t byte_loc = pos / 8;
    uint8_t offset = pos % 8;

    uint8_t bitfield = uint8_t(1 << offset);

    if (value == true) {
      // bit on
      _data[byte_loc] |= bitfield;
    } else {
      // turn off bit
      _data[byte_loc] &= (~bitfield);
    }
  }

  std::string to_string() const {
    std::stringstream ss;

    for (size_t i = 0; i < _num_bits; i++) {
      ss << ((*this)[_num_bits - i - 1] ? "1" : "0");
    }

    return ss.str();
  }

  //
  // bits are truncated to 32bit uint
  // In contrast to `std::bitset::to_ulong()`, this function does not throw overflow_error even num_bits are larger than 32.
  //
  uint32_t to_ulong() {
    uint32_t value = 0;

    // TODO(syoyo): optimize code
    size_t n = std::min(_num_bits, size_t(32));
    for (size_t i = 0; i < n; i++) {
      value |= uint32_t(((*this)[i])) << i;
    }

    return value;
  }

  //
  // bits are truncated to 64bit uint
  // In contrast to `std::bitset::to_ullong()`, this function does not throw overflow_error even num_bits are larger than 64.
  //
  uint64_t to_ullong() {
    uint64_t value = 0;

    // TODO(syoyo): optimize code
    size_t n = std::min(_num_bits, size_t(64));
    for (size_t i = 0; i < n; i++) {
      value |= uint64_t(((*this)[i])) << i;
    }

    return value;
  }

  bool operator[](size_t pos) const {
    size_t byte_loc = pos / 8;
    size_t offset = pos % 8;

    return (_data[byte_loc] >> offset) & 0x1;
  }

  // Return the storage size(in bytes)
  size_t storage_size() const {
    return _data.size();
  }

  // Return the number of bits.
  // equivalent to std::bitset::size
  size_t size() const {
    return _num_bits;
  }

  // Return memory address of bitfield(as an byte array)
  const uint8_t *data() const {
    return _data.data();
  }

  // Return memory address of bitfield(as an byte array)
  uint8_t *data() {
    return _data.data();
  }


 private:
  size_t _num_bits{0};

  // bitfields are reprentated as an array of bytes.
  std::vector<uint8_t> _data;
};
