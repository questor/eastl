
//NO GENERIC RANGE-CLASS!!!!
//THIS IS SPECIALISED FOR MEMORYBLOCKS WITH NON OVERLAPING RANGES!

#ifndef __EASTL_EXTRA_RANGE_H__
#define __EASTL_EXTRA_RANGE_H__

#include "eastl/vector.h"
#include "eastl/extra/debug.h"

namespace eastl {

class Range {
   typedef struct {
      int start;
      int numberElements;
   } FreeBlock;

public:
   Range()
      : mStart(-1)
      , mEnd(-1)
   {}

   Range(int start, int end) {
      init(start, end);
   }

   ~Range() {
   }

   void init(int start, int end) {
      mStart = start;
      mEnd = end;
      FreeBlock block;
      block.start = start;
      block.numberElements = end-start;
      mFreeBlocks.pushBack(block);
   }

   int getStartOfLastBlock() {
      return mFreeBlocks[mFreeBlocks.size()-1].start;
   }

   int findFreeBlockAndMarkAsUsed(int size) {
      for(unsigned int i=0; i<mFreeBlocks.size(); i++) {
         if(mFreeBlocks[i].numberElements >= size) {
            int startIndex = mFreeBlocks[i].start;
            mFreeBlocks[i].start += size;
            mFreeBlocks[i].numberElements -= size;
            return startIndex;
         }
      }
      return -1;
   }

   void modifyUsedBlock(int start, int oldLength, int newLength) {
      unsigned int i = 0;
      while(i<mFreeBlocks.size()) {
         if(start+oldLength == mFreeBlocks[i].start) {
            int dif = oldLength - newLength;
            mFreeBlocks[i].start -= dif;
            mFreeBlocks[i].numberElements += dif;
            return;
         } else if(start < mFreeBlocks[i].start) {
            //okay, used block was in front of mFreeBlocks[i]!
            FreeBlock block;
            block.start = start+newLength;
            block.numberElements = oldLength-newLength;
            mFreeBlocks.insert(mFreeBlocks.begin()+i, block);
            return;
         }
         ++i;
      }
      //used block was last one in range, so insert new free one
      FreeBlock block;
      block.start = mEnd-newLength;
      block.numberElements = newLength;
      mFreeBlocks.pushBack(block);
   }

   void markFree(int start, int length) {
      unsigned int i = 0;
      while(i<mFreeBlocks.size()) {
         if(mFreeBlocks[i].start+mFreeBlocks[i].numberElements == start) {
            mFreeBlocks[i].numberElements += length;
            //check for merge of blocks
            if(i != mFreeBlocks.size()-1) {
               if(mFreeBlocks[i].start+mFreeBlocks[i].numberElements == mFreeBlocks[i+1].start) {
                  //we can merge the two blocks!
                  mFreeBlocks[i].numberElements += mFreeBlocks[i+1].numberElements;
                  eastl::vector<FreeBlock>::iterator it = mFreeBlocks.begin() + i + 1;
                  mFreeBlocks.erase(it);
               }
            }
            return;
         } else if(mFreeBlocks[i].start == start+length) {   //at the beginning of another block?
            mFreeBlocks[i].start -= length;
            mFreeBlocks[i].numberElements += length;
            //check for merge of blocks
            if(i != mFreeBlocks.size()-1) {
               if(mFreeBlocks[i].start+mFreeBlocks[i].numberElements == mFreeBlocks[i+1].start) {
                  //we can merge the two blocks!
                  mFreeBlocks[i].numberElements += mFreeBlocks[i+1].numberElements;
                  eastl::vector<FreeBlock>::iterator it = mFreeBlocks.begin() + i + 1;
                  mFreeBlocks.erase(it);
               }
            }
            return;
         } else if(start+length < mFreeBlocks[i].start) {
            //insert new freeblock
            FreeBlock block;
            block.start = start;
            block.numberElements = length;
            eastl::vector<FreeBlock>::iterator it = mFreeBlocks.begin() + i;
            mFreeBlocks.insert(it, block);
            return;
         }
         i++;
      }
      ASSERT(!"should never be reached!");
   }

protected:
   int mStart, mEnd;
   eastl::vector<FreeBlock> mFreeBlocks;
};

}; //namespace eastl {

#endif   //#ifndef __EASTL_EXTRA_RANGE_H__
