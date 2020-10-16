/////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_ATOMIC_INTERNAL_ARCH_CMPXCHG_STRONG_H
#define EASTL_ATOMIC_INTERNAL_ARCH_CMPXCHG_STRONG_H

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif


/////////////////////////////////////////////////////////////////////////////////
//
// void EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_*_*_N(type, bool ret, type * ptr, type * expected, type desired)
//
#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_8)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_8)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_8)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_8)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_8)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_8)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_8)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_8)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_8_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_8)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_8_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_8_AVAILABLE 0
#endif


#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_16)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_16)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_16)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_16)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_16)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_16)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_16)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_16)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_16_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_16)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_16_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_16_AVAILABLE 0
#endif


#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_32)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_32)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_32)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_32)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_32)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_32)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_32)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_32)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_32_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_32)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_32_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_32_AVAILABLE 0
#endif


#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_64)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_64)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_64)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_64)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_64)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_64)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_64)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_64)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_64_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_64)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_64_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_64_AVAILABLE 0
#endif


#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_128)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_128)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_RELAXED_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_128)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_128)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_128)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_RELAXED_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_128)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_128)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_RELAXED_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_128)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_ACQUIRE_128_AVAILABLE 0
#endif

#if defined(EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_128)
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_128_AVAILABLE 1
#else
	#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_128_AVAILABLE 0
#endif


/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//
// void EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_*_N(type, bool ret, type * ptr, type * expected, type desired)
//
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_8_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_8_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_8(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_8(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_8_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_8_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_8(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_8(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_8_AVAILABLE	\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_8_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_8(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_8(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_8_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_8_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_8(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_8(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_8_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_8_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_8(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_8(type, ret, ptr, expected, desired)


#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_16_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_16_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_16(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_16(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_16_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_16_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_16(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_16(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_16_AVAILABLE	\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_16_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_16(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_16(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_16_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_16_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_16(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_16(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_16_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_16_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_16(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_16(type, ret, ptr, expected, desired)


#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_32_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_32_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_32(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_32(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_32_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_32_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_32(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_32(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_32_AVAILABLE	\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_32_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_32(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_32(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_32_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_32_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_32(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_32(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_32_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_32_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_32(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_32(type, ret, ptr, expected, desired)


#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_64_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_64_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_64(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_64(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_64_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_64_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_64(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_64(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_64_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_64_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_64(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_64(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_64_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_64_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_64(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_64(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_64_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_64_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_64(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_64(type, ret, ptr, expected, desired)


#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_128_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_128_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_128(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELAXED_RELAXED_128(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_128_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_128_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_128(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQUIRE_ACQUIRE_128(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_128_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_128_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_128(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_RELEASE_RELAXED_128(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_128_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_128_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_128(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_ACQ_REL_ACQUIRE_128(type, ret, ptr, expected, desired)

#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_128_AVAILABLE		\
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_128_AVAILABLE
#define EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_128(type, ret, ptr, expected, desired) \
	EASTL_ARCH_ATOMIC_CMPXCHG_STRONG_SEQ_CST_SEQ_CST_128(type, ret, ptr, expected, desired)


#endif /* EASTL_ATOMIC_INTERNAL_ARCH_CMPXCHG_STRONG_H */
