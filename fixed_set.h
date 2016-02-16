/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements a set and multiset which use a fixed size memory 
// pool for their nodes. 
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_SET_H
#define EASTL_FIXED_SET_H


#include <eastl/set.h>
#include <eastl/internal/fixed_pool.h>

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



namespace eastl
{
	/// EASTL_FIXED_SET_DEFAULT_NAME
	///
	/// Defines a default container name in the absence of a user-provided name.
	/// In the case of fixed-size containers, the allocator name always refers
	/// to overflow allocations. 
	///
	#ifndef EASTL_FIXED_SET_DEFAULT_NAME
		#define EASTL_FIXED_SET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixedSet" // Unless the user overrides something, this is "EASTL fixedSet".
	#endif

	#ifndef EASTL_FIXED_MULTISET_DEFAULT_NAME
		#define EASTL_FIXED_MULTISET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixedMultiset" // Unless the user overrides something, this is "EASTL fixedMultiset".
	#endif


	/// EASTL_FIXED_SET_DEFAULT_ALLOCATOR
	/// EASTL_FIXED_MULTISET_DEFAULT_ALLOCATOR
	///
	#ifndef EASTL_FIXED_SET_DEFAULT_ALLOCATOR
		#define EASTL_FIXED_SET_DEFAULT_ALLOCATOR overflow_allocator_type(EASTL_FIXED_SET_DEFAULT_NAME)
	#endif

	#ifndef EASTL_FIXED_MULTISET_DEFAULT_ALLOCATOR
		#define EASTL_FIXED_MULTISET_DEFAULT_ALLOCATOR overflow_allocator_type(EASTL_FIXED_MULTISET_DEFAULT_NAME)
	#endif



	/// fixedSet
	///
	/// Implements a set with a fixed block of memory identified by the 
	/// nodeCount template parameter. 
	///
	/// Template parameters:
	///     Key                    The type of object the set holds (a.k.a. value).
	///     nodeCount              The max number of objects to contain.
	///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
	///     Compare                Compare function/object for set ordering.
	///     OverflowAllocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
	///
	template <typename Key, size_t nodeCount, bool bEnableOverflow = true, typename Compare = eastl::less<Key>, typename OverflowAllocator = EASTLAllocatorType>
	class fixedSet : public set<Key, Compare, fixed_node_allocator<sizeof(typename set<Key>::node_type), 
								 nodeCount, EASTL_ALIGN_OF(Key), 0, bEnableOverflow, OverflowAllocator> >
	{
	public:
		typedef fixed_node_allocator<sizeof(typename set<Key>::node_type), nodeCount, 
					EASTL_ALIGN_OF(Key), 0, bEnableOverflow, OverflowAllocator>            fixedAllocator_type;
		typedef typename fixedAllocator_type::overflow_allocator_type                     overflow_allocator_type;
		typedef set<Key, Compare, fixedAllocator_type>                                    base_type;
		typedef fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>     this_type;
		typedef typename base_type::value_type                                             value_type;
		typedef typename base_type::node_type                                              node_type;
		typedef typename base_type::size_type                                              size_type;

		enum { kMaxSize = nodeCount };

		using base_type::insert;

	protected:
		char mBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

		using base_type::mAllocator;

	public:
		fixedSet();
		fixedSet(const overflow_allocator_type& overflowAllocator);
		explicit fixedSet(const Compare& compare);
		fixedSet(const this_type& x);
		#if EASTL_MOVE_SEMANTICS_ENABLED
			fixedSet(this_type&& x);
			fixedSet(this_type&& x, const overflow_allocator_type& overflowAllocator);
		#endif
		fixedSet(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator = EASTL_FIXED_SET_DEFAULT_ALLOCATOR);

		template <typename InputIterator>
		fixedSet(InputIterator first, InputIterator last);

		this_type& operator=(const this_type& x);
		this_type& operator=(std::initializer_list<value_type> ilist);
		#if EASTL_MOVE_SEMANTICS_ENABLED
			this_type& operator=(this_type&& x);
		#endif

		void swap(this_type& x);

		void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

		size_type maxSize() const;

		const overflow_allocator_type& getOverflowAllocator() const EASTL_NOEXCEPT;
		overflow_allocator_type&       getOverflowAllocator() EASTL_NOEXCEPT;
		void                           setOverflowAllocator(const overflow_allocator_type& allocator);

		#if EASTL_RESET_ENABLED
			void reset(); // This function name is deprecated; use reset_lose_memory instead.
		#endif
	}; // fixedSet






	/// fixedMultiset
	///
	/// Implements a multiset with a fixed block of memory identified by the 
	/// nodeCount template parameter. 
	///
	///     Key                    The type of object the set holds (a.k.a. value).
	///     nodeCount              The max number of objects to contain.
	///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
	///     Compare                Compare function/object for set ordering.
	///     OverflowAllocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
	///
	template <typename Key, size_t nodeCount, bool bEnableOverflow = true, typename Compare = eastl::less<Key>, typename OverflowAllocator = EASTLAllocatorType>
	class fixedMultiset : public multiset<Key, Compare, fixed_node_allocator<sizeof(typename multiset<Key>::node_type), 
										   nodeCount, EASTL_ALIGN_OF(Key), 0, bEnableOverflow, OverflowAllocator> >
	{
	public:
		typedef fixed_node_allocator<sizeof(typename multiset<Key>::node_type), nodeCount, 
					 EASTL_ALIGN_OF(Key), 0, bEnableOverflow, OverflowAllocator>                fixedAllocator_type;
		typedef typename fixedAllocator_type::overflow_allocator_type                          overflow_allocator_type;
		typedef multiset<Key, Compare, fixedAllocator_type>                                    base_type;
		typedef fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>     this_type;
		typedef typename base_type::value_type                                                  value_type;
		typedef typename base_type::node_type                                                   node_type;
		typedef typename base_type::size_type                                                   size_type;

		enum { kMaxSize = nodeCount };

		using base_type::insert;

	protected:
		char mBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

		using base_type::mAllocator;

	public:
		fixedMultiset();
		fixedMultiset(const overflow_allocator_type& overflowAllocator);
		explicit fixedMultiset(const Compare& compare);
		fixedMultiset(const this_type& x);
		#if EASTL_MOVE_SEMANTICS_ENABLED
			fixedMultiset(this_type&& x);
			fixedMultiset(this_type&& x, const overflow_allocator_type& overflowAllocator);
		#endif
		fixedMultiset(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator = EASTL_FIXED_MULTISET_DEFAULT_ALLOCATOR);

		template <typename InputIterator>
		fixedMultiset(InputIterator first, InputIterator last);

		this_type& operator=(const this_type& x);
		this_type& operator=(std::initializer_list<value_type> ilist);
		#if EASTL_MOVE_SEMANTICS_ENABLED
			this_type& operator=(this_type&& x);
		#endif

		void swap(this_type& x);

		void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

		size_type maxSize() const;

		const overflow_allocator_type& getOverflowAllocator() const EASTL_NOEXCEPT;
		overflow_allocator_type&       getOverflowAllocator() EASTL_NOEXCEPT;
		void                           setOverflowAllocator(const overflow_allocator_type& allocator);

		#if EASTL_RESET_ENABLED
			void reset(); // This function name is deprecated; use reset_lose_memory instead.
		#endif
	}; // fixedMultiset




	///////////////////////////////////////////////////////////////////////
	// fixedSet
	///////////////////////////////////////////////////////////////////////

	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedSet()
		: base_type(fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
		#endif
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedSet(const overflow_allocator_type& overflowAllocator)
		: base_type(fixedAllocator_type(mBuffer, overflowAllocator))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
		#endif
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedSet(const Compare& compare)
		: base_type(compare, fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
		#endif
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedSet(const this_type& x)
		: base_type(x.mCompare, fixedAllocator_type(mBuffer))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		base_type::operator=(x);
	}


	#if EASTL_MOVE_SEMANTICS_ENABLED
		template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
		inline fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedSet(this_type&& x)
			: base_type(x.mCompare, fixedAllocator_type(mBuffer))
		{
			mAllocator.copy_overflow_allocator(x.mAllocator);

			#if EASTL_NAME_ENABLED
				mAllocator.setName(x.mAllocator.getName());
			#endif

			base_type::operator=(x);
		}


		template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
		inline fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedSet(this_type&& x, const overflow_allocator_type& overflowAllocator)
			: base_type(x.mCompare, fixedAllocator_type(mBuffer, overflowAllocator))
		{
			mAllocator.copy_overflow_allocator(x.mAllocator);

			#if EASTL_NAME_ENABLED
				mAllocator.setName(x.mAllocator.getName());
			#endif

			base_type::operator=(x);
		}
	#endif


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedSet(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator)
		: base_type(fixedAllocator_type(mBuffer, overflowAllocator))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
		#endif

		insert(ilist.begin(), ilist.end());
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	template <typename InputIterator>
	fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedSet(InputIterator first, InputIterator last)
		: base_type(fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
		#endif

		insert(first, last);
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type& 
	fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(const this_type& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type&
	fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(std::initializer_list<value_type> ilist)
	{
		base_type::clear();
		insert(ilist.begin(), ilist.end());
		return *this;
	}


	#if EASTL_MOVE_SEMANTICS_ENABLED
		template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
		inline typename fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type&
		fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(this_type&& x)
		{
			base_type::operator=(x);
			return *this;
		}
	#endif


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::swap(this_type& x)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(*this, x);
	}


	#if EASTL_RESET_ENABLED
		// This function name is deprecated; use reset_lose_memory instead.
		template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
		inline void fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::reset()
		{
			reset_lose_memory();
		}
	#endif


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::reset_lose_memory()
	{
		base_type::reset_lose_memory();
		base_type::getAllocator().reset(mBuffer);
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::size_type 
	fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::maxSize() const
	{
		return kMaxSize;
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline const typename fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::overflow_allocator_type& 
	fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::getOverflowAllocator() const EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::overflow_allocator_type& 
	fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::getOverflowAllocator() EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::setOverflowAllocator(const overflow_allocator_type& allocator)
	{
		mAllocator.setOverflowAllocator(allocator);
	}


	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void swap(fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>& a, 
					 fixedSet<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>& b)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(a, b);
	}



	///////////////////////////////////////////////////////////////////////
	// fixedMultiset
	///////////////////////////////////////////////////////////////////////

	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultiset()
		: base_type(fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
		#endif
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultiset(const overflow_allocator_type& overflowAllocator)
		: base_type(fixedAllocator_type(mBuffer, overflowAllocator))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
		#endif
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultiset(const Compare& compare)
		: base_type(compare, fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
		#endif
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultiset(const this_type& x)
		: base_type(x.mCompare, fixedAllocator_type(mBuffer))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		base_type::operator=(x);
	}


	#if EASTL_MOVE_SEMANTICS_ENABLED
		template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
		inline fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultiset(this_type&& x)
			: base_type(x.mCompare, fixedAllocator_type(mBuffer))
		{
			mAllocator.copy_overflow_allocator(x.mAllocator);

			#if EASTL_NAME_ENABLED
				mAllocator.setName(x.mAllocator.getName());
			#endif

			base_type::operator=(x);
		}


		template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
		inline fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultiset(this_type&& x, const overflow_allocator_type& overflowAllocator)
			: base_type(x.mCompare, fixedAllocator_type(mBuffer, overflowAllocator))
		{
			mAllocator.copy_overflow_allocator(x.mAllocator);

			#if EASTL_NAME_ENABLED
				mAllocator.setName(x.mAllocator.getName());
			#endif

			base_type::operator=(x);
		}
	#endif


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultiset(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator)
		: base_type(fixedAllocator_type(mBuffer, overflowAllocator))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
		#endif

		insert(ilist.begin(), ilist.end());
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	template <typename InputIterator>
	fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultiset(InputIterator first, InputIterator last)
		: base_type(fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
		#endif

		insert(first, last);
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type& 
	fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(const this_type& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type&
	fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(std::initializer_list<value_type> ilist)
	{
		base_type::clear();
		insert(ilist.begin(), ilist.end());
		return *this;
	}


	#if EASTL_MOVE_SEMANTICS_ENABLED
		template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
		inline typename fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type&
		fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(this_type&& x)
		{
			base_type::operator=(x);
			return *this;
		}
	#endif


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::swap(this_type& x)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(*this, x);
	}


	#if EASTL_RESET_ENABLED
		// This function name is deprecated; use reset_lose_memory instead.
		template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
		inline void fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::reset()
		{
			reset_lose_memory();
		}
	#endif


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::reset_lose_memory()
	{
		base_type::reset_lose_memory();
		base_type::getAllocator().reset(mBuffer);
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::size_type 
	fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::maxSize() const
	{
		return kMaxSize;
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline const typename fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::overflow_allocator_type& 
	fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::getOverflowAllocator() const EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::overflow_allocator_type& 
	fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::getOverflowAllocator() EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::setOverflowAllocator(const overflow_allocator_type& allocator)
	{
		mAllocator.setOverflowAllocator(allocator);
	}


	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void swap(fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>& a, 
					 fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>& b)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(a, b);
	}



} // namespace eastl


#endif // Header include guard









