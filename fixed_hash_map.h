/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements a hashMap and hashMultimap which use a fixed size 
// memory pool for its buckets and nodes. 
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_HASH_MAP_H
#define EASTL_FIXED_HASH_MAP_H


#include <eastl/hash_map.h>
#include <eastl/internal/fixed_pool.h>

EA_DISABLE_VC_WARNING(4127) // Conditional expression is constant

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif


namespace eastl
{
	/// EASTL_FIXED_HASH_MAP_DEFAULT_NAME
	///
	/// Defines a default container name in the absence of a user-provided name.
	/// In the case of fixed-size containers, the allocator name always refers
	/// to overflow allocations. 
	///
	#ifndef EASTL_FIXED_HASH_MAP_DEFAULT_NAME
		#define EASTL_FIXED_HASH_MAP_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixedHashMap" // Unless the user overrides something, this is "EASTL fixedHashMap".
	#endif

	#ifndef EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME
		#define EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixedHashMultimap" // Unless the user overrides something, this is "EASTL fixedHashMultimap".
	#endif


	/// EASTL_FIXED_HASH_MAP_DEFAULT_ALLOCATOR
	/// EASTL_FIXED_HASH_MULTIMAP_DEFAULT_ALLOCATOR
	///
	#ifndef EASTL_FIXED_HASH_MAP_DEFAULT_ALLOCATOR
		#define EASTL_FIXED_HASH_MAP_DEFAULT_ALLOCATOR overflow_allocator_type(EASTL_FIXED_HASH_MAP_DEFAULT_NAME)
	#endif

	#ifndef EASTL_FIXED_HASH_MULTIMAP_DEFAULT_ALLOCATOR
		#define EASTL_FIXED_HASH_MULTIMAP_DEFAULT_ALLOCATOR overflow_allocator_type(EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME)
	#endif



	/// fixedHashMap
	///
	/// Implements a hashMap with a fixed block of memory identified by the nodeCount and bucketCount
	/// template parameters. 
	///
	/// Template parameters:
	///     Key                    The key type for the map. This is a map of Key to T (value).
	///     T                      The value type for the map.
	///     nodeCount              The max number of objects to contain. This value must be >= 1.
	///     bucketCount            The number of buckets to use. This value must be >= 2.
	///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
	///     Hash                   hashSet hash function. See hashSet.
	///     Predicate              hashSet equality testing function. See hashSet.
	///
	template <typename Key, typename T, size_t nodeCount, size_t bucketCount = nodeCount + 1, bool bEnableOverflow = true,
			  typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, bool bCacheHashCode = false, typename OverflowAllocator = EASTLAllocatorType>
	class fixedHashMap : public hashMap<Key, 
										   T,
										   Hash,
										   Predicate,
										   fixedHashtableAllocator<
												bucketCount + 1,
												sizeof(typename hashMap<Key, T, Hash, Predicate, OverflowAllocator, bCacheHashCode>::node_type), 
												nodeCount,
												EASTL_ALIGN_OF(eastl::pair<Key, T>), 
												0, 
												bEnableOverflow,
												OverflowAllocator>, 
										   bCacheHashCode>
	{
	public:
		typedef fixedHashtableAllocator<bucketCount + 1, sizeof(typename hashMap<Key, T, Hash, Predicate, 
						OverflowAllocator, bCacheHashCode>::node_type), nodeCount, EASTL_ALIGN_OF(eastl::pair<Key, T>), 0,
						bEnableOverflow, OverflowAllocator>                                                                         fixedAllocator_type;
		typedef typename fixedAllocator_type::overflow_allocator_type                                                              overflow_allocator_type;
		typedef hashMap<Key, T, Hash, Predicate, fixedAllocator_type, bCacheHashCode>                                             base_type;
		typedef fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator> this_type;
		typedef typename base_type::value_type                                                                                      value_type;
		typedef typename base_type::node_type                                                                                       node_type;
		typedef typename base_type::size_type                                                                                       size_type;

		enum { kMaxSize = nodeCount };

		using base_type::mAllocator;
		using base_type::clear;

	protected:
		node_type** mBucketBuffer[bucketCount + 1]; // '+1' because the hash table needs a null terminating bucket.
		char        mNodeBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

	public:
		explicit fixedHashMap(const overflow_allocator_type& overflowAllocator);

		explicit fixedHashMap(const Hash& hashFunction = Hash(), 
								const Predicate& predicate = Predicate());

		fixedHashMap(const Hash& hashFunction, 
					   const Predicate& predicate,
					   const overflow_allocator_type& overflowAllocator);

		template <typename InputIterator>
		fixedHashMap(InputIterator first, InputIterator last, 
						const Hash& hashFunction = Hash(), 
						const Predicate& predicate = Predicate());

		fixedHashMap(const this_type& x);
		fixedHashMap(this_type&& x);
		fixedHashMap(this_type&& x, const overflow_allocator_type& overflowAllocator);
		fixedHashMap(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator = EASTL_FIXED_HASH_MAP_DEFAULT_ALLOCATOR);

		this_type& operator=(const this_type& x);
		this_type& operator=(std::initializer_list<value_type> ilist);
		this_type& operator=(this_type&& x);

		void swap(this_type& x);

		void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

		size_type maxSize() const;

		const overflow_allocator_type& getOverflowAllocator() const EASTL_NOEXCEPT;
		overflow_allocator_type&       getOverflowAllocator() EASTL_NOEXCEPT;
		void                           setOverflowAllocator(const overflow_allocator_type& allocator);

		void clear(bool clearBuckets); 
	}; // fixedHashMap





	/// fixedHashMultimap
	///
	/// Implements a hashMultimap with a fixed block of memory identified by the nodeCount and bucketCount
	/// template parameters. 
	///
	/// Template parameters:
	///     Key                    The key type for the map. This is a map of Key to T (value).
	///     T                      The value type for the map.
	///     nodeCount              The max number of objects to contain. This value must be >= 1.
	///     bucketCount            The number of buckets to use. This value must be >= 2.
	///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
	///     Hash                   hashSet hash function. See hashSet.
	///     Predicate              hashSet equality testing function. See hashSet.
	///
	template <typename Key, typename T, size_t nodeCount, size_t bucketCount = nodeCount + 1, bool bEnableOverflow = true,
			  typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, bool bCacheHashCode = false, typename OverflowAllocator = EASTLAllocatorType>
	class fixedHashMultimap : public hashMultimap<Key,
													 T,
													 Hash,
													 Predicate,
													 fixedHashtableAllocator<
														bucketCount + 1, 
														sizeof(typename hashMultimap<Key, T, Hash, Predicate, OverflowAllocator, bCacheHashCode>::node_type), 
														nodeCount,
														EASTL_ALIGN_OF(eastl::pair<Key, T>),
														0, 
														bEnableOverflow,
														OverflowAllocator>, 
													 bCacheHashCode>
	{
	public:
		typedef fixedHashtableAllocator<bucketCount + 1, sizeof(typename hashMultimap<Key, T, Hash, Predicate, 
						OverflowAllocator, bCacheHashCode>::node_type), nodeCount, EASTL_ALIGN_OF(eastl::pair<Key, T>), 0, 
						bEnableOverflow, OverflowAllocator>                                                                              fixedAllocator_type;
		typedef typename fixedAllocator_type::overflow_allocator_type                                                                   overflow_allocator_type;
		typedef hashMultimap<Key, T, Hash, Predicate, fixedAllocator_type, bCacheHashCode>                                             base_type;
		typedef fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator> this_type;
		typedef typename base_type::value_type                                                                                           value_type;
		typedef typename base_type::node_type                                                                                            node_type;
		typedef typename base_type::size_type                                                                                            size_type;

		enum { kMaxSize = nodeCount };

		using base_type::mAllocator;
		using base_type::clear;

	protected:
		node_type** mBucketBuffer[bucketCount + 1]; // '+1' because the hash table needs a null terminating bucket.
		char        mNodeBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

	public:
		explicit fixedHashMultimap(const overflow_allocator_type& overflowAllocator);

		explicit fixedHashMultimap(const Hash& hashFunction = Hash(), 
										const Predicate& predicate = Predicate());

		fixedHashMultimap(const Hash& hashFunction,
							const Predicate& predicate,
							const overflow_allocator_type& overflowAllocator);

		template <typename InputIterator>
		fixedHashMultimap(InputIterator first, InputIterator last, 
						const Hash& hashFunction = Hash(), 
						const Predicate& predicate = Predicate());

		fixedHashMultimap(const this_type& x);
		fixedHashMultimap(this_type&& x);
		fixedHashMultimap(this_type&& x, const overflow_allocator_type& overflowAllocator);
		fixedHashMultimap(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator = EASTL_FIXED_HASH_MULTIMAP_DEFAULT_ALLOCATOR);

		this_type& operator=(const this_type& x);
		this_type& operator=(std::initializer_list<value_type> ilist);
		this_type& operator=(this_type&& x);

		void swap(this_type& x);

		void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

		size_type maxSize() const;

		const overflow_allocator_type& getOverflowAllocator() const EASTL_NOEXCEPT;
		overflow_allocator_type&       getOverflowAllocator() EASTL_NOEXCEPT;
		void                           setOverflowAllocator(const overflow_allocator_type& allocator);

		void clear(bool clearBuckets); 
	}; // fixedHashMultimap






	///////////////////////////////////////////////////////////////////////
	// fixedHashMap
	///////////////////////////////////////////////////////////////////////

	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMap(const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), Hash(), 
					Predicate(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMap(const Hash& hashFunction, 
				   const Predicate& predicate)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMap(const Hash& hashFunction, 
				   const Predicate& predicate,
				   const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	template <typename InputIterator>
	fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMap(InputIterator first, InputIterator last, 
					const Hash& hashFunction, 
					const Predicate& predicate)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
		base_type::insert(first, last);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMap(const this_type& x)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), x.hash_function(), 
					x.equal_function(), fixedAllocator_type(NULL, mBucketBuffer))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		mAllocator.reset(mNodeBuffer);
		base_type::insert(x.begin(), x.end());
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMap(this_type&& x)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), x.hash_function(), 
					x.equal_function(), fixedAllocator_type(NULL, mBucketBuffer))
	{
		// This implementation is the same as above. If we could rely on using C++11 delegating constructor support then we could just call that here.
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		mAllocator.reset(mNodeBuffer);
		base_type::insert(x.begin(), x.end());
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMap(this_type&& x, const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), x.hash_function(), 
					x.equal_function(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		// This implementation is the same as above. If we could rely on using C++11 delegating constructor support then we could just call that here.
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		mAllocator.reset(mNodeBuffer);
		base_type::insert(x.begin(), x.end());
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMap(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), Hash(), 
					Predicate(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
		base_type::insert(ilist.begin(), ilist.end());
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(const this_type& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(this_type&& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(std::initializer_list<value_type> ilist)
	{
		base_type::clear();
		base_type::insert(ilist.begin(), ilist.end());
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	swap(this_type& x)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(*this, x);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	reset_lose_memory()
	{
		base_type::mnBucketCount = (size_type)base_type::mRehashPolicy.GetPrevBucketCount((uint32_t)bucketCount);
		base_type::mnElementCount = 0;
		base_type::mRehashPolicy.mnNextResize = 0;
		base_type::getAllocator().reset(mNodeBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::size_type 
	fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::maxSize() const
	{
		return kMaxSize;
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline const typename fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::overflow_allocator_type& 
	fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::getOverflowAllocator() const EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::overflow_allocator_type& 
	fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::getOverflowAllocator() EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	setOverflowAllocator(const overflow_allocator_type& allocator)
	{
		mAllocator.setOverflowAllocator(allocator);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	clear(bool clearBuckets)
	{
		base_type::DoFreeNodes(base_type::mpBucketArray, base_type::mnBucketCount);
		if(clearBuckets)
		{
			base_type::DoFreeBuckets(base_type::mpBucketArray, base_type::mnBucketCount);
			reset_lose_memory();
		}
		base_type::mpBucketArray = (node_type**)mBucketBuffer;
		base_type::mnElementCount = 0;
	}


	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode>
	inline void swap(fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode>& a, 
					 fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode>& b)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(a, b);
	}




	///////////////////////////////////////////////////////////////////////
	// fixedHashMultimap
	///////////////////////////////////////////////////////////////////////

	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultimap(const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), Hash(), 
					Predicate(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultimap(const Hash& hashFunction, 
						const Predicate& predicate)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultimap(const Hash& hashFunction,
						const Predicate& predicate,
						const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	template <typename InputIterator>
	fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultimap(InputIterator first, InputIterator last, 
						const Hash& hashFunction, 
						const Predicate& predicate)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
		base_type::insert(first, last);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultimap(const this_type& x)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), x.hash_function(), 
					x.equal_function(),fixedAllocator_type(NULL, mBucketBuffer))
	{
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		mAllocator.reset(mNodeBuffer);
		base_type::insert(x.begin(), x.end());
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultimap(this_type&& x)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), x.hash_function(), 
					x.equal_function(),fixedAllocator_type(NULL, mBucketBuffer))
	{
		// This implementation is the same as above. If we could rely on using C++11 delegating constructor support then we could just call that here.
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		mAllocator.reset(mNodeBuffer);
		base_type::insert(x.begin(), x.end());
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultimap(this_type&& x, const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), x.hash_function(), 
					x.equal_function(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		// This implementation is the same as above. If we could rely on using C++11 delegating constructor support then we could just call that here.
		mAllocator.copy_overflow_allocator(x.mAllocator);

		#if EASTL_NAME_ENABLED
			mAllocator.setName(x.mAllocator.getName());
		#endif

		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		mAllocator.reset(mNodeBuffer);
		base_type::insert(x.begin(), x.end());
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultimap(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), Hash(), 
					Predicate(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
		base_type::insert(ilist.begin(), ilist.end());
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(const this_type& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(this_type&& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(std::initializer_list<value_type> ilist)
	{
		base_type::clear();
		base_type::insert(ilist.begin(), ilist.end());
		return *this;
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	swap(this_type& x)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(*this, x);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	reset_lose_memory()
	{
		base_type::mnBucketCount = (size_type)base_type::mRehashPolicy.GetPrevBucketCount((uint32_t)bucketCount);
		base_type::mnElementCount = 0;
		base_type::mRehashPolicy.mnNextResize = 0;
		base_type::getAllocator().reset(mNodeBuffer);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::size_type
	fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::maxSize() const
	{
		return kMaxSize;
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline const typename fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::overflow_allocator_type& 
	fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::getOverflowAllocator() const EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::overflow_allocator_type& 
	fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::getOverflowAllocator() EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::setOverflowAllocator(const overflow_allocator_type& allocator)
	{
		mAllocator.setOverflowAllocator(allocator);
	}


	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	clear(bool clearBuckets)
	{
		base_type::DoFreeNodes(base_type::mpBucketArray, base_type::mnBucketCount);
		if(clearBuckets)
		{
			base_type::DoFreeBuckets(base_type::mpBucketArray, base_type::mnBucketCount);
			reset_lose_memory();
		}
		base_type::mpBucketArray = (node_type**)mBucketBuffer;
		base_type::mnElementCount = 0;
	}


	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <typename Key, typename T, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode>
	inline void swap(fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode>& a, 
					 fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode>& b)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(a, b);
	}



} // namespace eastl

EA_RESTORE_VC_WARNING()

#endif // Header include guard












