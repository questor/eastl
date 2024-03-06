///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_FIXED_SUBSTRING_H
#define EASTL_FIXED_SUBSTRING_H


#include <eastl/string.h>

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



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
		typedef basicString<T>                     base_type;
		typedef fixedSubstring<T>                  this_type;
		typedef typename base_type::size_type       size_type;
		typedef typename base_type::value_type      value_type;
		typedef typename base_type::iterator        iterator;
		typedef typename base_type::const_iterator  const_iterator;

		using base_type::npos;
		using base_type::getAllocator;

	private:
		using base_type::mPair;
		using base_type::AllocateSelf;
		using base_type::internalLayout;

		void SetInternalHeapLayout(value_type* pBeginPtr, size_type nSize, size_type nCap)
		{
			internalLayout().SetHeapBeginPtr(pBeginPtr);
			internalLayout().SetHeapSize(nSize);
			internalLayout().SetHeapCapacity(nCap);
		}


	public:
		fixedSubstring()
			: base_type()
		{
		}

		fixedSubstring(const fixedSubstring& x)
			: fixedSubstring(static_cast<const base_type&>(x))
		{}

		fixedSubstring(const base_type& x)
			: base_type()
		{
			#if EASTL_NAME_ENABLED
				getAllocator().setName(x.getAllocator().getName());
			#endif

			assign(x);
		}

		// We gain no benefit from having an rvalue move constructor or assignment operator,
		// as this class is a const class.

		fixedSubstring(const base_type& x, size_type position, size_type n = base_type::npos)
			: base_type()
		{
			#if EASTL_NAME_ENABLED
				getAllocator().setName(x.getAllocator().getName());
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
			AllocateSelf();
		}

		this_type& operator=(const this_type& x)
		{
			assign(x);
			return *this;
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
			SetInternalHeapLayout(const_cast<value_type*>(x.data()), x.size(), x.size());
			return *this;
		}

		this_type& assign(const base_type& x, size_type position, size_type n)
		{
			// By design, we need to cast away const-ness here.
			SetInternalHeapLayout(const_cast<value_type*>(x.data()) + position, n, n);
			return *this;
		}

		this_type& assign(const value_type* p, size_type n)
		{
			// By design, we need to cast away const-ness here.
			SetInternalHeapLayout(const_cast<value_type*>(p), n, n);
			return *this;
		}

		this_type& assign(const value_type* p)
		{
			// By design, we need to cast away const-ness here.
			SetInternalHeapLayout(const_cast<value_type*>(p), (size_type)CharStrlen(p), (size_type)CharStrlen(p));
			return *this;
		}

		this_type& assign(const value_type* pBegin, const value_type* pEnd)
		{
			// By design, we need to cast away const-ness here.
			SetInternalHeapLayout(const_cast<value_type*>(pBegin), (size_type)(pEnd - pBegin), (size_type)(pEnd - pBegin));
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
		// functions cannot be used safely.

		#if 0 // !defined(EA_COMPILER_NO_DELETED_FUNCTIONS) We may want to enable these deletions after some investigation of possible user impact.
			this_type&  operator=(value_type c) = delete;
			void        resize(size_type n, value_type c) = delete;
			void        resize(size_type n) = delete;
			void        reserve(size_type = 0) = delete;
			void        setCapacity(size_type n) = delete;
			void        clear() = delete;
			this_type&  operator+=(const base_type& x) = delete;
			this_type&  operator+=(const value_type* p) = delete;
			this_type&  operator+=(value_type c) = delete;
			this_type&  append(const base_type& x) = delete;
			this_type&  append(const base_type& x, size_type position, size_type n) = delete;
			this_type&  append(const value_type* p, size_type n) = delete;
			this_type&  append(const value_type* p) = delete;
			this_type&  append(size_type n) = delete;
			this_type&  append(size_type n, value_type c) = delete;
			this_type&  append(const value_type* pBegin, const value_type* pEnd) = delete;
			this_type&  appendSprintfVaList(const value_type* pFormat, va_list arguments) = delete;
			this_type&  appendSprintf(const value_type* pFormat, ...) = delete;
			void        pushBack(value_type c) = delete;
			void        popBack() = delete;
			this_type&  assign(size_type n, value_type c) = delete;
			this_type&  insert(size_type position, const base_type& x) = delete;
			this_type&  insert(size_type position, const base_type& x, size_type beg, size_type n) = delete;
			this_type&  insert(size_type position, const value_type* p, size_type n) = delete;
			this_type&  insert(size_type position, const value_type* p) = delete;
			this_type&  insert(size_type position, size_type n, value_type c) = delete;
			iterator    insert(const_iterator p, value_type c) = delete;
			void        insert(const_iterator p, size_type n, value_type c) = delete;
			void        insert(const_iterator p, const value_type* pBegin, const value_type* pEnd) = delete;
			this_type&  erase(size_type position = 0, size_type n = npos) = delete;
			iterator    erase(const_iterator p) = delete;
			iterator    erase(const_iterator pBegin, const_iterator pEnd) = delete;
			void        swap(base_type& x) = delete;
			this_type&  sprintfVaList(const value_type* pFormat, va_list arguments) = delete;
			this_type&  sprintf(const value_type* pFormat, ...) = delete;
		#endif

	}; // fixedSubstring


} // namespace eastl



#endif // Header include guard
