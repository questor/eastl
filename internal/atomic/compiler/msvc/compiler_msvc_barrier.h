/////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_ATOMIC_INTERNAL_COMPILER_MSVC_BARRIER_H
#define EASTL_ATOMIC_INTERNAL_COMPILER_MSVC_BARRIER_H

#if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif


/////////////////////////////////////////////////////////////////////////////////
//
// void EASTL_COMPILER_ATOMIC_COMPILER_BARRIER()
//
#define EASTL_COMPILER_ATOMIC_COMPILER_BARRIER()	\
	EA_DISABLE_CLANG_WARNING(-Wdeprecated-declarations)		\
	_ReadWriteBarrier()										\
	EA_RESTORE_CLANG_WARNING()


/////////////////////////////////////////////////////////////////////////////////
//
// void EASTL_COMPILER_ATOMIC_COMPILER_BARRIER_DATA_DEPENDENCY(const T&, type)
//
#define EASTL_COMPILER_ATOMIC_COMPILER_BARRIER_DATA_DEPENDENCY(val, type) \
	EASTL_COMPILER_ATOMIC_COMPILER_BARRIER_DATA_DEPENDENCY_FUNC(const_cast<type*>(eastl::addressof((val)))); \
	EASTL_ATOMIC_COMPILER_BARRIER()


#endif /* EASTL_ATOMIC_INTERNAL_COMPILER_MSVC_BARRIER_H */
