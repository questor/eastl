/////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_ATOMIC_INTERNA_ATOMIC_FLAG_H
#define EASTL_ATOMIC_INTERNA_ATOMIC_FLAG_H

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif


#include "atomic_push_compiler_options.h"


namespace eastl
{


class atomic_flag
{
public: /* ctors */

	atomic_flag(bool desired)
		: mFlag{ desired }
	{
	}

	atomic_flag() EASTL_NOEXCEPT
		: mFlag{ false }
	{
	}

public: /* deleted ctors && assignment operators */

	atomic_flag(const atomic_flag&) EASTL_NOEXCEPT = delete;

	atomic_flag& operator =(const atomic_flag&)          EASTL_NOEXCEPT = delete;
	atomic_flag& operator =(const atomic_flag&) volatile EASTL_NOEXCEPT = delete;

public: /* clear */

	template <typename Order>
	void clear(Order order) volatile EASTL_NOEXCEPT
	{
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(Order);
	}

	template <typename Order>
	void clear(Order order) EASTL_NOEXCEPT
	{
		EASTL_ATOMIC_STATIC_ASSERT_INVALID_MEMORY_ORDER(Order);
	}

	void clear(eastl::internal::memory_order_relaxed_s) EASTL_NOEXCEPT
	{
		mFlag.store(false, eastl::memory_order_relaxed);
	}

	void clear(eastl::internal::memory_order_release_s) EASTL_NOEXCEPT
	{
		mFlag.store(false, eastl::memory_order_release);
	}

	void clear(eastl::internal::memory_order_seq_cst_s) EASTL_NOEXCEPT
	{
		mFlag.store(false, eastl::memory_order_seq_cst);
	}

	void clear() EASTL_NOEXCEPT
	{
		mFlag.store(false, eastl::memory_order_seq_cst);
	}

public: /* test_and_set */

	template <typename Order>
	bool test_and_set(Order order) volatile EASTL_NOEXCEPT
	{
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(Order);
		return false;
	}

	template <typename Order>
	bool test_and_set(Order order) EASTL_NOEXCEPT
	{
		EASTL_ATOMIC_STATIC_ASSERT_INVALID_MEMORY_ORDER(Order);
		return false;
	}

	bool test_and_set(eastl::internal::memory_order_relaxed_s) EASTL_NOEXCEPT
	{
		return mFlag.exchange(true, eastl::memory_order_relaxed);
	}

	bool test_and_set(eastl::internal::memory_order_acquire_s) EASTL_NOEXCEPT
	{
		return mFlag.exchange(true, eastl::memory_order_acquire);
	}

	bool test_and_set(eastl::internal::memory_order_release_s) EASTL_NOEXCEPT
	{
		return mFlag.exchange(true, eastl::memory_order_release);
	}

	bool test_and_set(eastl::internal::memory_order_acq_rel_s) EASTL_NOEXCEPT
	{
		return mFlag.exchange(true, eastl::memory_order_acq_rel);
	}

	bool test_and_set(eastl::internal::memory_order_seq_cst_s) EASTL_NOEXCEPT
	{
		return mFlag.exchange(true, eastl::memory_order_seq_cst);
	}

	bool test_and_set() EASTL_NOEXCEPT
	{
		return mFlag.exchange(true, eastl::memory_order_seq_cst);
	}

public: /* test */

	template <typename Order>
	bool test(Order order) const volatile EASTL_NOEXCEPT
	{
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(Order);
		return false;
	}

	template <typename Order>
	bool test(Order order) const EASTL_NOEXCEPT
	{
		EASTL_ATOMIC_STATIC_ASSERT_INVALID_MEMORY_ORDER(Order);
		return false;
	}

	bool test(eastl::internal::memory_order_relaxed_s) const EASTL_NOEXCEPT
	{
		return mFlag.load(eastl::memory_order_relaxed);
	}

	bool test(eastl::internal::memory_order_acquire_s) const EASTL_NOEXCEPT
	{
		return mFlag.load(eastl::memory_order_acquire);
	}

	bool test(eastl::internal::memory_order_seq_cst_s) const EASTL_NOEXCEPT
	{
		return mFlag.load(eastl::memory_order_seq_cst);
	}

	bool test() const EASTL_NOEXCEPT
	{
		return mFlag.load(eastl::memory_order_seq_cst);
	}

private:

	eastl::atomic<bool> mFlag;
};


} // namespace eastl


#include "atomic_pop_compiler_options.h"


#endif /* EASTL_ATOMIC_INTERNA_ATOMIC_FLAG_H */
