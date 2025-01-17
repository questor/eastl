/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef EASTL_FUNCTION_H
#define EASTL_FUNCTION_H

#include <eastl/internal/config.h>

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <eastl/internal/function_detail.h>

namespace eastl
{

	/// EASTL_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE
	///
	/// Defines the size of the SSO buffer which is used to hold the specified capture state of the callable.
	///
	#ifndef EASTL_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE
		#define EASTL_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE (2 * sizeof(void*))
	#endif

	static_assert(EASTL_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE >= sizeof(void*), "functor storage must be able to hold at least a pointer!");

	template <typename>
	class function;

	template <typename R, typename... Args>
	class function<R(Args...)> : public internal::function_detail<EASTL_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE, R(Args...)>
	{
	private:
		using Base = internal::function_detail<EASTL_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE, R(Args...)>;
	public:
		using typename Base::result_type;

		function() EASTL_NOEXCEPT = default;
		function(std::nullptr_t p) EASTL_NOEXCEPT
			: Base(p)
		{
		}

		function(const function& other)
			: Base(other)
		{
		}

		function(function&& other)
			: Base(eastl::move(other))
		{
		}

		template <typename Functor, typename = EASTL_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(Functor, R, Args..., Base, function)>
		function(Functor functor)
			: Base(eastl::move(functor))
		{
		}

		~function() EASTL_NOEXCEPT = default;

		function& operator=(const function& other)
		{
			Base::operator=(other);
			return *this;
		}

		function& operator=(function&& other)
		{
			Base::operator=(eastl::move(other));
			return *this;
		}

		function& operator=(std::nullptr_t p) EASTL_NOEXCEPT
		{
			Base::operator=(p);
			return *this;
		}

		template <typename Functor, typename = EASTL_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(Functor, R, Args..., Base, function)>
		function& operator=(Functor&& functor)
		{
			Base::operator=(eastl::forward<Functor>(functor));
			return *this;
		}

		template <typename Functor>
		function& operator=(eastl::reference_wrapper<Functor> f) EASTL_NOEXCEPT
		{
			Base::operator=(f);
			return *this;
		}

		void swap(function& other) EASTL_NOEXCEPT
		{
			Base::swap(other);
		}

		explicit operator bool() const EASTL_NOEXCEPT
		{
			return Base::operator bool();
		}

		R operator ()(Args... args) const
		{
			return Base::operator ()(eastl::forward<Args>(args)...);
		}

	#if EASTL_RTTI_ENABLED
		const std::type_info& target_type() const EASTL_NOEXCEPT
		{
			return Base::target_type();
		}

		template <typename Functor>
		Functor* target() EASTL_NOEXCEPT
		{
			return Base::template target<Functor>();
		}

		template <typename Functor>
		const Functor* target() const EASTL_NOEXCEPT
		{
			return Base::template target<Functor>();
		}
	#endif // EASTL_RTTI_ENABLED
	};

	template <typename R, typename... Args>
	bool operator==(const function<R(Args...)>& f, std::nullptr_t) EASTL_NOEXCEPT
	{
		return !f;
	}
#if !defined(EA_COMPILER_HAS_THREE_WAY_COMPARISON)
	template <typename R, typename... Args>
	bool operator==(std::nullptr_t, const function<R(Args...)>& f) EASTL_NOEXCEPT
	{
		return !f;
	}

	template <typename R, typename... Args>
	bool operator!=(const function<R(Args...)>& f, std::nullptr_t) EASTL_NOEXCEPT
	{
		return !!f;
	}

	template <typename R, typename... Args>
	bool operator!=(std::nullptr_t, const function<R(Args...)>& f) EASTL_NOEXCEPT
	{
		return !!f;
	}
#endif
	template <typename R, typename... Args>
	void swap(function<R(Args...)>& lhs, function<R(Args...)>& rhs)
	{
		lhs.swap(rhs);
	}

#ifdef __cpp_deduction_guides
	template<typename ReturnType, typename... Args>
	function(ReturnType(*)(Args...)) -> function<ReturnType(Args...)>;

	template<typename Callable>
	function(Callable) -> function<internal::extract_signature_from_callable_t<decltype(&Callable::operator())>>;
#endif

} // namespace eastl

#endif // EASTL_FUNCTION_H
