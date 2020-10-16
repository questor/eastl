
#pragma once

#include <stdint.h>

// https://gist.github.com/ruby0x1/81308642d0325fd386237cfa3b44785c

// FNV1a c++11 constexpr compile time hash functions, 32 and 64 bit
// str should be a null terminated string literal, value should be left out 
// e.g hash_32_fnv1a_const("example")
// code license: public domain or equivalent
// post: https://notes.underscorediscovery.com/constexpr-fnv1a/

constexpr uint32_t val_32_const = 0x811c9dc5;
constexpr uint32_t prime_32_const = 0x1000193;
constexpr uint64_t val_64_const = 0xcbf29ce484222325;
constexpr uint64_t prime_64_const = 0x100000001b3;

inline constexpr uint32_t StringHash32(const char* const str, const uint32_t value = val_32_const) noexcept {
	return (str[0] == '\0') ? value : StringHash32(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
}

inline constexpr uint64_t StringHash64(const char* const str, const uint64_t value = val_64_const) noexcept {
	return (str[0] == '\0') ? value : StringHash64(&str[1], (value ^ uint64_t(str[0])) * prime_64_const);
}

// constexpr uint32_t check = "01234"_StringHash32;
constexpr uint32_t operator"" _StringHash32(char const *s, size_t count) {
	return StringHash32(s);
}
// constexpr uint64_t check = "01234"_StringHash64;
constexpr uint64_t operator"" _StringHash64(char const *s, size_t count) {
	return StringHash64(s);
}


inline const uint32_t StringHash32Runtime(const void* key, const uint32_t len) {
    const char* data = (char*)key;
    uint32_t hash = 0x811c9dc5;
    uint32_t prime = 0x1000193;

    for(int i = 0; i < len; ++i) {
        uint8_t value = data[i];
        hash = hash ^ value;
        hash *= prime;
    }
    return hash;
}

inline const uint64_t StringHash64Runtime(const void* key, const uint64_t len) {
    const char* data = (char*)key;
    uint64_t hash = 0xcbf29ce484222325;
    uint64_t prime = 0x100000001b3;
    
    for(int i = 0; i < len; ++i) {
        uint8_t value = data[i];
        hash = hash ^ value;
        hash *= prime;
    }
    return hash;
}

/*
class FixedStringHash {
   //from Aslan Dzodzikov
   //taken from http://www.humus.name/index.php?page=Comments&ID=296&start=16
   //  void PrintHash(const FixedStringHash& _hash) {
   //    printf( "%x", (unsigned)_hash );
   //  }
   //  PrintHash("Creating Device! Just a test for StringHash");
   //this version seems to work only in vs2008 and above :/
   uint32_t mVal;

   template<size_t N> unsigned _Hash(const char (&str)[N]) {
      typedef const char (&truncated_str)[N-1];
      return str[N-1] + 65599 * _Hash((truncated_str)str);
   }

   unsigned _Hash(const char (&str)[2]) { return str[1] + 65599 * str[0]; }
   
public:
   template <size_t N> FixedStringHash(const char (&str)[N]) {
      mVal = _Hash(str);
   }
   operator const uint32_t&() const {
      return mVal;
   }
};*/

