// A stripped down version of FOREACH for
// illustration purposes. NOT FOR GENERAL USE.
// For a complete implementation, see BOOST_FOREACH at
// http://boost-sandbox.sourceforge.net/vault/index.php?directory=eric_niebler
//
// Copyright 2004 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This version has problems—it evaluates the container expression multiple times, and 
// it doesn't work if the container is an unnamed temporary object. 

#ifndef EASTL_EXTRA_FOREACH_H_
#define EASTL_EXTRA_FOREACH_H_

namespace EricNiebler {

	///////////////////////////////////////////////////////////////////////////////
	// auto_any

	struct auto_any_base
	{
		operator bool() const { return false; }
	};

	template<typename T>
	struct auto_any : auto_any_base
	{
		auto_any(T const& t) : item(t) {}
		mutable T item;
	};

	template<typename T>
	T& auto_any_cast(auto_any_base const& any)
	{
		return static_cast<auto_any<T> const&>(any).item;
	}

	///////////////////////////////////////////////////////////////////////////////
	// FOREACH helper function

	template<typename T>
	auto_any<typename T::const_iterator> begin(T const& t)
	{
		return t.begin();
	}

	template<typename T>
	auto_any<typename T::const_iterator> end(T const& t)
	{
		return t.end();
	}

	template<typename T>
	bool done(auto_any_base const& cur, auto_any_base const& end, T const&)
	{
		typedef typename T::const_iterator iter_type;
		return auto_any_cast<iter_type>(cur) == auto_any_cast<iter_type>(end);
	}

	template<typename T>
	void next(auto_any_base const& cur, T const&)
	{
		typedef typename T::const_iterator iter_type;
		++auto_any_cast<iter_type>(cur);
	}

	template<typename T>
	typename T::const_reference deref(auto_any_base const& cur, T const&)
	{
		typedef typename T::const_iterator iter_type;
		return *auto_any_cast<iter_type>(cur);
	}

}	//namespace EricNiebler

///////////////////////////////////////////////////////////////////////////////
// FOREACH
#define foreach(item, container)													\
	if(EricNiebler::auto_any_base const& b = EricNiebler::begin(container)) {} else \
	if(EricNiebler::auto_any_base const& e = EricNiebler::end(container)) {} else   \
    for(bool more = true;															\
		more && !EricNiebler::done(b,e,container);									\
		more ? EricNiebler::next(b,container) : (void)0)							\
        if (more = false) {} else													\
		for(item = EricNiebler::deref(b,container); !more; more = true)

#endif   //EASTL_EXTRA_FOREACH_H_
