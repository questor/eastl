/////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_ATOMIC_INTERNAL_ATOMIC_MACROS_H
#define EASTL_ATOMIC_INTERNAL_ATOMIC_MACROS_H

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif


#include "atomic_macros_base.h"

#include "atomic_macros_fetch_add.h"
#include "atomic_macros_fetch_sub.h"

#include "atomic_macros_fetch_and.h"
#include "atomic_macros_fetch_xor.h"
#include "atomic_macros_fetch_or.h"

#include "atomic_macros_add_fetch.h"
#include "atomic_macros_sub_fetch.h"

#include "atomic_macros_and_fetch.h"
#include "atomic_macros_xor_fetch.h"
#include "atomic_macros_or_fetch.h"

#include "atomic_macros_exchange.h"

#include "atomic_macros_cmpxchg_weak.h"
#include "atomic_macros_cmpxchg_strong.h"

#include "atomic_macros_load.h"
#include "atomic_macros_store.h"

#include "atomic_macros_compiler_barrier.h"

#include "atomic_macros_cpu_pause.h"

#include "atomic_macros_memory_barrier.h"

#include "atomic_macros_signal_fence.h"

#include "atomic_macros_thread_fence.h"


/////////////////////////////////////////////////////////////////////////////////


#if defined(EASTL_COMPILER_ATOMIC_HAS_8BIT) || defined(EASTL_ARCH_ATOMIC_HAS_8BIT)

	#define EASTL_ATOMIC_HAS_8BIT

#endif


#if defined(EASTL_COMPILER_ATOMIC_HAS_16BIT) || defined(EASTL_ARCH_ATOMIC_HAS_16BIT)

	#define EASTL_ATOMIC_HAS_16BIT

#endif


#if defined(EASTL_COMPILER_ATOMIC_HAS_32BIT) || defined(EASTL_ARCH_ATOMIC_HAS_32BIT)

	#define EASTL_ATOMIC_HAS_32BIT

#endif


#if defined(EASTL_COMPILER_ATOMIC_HAS_64BIT) || defined(EASTL_ARCH_ATOMIC_HAS_64BIT)

	#define EASTL_ATOMIC_HAS_64BIT

#endif


#if defined(EASTL_COMPILER_ATOMIC_HAS_128BIT) || defined(EASTL_ARCH_ATOMIC_HAS_128BIT)

	#define EASTL_ATOMIC_HAS_128BIT

#endif


#endif /* EASTL_ATOMIC_INTERNAL_ATOMIC_MACROS_H */
