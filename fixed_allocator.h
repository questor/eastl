/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements the following
//     fixedAllocator
//     fixedAllocatorWithOverflow
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_ALLOCATOR_H
#define EASTL_FIXED_ALLOCATOR_H


#include <eastl/internal/config.h>
#include <eastl/internal/fixed_pool.h>
#include <eastl/functional.h>
#include <eastl/memory.h>
#include <eastl/allocator.h>
#include <eastl/type_traits.h>

EA_DISABLE_ALL_VC_WARNINGS();

#include <new>

EA_RESTORE_ALL_VC_WARNINGS();

EA_DISABLE_VC_WARNING(4275); // non dll-interface class used as base for DLL-interface classkey 'identifier'

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



namespace eastl
{

	///////////////////////////////////////////////////////////////////////////
	// fixedAllocator
	///////////////////////////////////////////////////////////////////////////

	/// fixedAllocator
	///
	/// Implements an allocator which allocates a single fixed size where 
	/// the size, alignment, and memory used for the pool is defined at 
	/// runtime by the user. This is different from fixed containers 
	/// such as fixedList whereby the size and alignment are determined
	/// at compile time and the memory is directly built into the container's
	/// member data.
	///
	/// If the pool's memory is exhausted or was never initialized, the 
	/// allocate function returns NULL. Consider the fixedAllocatorWithOverflow 
	/// class as an alternative in order to deal with this situation.
	///
	/// This class requires the user to call container.getAllocator().init() 
	/// after constructing the container. There currently isn't a way to 
	/// construct the container with the initialization parameters, though
	/// with some effort such a thing could probably be made possible. 
	/// It's not as simple as it might first seem, due to the non-copyable
	/// nature of fixed allocators. A side effect of this limitation is that
	/// you cannot copy-construct a container using fixedAllocators.
	///
	/// Another side-effect is that you cannot swap two containers using
	/// a fixedAllocator, as a swap requires temporary memory allocated by
	/// an equivalent allocator, and such a thing cannot be done implicitly.
	/// A workaround for the swap limitation is that you can implement your
	/// own swap whereby you provide an explicitly created temporary object.
	///
	/// Note: Be careful to set the allocator's node size to the size of the 
	/// container node and not the size of the contained object. Note that the 
	/// example code below uses IntListNode.
	///
	/// Example usage:
	///     typedef eastl::list<int, fixedAllocator> IntList;
	///     typedef IntList::node_type                IntListNode;
	///
	///     IntListNode buffer[200];
	///     IntList     intList;
	///     intList.getAllocator().init(buffer, sizeof(buffer), sizeof(IntListNode), __alignof(IntListNode));
	///
	class EASTL_API fixedAllocator : public fixed_pool_base
	{
	public:
		/// fixedAllocator
		///
		/// Default constructor. The user usually will need to call init() after  
		/// constructing via this constructor. 
		///
		fixedAllocator(const char* /*pName*/ = EASTL_FIXED_POOL_DEFAULT_NAME)
			: fixed_pool_base(NULL)
		{
		}


		/// fixedAllocator
		///
		/// Copy constructor. The user usually will need to call init() after  
		/// constructing via this constructor. By their nature, fixed-allocators
		/// cannot be copied in any useful way, as by their nature the user
		/// must manually initialize them.
		///
		fixedAllocator(const fixedAllocator&)
			: fixed_pool_base(NULL)
		{
		}


		/// operator=
		///
		/// By their nature, fixed-allocators cannot be copied in any 
		/// useful way, as by their nature the user must manually 
		/// initialize them.
		///
		fixedAllocator& operator=(const fixedAllocator&)
		{
			return *this;
		}


		// init
		//
		// No init here, as the base class version is sufficient.
		//
		//void init(void* pMemory, size_t memorySize, size_t nodeSize,
		//            size_t alignment, size_t alignmentOffset = 0);


		/// allocate
		///
		/// Allocates a new object of the size specified upon class initialization.
		/// Returns NULL if there is no more memory. 
		///
		void* allocate(size_t n, int /*flags*/ = 0)
		{
			// To consider: Verify that 'n' is what the user initialized us with.

			Link* pLink = mpHead;

			if(pLink) // If we have space...
			{
				#if EASTL_FIXED_SIZE_TRACKING_ENABLED
					if(++mnCurrentSize > mnPeakSize)
						mnPeakSize = mnCurrentSize;
				#endif

				mpHead = pLink->mpNext;
				return pLink;
			}
			else
			{
				// If there's no free node in the free list, just
				// allocate another from the reserved memory area

				if(mpNext != mpCapacity)
				{
					pLink = mpNext;
					
					mpNext = reinterpret_cast<Link*>(reinterpret_cast<char*>(mpNext) + n);

					#if EASTL_FIXED_SIZE_TRACKING_ENABLED
						if(++mnCurrentSize > mnPeakSize)
							mnPeakSize = mnCurrentSize;
					#endif

					return pLink;
				}

				// EASTL_ASSERT(false); To consider: enable this assert. However, we intentionally disable it because this isn't necessarily an assertable error.
				return NULL;
			}
		}


		/// allocate
		///
		void* allocate(size_t n, size_t /*alignment*/, size_t /*offset*/, int flags = 0)
		{
			return allocate(n, flags);
		}


		/// deallocate
		///
		/// Frees the given object which was allocated by allocate(). 
		/// If the given node was not allocated by allocate() then the behaviour 
		/// is undefined.
		///
		void deallocate(void* p, size_t)
		{
			#if EASTL_FIXED_SIZE_TRACKING_ENABLED
				--mnCurrentSize;
			#endif

			((Link*)p)->mpNext = mpHead;
			mpHead = ((Link*)p);
		}


		using fixed_pool_base::can_allocate;


		const char* getName() const
		{
			return EASTL_FIXED_POOL_DEFAULT_NAME;
		}


		void setName(const char*)
		{
			// Nothing to do. We don't allocate memory.
		}

	}; // fixedAllocator

	bool operator==(const fixedAllocator& a, const fixedAllocator& b);
	bool operator!=(const fixedAllocator& a, const fixedAllocator& b);



	///////////////////////////////////////////////////////////////////////////
	// fixedAllocatorWithOverflow
	///////////////////////////////////////////////////////////////////////////

	/// fixedAllocatorWithOverflow
	///
	/// Implements an allocator which allocates a single fixed size where 
	/// the size, alignment, and memory used for the pool is defined at 
	/// runtime by the user. This is different from fixed containers 
	/// such as fixedList whereby the size and alignment are determined
	/// at compile time and the memory is directly built into the container's
	/// member data.
	///
	/// Note: Be careful to set the allocator's node size to the size of the 
	/// container node and not the size of the contained object. Note that the 
	/// example code below uses IntListNode.
	///
	/// This class requires the user to call container.getAllocator().init() 
	/// after constructing the container. There currently isn't a way to 
	/// construct the container with the initialization parameters, though
	/// with some effort such a thing could probably be made possible. 
	/// It's not as simple as it might first seem, due to the non-copyable
	/// nature of fixed allocators. A side effect of this limitation is that
	/// you cannot copy-construct a container using fixedAllocators.
	///
	/// Another side-effect is that you cannot swap two containers using
	/// a fixedAllocator, as a swap requires temporary memory allocated by
	/// an equivalent allocator, and such a thing cannot be done implicitly.
	/// A workaround for the swap limitation is that you can implement your
	/// own swap whereby you provide an explicitly created temporary object.
	///
	/// Example usage:
	///     typedef eastl::list<int, fixedAllocatorWithOverflow> IntList;
	///     typedef IntList::node_type                              IntListNode;
	///
	///     IntListNode buffer[200];
	///     IntList     intList;
	///     intList.getAllocator().init(buffer, sizeof(buffer), sizeof(IntListNode), __alignof(IntListNode));
	///
	class EASTL_API fixedAllocatorWithOverflow : public fixed_pool_base
	{
	public:
		/// fixedAllocatorWithOverflow
		///
		/// Default constructor. The user usually will need to call init() after  
		/// constructing via this constructor. 
		///
		fixedAllocatorWithOverflow(const char* pName = EASTL_FIXED_POOL_DEFAULT_NAME)
			: fixed_pool_base(NULL)
			, mOverflowAllocator(pName)
			, mpPoolBegin(nullptr)
			, mpPoolEnd(nullptr)
			, mnNodeSize(0)
		{
		}


		/// fixedAllocatorWithOverflow
		///
		/// Copy constructor. The user usually will need to call init() after  
		/// constructing via this constructor. By their nature, fixed-allocators
		/// cannot be copied in any useful way, as by their nature the user
		/// must manually initialize them.
		///
		fixedAllocatorWithOverflow(const fixedAllocatorWithOverflow&)
			: fixed_pool_base(NULL)
			, mpPoolBegin(nullptr)
			, mpPoolEnd(nullptr)
			, mnNodeSize(0)
		{
		}


		/// operator=
		///
		/// By their nature, fixed-allocators cannot be copied in any 
		/// useful way, as by their nature the user must manually 
		/// initialize them.
		///
		fixedAllocatorWithOverflow& operator=(const fixedAllocatorWithOverflow& x)
		{
			#if EASTL_ALLOCATOR_COPY_ENABLED
				mOverflowAllocator = x.mOverflowAllocator;
			#else
				(void)x;
			#endif

			return *this;
		}


		/// init
		///
		void init(void* pMemory, size_t memorySize, size_t nodeSize,
					size_t alignment, size_t alignmentOffset = 0)
		{
			fixed_pool_base::init(pMemory, memorySize, nodeSize, alignment, alignmentOffset);

			mpPoolBegin = pMemory;
			mpPoolEnd   = (void*)((uintptr_t)pMemory + memorySize);
			mnNodeSize  = (eastl_size_t)nodeSize;
		}


		/// allocate
		///
		/// Allocates a new object of the size specified upon class initialization.
		/// Returns NULL if there is no more memory. 
		///
		void* allocate(size_t /*n*/, int /*flags*/ = 0)
		{
			// To consider: Verify that 'n' is what the user initialized us with.

			void* p;

			if(mpHead) // If we have space...
			{
				p      = mpHead;
				mpHead = mpHead->mpNext;
			}
			else
			{
				// If there's no free node in the free list, just
				// allocate another from the reserved memory area

				if (mpNext != mpCapacity)
				{
					p = mpNext;
					mpNext = reinterpret_cast<Link*>(reinterpret_cast<char*>(mpNext) + mnNodeSize);
				}
				else
					p = mOverflowAllocator.allocate(mnNodeSize);
			}

			#if EASTL_FIXED_SIZE_TRACKING_ENABLED
				if(p && (++mnCurrentSize > mnPeakSize))
					mnPeakSize = mnCurrentSize;
			#endif

			return p;
		}


		/// allocate
		///
		void* allocate(size_t n, size_t /*alignment*/, size_t /*offset*/, int flags = 0)
		{
			return allocate(n, flags);
		}


		/// deallocate
		///
		/// Frees the given object which was allocated by allocate(). 
		/// If the given node was not allocated by allocate() then the behaviour 
		/// is undefined.
		///
		void deallocate(void* p, size_t)
		{
			#if EASTL_FIXED_SIZE_TRACKING_ENABLED
				--mnCurrentSize;
			#endif

			if((p >= mpPoolBegin) && (p < mpPoolEnd))
			{
				((Link*)p)->mpNext = mpHead;
				mpHead = ((Link*)p);
			}
			else
				mOverflowAllocator.deallocate(p, (size_t)mnNodeSize);
		}


		using fixed_pool_base::can_allocate;


		const char* getName() const
		{
			return mOverflowAllocator.getName();
		}


		void setName(const char* pName)
		{
			mOverflowAllocator.setName(pName);
		}

	protected:
		EASTLAllocatorType mOverflowAllocator;  // To consider: Allow the user to define the type of this, presumably via a template parameter.
		void*              mpPoolBegin;         // To consider: We have these member variables and ideally we shouldn't need them. The problem is that 
		void*              mpPoolEnd;           //              the information about the pool buffer and object size is stored in the owning container 
		eastl_size_t       mnNodeSize;          //              and we can't have access to it without increasing the amount of code we need and by templating 
												//              more code. It may turn out that simply storing data here is smaller in the end.
	}; // fixedAllocatorWithOverflow         //              Granted, this class is usually used for debugging purposes, but perhaps there is an elegant solution.

	bool operator==(const fixedAllocatorWithOverflow& a, const fixedAllocatorWithOverflow& b);
	bool operator!=(const fixedAllocatorWithOverflow& a, const fixedAllocatorWithOverflow& b);






	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	inline bool operator==(const fixedAllocator&, const fixedAllocator&)
	{
		return false;
	}

	inline bool operator!=(const fixedAllocator&, const fixedAllocator&)
	{
		return false;
	}

	inline bool operator==(const fixedAllocatorWithOverflow&, const fixedAllocatorWithOverflow&)
	{
		return false;
	}

	inline bool operator!=(const fixedAllocatorWithOverflow&, const fixedAllocatorWithOverflow&)
	{
		return false;
	}


} // namespace eastl


EA_RESTORE_VC_WARNING();

#endif // Header include guard
