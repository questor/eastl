
#pragma once

#include <vector>
#include <stdint.h>

#include "dynamic_bitset.h"

// this class uses a underlaying base class (for example a vector) where you can add elements,
// but if you remove elements they will not be deleted but stored in a freelist which is managed
// by this class (which uses the underlaying memory to track free elements).
template<typename T, typename Container=std::vector<T>, typename IndexType=uint16_t> class FreeList {
	static_assert(sizeof(T) >= sizeof(IndexType), "Need place in Type for IndexType!");

public:
	FreeList() {
		mFreelistEntry = -1;
	}
	virtual ~FreeList() {
	}

	void push_back(T item) {
		if(mFreelistEntry == (IndexType)-1) {
			mContainer.push_back(item);
		} else {
			IndexType *freePtr = (IndexType*)&mContainer[mFreelistEntry];
			IndexType nextFreeItem = *freePtr;
			mContainer[mFreelistEntry] = item;
			mFreelistEntry = nextFreeItem;
		}
	}

	IndexType getNextFreeSlot() {
		if(mFreelistEntry == (IndexType)-1) {
			return mContainer.size();
		} else {
			return mFreelistEntry;
		}
	}

	void erase(IndexType index) {
		//TODO: shrink underlaying array if possible
		IndexType *idx = (IndexType*)&mContainer[index];
		*idx = mFreelistEntry;
		mFreelistEntry = index;
	}

	T& operator[](const int index) {
		return mContainer[index];
	}

	const T& operator[](const int index) const {
		return mContainer[index];
	}	

	dynamic_bitset getUsedElementsBitmap() {
		dynamic_bitset bitset;
		bitset.resize(mContainer.size());
		bitset.setall(true);
		IndexType iter = mFreelistEntry;
		while(iter != (IndexType)-1) {
			bitset.set(iter, false);
			IndexType *idx = (IndexType*)&mContainer[iter];
			iter = *idx;
		}
		return bitset;
	}

	//to not need to provide container to usercode
	template <typename Op> void callOnUsedElements(Op&& op) {
		dynamic_bitset bs = getUsedElementsBitmap();
		for(int i=0, end=mContainer.size(); i<end; ++i) {
			if(bs[i]) {
				op(mContainer[i]);
			}
		}
	}

protected:
	Container mContainer;
	IndexType mFreelistEntry;
};
