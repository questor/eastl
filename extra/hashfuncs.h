/*  _______         __              
   |_     _|.-----.|  |_.-----.----.
    _|   |_ |     ||   _|  -__|   _|
   |_______||__|__||____|_____|__|  
       coded by Questor / Inter      */

/*  Paul Hsieh's hash sees collisions after 2**17 keypairs, 
    even if we take two hashes with different seeds

	 bob jenkin's hash gets its first collision somewhere beyond 2**63 keypairs      */

//fast integer hashing
//http://www.concentric.net/%7ETtwang/tech/inthash.htm


#ifndef __EASTL_HASHFUNCS_H__
#define __EASTL_HASHFUNCS_H__

#include "eastl/types.h"
#include "murmurhash.h"

namespace eastl {

//class FixedStringHash {
//   //from Aslan Dzodzikov
//   //taken from http://www.humus.name/index.php?page=Comments&ID=296&start=16
//   //  void PrintHash(const FixedStringHash& _hash) {
//   //    printf( "%x", (unsigned)_hash );
//   //  }
//   //  PrintHash("Creating Device! Just a test for StringHash");
//   //this version seems to work only in vs2008 and above :/
//   uint32_t mVal;

//   template<size_t N> unsigned _Hash(const char (&str)[N]) {
//      typedef const char (&truncated_str)[N-1];
//      return str[N-1] + 65599 * _Hash((truncated_str)str);
//   }
//   unsigned _Hash(const char (&str)[2]) { return str[1] + 65599 * str[0]; }
//public:
//   template <size_t N> FixedStringHash(const char (&str)[N]) {
//      mVal = _Hash(str);
//   }
//   operator const uint32_t&() const {
//      return mVal;
//   }
//};

uint32_t getHash(const char *string);
uint32_t getHashForceLower(const char *string);
uint32_t getHashShift(uint32_t key);
uint32_t getHashJenkins(uint32_t a);
uint32_t getHashMult(uint32_t key);
uint32_t getHashFrom64To32(uint64_t key);
uint64_t getHashFrom64(uint64_t key);

}; //namespace eastl

#endif

