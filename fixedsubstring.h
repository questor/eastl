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
// EASTL/fixedSubstring.h
//
// Copyright (c) 2005, Electronic Arts. All rights reserved.
// Written and maintained by Paul Pedriana.
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_SUBSTRING_H
#define EASTL_FIXED_SUBSTRING_H


#include <eastl/string.h>


namespace eastl
{

    /// fixedSubstring
    ///
    /// Implements a string which is a reference to a segment of characters. 
    /// This class is efficient because it allocates no memory and copies no
    /// memory during construction and assignment, but rather refers directly 
    /// to the segment of chracters. A common use of this is to have a 
    /// fixedSubstring efficiently refer to a substring within another string.
    ///
    /// You cannot directly resize a fixedSubstring (e.g. via resize, insert,
    /// append, erase), but you can assign a different substring to it. 
    /// You can modify the characters within a substring in place.
    /// As of this writing, in the name of being lean and simple it is the 
    /// user's responsibility to not call unsupported resizing functions
    /// such as those listed above. A detailed listing of the functions which
    /// are not supported is given below in the class declaration.
    ///
    /// The c_str function doesn't act as one might hope, as it simply 
    /// returns the pointer to the beginning of the string segment and the
    /// 0-terminator may be beyond the end of the segment. If you want to 
    /// always be able to use c_str as expected, use the fixed string solution 
    /// we describe below.
    ///
    /// Another use of fixedSubstring is to provide C++ string-like functionality
    /// with a C character array. This allows you to work on a C character array
    /// as if it were a C++ string as opposed using the C string API. Thus you 
    /// can do this:
    ///
    ///    void DoSomethingForUser(char* timeStr, size_t timeStrCapacity)
    ///    {
    ///        fixedSubstring tmp(timeStr, timeStrCapacity);
    ///        tmp  = "hello ";
    ///        tmp += "world";
    ///    }
    ///
    /// Note that this class constructs and assigns from const string pointers
    /// and const string objects, yet this class does not declare its member
    /// data as const. This is a concession in order to allow this implementation
    /// to be simple and lean. It is the user's responsibility to make sure
    /// that strings that should not or can not be modified are either not
    /// used by fixedSubstring or are not modified by fixedSubstring.
    ///
    /// A more flexible alternative to fixedSubstring is fixedString.
    /// fixedString has none of the functional limitations that fixedSubstring
    /// has and like fixedSubstring it doesn't allocate memory. However,
    /// fixedString makes a *copy* of the source string and uses local
    /// memory to store that copy. Also, fixedString objects on the stack
    /// are going to have a limit as to their maximum size.
    ///
    /// Notes:
    ///     As of this writing, the string class necessarily reallocates when 
    ///     an insert of self is done into self. As a result, the fixedSubstring 
    ///     class doesn't support inserting self into self. 
    ///
    /// Example usage:
    ///     basicString<char>    str("hello world");
    ///     fixedSubstring<char> sub(str, 2, 5);      // sub == "llo w"
    /// 
    template <typename T>
    class fixedSubstring : public basicString<T>
    {
    public:
        typedef basicString<T>                   base_type;
        typedef fixedSubstring<T>                this_type;
        typedef typename base_type::size_type     size_type;
        typedef typename base_type::value_type    value_type;

        using base_type::npos;
        using base_type::mpBegin;
        using base_type::mpEnd;
        using base_type::mpCapacity;
        using base_type::reset;
        using base_type::mAllocator;

    public:
        fixedSubstring()
            : base_type()
        {
        }

        fixedSubstring(const base_type& x)
            : base_type()
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(x.getAllocator().getName());
            #endif

            assign(x);
        }

        fixedSubstring(const base_type& x, size_type position, size_type n = base_type::npos)
            : base_type()
        {
            #if EASTL_NAME_ENABLED
                mAllocator.setName(x.getAllocator().getName());
            #endif

            assign(x, position, n);
        }

        fixedSubstring(const value_type* p, size_type n)
            : base_type()
        {
            assign(p, n);
        }

        fixedSubstring(const value_type* p)
            : base_type()
        {
             assign(p);
        }

        fixedSubstring(const value_type* pBegin, const value_type* pEnd)
            : base_type()
        {
            assign(pBegin, pEnd);
        }

        ~fixedSubstring()
        {
            // We need to reset, as otherwise the parent destructor will
            // attempt to free our memory.
            reset();
        }

        this_type& operator=(const base_type& x)
        {
            assign(x);
            return *this;
        }

        this_type& operator=(const value_type* p)
        {
            assign(p);
            return *this;
        }

        this_type& assign(const base_type& x)
        {
            // By design, we need to cast away const-ness here. 
            mpBegin    = const_cast<value_type*>(x.data());
            mpEnd      = mpBegin + x.size();
            mpCapacity = mpEnd;
            return *this;
        }

        this_type& assign(const base_type& x, size_type position, size_type n)
        {
            // By design, we need to cast away const-ness here. 
            mpBegin    = const_cast<value_type*>(x.data()) + position;
            mpEnd      = mpBegin + n;
            mpCapacity = mpEnd;
            return *this;
        }

        this_type& assign(const value_type* p, size_type n)
        {
            // By design, we need to cast away const-ness here. 
            mpBegin    = const_cast<value_type*>(p);
            mpEnd      = mpBegin + n;
            mpCapacity = mpEnd;
            return *this;
        }

        this_type& assign(const value_type* p)
        {
            // By design, we need to cast away const-ness here. 
            mpBegin    = const_cast<value_type*>(p);
            mpEnd      = mpBegin + CharStrlen(p);
            mpCapacity = mpEnd;
            return *this;
        }

        this_type& assign(const value_type* pBegin, const value_type* pEnd)
        {
            // By design, we need to cast away const-ness here. 
            mpBegin    = const_cast<value_type*>(pBegin);
            mpEnd      = const_cast<value_type*>(pEnd);
            mpCapacity = mpEnd;
            return *this;
        }


        // Partially supported functionality
        //
        // When using fixedSubstring on a character sequence that is within another
        // string, the following functions may do one of two things:
        //     1 Attempt to reallocate
        //     2 Write a 0 char at the end of the fixedSubstring
        //
        // Item #1 will result in a crash, due to the attempt by the underlying 
        // string class to free the substring memory. Item #2 will result in a 0 
        // char being written to the character array. Item #2 may or may not be 
        // a problem, depending on how you use fixedSubstring. Thus the following
        // functions should be used carefully.
        //
        // basicString&  operator=(const basicString& x);
        // basicString&  operator=(value_type c);
        // void           resize(size_type n, value_type c);
        // void           resize(size_type n);
        // void           reserve(size_type = 0);
        // void           setCapacity(size_type n);
        // void           clear();
        // basicString&  operator+=(const basicString& x);
        // basicString&  operator+=(const value_type* p);
        // basicString&  operator+=(value_type c);
        // basicString&  append(const basicString& x);
        // basicString&  append(const basicString& x, size_type position, size_type n);
        // basicString&  append(const value_type* p, size_type n);
        // basicString&  append(const value_type* p);
        // basicString&  append(size_type n);
        // basicString&  append(size_type n, value_type c);
        // basicString&  append(const value_type* pBegin, const value_type* pEnd);
        // basicString&  appendSprintfVaList(const value_type* pFormat, va_list arguments);
        // basicString&  appendSprintf(const value_type* pFormat, ...);
        // void           pushBack(value_type c);
        // void           popBack();
        // basicString&  assign(const value_type* p, size_type n);
        // basicString&  assign(size_type n, value_type c);
        // basicString&  insert(size_type position, const basicString& x);
        // basicString&  insert(size_type position, const basicString& x, size_type beg, size_type n);
        // basicString&  insert(size_type position, const value_type* p, size_type n);
        // basicString&  insert(size_type position, const value_type* p);
        // basicString&  insert(size_type position, size_type n, value_type c);
        // iterator       insert(iterator p, value_type c);
        // void           insert(iterator p, size_type n, value_type c);
        // void           insert(iterator p, const value_type* pBegin, const value_type* pEnd);
        // basicString&  erase(size_type position = 0, size_type n = npos);
        // iterator       erase(iterator p);
        // iterator       erase(iterator pBegin, iterator pEnd);
        // void           swap(basicString& x);
        // basicString&  sprintfVaList(const value_type* pFormat, va_list arguments);
        // basicString&  sprintf(const value_type* pFormat, ...);


    }; // fixedSubstring


} // namespace eastl



#endif // Header include guard












