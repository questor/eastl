
#ifndef __EASTL_DYNAMICBITSET_H__
#define __EASTL_DYNAMICBITSET_H__

#include "types.h"

namespace eastl {

template<class StorageType = uint32_t>
class dynamicBitset {
   enum { eWORDSIZE = sizeof(StorageType) * 8 };

   inline int blockIndex(int b) { return b / eWORDSIZE; }
   inline int blockOffset(int b) { return b % eWORDSIZE; }

public:
   dynamicBitset(uint32_t numberBits) {
      mNumberBlocks = numberBits / eWORDSIZE + 1;
      mStorage = new StorageType[mNumberBlocks];
   }
   ~dynamicBitset() {
      delete[] mStorage;
   }

   void clearAll() {
      for(size_t i=0; i<mNumberBlocks; ++i) {
         mStorage[i] = 0;
      }
   }
   void setAll() {
      for(size_t i=0; i<mNumberBlocks; ++i) {
         mStorage[i] = -1;
      }
   }

   void setBit(int index) {
      mStorage[blockIndex(index)] |= (1 << (blockOffset(index)));
   }
   void clearBit(int index) {
      mStorage[blockIndex(index)] &= ~(1 << (blockOffset(index)));
   }
   bool getBit(int index) {
      return mStorage[blockIndex(index)] & (1 << (blockOffset(index))) ? true : false;
   }

protected:
   size_t mNumberBlocks;
   StorageType *mStorage;
};

};    //namespace eastl {


#endif   //#ifndef __EASTL_DYNAMICBITSET_H__

