/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// EASTL/fixedHashMap.h
//
// Copyright (c) 2005, Electronic Arts. All rights reserved.
// Written and maintained by Paul Pedriana.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements a hashMap and hashMultimap which use a fixed size 
// memory pool for its buckets and nodes. 
//
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_HASH_MAP_H
#define EASTL_FIXED_HASH_MAP_H


#include <eastl/hash_map.h>
#include <eastl/internal/fixed_pool.h>


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
              typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, bool bCacheHashCode = false, typename Allocator = EASTLAllocatorType>
    class fixedHashMap : public hashMap<Key, 
                                           T,
                                           Hash,
                                           Predicate,
                                           fixedHashtableAllocator<
                                                bucketCount + 1,
                                                sizeof(typename hashMap<Key, T, Hash, Predicate, Allocator, bCacheHashCode>::node_type), 
                                                nodeCount,
                                                hashMap<Key, T, Hash, Predicate, Allocator, bCacheHashCode>::kValueAlignment, 
                                                hashMap<Key, T, Hash, Predicate, Allocator, bCacheHashCode>::kValueAlignmentOffset, 
                                                bEnableOverflow,
                                                Allocator>, 
                                           bCacheHashCode>
    {
    public:
        typedef fixedHashMap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, Allocator> this_type;
        typedef fixedHashtableAllocator<bucketCount + 1, sizeof(typename hashMap<Key, T, Hash, Predicate, 
                        Allocator, bCacheHashCode>::node_type), nodeCount, hashMap<Key, T, Hash, Predicate, 
                        Allocator, bCacheHashCode>::kValueAlignment, hashMap<Key, T, Hash, Predicate, 
                        Allocator, bCacheHashCode>::kValueAlignmentOffset, bEnableOverflow, Allocator>                  fixedAllocator_type;
        typedef hashMap<Key, T, Hash, Predicate, fixedAllocator_type, bCacheHashCode>                                 base_type;
        typedef typename base_type::node_type                                                                           node_type;
        typedef typename base_type::size_type                                                                           size_type;

        enum
        {
            kMaxSize = nodeCount
        };

        using base_type::mAllocator;

    protected:
        node_type** mBucketBuffer[bucketCount + 1]; // '+1' because the hash table needs a null terminating bucket.
        char        mNodeBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

    public:
        /// fixedHashMap
        ///
        /// Construct an empty fixedHashMap with a given set of parameters.
        ///
        explicit fixedHashMap(const Hash& hashFunction = Hash(), 
                                const Predicate& predicate = Predicate())
            : base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
                        predicate, fixedAllocator_type(NULL, mBucketBuffer))
        {
            EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));
            base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_HASH_MAP_DEFAULT_NAME);
            #endif

            mAllocator.reset(mNodeBuffer);
        }


        /// fixedHashMap
        ///
        /// Construct a fixedHashMap from a source sequence and with a given set of parameters.
        ///
        template <typename InputIterator>
        fixedHashMap(InputIterator first, InputIterator last, 
                        const Hash& hashFunction = Hash(), 
                        const Predicate& predicate = Predicate())
            : base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
                        predicate, fixedAllocator_type(NULL, mBucketBuffer))
        {
            EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));
            base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_HASH_MAP_DEFAULT_NAME);
            #endif

            mAllocator.reset(mNodeBuffer);
            base_type::insert(first, last);
        }


        /// fixedHashMap
        ///
        /// Copy constructor
        ///
        fixedHashMap(const this_type& x)
            : base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), x.hash_function(), 
                        x.equal_function(), fixedAllocator_type(NULL, mBucketBuffer))
        {
            EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));
            base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

            #if EASTL_NAME_ENABLED
                mAllocator.setName(x.mAllocator.getName());
            #endif

            mAllocator.reset(mNodeBuffer);
            base_type::insert(x.begin(), x.end());
        }


        /// operator=
        ///
        /// We provide an override so that assignment is done correctly.
        ///
        this_type& operator=(const this_type& x)
        {
            if(this != &x)
            {
                base_type::clear();
                base_type::insert(x.begin(), x.end());
            }
            return *this;
        }


        void swap(this_type& x)
        {
            // Fixed containers use a special swap that can deal with excessively large buffers.
            eastl::fixedSwap(*this, x);
        }


        void reset()
        {
            base_type::reset();
            base_type::getAllocator().reset(mNodeBuffer);
        }


        size_type maxSize() const
        {
            return kMaxSize;
        }

    }; // fixedHashMap


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
              typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, bool bCacheHashCode = false, typename Allocator = EASTLAllocatorType>
    class fixedHashMultimap : public hashMultimap<Key,
                                                     T,
                                                     Hash,
                                                     Predicate,
                                                     fixedHashtableAllocator<
                                                        bucketCount + 1, 
                                                        sizeof(typename hashMultimap<Key, T, Hash, Predicate, Allocator, bCacheHashCode>::node_type), 
                                                        nodeCount,
                                                        hashMultimap<Key, T, Hash, Predicate, Allocator, bCacheHashCode>::kValueAlignment, 
                                                        hashMultimap<Key, T, Hash, Predicate, Allocator, bCacheHashCode>::kValueAlignmentOffset, 
                                                        bEnableOverflow,
                                                        Allocator>, 
                                                     bCacheHashCode>
    {
    public:
        typedef fixedHashMultimap<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, Allocator> this_type;
        typedef fixedHashtableAllocator<bucketCount + 1, sizeof(typename hashMultimap<Key, T, Hash, Predicate, 
                        Allocator, bCacheHashCode>::node_type), nodeCount, hashMultimap<Key, T, Hash, Predicate, 
                        Allocator, bCacheHashCode>::kValueAlignment, hashMultimap<Key, T, Hash, Predicate, 
                        Allocator, bCacheHashCode>::kValueAlignmentOffset, bEnableOverflow, Allocator>                          fixedAllocator_type;
        typedef hashMultimap<Key, T, Hash, Predicate, fixedAllocator_type, bCacheHashCode>                                    base_type;
        typedef typename base_type::node_type                                                                                   node_type;
        typedef typename base_type::size_type                                                                                   size_type;

        enum
        {
            kMaxSize = nodeCount
        };

        using base_type::mAllocator;

    protected:
        node_type** mBucketBuffer[bucketCount + 1]; // '+1' because the hash table needs a null terminating bucket.
        char        mNodeBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

    public:
        /// fixedHashMultimap
        ///
        /// Construct an empty fixedHashMultimap with a given set of parameters.
        ///
        explicit fixedHashMultimap(const Hash& hashFunction = Hash(), 
                                        const Predicate& predicate = Predicate())
            : base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
                        predicate, fixedAllocator_type(NULL, mBucketBuffer))
        {
            EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));
            base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME);
            #endif

            mAllocator.reset(mNodeBuffer);
        }


        /// fixedHashMultimap
        ///
        /// Construct a fixedHashMultimap from a source sequence and with a given set of parameters.
        ///
        template <typename InputIterator>
        fixedHashMultimap(InputIterator first, InputIterator last, 
                        const Hash& hashFunction = Hash(), 
                        const Predicate& predicate = Predicate())
            : base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), hashFunction, 
                        predicate, fixedAllocator_type(NULL, mBucketBuffer))
        {
            EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));
            base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_HASH_MULTIMAP_DEFAULT_NAME);
            #endif

            mAllocator.reset(mNodeBuffer);
            base_type::insert(first, last);
        }


        /// fixedHashMultimap
        ///
        /// Copy constructor
        ///
        fixedHashMultimap(const this_type& x)
            : base_type(prime_rehash_policy::GetPrevBucketCountOnly(bucketCount), x.hash_function(), 
                        x.equal_function(),fixedAllocator_type(NULL, mBucketBuffer))
        {
            EASTL_CT_ASSERT((nodeCount >= 1) && (bucketCount >= 2));
            base_type::set_max_load_factor(10000.f); // Set it so that we will never resize.

            #if EASTL_NAME_ENABLED
                mAllocator.setName(x.mAllocator.getName());
            #endif

            mAllocator.reset(mNodeBuffer);
            base_type::insert(x.begin(), x.end());
        }


        /// operator=
        ///
        /// We provide an override so that assignment is done correctly.
        ///
        this_type& operator=(const this_type& x)
        {
            if(this != &x)
            {
                base_type::clear();
                base_type::insert(x.begin(), x.end());
            }
            return *this;
        }


        void swap(this_type& x)
        {
            // Fixed containers use a special swap that can deal with excessively large buffers.
            eastl::fixedSwap(*this, x);
        }


        void reset()
        {
            base_type::reset();
            base_type::getAllocator().reset(mNodeBuffer);
        }


        size_type maxSize() const
        {
            return kMaxSize;
        }

    }; // fixedHashMultimap


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




#endif // Header include guard












