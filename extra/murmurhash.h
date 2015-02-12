
#ifndef __EASTL_MURMURHASH_H__
#define __EASTL_MURMURHASH_H__

#include "eastl/types.h"

// this is the MurmurHash 3 (based on r136)
// http://code.google.com/p/smhasher/

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.

#include <stdlib.h>
#include "eastl/string.h"

namespace eastl {

uint32_t murmurHash(const uint8_t *data, uint32_t len);

//128bit hash function for 32bit systems
void murmurHash_x86_128( const uint8_t *data, uint32_t len, void *out);

//128bit hash function for 64bit systems
void murmurHash_x64_128( const uint8_t *data, uint32_t len, void *out);

//on 64bit systems cityhash is reported to be much faster for 64bit hash values!

inline uint32_t murmurString(const char *buffer) {
   uint32_t len = strlen(buffer);
   return murmurHash((uint8_t*)buffer, len);
}

#ifdef _MSC_VER
   #define FORCE_INLINE    __forceinline
   #define ROTL32(x,y)     _rotl(x,y)
   #define ROTL64(x,y)     _rotl64(x,y)
#else
   #define FORCE_INLINE __attribute__((always_inline))

   uint32_t inline rotl32 ( uint32_t x, int8_t r ) {
      return (x << r) | (x >> (32 - r));
   }
   uint64_t inline rotl64 ( uint64_t x, int8_t r ) {
     return (x << r) | (x >> (64 - r));
   }
   #define ROTL32(x,y)     rotl32(x,y)
   #define ROTL64(x,y)     rotl64(x,y)
#endif


#define MIXDATA(x) k1 = x;                   \
                   k1 *= c1;                 \
                   k1 = ROTL32(k1, 15);      \
                   k1 *= c2;                 \
                   h1 ^= k1;                 \
                   h1 = ROTL32(h1, 13);      \
                   h1 = h1 * 5 + 0xe6546b64;
#define MIXDATATAIL(x)  k1 = x;              \
                        k1 *= c1;            \
                        k1 = ROTL32(k1, 15); \
                        k1 *= c2;            \
                        h1 ^= k1;

class FixedMurmurHash {
   uint32_t mHash;
public:
   operator const uint32_t&() const {
      return mHash;
   }
   struct ConstCharWrapper {  //neat trick taken from http://altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/
      inline ConstCharWrapper(const char *str)
         : mStr(str) {}
      const char* mStr;
   };

   FixedMurmurHash(ConstCharWrapper str)  //for dynamic strings
      : mHash(murmurString(str.mStr))
   {}

   FixedMurmurHash(uint32_t hashValue)
      : mHash(hashValue)
   {}

   FixedMurmurHash(const char (&str)[2]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATATAIL(((str[ 0])));
      h1 ^= 1;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[3]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATATAIL(((str[ 0]) | ((str[ 1])<<8)));
      h1 ^= 2;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[4]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATATAIL(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16)));
      h1 ^= 3;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[5]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
      h1 ^= 4;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[6]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATATAIL(((str[ 4])));
      h1 ^= 5;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[7]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATATAIL(((str[ 4]) | ((str[ 5])<<8)));
      h1 ^= 6;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[8]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATATAIL(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16)));
      h1 ^= 7;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[9]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
      h1 ^= 8;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[10]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATATAIL(((str[ 8])));
      h1 ^= 9;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[11]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATATAIL(((str[ 8]) | ((str[ 9])<<8)));
      h1 ^= 10;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[12]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATATAIL(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16)));
      h1 ^= 11;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[13]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
      h1 ^= 12;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[14]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATATAIL(((str[12])));
      h1 ^= 13;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[15]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATATAIL(((str[12]) | ((str[13])<<8)));
      h1 ^= 14;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[16]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATATAIL(((str[12]) | ((str[13])<<8) | ((str[14]) << 16)));
      h1 ^= 15;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[17]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
      h1 ^= 16;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[18]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATATAIL(((str[16])));
      h1 ^= 17;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[19]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATATAIL(((str[16]) | ((str[17])<<8)));
      h1 ^= 18;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[20]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATATAIL(((str[16]) | ((str[17])<<8) | ((str[18]) << 16)));
      h1 ^= 19;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[21]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
      h1 ^= 20;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[22]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATATAIL(((str[20])));
      h1 ^= 21;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[23]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATATAIL(((str[20]) | ((str[21])<<8)));
      h1 ^= 22;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[24]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATATAIL(((str[20]) | ((str[21])<<8) | ((str[22]) << 16)));
      h1 ^= 23;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[25]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
      h1 ^= 24;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[26]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATATAIL(((str[24])));
      h1 ^= 25;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[27]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATATAIL(((str[24]) | ((str[25])<<8) ));
      h1 ^= 26;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[28]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATATAIL(((str[24]) | ((str[25])<<8) | ((str[26]) << 16)));
      h1 ^= 27;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[29]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[30]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATATAIL((str[28]));

      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[31]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATATAIL((str[28]) | ((str[29])<<8));

      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[32]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATATAIL(((str[28]) | ((str[29])<<8) | ((str[30]) << 16)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[33]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[34]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATATAIL((str[32]));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[35]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATATAIL((str[32]) | ((str[33])<<8));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[36]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATATAIL(((str[32]) | ((str[33])<<8) | ((str[34]) << 16)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[37]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[38]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATATAIL((str[36]));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[39]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATATAIL((str[36]) | ((str[37])<<8));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[40]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATATAIL(((str[36]) | ((str[37])<<8) | ((str[38]) << 16)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[41]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[42]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
         MIXDATATAIL((str[40]));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[43]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
         MIXDATATAIL((str[40]) | ((str[41])<<8));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[44]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
         MIXDATATAIL(((str[40]) | ((str[41])<<8) | ((str[42]) << 16)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[45]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
         MIXDATA(((str[40]) | ((str[41])<<8) | ((str[42]) << 16) | ((str[43]) << 24)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[46]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
         MIXDATA(((str[40]) | ((str[41])<<8) | ((str[42]) << 16) | ((str[43]) << 24)));
         MIXDATATAIL((str[44]));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[47]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
         MIXDATA(((str[40]) | ((str[41])<<8) | ((str[42]) << 16) | ((str[43]) << 24)));
         MIXDATATAIL((str[44]) | ((str[45])<<8));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[48]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
         MIXDATA(((str[40]) | ((str[41])<<8) | ((str[42]) << 16) | ((str[43]) << 24)));
         MIXDATATAIL(((str[44]) | ((str[45])<<8) | ((str[46]) << 16)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[49]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
         MIXDATA(((str[40]) | ((str[41])<<8) | ((str[42]) << 16) | ((str[43]) << 24)));
         MIXDATA(((str[44]) | ((str[45])<<8) | ((str[46]) << 16) | ((str[47]) << 24)));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }
   FixedMurmurHash(const char (&str)[50]) {
      const uint32_t seed = 0;
      uint32_t h1 = seed;
      uint32_t c1 = 0xcc9e2d51;
      uint32_t c2 = 0x1b873593;
         uint32_t k1;
         MIXDATA(((str[ 0]) | ((str[ 1])<<8) | ((str[ 2]) << 16) | ((str[ 3]) << 24)));
         MIXDATA(((str[ 4]) | ((str[ 5])<<8) | ((str[ 6]) << 16) | ((str[ 7]) << 24)));
         MIXDATA(((str[ 8]) | ((str[ 9])<<8) | ((str[10]) << 16) | ((str[11]) << 24)));
         MIXDATA(((str[12]) | ((str[13])<<8) | ((str[14]) << 16) | ((str[15]) << 24)));
         MIXDATA(((str[16]) | ((str[17])<<8) | ((str[18]) << 16) | ((str[19]) << 24)));
         MIXDATA(((str[20]) | ((str[21])<<8) | ((str[22]) << 16) | ((str[23]) << 24)));
         MIXDATA(((str[24]) | ((str[25])<<8) | ((str[26]) << 16) | ((str[27]) << 24)));
         MIXDATA(((str[28]) | ((str[29])<<8) | ((str[30]) << 16) | ((str[31]) << 24)));
         MIXDATA(((str[32]) | ((str[33])<<8) | ((str[34]) << 16) | ((str[35]) << 24)));
         MIXDATA(((str[36]) | ((str[37])<<8) | ((str[38]) << 16) | ((str[39]) << 24)));
         MIXDATA(((str[40]) | ((str[41])<<8) | ((str[42]) << 16) | ((str[43]) << 24)));
         MIXDATA(((str[44]) | ((str[45])<<8) | ((str[46]) << 16) | ((str[47]) << 24)));
         MIXDATATAIL((str[48]));
      h1 ^= 28;
      h1 ^= h1 >> 16;
      h1 *= 0x85ebca6b;
      h1 ^= h1 >> 13;
      h1 *= 0xc2b2ae35;
      h1 ^= h1 >> 16;
      mHash = h1;
   }

};

#undef MIXDATA
#undef FORCE_INLINE
#undef ROTL32
#undef ROTL64

//void main() {
//   uint32_t test3 = FixedMurmurHash("testtesttesttesttesttesttest");
//   printf("0x%x\n", test3);

//   uint32_t test4 = murmurHash("testtesttesttesttesttesttest", strlen("testtesttesttesttesttesttest"));
//   printf("0x%x\n", test4);

//   const char *test = "Testtest";
//   uint32_t testHash = murmurHash(test, strlen(test));
//   printf("0x%x\n", testHash);
//}

}    //namespace eastl

#endif
