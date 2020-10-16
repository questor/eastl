/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements a hashSet which uses a fixed size memory pool for 
// its buckets and nodes. 
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_HASH_SET_H
#define EASTL_FIXED_HASH_SET_H


#include <eastl/hash_set.h>
#include <eastl/internal/fixed_pool.h>

EA_DISABLE_VC_WARNING(4127) // Conditional expression is constant

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



namespace eastl
{
	/// EASTL_FIXED_HASH_SET_DEFAULT_NAME
	///
	/// Defines a default container name in the absence of a user-provided name.
	/// In the case of fixed-size containers, the allocator name always refers
	/// to overflow allocations. 
	///
	#ifndef EASTL_FIXED_HASH_SET_DEFAULT_NAME
		#define EASTL_FIXED_HASH_SET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixedHashSet" // Unless the user overrides something, this is "EASTL fixedHashSet".
	#endif

	#ifndef EASTL_FIXED_HASH_MULTISET_DEFAULT_NAME
		#define EASTL_FIXED_HASH_MULTISET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixedHashMultiset" // Unless the user overrides something, this is "EASTL fixedHashMultiset".
	#endif


	/// EASTL_FIXED_HASH_SET_DEFAULT_ALLOCATOR
	/// EASTL_FIXED_HASH_MULTISET_DEFAULT_ALLOCATOR
	///
	#ifndef EASTL_FIXED_HASH_SET_DEFAULT_ALLOCATOR
		#define EASTL_FIXED_HASH_SET_DEFAULT_ALLOCATOR overflow_allocator_type(EASTL_FIXED_HASH_SET_DEFAULT_NAME)
	#endif

	#ifndef EASTL_FIXED_HASH_MULTISET_DEFAULT_ALLOCATOR
		#define EASTL_FIXED_HASH_MULTISET_DEFAULT_ALLOCATOR overflow_allocator_type(EASTL_FIXED_HASH_MULTISET_DEFAULT_NAME)
	#endif



	/// fixedHashSet
	///
	/// Implements a hashSet with a fixed block of memory identified by the nodeCount and bucketCount
	/// template parameters. 
	///
	/// Template parameters:
	///     Value                  The type of object the hashSet holds.
	///     nodeCount              The max number of objects to contain. This value must be >= 1.
	///     bucketCount            The number of buckets to use. This value must be >= 2.
	///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
	///     Hash                   hashSet hash function. See hashSet.
	///     Predicate              hashSet equality testing function. See hashSet.
	///
	template <typename Value, size_t nodeCount, size_t bucketCount = nodeCount + 1, bool bEnableOverflow = true,
			  typename Hash = eastl::hash<Value>, typename Predicate = eastl::equal_to<Value>, bool bCacheHashCode = false, typename OverflowAllocator = EASTLAllocatorType>
	class fixedHashSet : public hashSet<Value,
										   Hash,
										   Predicate,
										   fixedHashtableAllocator<
												bucketCount + 1, 
												sizeof(typename hashSet<Value, Hash, Predicate, OverflowAllocator, bCacheHashCode>::node_type), 
												nodeCount, 
												EASTL_ALIGN_OF(Value), 
												0,
												bEnableOverflow,
												OverflowAllocator>, 
										   bCacheHashCode>
	{
	public:
		typedef fixedHashtableAllocator<bucketCount + 1, sizeof(typename hashSet<Value, Hash, Predicate, 
						OverflowAllocator, bCacheHashCode>::node_type), nodeCount, EASTL_ALIGN_OF(Value), 0,
						bEnableOverflow, OverflowAllocator>                                                                        fixedAllocator_type;
		typedef typename fixedAllocator_type::overflow_allocator_type                                                             overflow_allocator_type;
		typedef fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator> this_type;
		typedef hashSet<Value, Hash, Predicate, fixedAllocator_type, bCacheHashCode>                                             base_type;
		typedef typename base_type::value_type                                                                                     value_type;
		typedef typename base_type::node_type                                                                                      node_type;
		typedef typename base_type::size_type                                                                                      size_type;

		enum { kMaxSize = nodeCount };

		using base_type::mAllocator;

	protected:
		node_type** mBucketBuffer[bucketCount + 1]; // '+1' because the hash table needs a null terminating bucket.
		char        mNodeBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

	public:
		explicit fixedHashSet(const overflow_allocator_type& overflowAllocator);

		explicit fixedHashSet(const Hash& hashFunction = Hash(), 
								const Predicate& predicate = Predicate());

		fixedHashSet(const Hash& hashFunction, 
					   const Predicate& predicate,
					   const overflow_allocator_type& overflowAllocator);

		template <typename InputIterator>
		fixedHashSet(InputIterator first, InputIterator last,
					   const Hash& hashFunction = Hash(),
					   const Predicate& predicate = Predicate());

		fixedHashSet(const this_type& x);
		fixedHashSet(this_type&& x);
		fixedHashSet(this_type&& x, const overflow_allocator_type& overflowAllocator);

		fixedHashSet(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator = EASTL_FIXED_HASH_SET_DEFAULT_ALLOCATOR);

		this_type& operator=(const this_type& x);
		this_type& operator=(std::initializer_list<value_type> ilist);
		this_type& operator=(this_type&& x);

		void swap(this_type& x);

		void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

		size_type maxSize() const;

		const overflow_allocator_type& getOverflowAllocator() const EASTL_NOEXCEPT;
		overflow_allocator_type&       getOverflowAllocator() EASTL_NOEXCEPT;
		void                           setOverflowAllocator(const overflow_allocator_type& allocator);
	}; // fixedHashSet






	/// fixedHashMultiset
	///
	/// Implements a hashMultiset with a fixed block of memory identified by the nodeCount and bucketCount
	/// template parameters. 
	///
	///     Value                  The type of object the hashSet holds.
	///     nodeCount              The max number of objects to contain. This value must be >= 1.
	///     bucketCount            The number of buckets to use. This value must be >= 2.
	///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
	///     Hash                   hashSet hash function. See hashSet.
	///     Predicate              hashSet equality testing function. See hashSet.
	///
	template <typename Value, size_t nodeCount, size_t bucketCount = nodeCount + 1, bool bEnableOverflow = true,
			  typename Hash = eastl::hash<Value>, typename Predicate = eastl::equal_to<Value>, bool bCacheHashCode = false, typename OverflowAllocator = EASTLAllocatorType>
	class fixedHashMultiset : public hashMultiset<Value,
													 Hash,
													 Predicate,
													 fixedHashtableAllocator<
														bucketCount + 1, 
														sizeof(typename hashMultiset<Value, Hash, Predicate, OverflowAllocator, bCacheHashCode>::node_type),
														nodeCount,
														EASTL_ALIGN_OF(Value), 
														0, 
														bEnableOverflow,
														OverflowAllocator>,
													 bCacheHashCode>
	{
	public:
		typedef fixedHashtableAllocator<bucketCount + 1, sizeof(typename hashMultiset<Value, Hash, Predicate, 
					OverflowAllocator, bCacheHashCode>::node_type), nodeCount, EASTL_ALIGN_OF(Value), 0,
					bEnableOverflow, OverflowAllocator>                                                                                 fixedAllocator_type;
		typedef typename fixedAllocator_type::overflow_allocator_type                                                                  overflow_allocator_type;
		typedef hashMultiset<Value, Hash, Predicate, fixedAllocator_type, bCacheHashCode>                                             base_type;
		typedef fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator> this_type;
		typedef typename base_type::value_type                                                                                          value_type;
		typedef typename base_type::node_type                                                                                           node_type;
		typedef typename base_type::size_type                                                                                           size_type;

		enum { kMaxSize = nodeCount };

		using base_type::mAllocator;

	protected:
		node_type** mBucketBuffer[bucketCount + 1]; // '+1' because the hash table needs a null terminating bucket.
		char        mNodeBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

	public:
		explicit fixedHashMultiset(const overflow_allocator_type& overflowAllocator);

		explicit fixedHashMultiset(const Hash& hashFunction = Hash(), 
									 const Predicate& predicate = Predicate());

		fixedHashMultiset(const Hash& hashFunction, 
							const Predicate& predicate,
							const overflow_allocator_type& overflowAllocator);

		template <typename InputIterator>
		fixedHashMultiset(InputIterator first, InputIterator last, 
							const Hash& hashFunction = Hash(), 
							const Predicate& predicate = Predicate());

		fixedHashMultiset(const this_type& x);
		fixedHashMultiset(this_type&& x);
		fixedHashMultiset(this_type&& x, const overflow_allocator_type& overflowAllocator);
		fixedHashMultiset(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator = EASTL_FIXED_HASH_MULTISET_DEFAULT_ALLOCATOR);

		this_type& operator=(const this_type& x);
		this_type& operator=(std::initializer_list<value_type> ilist);
		this_type& operator=(this_type&& x);

		void swap(this_type& x);

		void reset_lose_memory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

		size_type maxSize() const;

		const overflow_allocator_type& getOverflowAllocator() const EASTL_NOEXCEPT;
		overflow_allocator_type&       getOverflowAllocator() EASTL_NOEXCEPT;
		void                           setOverflowAllocator(const overflow_allocator_type& allocator);
	}; // fixedHashMultiset





	///////////////////////////////////////////////////////////////////////
	// fixedHashSet
	///////////////////////////////////////////////////////////////////////

	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashSet(const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), 
					Hash(), Predicate(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_SET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashSet(const Hash& hashFunction, 
				   const Predicate& predicate)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), 
					hashFunction, predicate, fixedAllocator_type(NULL, mBucketBuffer))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_SET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashSet(const Hash& hashFunction, 
				   const Predicate& predicate,
				   const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), 
					hashFunction, predicate, fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_SET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	template <typename InputIterator>
	fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashSet(InputIterator first, InputIterator last,
				   const Hash& hashFunction,
				   const Predicate& predicate)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_SET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
		base_type::insert(first, last);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashSet(const this_type& x)
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


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashSet<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::fixedHashSet(this_type&& x)
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


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashSet<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::fixedHashSet(this_type&& x, const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), 
					x.hash_function(), x.equal_function(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
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


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashSet<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashSet(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), Hash(), 
					Predicate(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_SET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
		base_type::insert(ilist.begin(), ilist.end());
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	typename fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(const this_type& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashSet<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashSet<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(this_type&& x)
	{
		operator=(x);
		return *this;
	}


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashSet<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashSet<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(std::initializer_list<value_type> ilist)
	{
		base_type::clear();
		base_type::insert(ilist.begin(), ilist.end());
		return *this;
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	swap(this_type& x)
	{
		// We must do a brute-force swap, because fixed containers cannot share memory allocations.
		// Note that we create a temp value on the stack. This approach may fail if the size of the 
		// container is too large. We have a rule against allocating memory from the heap, and so 
		// if the user wants to swap two large objects of this class, the user will currently need 
		// to implement it manually. To consider: add code to allocate a temporary buffer if the 
		// size of the container is too large for the stack.
		EASTL_ASSERT(sizeof(x) < EASTL_MAX_STACK_USAGE); // It is dangerous to try to create objects that are too big for the stack.

		const this_type temp(*this); // Can't call eastl::swap because that would
		*this = x;                   // itself call this member swap function.
		x     = temp;
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	void fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	reset_lose_memory()
	{
		base_type::reset_lose_memory();
		base_type::getAllocator().reset(mNodeBuffer);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::size_type 
	fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::maxSize() const
	{
		return kMaxSize;
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline const typename fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::overflow_allocator_type& 
	fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::getOverflowAllocator() const EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::overflow_allocator_type& 
	fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::getOverflowAllocator() EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	setOverflowAllocator(const overflow_allocator_type& allocator)
	{
		mAllocator.setOverflowAllocator(allocator);
	}

	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode>
	inline void swap(fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode>& a, 
					 fixedHashSet<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode>& b)
	{
		a.swap(b);
	}




	///////////////////////////////////////////////////////////////////////
	// fixedHashMultiset
	///////////////////////////////////////////////////////////////////////

	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultiset(const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), Hash(), 
					Predicate(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTISET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultiset(const Hash& hashFunction, 
						const Predicate& predicate)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTISET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultiset(const Hash& hashFunction, 
						const Predicate& predicate,
						const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTISET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	template <typename InputIterator>
	inline fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultiset(InputIterator first, InputIterator last, 
						const Hash& hashFunction, 
						const Predicate& predicate)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
					predicate, fixedAllocator_type(NULL, mBucketBuffer))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTISET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
		base_type::insert(first, last);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultiset(const this_type& x)
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


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultiset<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::fixedHashMultiset(this_type&& x)
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


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultiset<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::fixedHashMultiset(this_type&& x, const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), 
					x.hash_function(), x.equal_function(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
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


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline fixedHashMultiset<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	fixedHashMultiset(std::initializer_list<value_type> ilist, const overflow_allocator_type& overflowAllocator)
		: base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), Hash(), 
					Predicate(), fixedAllocator_type(NULL, mBucketBuffer, overflowAllocator))
	{
		EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));

		if(!bEnableOverflow)
			base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

		#if EASTL_NAME_ENABLED
			mAllocator.setName(EASTL_FIXED_HASH_MULTISET_DEFAULT_NAME);
		#endif

		mAllocator.reset(mNodeBuffer);
		base_type::insert(ilist.begin(), ilist.end());
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(const this_type& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultiset<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashMultiset<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(this_type&& x)
	{
		base_type::operator=(x);
		return *this;
	}


	template <typename Key, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultiset<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::this_type& 
	fixedHashMultiset<Key, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::operator=(std::initializer_list<value_type> ilist)
	{
		base_type::clear();
		base_type::insert(ilist.begin(), ilist.end());
		return *this;
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	swap(this_type& x)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(*this, x);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	reset_lose_memory()
	{
		base_type::reset_lose_memory();
		base_type::getAllocator().reset(mNodeBuffer);
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::size_type 
	fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::maxSize() const
	{
		return kMaxSize;
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline const typename fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::overflow_allocator_type& 
	fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::getOverflowAllocator() const EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline typename fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::overflow_allocator_type& 
	fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::getOverflowAllocator() EASTL_NOEXCEPT
	{
		return mAllocator.getOverflowAllocator();
	}


	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode, typename OverflowAllocator>
	inline void fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>::
	setOverflowAllocator(const overflow_allocator_type& allocator)
	{
		mAllocator.setOverflowAllocator(allocator);
	}


	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <typename Value, size_t nodeCount, size_t bucketCount, bool bEnableOverflow, typename Hash, typename Predicate, bool bCacheHashCode>
	inline void swap(fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode>& a, 
					 fixedHashMultiset<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode>& b)
	{
		// Fixed containers use a special swap that can deal with excessively large buffers.
		eastl::fixedSwap(a, b);
	}


} // namespace eastl

EA_RESTORE_VC_WARNING()

#endif // Header include guard












