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
// EASTL/fixed_set.h
//
// Copyright (c) 2005, Electronic Arts. All rights reserved.
// Written by Paul Pedriana.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements a set and multiset which use a fixed size memory 
// pool for their nodes. 
//
///////////////////////////////////////////////////////////////////////////////



#ifndef EASTL_FIXED_SET_H
#define EASTL_FIXED_SET_H


#include <eastl/set.h>
#include <eastl/internal/fixed_pool.h>


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
    ///     Allocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
    ///
    template <typename Key, size_t nodeCount, bool bEnableOverflow = true, typename Compare = eastl::less<Key>, typename Allocator = EASTLAllocatorType>
    class fixedSet : public set<Key, Compare, fixed_node_allocator<sizeof(typename set<Key>::node_type), 
                                 nodeCount, set<Key>::kValueAlignment, set<Key>::kValueAlignmentOffset, bEnableOverflow, Allocator> >
    {
    public:
        typedef fixedSet<Key, nodeCount, bEnableOverflow, Compare, Allocator>                              this_type;
        typedef fixed_node_allocator<sizeof(typename set<Key>::node_type), nodeCount, 
                    set<Key>::kValueAlignment, set<Key>::kValueAlignmentOffset, bEnableOverflow, Allocator> fixedAllocator_type;
        typedef set<Key, Compare, fixedAllocator_type>                                                     base_type;
        typedef typename base_type::node_type                                                               node_type;
        typedef typename base_type::size_type                                                               size_type;

        enum
        {
            kMaxSize = nodeCount
        };

        using base_type::insert;

    protected:
        char mBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

        using base_type::mAllocator;

    public:
        /// fixedSet
        ///
        fixedSet()
            : base_type(fixedAllocator_type(NULL))
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
            #endif

            mAllocator.reset(mBuffer);
        }


        /// fixedSet
        ///
        explicit fixedSet(const Compare& compare)
            : base_type(compare, fixedAllocator_type(NULL))
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
            #endif

            mAllocator.reset(mBuffer);
        }


        /// fixedSet
        ///
        fixedSet(const this_type& x)
            : base_type(x.mCompare, fixedAllocator_type(NULL))
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(x.mAllocator.getName());
            #endif

            mAllocator.reset(mBuffer);
            base_type::operator=(x);
        }


        /// fixedSet
        ///
        template <typename InputIterator>
        fixedSet(InputIterator first, InputIterator last)
            : base_type(fixedAllocator_type(NULL))
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
            #endif

            mAllocator.reset(mBuffer);
            insert(first, last);
        }


        /// operator=
        ///
        this_type& operator=(const this_type& x)
        {
            base_type::operator=(x);
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
            base_type::getAllocator().reset(mBuffer);
        }


        size_type maxSize() const
        {
            return kMaxSize;
        }

    }; // fixedSet


    ///////////////////////////////////////////////////////////////////////
    // global operators
    ///////////////////////////////////////////////////////////////////////

    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename Allocator>
    inline void swap(fixedSet<Key, nodeCount, bEnableOverflow, Compare, Allocator>& a, 
                     fixedSet<Key, nodeCount, bEnableOverflow, Compare, Allocator>& b)
    {
        // Fixed containers use a special swap that can deal with excessively large buffers.
        eastl::fixedSwap(a, b);
    }








    /// fixedMultiset
    ///
    /// Implements a multiset with a fixed block of memory identified by the 
    /// nodeCount template parameter. 
    ///
    ///     Key                    The type of object the set holds (a.k.a. value).
    ///     nodeCount              The max number of objects to contain.
    ///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
    ///     Compare                Compare function/object for set ordering.
    ///     Allocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
    ///
    template <typename Key, size_t nodeCount, bool bEnableOverflow = true, typename Compare = eastl::less<Key>, typename Allocator = EASTLAllocatorType>
    class fixedMultiset : public multiset<Key, Compare, fixed_node_allocator<sizeof(typename multiset<Key>::node_type), 
                                           nodeCount, multiset<Key>::kValueAlignment, multiset<Key>::kValueAlignmentOffset, bEnableOverflow, Allocator> >
    {
    public:
        typedef fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, Allocator>                                     this_type;
        typedef fixed_node_allocator<sizeof(typename multiset<Key>::node_type), nodeCount, 
                     multiset<Key>::kValueAlignment, multiset<Key>::kValueAlignmentOffset, bEnableOverflow, Allocator>  fixedAllocator_type;
        typedef multiset<Key, Compare, fixedAllocator_type>                                                            base_type;
        typedef typename base_type::node_type                                                                           node_type;
        typedef typename base_type::size_type                                                                           size_type;

        enum
        {
            kMaxSize = nodeCount
        };

        using base_type::insert;

    protected:
        char mBuffer[fixedAllocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

        using base_type::mAllocator;

    public:
        /// fixedMultiset
        ///
        fixedMultiset()
            : base_type(fixedAllocator_type(NULL))
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
            #endif

            mAllocator.reset(mBuffer);
        }


        /// fixedMultiset
        ///
        explicit fixedMultiset(const Compare& compare)
            : base_type(compare, fixedAllocator_type(NULL))
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
            #endif

            mAllocator.reset(mBuffer);
        }


        /// fixedMultiset
        ///
        fixedMultiset(const this_type& x)
            : base_type(x.mCompare, fixedAllocator_type(NULL))
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(x.mAllocator.getName());
            #endif

            mAllocator.reset(mBuffer);
            base_type::operator=(x);
        }


        /// fixedMultiset
        ///
        template <typename InputIterator>
        fixedMultiset(InputIterator first, InputIterator last)
            : base_type(fixedAllocator_type(NULL))
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
            #endif

            mAllocator.reset(mBuffer);
            insert(first, last);
        }


        /// operator=
        ///
        this_type& operator=(const this_type& x)
        {
            base_type::operator=(x);
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
            base_type::getAllocator().reset(mBuffer);
        }


        size_type maxSize() const
        {
            return kMaxSize;
        }

    }; // fixedMultiset


    ///////////////////////////////////////////////////////////////////////
    // global operators
    ///////////////////////////////////////////////////////////////////////

    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename Compare, typename Allocator>
    inline void swap(fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, Allocator>& a, 
                     fixedMultiset<Key, nodeCount, bEnableOverflow, Compare, Allocator>& b)
    {
        // Fixed containers use a special swap that can deal with excessively large buffers.
        eastl::fixedSwap(a, b);
    }



} // namespace eastl


#endif // Header include guard









