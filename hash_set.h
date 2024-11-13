///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file is based on the TR1 (technical report 1) reference implementation
// of the unordered_set/unordered_map C++ classes as of about 4/2005. Most likely
// many or all C++ library vendors' implementations of this classes will be 
// based off of the reference version and so will look pretty similar to this
// file as well as other vendors' versions. 
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_HASH_SET_H
#define EASTL_HASH_SET_H


#include <eastl/internal/config.h>
#include <eastl/internal/hashtable.h>
#include <eastl/functional.h>
#include <eastl/utility.h>

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



namespace eastl
{

	/// EASTL_HASH_SET_DEFAULT_NAME
	///
	/// Defines a default container name in the absence of a user-provided name.
	///
	#ifndef EASTL_HASH_SET_DEFAULT_NAME
		#define EASTL_HASH_SET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " hashSet" // Unless the user overrides something, this is "EASTL hashSet".
	#endif


	/// EASTL_HASH_MULTISET_DEFAULT_NAME
	///
	/// Defines a default container name in the absence of a user-provided name.
	///
	#ifndef EASTL_HASH_MULTISET_DEFAULT_NAME
		#define EASTL_HASH_MULTISET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " hashMultiset" // Unless the user overrides something, this is "EASTL hashMultiset".
	#endif


	/// EASTL_HASH_SET_DEFAULT_ALLOCATOR
	///
	#ifndef EASTL_HASH_SET_DEFAULT_ALLOCATOR
		#define EASTL_HASH_SET_DEFAULT_ALLOCATOR allocator_type(EASTL_HASH_SET_DEFAULT_NAME)
	#endif

	/// EASTL_HASH_MULTISET_DEFAULT_ALLOCATOR
	///
	#ifndef EASTL_HASH_MULTISET_DEFAULT_ALLOCATOR
		#define EASTL_HASH_MULTISET_DEFAULT_ALLOCATOR allocator_type(EASTL_HASH_MULTISET_DEFAULT_NAME)
	#endif



	/// hashSet
	///
	/// Implements a hashSet, which is a hashed unique-item container.
	/// Lookups are O(1) (that is, they are fast) but the container is 
	/// not sorted. Note that lookups are only O(1) if the hash table
	/// is well-distributed (non-colliding). The lookup approaches
	/// O(n) behavior as the table becomes increasingly poorly distributed.
	///
	/// set_max_load_factor
	/// If you want to make a hashtable never increase its bucket usage,
	/// call set_max_load_factor with a very high value such as 100000.f.
	///
	/// bCacheHashCode
	/// We provide the boolean bCacheHashCode template parameter in order 
	/// to allow the storing of the hash code of the key within the map. 
	/// When this option is disabled, the rehashing of the table will 
	/// call the hash function on the key. Setting bCacheHashCode to true 
	/// is useful for cases whereby the calculation of the hash value for
	/// a contained object is very expensive.
	///
	/// find_as
	/// In order to support the ability to have a hashtable of strings but
	/// be able to do efficiently lookups via char pointers (i.e. so they 
	/// aren't converted to string objects), we provide the find_as 
	/// function. This function allows you to do a find with a key of a
	/// type other than the hashtable key type.
	///
	/// Example find_as usage:
	///     hashSet<string> hashSet;
	///     i = hashSet.find_as("hello");    // Use default hash and compare.
	///
	/// Example find_as usage (namespaces omitted for brevity):
	///     hashSet<string> hashSet;
	///     i = hashSet.find_as("hello", hash<char*>(), equal_to<>());
	///
	template <typename Value, typename Hash = eastl::hash<Value>, typename Predicate = eastl::equal_to<Value>, 
			  typename Allocator = EASTLAllocatorType, bool bCacheHashCode = false>
	class hashSet
		: public hashtable<Value, Value, Allocator, eastl::useSelf<Value>, Predicate,
						   Hash, mod_range_hashing, default_ranged_hash, 
						   prime_rehash_policy, bCacheHashCode, false, true>
	{
	public:
		typedef hashtable<Value, Value, Allocator, eastl::useSelf<Value>, Predicate, 
						  Hash, mod_range_hashing, default_ranged_hash,
						  prime_rehash_policy, bCacheHashCode, false, true>       base_type;
		typedef hashSet<Value, Hash, Predicate, Allocator, bCacheHashCode>       this_type;
		typedef typename base_type::size_type                                     size_type;
		typedef typename base_type::value_type                                    value_type;
		typedef typename base_type::allocator_type                                allocator_type;
		typedef typename base_type::node_type                                     node_type;

	public:
		/// hashSet
		///
		/// Default constructor.
		///
		hashSet()
			: this_type(EASTL_HASH_SET_DEFAULT_ALLOCATOR)
		{
			// Empty
		}


		/// hashSet
		///
		/// Constructor which creates an empty container with allocator.
		///
		explicit hashSet(const allocator_type& allocator)
			: base_type(0, Hash(), mod_range_hashing(), default_ranged_hash(), Predicate(), eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}


		/// hashSet
		///
		/// Constructor which creates an empty container, but start with nBucketCount buckets.
		/// We default to a small nBucketCount value, though the user really should manually 
		/// specify an appropriate value in order to prevent memory from being reallocated.
		///
		/// note: difference in explicit keyword from the standard.
		explicit hashSet(size_type nBucketCount, const Hash& hashFunction = Hash(), const Predicate& predicate = Predicate(), 
						  const allocator_type& allocator = EASTL_HASH_SET_DEFAULT_ALLOCATOR)
			: base_type(nBucketCount, hashFunction, mod_range_hashing(), default_ranged_hash(), predicate, eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}

		// hashSet(size_type nBucketCount, const allocator_type& allocator)
		// hashSet(size_type nBucketCount, const Hash& hashFunction, const allocator_type& allocator)


		hashSet(const this_type& x)
		  : base_type(x)
		{
		}

		// hashSet(const this_type& x, const allocator_type& allocator)


		hashSet(this_type&& x)
		  : base_type(eastl::move(x))
		{
		}


		hashSet(this_type&& x, const allocator_type& allocator)
		  : base_type(eastl::move(x), allocator)
		{
		}


		/// hashSet
		///
		/// initializer_list-based constructor. 
		/// Allows for initializing with brace values (e.g. hashSet<int> hs = { 3, 4, 5, }; )
		///     
		hashSet(std::initializer_list<value_type> ilist, size_type nBucketCount = 0, const Hash& hashFunction = Hash(), 
				   const Predicate& predicate = Predicate(), const allocator_type& allocator = EASTL_HASH_SET_DEFAULT_ALLOCATOR)
			: base_type(ilist.begin(), ilist.end(), nBucketCount, hashFunction, mod_range_hashing(), default_ranged_hash(), predicate, eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}

		hashSet(std::initializer_list<value_type> ilist, const allocator_type& allocator)
			: base_type(ilist.begin(), ilist.end(), 0, Hash(), mod_range_hashing(), default_ranged_hash(), Predicate(), eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}

		// hashSet(std::initializer_list<value_type> ilist, size_type nBucketCount, const allocator_type& allocator)

		// hashSet(std::initializer_list<value_type> ilist, size_type nBucketCount, const Hash& hashFunction,
		// 	const allocator_type& allocator)


		/// hashSet
		///
		/// An input bucket count of <= 1 causes the bucket count to be equal to the number of 
		/// elements in the input range.
		///
		template <typename FowardIterator>
		hashSet(FowardIterator first, FowardIterator last, size_type nBucketCount = 0, const Hash& hashFunction = Hash(), 
				 const Predicate& predicate = Predicate(), const allocator_type& allocator = EASTL_HASH_SET_DEFAULT_ALLOCATOR)
			: base_type(first, last, nBucketCount, hashFunction, mod_range_hashing(), default_ranged_hash(), predicate, eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}

		// template <typename ForwardIterator>
		// hashSet(ForwardIterator first, ForwardIterator last, size_type nBucketCount, const allocator_type& allocator)

		// template <typename ForwardIterator>
		// hashSet(ForwardIterator first, ForwardIterator last, size_type nBucketCount, const Hash& hashFunction, const allocator_type& allocator)


		this_type& operator=(const this_type& x)
		{
			return static_cast<this_type&>(base_type::operator=(x));
		}


		this_type& operator=(std::initializer_list<value_type> ilist)
		{
			return static_cast<this_type&>(base_type::operator=(ilist));
		}


		this_type& operator=(this_type&& x)
		{
			return static_cast<this_type&>(base_type::operator=(eastl::move(x)));
		}

	}; // hashSet

	/// hashSet erase_if
	///
	/// https://en.cppreference.com/w/cpp/container/unordered_set/erase_if
	template <typename Value, typename Hash, typename Predicate, typename Allocator, bool bCacheHashCode, typename UserPredicate>
	typename eastl::hashSet<Value, Hash, Predicate, Allocator, bCacheHashCode>::size_type erase_if(eastl::hashSet<Value, Hash, Predicate, Allocator, bCacheHashCode>& c, UserPredicate predicate)
	{
		auto oldSize = c.size();
		// Erases all elements that satisfy the predicate pred from the container.
		for (auto i = c.begin(), last = c.end(); i != last;)
		{
			if (predicate(*i))
			{
				i = c.erase(i);
			}
			else
			{
				++i;
			}
		}
		return oldSize - c.size();
	}


	/// hashMultiset
	///
	/// Implements a hashMultiset, which is the same thing as a hashSet 
	/// except that contained elements need not be unique. See the documentation 
	/// for hashSet for details.
	///
	template <typename Value, typename Hash = eastl::hash<Value>, typename Predicate = eastl::equal_to<Value>, 
			  typename Allocator = EASTLAllocatorType, bool bCacheHashCode = false>
	class hashMultiset
		: public hashtable<Value, Value, Allocator, eastl::useSelf<Value>, Predicate,
						   Hash, mod_range_hashing, default_ranged_hash,
						   prime_rehash_policy, bCacheHashCode, false, false>
	{
	public:
		typedef hashtable<Value, Value, Allocator, eastl::useSelf<Value>, Predicate,
						  Hash, mod_range_hashing, default_ranged_hash,
						  prime_rehash_policy, bCacheHashCode, false, false>          base_type;
		typedef hashMultiset<Value, Hash, Predicate, Allocator, bCacheHashCode>      this_type;
		typedef typename base_type::size_type                                         size_type;
		typedef typename base_type::value_type                                        value_type;
		typedef typename base_type::allocator_type                                    allocator_type;
		typedef typename base_type::node_type                                         node_type;

	public:
		/// hashMultiset
		///
		/// Default constructor.
		/// 
		explicit hashMultiset(const allocator_type& allocator = EASTL_HASH_MULTISET_DEFAULT_ALLOCATOR)
			: base_type(0, Hash(), mod_range_hashing(), default_ranged_hash(), Predicate(), eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}


		/// hashMultiset
		///
		/// Constructor which creates an empty container, but start with nBucketCount buckets.
		/// We default to a small nBucketCount value, though the user really should manually 
		/// specify an appropriate value in order to prevent memory from being reallocated.
		///
		/// note: difference in explicit keyword from the standard.
		explicit hashMultiset(size_type nBucketCount, const Hash& hashFunction = Hash(), 
							   const Predicate& predicate = Predicate(), const allocator_type& allocator = EASTL_HASH_MULTISET_DEFAULT_ALLOCATOR)
			: base_type(nBucketCount, hashFunction, mod_range_hashing(), default_ranged_hash(), predicate, eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}

		// hashMultiset(size_type nBucketCount, const allocator_type& allocator)
		// hashMultiset(size_type nBucketCount, const Hash& hashFunction, const allocator_type& allocator)


		hashMultiset(const this_type& x)
		  : base_type(x)
		{
		}

		// hashMultiset(const this_type& x, const allocator_type& allocator)


		hashMultiset(this_type&& x)
		  : base_type(eastl::move(x))
		{
		}


		hashMultiset(this_type&& x, const allocator_type& allocator)
		  : base_type(eastl::move(x), allocator)
		{
		}


		/// hashMultiset
		///
		/// initializer_list-based constructor. 
		/// Allows for initializing with brace values (e.g. hashSet<int> hs = { 3, 3, 4, }; )
		///     
		hashMultiset(std::initializer_list<value_type> ilist, size_type nBucketCount = 0, const Hash& hashFunction = Hash(), 
				   const Predicate& predicate = Predicate(), const allocator_type& allocator = EASTL_HASH_MULTISET_DEFAULT_ALLOCATOR)
			: base_type(ilist.begin(), ilist.end(), nBucketCount, hashFunction, mod_range_hashing(), default_ranged_hash(), predicate, eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}

		hashMultiset(std::initializer_list<value_type> ilist, const allocator_type& allocator)
			: base_type(ilist.begin(), ilist.end(), 0, Hash(), mod_range_hashing(), default_ranged_hash(), Predicate(), eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}

		// hashMultiset(std::initializer_list<value_type> ilist, size_type nBucketCount, const allocator_type& allocator)

		// hashMultiset(std::initializer_list<value_type> ilist, size_type nBucketCount, const Hash& hashFunction,
		// 	const allocator_type& allocator)


		/// hashMultiset
		///
		/// An input bucket count of <= 1 causes the bucket count to be equal to the number of 
		/// elements in the input range.
		///
		template <typename FowardIterator>
		hashMultiset(FowardIterator first, FowardIterator last, size_type nBucketCount = 0, const Hash& hashFunction = Hash(), 
					  const Predicate& predicate = Predicate(), const allocator_type& allocator = EASTL_HASH_MULTISET_DEFAULT_ALLOCATOR)
			: base_type(first, last, nBucketCount, hashFunction, mod_range_hashing(), default_ranged_hash(), predicate, eastl::useSelf<Value>(), allocator)
		{
			// Empty
		}

		// template <typename ForwardIterator>
		// hashMultiset(ForwardIterator first, ForwardIterator last, size_type nBucketCount, const allocator_type& allocator)

		// template <typename ForwardIterator>
		// hashMultiset(ForwardIterator first, ForwardIterator last, size_type nBucketCount, const Hash& hashFunction, const allocator_type& allocator)


		this_type& operator=(const this_type& x)
		{
			return static_cast<this_type&>(base_type::operator=(x));
		}


		this_type& operator=(std::initializer_list<value_type> ilist)
		{
			return static_cast<this_type&>(base_type::operator=(ilist));
		}


		this_type& operator=(this_type&& x)
		{
			return static_cast<this_type&>(base_type::operator=(eastl::move(x)));
		}

	}; // hashMultiset

	/// hashMultiset erase_if
	///
	/// https://en.cppreference.com/w/cpp/container/unordered_multiset/erase_if
	template <typename Value, typename Hash, typename Predicate, typename Allocator, bool bCacheHashCode, typename UserPredicate>
	typename eastl::hashMultiset<Value, Hash, Predicate, Allocator, bCacheHashCode>::size_type erase_if(eastl::hashMultiset<Value, Hash, Predicate, Allocator, bCacheHashCode>& c, UserPredicate predicate)
	{
		auto oldSize = c.size();
		// Erases all elements that satisfy the predicate pred from the container.
		for (auto i = c.begin(), last = c.end(); i != last;)
		{
			if (predicate(*i))
			{
				i = c.erase(i);
			}
			else
			{
				++i;
			}
		}
		return oldSize - c.size();
	}



	///////////////////////////////////////////////////////////////////////
	// global operators
	///////////////////////////////////////////////////////////////////////

	template <typename Value, typename Hash, typename Predicate, typename Allocator, bool bCacheHashCode>
	inline bool operator==(const hashSet<Value, Hash, Predicate, Allocator, bCacheHashCode>& a, 
						   const hashSet<Value, Hash, Predicate, Allocator, bCacheHashCode>& b)
	{
		typedef typename hashSet<Value, Hash, Predicate, Allocator, bCacheHashCode>::const_iterator const_iterator;

		// We implement branching with the assumption that the return value is usually false.
		if(a.size() != b.size())
			return false;

		// For set (with its unique keys), we need only test that each element in a can be found in b,
		// as there can be only one such pairing per element. multiset needs to do a something more elaborate.
		for(const_iterator ai = a.begin(), aiEnd = a.end(), biEnd = b.end(); ai != aiEnd; ++ai)
		{
			const_iterator bi = b.find(*ai);

			if((bi == biEnd) || !(*ai == *bi)) // We have to compare values in addition to making sure the lookups succeeded. This is because the lookup is done via the user-supplised Predicate
				return false;                  // which isn't strictly required to be identical to the Value operator==, though 99% of the time it will be so.  
		}

		return true;
	}

#if !defined(EA_COMPILER_HAS_THREE_WAY_COMPARISON)
	template <typename Value, typename Hash, typename Predicate, typename Allocator, bool bCacheHashCode>
	inline bool operator!=(const hashSet<Value, Hash, Predicate, Allocator, bCacheHashCode>& a, 
						   const hashSet<Value, Hash, Predicate, Allocator, bCacheHashCode>& b)
	{
		return !(a == b);
	}
#endif

	template <typename Value, typename Hash, typename Predicate, typename Allocator, bool bCacheHashCode>
	inline bool operator==(const hashMultiset<Value, Hash, Predicate, Allocator, bCacheHashCode>& a, 
						   const hashMultiset<Value, Hash, Predicate, Allocator, bCacheHashCode>& b)
	{
		typedef typename hashMultiset<Value, Hash, Predicate, Allocator, bCacheHashCode>::const_iterator const_iterator;
		typedef typename eastl::iterator_traits<const_iterator>::difference_type difference_type;

		// We implement branching with the assumption that the return value is usually false.
		if(a.size() != b.size())
			return false;

		// We can't simply search for each element of a in b, as it may be that the bucket for 
		// two elements in a has those same two elements in b but in different order (which should 
		// still result in equality). Also it's possible that one bucket in a has two elements which 
		// both match a solitary element in the equivalent bucket in b (which shouldn't result in equality).
		eastl::pair<const_iterator, const_iterator> aRange;
		eastl::pair<const_iterator, const_iterator> bRange;

		for(const_iterator ai = a.begin(), aiEnd = a.end(); ai != aiEnd; ai = aRange.second) // For each element in a...
		{
			aRange = a.equalRange(*ai); // Get the range of elements in a that are equal to ai.
			bRange = b.equalRange(*ai); // Get the range of elements in b that are equal to ai.

			// We need to verify that aRange == bRange. First make sure the range sizes are equivalent...
			const difference_type aDistance = eastl::distance(aRange.first, aRange.second);
			const difference_type bDistance = eastl::distance(bRange.first, bRange.second);

			if(aDistance != bDistance)
				return false;

			// At this point, aDistance > 0 and aDistance == bDistance.
			// Implement a fast pathway for the case that there's just a single element.
			if(aDistance == 1)
			{
				if(!(*aRange.first == *bRange.first))   // We have to compare values in addition to making sure the distance (element count) was equal. This is because the lookup is done via the user-supplised Predicate
					return false;                       // which isn't strictly required to be identical to the Value operator==, though 99% of the time it will be so. Ditto for the is_permutation usage below.
			}
			else
			{
				// Check to see if these aRange and bRange are any permutation of each other. 
				// This check gets slower as there are more elements in the range.
				if(!eastl::is_permutation(aRange.first, aRange.second, bRange.first))
					return false;
			}
		}

		return true;
	}

#if !defined(EA_COMPILER_HAS_THREE_WAY_COMPARISON)
	template <typename Value, typename Hash, typename Predicate, typename Allocator, bool bCacheHashCode>
	inline bool operator!=(const hashMultiset<Value, Hash, Predicate, Allocator, bCacheHashCode>& a, 
						   const hashMultiset<Value, Hash, Predicate, Allocator, bCacheHashCode>& b)
	{
		return !(a == b);
	}
#endif

} // namespace eastl


#endif // Header include guard












