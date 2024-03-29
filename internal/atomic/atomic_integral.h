/////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_ATOMIC_INTERNAL_INTEGRAL_H
#define EASTL_ATOMIC_INTERNAL_INTEGRAL_H

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif


namespace eastl
{


namespace internal
{


// 'class' : multiple assignment operators specified
EA_DISABLE_VC_WARNING(4522);

// misaligned atomic operation may incur significant performance penalty
// The above warning is emitted in earlier versions of clang incorrectly.
// All eastl::atomic<T> objects are size aligned.
// This is static and runtime asserted.
// Thus we disable this warning.
EA_DISABLE_CLANG_WARNING(-Watomic-alignment);


#define EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(funcName)	\
	template <typename Order>										\
	T funcName(T /*arg*/, Order /*order*/) EASTL_NOEXCEPT				\
	{																\
		EASTL_ATOMIC_STATIC_ASSERT_INVALID_MEMORY_ORDER(T);			\
	}																\
																	\
	template <typename Order>										\
	T funcName(T /*arg*/, Order /*order*/) volatile EASTL_NOEXCEPT		\
	{																\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);				\
	}																\
																	\
	T funcName(T /*arg*/) volatile EASTL_NOEXCEPT						\
	{																\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);				\
	}


#define EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_INC_DEC_OPERATOR_IMPL(operatorOp) \
	T operator operatorOp() volatile EASTL_NOEXCEPT						\
	{																	\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);					\
	}																	\
																		\
	T operator operatorOp(int) volatile EASTL_NOEXCEPT						\
	{																	\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);					\
	}


#define EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_ASSIGNMENT_OPERATOR_IMPL(operatorOp) \
	T operator operatorOp(T /*arg*/) volatile EASTL_NOEXCEPT				\
	{																	\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);					\
	}


	template <typename T, unsigned width = sizeof(T)>
	struct atomic_integral_base : public atomic_base_width<T, width>
	{
	private:

		using Base = atomic_base_width<T, width>;

	public: /* ctors */

		EA_CONSTEXPR atomic_integral_base(T desired) EASTL_NOEXCEPT
			: Base{ desired }
		{
		}

		EA_CONSTEXPR atomic_integral_base() EASTL_NOEXCEPT = default;

		atomic_integral_base(const atomic_integral_base&) EASTL_NOEXCEPT = delete;

	public: /* assignment operator */

		using Base::operator=;

		atomic_integral_base& operator=(const atomic_integral_base&)          EASTL_NOEXCEPT = delete;
		atomic_integral_base& operator=(const atomic_integral_base&) volatile EASTL_NOEXCEPT = delete;

	public: /* fetch_add */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(fetch_add)

	public: /* add_fetch */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(add_fetch)

	public: /* fetch_sub */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(fetch_sub)

	public: /* sub_fetch */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(sub_fetch)

	public: /* fetch_and */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(fetch_and)

	public: /* and_fetch */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(and_fetch)

	public: /* fetch_or */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(fetch_or)

	public: /* or_fetch */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(or_fetch)

	public: /* fetch_xor */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(fetch_xor)

	public: /* xor_fetch */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_FUNCS_IMPL(xor_fetch)

	public: /* operator++ && operator-- */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_INC_DEC_OPERATOR_IMPL(++)

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_INC_DEC_OPERATOR_IMPL(--)

	public: /* operator+= && operator-= */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_ASSIGNMENT_OPERATOR_IMPL(+=)

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_ASSIGNMENT_OPERATOR_IMPL(-=)

	public: /* operator&= */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_ASSIGNMENT_OPERATOR_IMPL(&=)

	public: /* operator|= */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_ASSIGNMENT_OPERATOR_IMPL(|=)

	public: /* operator^= */

		EASTL_ATOMIC_INTEGRAL_STATIC_ASSERT_ASSIGNMENT_OPERATOR_IMPL(^=)

	};


	template <typename T, unsigned width = sizeof(T)>
	struct atomic_integral_width;

#define EASTL_ATOMIC_INTEGRAL_FUNC_IMPL(op, bits)						\
	EASTL_ATOMIC_DEFAULT_INIT(T, retVal);	             				\
	EASTL_PREPROCESSOR_JOIN(op, bits)(T, retVal, this->GetAtomicAddress(), arg); \
	return retVal;

#define EASTL_ATOMIC_INTEGRAL_FETCH_IMPL(funcName, op, bits)	\
	T funcName(T arg) EASTL_NOEXCEPT								\
	{															\
		EASTL_ATOMIC_INTEGRAL_FUNC_IMPL(op, bits);				\
	}

#define EASTL_ATOMIC_INTEGRAL_FETCH_ORDER_IMPL(funcName, orderType, op, bits) \
	T funcName(T arg, orderType) EASTL_NOEXCEPT							\
	{																	\
		EASTL_ATOMIC_INTEGRAL_FUNC_IMPL(op, bits);						\
	}

#define EASTL_ATOMIC_INTEGRAL_FETCH_OP_JOIN(fetchOp, Order)				\
	EASTL_PREPROCESSOR_JOIN(EASTL_PREPROCESSOR_JOIN(EASTL_ATOMIC_, fetchOp), Order)

#define EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(funcName, fetchOp, bits) \
	using Base::funcName;												\
																		\
	EASTL_ATOMIC_INTEGRAL_FETCH_IMPL(funcName, EASTL_ATOMIC_INTEGRAL_FETCH_OP_JOIN(fetchOp, _SEQ_CST_), bits) \
																		\
	EASTL_ATOMIC_INTEGRAL_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_relaxed_s, \
										   EASTL_ATOMIC_INTEGRAL_FETCH_OP_JOIN(fetchOp, _RELAXED_), bits) \
																		\
	EASTL_ATOMIC_INTEGRAL_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_acquire_s, \
										   EASTL_ATOMIC_INTEGRAL_FETCH_OP_JOIN(fetchOp, _ACQUIRE_), bits) \
																		\
	EASTL_ATOMIC_INTEGRAL_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_release_s, \
										   EASTL_ATOMIC_INTEGRAL_FETCH_OP_JOIN(fetchOp, _RELEASE_), bits) \
																		\
	EASTL_ATOMIC_INTEGRAL_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_acq_rel_s, \
										   EASTL_ATOMIC_INTEGRAL_FETCH_OP_JOIN(fetchOp, _ACQ_REL_), bits) \
																		\
	EASTL_ATOMIC_INTEGRAL_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_seq_cst_s, \
										   EASTL_ATOMIC_INTEGRAL_FETCH_OP_JOIN(fetchOp, _SEQ_CST_), bits)

#define EASTL_ATOMIC_INTEGRAL_FETCH_INC_DEC_OPERATOR_IMPL(operatorOp, preFuncName, postFuncName) \
	using Base::operator operatorOp;									\
																		\
	T operator operatorOp() EASTL_NOEXCEPT									\
	{																	\
		return preFuncName(1, eastl::memory_order_seq_cst);				\
	}																	\
																		\
	T operator operatorOp(int) EASTL_NOEXCEPT								\
	{																	\
		return postFuncName(1, eastl::memory_order_seq_cst);			\
	}

#define EASTL_ATOMIC_INTEGRAL_FETCH_ASSIGNMENT_OPERATOR_IMPL(operatorOp, funcName) \
	using Base::operator operatorOp;									\
																		\
	T operator operatorOp(T arg) EASTL_NOEXCEPT							\
	{																	\
		return funcName(arg, eastl::memory_order_seq_cst);				\
	}


#define EASTL_ATOMIC_INTEGRAL_WIDTH_SPECIALIZE(bytes, bits)				\
	template <typename T>												\
	struct atomic_integral_width<T, bytes> : public atomic_integral_base<T, bytes> \
	{																	\
	private:															\
																		\
		using Base = atomic_integral_base<T, bytes>;					\
																		\
	public: /* ctors */													\
																		\
		EA_CONSTEXPR atomic_integral_width(T desired) EASTL_NOEXCEPT		\
			: Base{ desired }											\
		{																\
		}																\
																		\
		EA_CONSTEXPR atomic_integral_width() EASTL_NOEXCEPT = default;		\
																		\
		atomic_integral_width(const atomic_integral_width&) EASTL_NOEXCEPT = delete; \
																		\
	public: /* assignment operator */									\
																		\
		using Base::operator=;											\
																		\
		atomic_integral_width& operator=(const atomic_integral_width&)          EASTL_NOEXCEPT = delete; \
		atomic_integral_width& operator=(const atomic_integral_width&) volatile EASTL_NOEXCEPT = delete; \
																		\
	public: /* fetch_add */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(fetch_add, FETCH_ADD, bits) \
																		\
	public: /* add_fetch */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(add_fetch, ADD_FETCH, bits) \
																		\
	public: /* fetch_sub */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(fetch_sub, FETCH_SUB, bits) \
																		\
	public: /* sub_fetch */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(sub_fetch, SUB_FETCH, bits) \
																		\
	public: /* fetch_and */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(fetch_and, FETCH_AND, bits) \
																		\
	public: /* and_fetch */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(and_fetch, AND_FETCH, bits) \
																		\
	public: /* fetch_or */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(fetch_or, FETCH_OR, bits) \
																		\
	public: /* or_fetch */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(or_fetch, OR_FETCH, bits) \
																		\
	public: /* fetch_xor */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(fetch_xor, FETCH_XOR, bits) \
																		\
	public: /* xor_fetch */												\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_FUNCS_IMPL(xor_fetch, XOR_FETCH, bits) \
																		\
	public: /* operator++ && operator-- */								\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_INC_DEC_OPERATOR_IMPL(++, add_fetch, fetch_add) \
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_INC_DEC_OPERATOR_IMPL(--, sub_fetch, fetch_sub) \
																		\
	public: /* operator+= && operator-= */								\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_ASSIGNMENT_OPERATOR_IMPL(+=, add_fetch) \
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_ASSIGNMENT_OPERATOR_IMPL(-=, sub_fetch) \
																		\
	public: /* operator&= */											\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_ASSIGNMENT_OPERATOR_IMPL(&=, and_fetch) \
																		\
	public: /* operator|= */											\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_ASSIGNMENT_OPERATOR_IMPL(|=, or_fetch) \
																		\
	public: /* operator^= */											\
																		\
		EASTL_ATOMIC_INTEGRAL_FETCH_ASSIGNMENT_OPERATOR_IMPL(^=, xor_fetch) \
																		\
	};


#if defined(EASTL_ATOMIC_HAS_8BIT)
	EASTL_ATOMIC_INTEGRAL_WIDTH_SPECIALIZE(1, 8)
#endif

#if defined(EASTL_ATOMIC_HAS_16BIT)
	EASTL_ATOMIC_INTEGRAL_WIDTH_SPECIALIZE(2, 16)
#endif

#if defined(EASTL_ATOMIC_HAS_32BIT)
	EASTL_ATOMIC_INTEGRAL_WIDTH_SPECIALIZE(4, 32)
#endif

#if defined(EASTL_ATOMIC_HAS_64BIT)
	EASTL_ATOMIC_INTEGRAL_WIDTH_SPECIALIZE(8, 64)
#endif

#if defined(EASTL_ATOMIC_HAS_128BIT)
	EASTL_ATOMIC_INTEGRAL_WIDTH_SPECIALIZE(16, 128)
#endif

EA_RESTORE_VC_WARNING();

EA_RESTORE_CLANG_WARNING();


} // namespace internal


} // namespace eastl

#endif /* EASTL_ATOMIC_INTERNAL_INTEGRAL_H */
