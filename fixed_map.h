/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements a map and multimap which use a fixed size memory 
// pool for their nodes. 
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_MAP_H
#define EASTL_FIXED_MAP_H


#include <eastl/map.h>
#include <eastl/fixed_set.h> // Included because fixed_rbtree_base resides here.

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



namespace eastl
{
	/// EASTL_FIXED_MAP_DEFAULT_NAME
	///
	/// Defines a default container name in the absence of a user-provided name.
	/// In the case of fixed-size containers, the allocator name always refers
	/// to overflow allocations. 
	///
	#ifndef EASTL_FIXED_MAP_DEFAULT_NAME
		#define EASTL_FIXED_MAP_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixedMap" // Unless the user overrides something, this is "EASTL fixedMap".
	#endif

	#ifndef EASTL_FIXED_MULTIMAP_DEFAULT_NAME
		#define EASTL_FIXED_MULTIMAP_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixedMultimap" // Unless the user overrides something, this is "EASTL fixedMultimap".
	#endif


	/// EASTL_FIXED_MAP_DEFAULT_ALLOCATOR
	/// EASTL_FIXED_MULTIMAP_DEFAULT_ALLOCATOR
	///
	#ifndef EASTL_FIXED_MAP_DEFAULT_ALLOCATOR
		#define EASTL_FIXED_MAP_DEFAULT_ALLOCATOR overflow_allocator_type(EASTL_FIXED_MAP_DEFAULT_NAME)
	#endif

	#ifndef EASTL_FIXED_MULTIMAP_DEFAULT_ALLOCATOR
		#define EASTL_FIXED_MULTIMAP_DEFAULT_ALLOCATOR overflow_allocator_type(EASTL_FIXED_MULTIMAP_DEFAULT_NAME)
	#endif



	/// fixedMap
	///
	/// Implements a map with a fixed block of memory identified by the 
	/// nodeCount template parameter. 
	///
	///     Key                    The key object (key in the key/value pair).
	///     T                      The mapped object (value in the key/value pair).
	///     nodeCount              The max number of objects to contain.
	///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
	///     Compare                Compare function/object for set ordering.
	///     OverflowAllocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
	///
	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow = true, typename Compare = eastl::less<Key>, typename OverflowAllocator = EASTLAllocatorType>
	class fixedMap : public map<Key, T, Compare, fixed_node_allocator<sizeof(typename map<Key, T>::node_type), 
		nodeCount, EASTL_ALIGN_OF(eastl::pair<Key, T>), 0, bEnableOverflow, OverflowAllocator> >
	{
	public:
		typedef fixed_node_allocator<sizeof(typename map<Key, T>::node_type), nodeCount, 
					 EASTL_ALIGN_OF(eastl::pair<Key, T>), 0, bEnableOverflow, OverflowAllocator>                           fixedAllocator_type;
		typedef typename fixedAllocator_type::overflow_allocator_type                                                     overflow_allocator_type;
		typedef fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>                                  this_type;
		typedef map<Key, T, Compare, fixedAllocator_type>                                                                 base_type;
		typedef typename base_type::value_type                                                                             value_type;
		typedef typename base_type::node_type                                                                              node_type;
		typedef typename base_type::size_type                                                                              size_type;

		enum { kMaxSize = nodeCount };
		   
		using base_type::insert;

	protected:
		char mBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

		using base_type::mAllocator;

	public:
		fixedMap();
		explicit fixedMap(const overflow_allocator_type& overflowAllocator);
		explicit fixedMap(const Compare& compare);
		fixedMap(const this_type& x);
		fixedMap(this_type&& x);
		fixedMap(this_type&& x, const overflow_allocator_type& overflowAllocator);
		fixedMap(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator = EASTL_FIXED_MAP_DEFAULT_ALLOCATOR);

		template <typename InputIterator>
		fixedMap(InputIterator first, InputIterator last);

		this_type& operator=(const this_type& x);
		this_type& operator=(std::initializer_list<value_type> ilist);
		this_type& operator=(this_type&& x);

		void swap(this_type& x);

		void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

		size_type maxSize() const;

		const overflow_allocator_type& getOverflowAllocator() const EASTL_NOEXCEPT;
		overflow_allocator_type&       getOverflowAllocator() EASTL_NOEXCEPT;
		void                           setOverflowAllocator(const overflow_allocator_type& allocator);
	}; // fixedMap




	/// fixedMultimap
	///
	/// Implements a multimap with a fixed block of memory identified by the 
	/// nodeCount template parameter. 
	///
	///     Key                    The key object (key in the key/value pair).
	///     T                      The mapped object (value in the key/value pair).
	///     nodeCount              The max number of objects to contain.
	///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
	///     Compare                Compare function/object for set ordering.
	///     OverflowAllocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
	///
	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow = true, typename Compare = eastl::less<Key>, typename OverflowAllocator = EASTLAllocatorType>
	class fixedMultimap : public multimap<Key, T, Compare, fixed_node_allocator<sizeof(typename multimap<Key, T>::node_type), 
										   nodeCount, EASTL_ALIGN_OF(eastl::pair<Key, T>), 0, bEnableOverflow, OverflowAllocator> >
	{
	public:
		typedef fixed_node_allocator<sizeof(typename multimap<Key, T>::node_type), nodeCount, 
					EASTL_ALIGN_OF(eastl::pair<Key, T>), 0, bEnableOverflow, OverflowAllocator>                                      fixedAllocator_type;
		typedef typename fixedAllocator_type::overflow_allocator_type                                                               overflow_allocator_type;
		typedef multimap<Key, T, Compare, fixedAllocator_type>                                                                      base_type;
		typedef fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>                                       this_type;
		typedef typename base_type::value_type                                                                                       value_type;
		typedef typename base_type::node_type                                                                                        node_type;
		typedef typename base_type::size_type                                                                                        size_type;

		enum { kMaxSize = nodeCount };

		using base_type::insert;

	protected:
		char mBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

		using base_type::mAllocator;
		using base_type::get_compare;

	public:
		fixedMultimap();
		fixedMultimap(const overflow_allocator_type& overflowAllocator);
		explicit fixedMultimap(const Compare& compare);
		fixedMultimap(const this_type& x);
		fixedMultimap(this_type&& x);
		fixedMultimap(this_type&& x, const overflow_allocator_type& overflowAllocator);
		fixedMultimap(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator = EASTL_FIXED_MULTIMAP_DEFAULT_ALLOCATOR);

		template <typename InputIterator>
		fixedMultimap(InputIterator first, InputIterator last);

		this_type& operator=(const this_type& x);
		this_type& operator=(std::initializer_list<value_type> ilist);
		this_type& operator=(this_type&& x);

		void swap(this_type& x);

		void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

		size_type maxSize() const;

		const overflow_allocator_type& getOverflowAllocator() const EASTL_NOEXCEPT;
		overflow_allocator_type&       getOverflowAllocator() EASTL_NOEXCEPT;
		void                           setOverflowAllocator(const overflow_allocator_type& allocator);
	}; // fixedMultimap





	///////////////////////////////////////////////////////////////////////
	// fixedMap
	///////////////////////////////////////////////////////////////////////

	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMap()
		: base_type(fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MAP_DEFAULT_NAME);
		#endif
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMap(const overflow_allocator_type& overflowAllocator)
		: base_type(fixedAllocator_type(mBuffer, overflowAllocator))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MAP_DEFAULT_NAME);
		#endif
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMap(const Compare& compare)
		: base_type(compare, fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MAP_DEFAULT_NAME);
		#endif
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMap(const this_type& x)
		: base_type(x.get_compare(), fixedAllocator_type(mBuffer))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		base_type::operator=(x);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMap(this_type&& x)
		: base_type(x.get_compare(), fixedAllocator_type(mBuffer))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		base_type::operator=(x);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMap(this_type&& x, const overflow_allocator_type& overflowAllocator)
		: base_type(x.get_compare(), fixedAllocator_type(mBuffer, overflowAllocator))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		base_type::operator=(x);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMap(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator)
		: base_type(fixedAllocator_type(mBuffer, overflowAllocator))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MAP_DEFAULT_NAME);
		#endif

		insert(ilist.begin(), ilist.end());
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	template <typename InputIterator>
	fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMap(InputIterator first, InputIterator last)
		: base_type(fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MAP_DEFAULT_NAME);
		#endif

		insert(first, last);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type&
	fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(const this_type& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type&
	fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(std::initializer_list<value_type> ilist)
	{
		base_type::clear();
		insert(ilist.begin(), ilist.end());
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type&
	fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(this_type&& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::swap(this_type& x)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(*this, x);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::reset_lose_memory()
	{
		base_type::reset_lose_memory();
		base_type::getAllocator().reset(mBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::size_type
	fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::maxSize() const
	{
		return kMaxSize;
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline const typename fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::overflow_allocator_type&
	fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::getOverflowAllocator() const EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::overflow_allocator_type&
	fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::getOverflowAllocator() EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void
	fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::setOverflowAllocator(const overflow_allocator_type& allocator)
	{
		mAllocator.setOverflowAllocator(allocator);
	}

	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void swap(fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>& a, 
					 fixedMap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>& b)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(a, b);
	}




	///////////////////////////////////////////////////////////////////////
	// fixedMultimap
	///////////////////////////////////////////////////////////////////////

	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultimap()
		: base_type(fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTIMAP_DEFAULT_NAME);
		#endif
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultimap(const overflow_allocator_type& overflowAllocator)
		: base_type(fixedAllocator_type(mBuffer, overflowAllocator))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTIMAP_DEFAULT_NAME);
		#endif
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultimap(const Compare& compare)
		: base_type(compare, fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTIMAP_DEFAULT_NAME);
		#endif
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultimap(const this_type& x)
		: base_type(x.get_compare(), fixedAllocator_type(mBuffer))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		base_type::operator=(x);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultimap(this_type&& x)
		: base_type(x.get_compare(), fixedAllocator_type(mBuffer))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		base_type::operator=(x);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultimap(this_type&& x, const overflow_allocator_type& overflowAllocator)
		: base_type(x.get_compare(), fixedAllocator_type(mBuffer, overflowAllocator))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		base_type::operator=(x);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::fixedMultimap(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator)
		: base_type(fixedAllocator_type(mBuffer, overflowAllocator))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTIMAP_DEFAULT_NAME);
		#endif

		insert(ilist.begin(), ilist.end());
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	template <typename InputIterator>
	fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::
	fixedMultimap(InputIterator first, InputIterator last)
		: base_type(fixedAllocator_type(mBuffer))
	{
		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_MULTIMAP_DEFAULT_NAME);
		#endif

		insert(first, last);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type& 
	fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(const this_type& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type&
	fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(std::initializer_list<value_type> ilist)
	{
		base_type::clear();
		insert(ilist.begin(), ilist.end());
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::this_type&
	fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::operator=(this_type&& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::swap(this_type& x)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(*this, x);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::reset_lose_memory()
	{
		base_type::reset_lose_memory();
		base_type::getAllocator().reset(mBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::size_type 
	fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::maxSize() const
	{
		return kMaxSize;
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline const typename fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::overflow_allocator_type&
	fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::getOverflowAllocator() const EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline typename fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::overflow_allocator_type&
	fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::getOverflowAllocator() EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void
	fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>::setOverflowAllocator(const overflow_allocator_type& allocator)
	{
		mAllocator.setOverflowAllocator(allocator);
	}


	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow, typename Compare, typename OverflowAllocator>
	inline void swap(fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>& a, 
					 fixedMultimap<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>& b)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(a, b);
	}


} // namespace eastl


#endif // Header include guard









