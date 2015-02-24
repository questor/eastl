/*
Copyright (C) 2009,2010,2012 Electronic Arts, Inc.  All rights reserved.

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
// This file implements a set and multiset which use a fixed size memory 
// pool for their nodes. 
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_SET_H
#define EASTL_FIXED_SET_H


#include <eastl/set.h>
#include <eastl/internal/fixed_pool.h>

#if defined(EA_PRAGMA_ONCE_SUPPORTED)
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
        #define EASTL_FIXED_SET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixed_set" // Unless the user overrides something, this is "EASTL fixed_set".
    #endif

    #ifndef EASTL_FIXED_MULTISET_DEFAULT_NAME
        #define EASTL_FIXED_MULTISET_DEFAULT_NAME EASTL_DEFAULT_NAME_PREFIX " fixed_multiset" // Unless the user overrides something, this is "EASTL fixed_multiset".
    #endif



    /// fixed_set
    ///
    /// Implements a set with a fixed block of memory identified by the 
    /// nodeCount template parameter. 
    ///
    /// Template parameters:
    ///     Key                    The type of object the set holds (a.k.a. value).
    ///     nodeCount              The max number of objects to contain.
    ///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
    ///     compare                compare function/object for set ordering.
    ///     OverflowAllocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
    ///
    template <typename Key, size_t nodeCount, bool bEnableOverflow = true, typename compare = eastl::less<Key>, typename OverflowAllocator = EASTLAllocatorType>
    class fixed_set : public set<Key, compare, fixed_node_allocator<sizeof(typename set<Key>::node_type), 
                                 nodeCount, EASTL_ALIGN_OF(Key), 0, bEnableOverflow, OverflowAllocator> >
    {
    public:
        typedef fixed_node_allocator<sizeof(typename set<Key>::node_type), nodeCount, 
                    EASTL_ALIGN_OF(Key), 0, bEnableOverflow, OverflowAllocator>            fixed_allocator_type;
        typedef typename fixed_allocator_type::overflow_allocator_type                     overflow_allocator_type;
        typedef set<Key, compare, fixed_allocator_type>                                    base_type;
        typedef fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>     this_type;
        typedef typename base_type::node_type                                              node_type;
        typedef typename base_type::size_type                                              size_type;

        enum { kMaxSize = nodeCount };

        using base_type::insert;

    protected:
        char mBuffer[fixed_allocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

        using base_type::mAllocator;

    public:
        fixed_set();
        fixed_set(const overflow_allocator_type& overflowAllocator);
        explicit fixed_set(const compare& compare);
        fixed_set(const this_type& x);

        template <typename InputIterator>
        fixed_set(InputIterator first, InputIterator last);

        this_type& operator=(const this_type& x);

        void swap(this_type& x);

        void resetLoseMemory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

        size_type maxSize() const;

        overflow_allocator_type& getOverflowAllocator();
        void                     setOverflowAllocator(const overflow_allocator_type& allocator);

        #if EASTL_RESET_ENABLED
            void reset(); // This function name is deprecated; use resetLoseMemory instead.
        #endif
    }; // fixed_set






    /// fixed_multiset
    ///
    /// Implements a multiset with a fixed block of memory identified by the 
    /// nodeCount template parameter. 
    ///
    ///     Key                    The type of object the set holds (a.k.a. value).
    ///     nodeCount              The max number of objects to contain.
    ///     bEnableOverflow        Whether or not we should use the global heap if our object pool is exhausted.
    ///     compare                compare function/object for set ordering.
    ///     OverflowAllocator              Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
    ///
    template <typename Key, size_t nodeCount, bool bEnableOverflow = true, typename compare = eastl::less<Key>, typename OverflowAllocator = EASTLAllocatorType>
    class fixed_multiset : public multiset<Key, compare, fixed_node_allocator<sizeof(typename multiset<Key>::node_type), 
                                           nodeCount, EASTL_ALIGN_OF(Key), 0, bEnableOverflow, OverflowAllocator> >
    {
    public:
        typedef fixed_node_allocator<sizeof(typename multiset<Key>::node_type), nodeCount, 
                     EASTL_ALIGN_OF(Key), 0, bEnableOverflow, OverflowAllocator>                fixed_allocator_type;
        typedef typename fixed_allocator_type::overflow_allocator_type                          overflow_allocator_type;
        typedef multiset<Key, compare, fixed_allocator_type>                                    base_type;
        typedef fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>     this_type;
        typedef typename base_type::node_type                                                   node_type;
        typedef typename base_type::size_type                                                   size_type;

        enum { kMaxSize = nodeCount };

        using base_type::insert;

    protected:
        char mBuffer[fixed_allocator_type::kBufferSize]; // kBufferSize will take into account alignment requirements.

        using base_type::mAllocator;

    public:
        fixed_multiset();
        fixed_multiset(const overflow_allocator_type& overflowAllocator);
        explicit fixed_multiset(const compare& compare);
        fixed_multiset(const this_type& x);

        template <typename InputIterator>
        fixed_multiset(InputIterator first, InputIterator last);

        this_type& operator=(const this_type& x);

        void swap(this_type& x);

        void resetLoseMemory(); // This is a unilateral reset to an initially empty state. No destructors are called, no deallocation occurs.

        size_type maxSize() const;

        overflow_allocator_type& getOverflowAllocator();
        void                     setOverflowAllocator(const overflow_allocator_type& allocator);

        #if EASTL_RESET_ENABLED
            void reset(); // This function name is deprecated; use resetLoseMemory instead.
        #endif
    }; // fixed_multiset




    ///////////////////////////////////////////////////////////////////////
    // fixed_set
    ///////////////////////////////////////////////////////////////////////

    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_set()
        : base_type(fixed_allocator_type(NULL))
    {
        #if EASTL_NAME_ENABLED
            mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
        #endif

        mAllocator.reset(mBuffer);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_set(const overflow_allocator_type& overflowAllocator)
        : base_type(fixed_allocator_type(NULL, overflowAllocator))
    {
        #if EASTL_NAME_ENABLED
            mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
        #endif

        mAllocator.reset(mBuffer);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_set(const compare& compare)
        : base_type(compare, fixed_allocator_type(NULL))
    {
        #if EASTL_NAME_ENABLED
            mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
        #endif

        mAllocator.reset(mBuffer);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_set(const this_type& x)
        : base_type(x.mCompare, fixed_allocator_type(NULL))
    {
        mAllocator.copy_overflow_allocator(x.mAllocator);

        #if EASTL_NAME_ENABLED
            mAllocator.setName(x.mAllocator.getName());
        #endif

        mAllocator.reset(mBuffer);
        base_type::operator=(x);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    template <typename InputIterator>
    fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_set(InputIterator first, InputIterator last)
        : base_type(fixed_allocator_type(NULL))
    {
        #if EASTL_NAME_ENABLED
            mAllocator.setName(EASTL_FIXED_SET_DEFAULT_NAME);
        #endif

        mAllocator.reset(mBuffer);
        insert(first, last);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline typename fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::this_type& 
    fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::operator=(const this_type& x)
    {
        base_type::operator=(x);
        return *this;
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline void fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::swap(this_type& x)
    {
        // Fixed containers use a special swap that can deal with excessively large buffers.
        eastl::fixed_swap(*this, x);
    }


    #if EASTL_RESET_ENABLED
        // This function name is deprecated; use resetLoseMemory instead.
        template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
        inline void fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::reset()
        {
            resetLoseMemory();
        }
    #endif


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline void fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::resetLoseMemory()
    {
        base_type::resetLoseMemory();
        base_type::getAllocator().reset(mBuffer);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline typename fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::size_type 
    fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::maxSize() const
    {
        return kMaxSize;
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline typename fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::overflow_allocator_type& 
    fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::getOverflowAllocator()
    {
        return mAllocator.getOverflowAllocator();
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline void fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::setOverflowAllocator(const overflow_allocator_type& allocator)
    {
        mAllocator.setOverflowAllocator(allocator);
    }


    ///////////////////////////////////////////////////////////////////////
    // global operators
    ///////////////////////////////////////////////////////////////////////

    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline void swap(fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>& a, 
                     fixed_set<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>& b)
    {
        // Fixed containers use a special swap that can deal with excessively large buffers.
        eastl::fixed_swap(a, b);
    }



    ///////////////////////////////////////////////////////////////////////
    // fixed_multiset
    ///////////////////////////////////////////////////////////////////////

    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_multiset()
        : base_type(fixed_allocator_type(NULL))
    {
        #if EASTL_NAME_ENABLED
            mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
        #endif

        mAllocator.reset(mBuffer);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_multiset(const overflow_allocator_type& overflowAllocator)
        : base_type(fixed_allocator_type(NULL, overflowAllocator))
    {
        #if EASTL_NAME_ENABLED
            mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
        #endif

        mAllocator.reset(mBuffer);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_multiset(const compare& compare)
        : base_type(compare, fixed_allocator_type(NULL))
    {
        #if EASTL_NAME_ENABLED
            mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
        #endif

        mAllocator.reset(mBuffer);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_multiset(const this_type& x)
        : base_type(x.mCompare, fixed_allocator_type(NULL))
    {
        mAllocator.copy_overflow_allocator(x.mAllocator);

        #if EASTL_NAME_ENABLED
            mAllocator.setName(x.mAllocator.getName());
        #endif

        mAllocator.reset(mBuffer);
        base_type::operator=(x);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    template <typename InputIterator>
    fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::fixed_multiset(InputIterator first, InputIterator last)
        : base_type(fixed_allocator_type(NULL))
    {
        #if EASTL_NAME_ENABLED
            mAllocator.setName(EASTL_FIXED_MULTISET_DEFAULT_NAME);
        #endif

        mAllocator.reset(mBuffer);
        insert(first, last);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline typename fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::this_type& 
    fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::operator=(const this_type& x)
    {
        base_type::operator=(x);
        return *this;
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline void fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::swap(this_type& x)
    {
        // Fixed containers use a special swap that can deal with excessively large buffers.
        eastl::fixed_swap(*this, x);
    }


    #if EASTL_RESET_ENABLED
        // This function name is deprecated; use resetLoseMemory instead.
        template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
        inline void fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::reset()
        {
            resetLoseMemory();
        }
    #endif


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline void fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::resetLoseMemory()
    {
        base_type::resetLoseMemory();
        base_type::getAllocator().reset(mBuffer);
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline typename fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::size_type 
    fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::maxSize() const
    {
        return kMaxSize;
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline typename fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::overflow_allocator_type& 
    fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::getOverflowAllocator()
    {
        return mAllocator.getOverflowAllocator();
    }


    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline void fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>::setOverflowAllocator(const overflow_allocator_type& allocator)
    {
        mAllocator.setOverflowAllocator(allocator);
    }


    ///////////////////////////////////////////////////////////////////////
    // global operators
    ///////////////////////////////////////////////////////////////////////

    template <typename Key, size_t nodeCount, bool bEnableOverflow, typename compare, typename OverflowAllocator>
    inline void swap(fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>& a, 
                     fixed_multiset<Key, nodeCount, bEnableOverflow, compare, OverflowAllocator>& b)
    {
        // Fixed containers use a special swap that can deal with excessively large buffers.
        eastl::fixed_swap(a, b);
    }



} // namespace eastl


#endif // Header include guard









