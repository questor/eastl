/////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_ATOMIC_INTERNAL_POINTER_H
#define EASTL_ATOMIC_INTERNAL_POINTER_H

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


	template <typename T, unsigned width = sizeof(T)>
	struct atomic_pointer_base;

#define EASTL_ATOMIC_POINTER_STATIC_ASSERT_FUNCS_IMPL(funcName)		\
	template <typename Order>										\
	T* funcName(ptrdiff_t /*arg*/, Order /*order*/) EASTL_NOEXCEPT		\
	{																\
		EASTL_ATOMIC_STATIC_ASSERT_INVALID_MEMORY_ORDER(T);			\
	}																\
																	\
	template <typename Order>										\
	T* funcName(ptrdiff_t /*arg*/, Order /*order*/) volatile EASTL_NOEXCEPT	\
	{																\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);				\
	}																\
																	\
	T* funcName(ptrdiff_t /*arg*/) volatile EASTL_NOEXCEPT				\
	{																\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);				\
	}

#define EASTL_ATOMIC_POINTER_STATIC_ASSERT_INC_DEC_OPERATOR_IMPL(operatorOp) \
	T* operator operatorOp() volatile EASTL_NOEXCEPT						\
	{																	\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);					\
	}																	\
																		\
	T* operator operatorOp(int) volatile EASTL_NOEXCEPT					\
	{																	\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);					\
	}

#define EASTL_ATOMIC_POINTER_STATIC_ASSERT_ASSIGNMENT_OPERATOR_IMPL(operatorOp) \
	T* operator operatorOp(ptrdiff_t /*arg*/) volatile EASTL_NOEXCEPT		\
	{																	\
		EASTL_ATOMIC_STATIC_ASSERT_VOLATILE_MEM_FN(T);					\
	}


	template <typename T, unsigned width>
	struct atomic_pointer_base<T*, width> : public atomic_base_width<T*, width>
	{
	private:

		using Base = atomic_base_width<T*, width>;

	public: /* ctors */

		EA_CONSTEXPR atomic_pointer_base(T* desired) EASTL_NOEXCEPT
			: Base{ desired }
		{
		}

		EA_CONSTEXPR atomic_pointer_base() EASTL_NOEXCEPT = default;

		atomic_pointer_base(const atomic_pointer_base&) EASTL_NOEXCEPT = delete;

	public: /* assignment operators */

		using Base::operator=;

		atomic_pointer_base& operator=(const atomic_pointer_base&)          EASTL_NOEXCEPT = delete;
		atomic_pointer_base& operator=(const atomic_pointer_base&) volatile EASTL_NOEXCEPT = delete;

	public: /* fetch_add */

		EASTL_ATOMIC_POINTER_STATIC_ASSERT_FUNCS_IMPL(fetch_add)

	public: /* add_fetch */

		EASTL_ATOMIC_POINTER_STATIC_ASSERT_FUNCS_IMPL(add_fetch)

	public: /* fetch_sub */

		EASTL_ATOMIC_POINTER_STATIC_ASSERT_FUNCS_IMPL(fetch_sub)

	public: /* sub_fetch */

		EASTL_ATOMIC_POINTER_STATIC_ASSERT_FUNCS_IMPL(sub_fetch)

	public: /* operator++ && operator-- */

		EASTL_ATOMIC_POINTER_STATIC_ASSERT_INC_DEC_OPERATOR_IMPL(++)

		EASTL_ATOMIC_POINTER_STATIC_ASSERT_INC_DEC_OPERATOR_IMPL(--)

	public: /* operator+= && operator-= */

		EASTL_ATOMIC_POINTER_STATIC_ASSERT_ASSIGNMENT_OPERATOR_IMPL(+=)

		EASTL_ATOMIC_POINTER_STATIC_ASSERT_ASSIGNMENT_OPERATOR_IMPL(-=)

	};


	template <typename T, unsigned width = sizeof(T)>
	struct atomic_pointer_width;

#define EASTL_ATOMIC_POINTER_FUNC_IMPL(op, bits)						\
	T* retVal;															\
	{																	\
		ptr_integral_type retType;										\
		ptr_integral_type addend = static_cast<ptr_integral_type>(arg) * static_cast<ptr_integral_type>(sizeof(T)); \
																		\
		EASTL_PREPROCESSOR_JOIN(op, bits)(ptr_integral_type, retType, EASTL_ATOMIC_INTEGRAL_CAST(ptr_integral_type, this->GetAtomicAddress()), addend); \
																		\
		retVal = reinterpret_cast<T*>(retType);							\
	}																	\
	return retVal;

#define EASTL_ATOMIC_POINTER_FETCH_IMPL(funcName, op, bits)		\
	T* funcName(ptrdiff_t arg) EASTL_NOEXCEPT						\
	{															\
		EASTL_ATOMIC_STATIC_ASSERT_TYPE_IS_OBJECT(T);			\
		EASTL_ATOMIC_POINTER_FUNC_IMPL(op, bits);				\
	}

#define EASTL_ATOMIC_POINTER_FETCH_ORDER_IMPL(funcName, orderType, op, bits) \
	T* funcName(ptrdiff_t arg, orderType) EASTL_NOEXCEPT					\
	{																	\
		EASTL_ATOMIC_STATIC_ASSERT_TYPE_IS_OBJECT(T);					\
		EASTL_ATOMIC_POINTER_FUNC_IMPL(op, bits);						\
	}

#define EASTL_ATOMIC_POINTER_FETCH_OP_JOIN(fetchOp, Order)				\
	EASTL_PREPROCESSOR_JOIN(EASTL_PREPROCESSOR_JOIN(EASTL_ATOMIC_, fetchOp), Order)

#define EASTL_ATOMIC_POINTER_FETCH_FUNCS_IMPL(funcName, fetchOp, bits)	\
	using Base::funcName;												\
																		\
	EASTL_ATOMIC_POINTER_FETCH_IMPL(funcName, EASTL_ATOMIC_POINTER_FETCH_OP_JOIN(fetchOp, _SEQ_CST_), bits) \
																		\
	EASTL_ATOMIC_POINTER_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_relaxed_s, \
										  EASTL_ATOMIC_POINTER_FETCH_OP_JOIN(fetchOp, _RELAXED_), bits) \
																		\
	EASTL_ATOMIC_POINTER_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_acquire_s, \
										  EASTL_ATOMIC_POINTER_FETCH_OP_JOIN(fetchOp, _ACQUIRE_), bits) \
																		\
	EASTL_ATOMIC_POINTER_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_release_s, \
										  EASTL_ATOMIC_POINTER_FETCH_OP_JOIN(fetchOp, _RELEASE_), bits) \
																		\
	EASTL_ATOMIC_POINTER_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_acq_rel_s, \
										  EASTL_ATOMIC_POINTER_FETCH_OP_JOIN(fetchOp, _ACQ_REL_), bits) \
																		\
	EASTL_ATOMIC_POINTER_FETCH_ORDER_IMPL(funcName, eastl::internal::memory_order_seq_cst_s, \
										  EASTL_ATOMIC_POINTER_FETCH_OP_JOIN(fetchOp, _SEQ_CST_), bits)

#define EASTL_ATOMIC_POINTER_FETCH_INC_DEC_OPERATOR_IMPL(operatorOp, preFuncName, postFuncName) \
	using Base::operator operatorOp;									\
																		\
	T* operator operatorOp() EASTL_NOEXCEPT								\
	{																	\
		return preFuncName(1, eastl::memory_order_seq_cst);				\
	}																	\
																		\
	T* operator operatorOp(int) EASTL_NOEXCEPT								\
	{																	\
		return postFuncName(1, eastl::memory_order_seq_cst);			\
	}

#define EASTL_ATOMIC_POINTER_FETCH_ASSIGNMENT_OPERATOR_IMPL(operatorOp, funcName) \
	using Base::operator operatorOp;									\
																		\
	T* operator operatorOp(ptrdiff_t arg) EASTL_NOEXCEPT					\
	{																	\
		return funcName(arg, eastl::memory_order_seq_cst);				\
	}


#define EASTL_ATOMIC_POINTER_WIDTH_SPECIALIZE(bytes, bits)				\
	template <typename T>												\
	struct atomic_pointer_width<T*, bytes> : public atomic_pointer_base<T*, bytes> \
	{																	\
	private:															\
																		\
		using Base = atomic_pointer_base<T*, bytes>;					\
		using u_ptr_integral_type = EASTL_PREPROCESSOR_JOIN(EASTL_PREPROCESSOR_JOIN(uint, bits), _t); \
		using ptr_integral_type = EASTL_PREPROCESSOR_JOIN(EASTL_PREPROCESSOR_JOIN(int, bits), _t); \
																		\
	public: /* ctors */													\
																		\
		EA_CONSTEXPR atomic_pointer_width(T* desired) EASTL_NOEXCEPT		\
			: Base{ desired }											\
		{																\
		}																\
																		\
		EA_CONSTEXPR atomic_pointer_width() EASTL_NOEXCEPT = default;		\
																		\
		atomic_pointer_width(const atomic_pointer_width&) EASTL_NOEXCEPT = delete; \
																		\
	public: /* assignment operators */									\
																		\
		using Base::operator=;											\
																		\
		atomic_pointer_width& operator=(const atomic_pointer_width&)          EASTL_NOEXCEPT = delete; \
		atomic_pointer_width& operator=(const atomic_pointer_width&) volatile EASTL_NOEXCEPT = delete; \
																		\
	public: /* fetch_add */												\
																		\
		EASTL_ATOMIC_POINTER_FETCH_FUNCS_IMPL(fetch_add, FETCH_ADD, bits) \
																		\
	public: /* add_fetch */												\
																		\
		EASTL_ATOMIC_POINTER_FETCH_FUNCS_IMPL(add_fetch, ADD_FETCH, bits) \
																		\
	public: /* fetch_sub */												\
																		\
		EASTL_ATOMIC_POINTER_FETCH_FUNCS_IMPL(fetch_sub, FETCH_SUB, bits) \
																		\
	public: /* sub_fetch */												\
																		\
		EASTL_ATOMIC_POINTER_FETCH_FUNCS_IMPL(sub_fetch, SUB_FETCH, bits) \
																		\
	public: /* operator++ && operator-- */								\
																		\
		EASTL_ATOMIC_POINTER_FETCH_INC_DEC_OPERATOR_IMPL(++, add_fetch, fetch_add) \
																		\
		EASTL_ATOMIC_POINTER_FETCH_INC_DEC_OPERATOR_IMPL(--, sub_fetch, fetch_sub) \
																		\
	public: /* operator+= && operator-= */								\
																		\
		EASTL_ATOMIC_POINTER_FETCH_ASSIGNMENT_OPERATOR_IMPL(+=, add_fetch) \
																		\
		EASTL_ATOMIC_POINTER_FETCH_ASSIGNMENT_OPERATOR_IMPL(-=, sub_fetch) \
																		\
	public:																\
																		\
		using Base::load;												\
																		\
		T* load(eastl::internal::memory_order_read_depends_s) EASTL_NOEXCEPT \
		{																\
			T* retPointer;												\
			EASTL_PREPROCESSOR_JOIN(EASTL_ATOMIC_LOAD_READ_DEPENDS_, bits)(T*, retPointer, this->GetAtomicAddress()); \
			return retPointer;											\
		}																\
	};


#if defined(EASTL_ATOMIC_HAS_32BIT) && EA_PLATFORM_PTR_SIZE == 4
	EASTL_ATOMIC_POINTER_WIDTH_SPECIALIZE(4, 32)
#endif

#if defined(EASTL_ATOMIC_HAS_64BIT) && EA_PLATFORM_PTR_SIZE == 8
	EASTL_ATOMIC_POINTER_WIDTH_SPECIALIZE(8, 64)
#endif

EA_RESTORE_VC_WARNING();

EA_RESTORE_CLANG_WARNING();


} // namespace internal


} // namespace eastl

#endif /* EASTL_ATOMIC_INTERNAL_POINTER_H */
