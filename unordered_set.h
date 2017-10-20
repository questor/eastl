///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef EASTL_UNORDERED_SET_H
#define EASTL_UNORDERED_SET_H

#include <eastl/internal/config.h>
#include <eastl/hash_set.h>

EA_ONCE()

namespace eastl
{

	/// unordered_set 
	///
	/// The original TR1 (technical report 1) used "hashSet" to name a hash
	/// table backed associative container of unique "Key" type objects.  When
	/// the container was added to the C++11 standard the committee chose the
	/// name "unordered_set" to clarify that internally the elements are NOT
	/// sorted in any particular order.  We provide a template alias here to
	/// ensure feature parity with the original eastl::hashSet.
	///
	#if !defined(EA_COMPILER_NO_TEMPLATE_ALIASES)
		template <typename Value,
				  typename Hash = eastl::hash<Value>,
				  typename Predicate = eastl::equal_to<Value>,
				  typename Allocator = EASTLAllocatorType,
				  bool bCacheHashCode = false>
		using unordered_set = hashSet<Value, Hash, Predicate, Allocator, bCacheHashCode>;
	#endif

    /// unordered_multiset 
	///
	/// Similar template alias as "unordered_set" except the contained elements
	/// need not be unique. See "hashMultiset" for more details. 
	///
	#if !defined(EA_COMPILER_NO_TEMPLATE_ALIASES)
		template <typename Value,
				  typename Hash = eastl::hash<Value>,
				  typename Predicate = eastl::equal_to<Value>,
				  typename Allocator = EASTLAllocatorType,
				  bool bCacheHashCode = false>
		using unordered_multiset = hashMultiset<Value, Hash, Predicate, Allocator, bCacheHashCode>;
	#endif

} // namespace eastl

#endif // Header include guard

