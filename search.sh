#!/bin/sh

grep -RiIl 'EA_ALIGN_OF' | xargs sed -i 's/EA_ALIGN_OF/EASTL_ALIGN_OF/g'
grep -RiIl 'EA_ALIGN' | xargs sed -i 's/EA_ALIGN/EASTL_ALIGN/g'
grep -RiIl 'EA_PACKED' | xargs sed -i 's/EA_PACKED/EASTL_PACKED/g'
grep -RiIl 'EA_LIKELY' | xargs sed -i 's/EA_LIKELY/EASTL_LIKELY/g'
grep -RiIl 'EA_UNLIKELY' | xargs sed -i 's/EA_UNLIKELY/EASTL_UNLIKELY/g'
grep -RiIl 'EA_INIT_PRIORITY' | xargs sed -i 's/EA_INIT_PRIORITY/EASTL_INIT_PRIORITY/g'
grep -RiIl 'EA_MAY_ALIAS' | xargs sed -i 's/EA_MAY_ALIAS/EASTL_MAY_ALIAS/g'
grep -RiIl 'EA_ASSUME' | xargs sed -i 's/EA_ASSUME/EASTL_ASSUME/g'
grep -RiIl 'EA_PURE' | xargs sed -i 's/EA_PURE/EASTL_PURE/g'
grep -RiIl 'EA_WEAK' | xargs sed -i 's/EA_WEAK/EASTL_WEAK/g'
grep -RiIl 'EA_WCHAR_T_NON_NATIVE' | xargs sed -i 's/EA_WCHAR_T_NON_NATIVE/EASTL_WCHAR_T_NON_NATIVE/g'
grep -RiIl 'EA_WCHAR_SIZE' | xargs sed -i 's/EA_WCHAR_SIZE/EASTL_WCHAR_SIZE/g'
grep -RiIl 'EA_RESTRICT' | xargs sed -i 's/EA_RESTRICT/EASTL_RESTRICT/g'
grep -RiIl 'EA_DEPRECATED' | xargs sed -i 's/EA_DEPRECATED/EASTL_DEPRECATED/g'
grep -RiIl 'EA_PREFIX_' | xargs sed -i 's/EA_PREFIX_/EASTL_PREFIX_/g'
grep -RiIl 'EA_POSTFIX_' | xargs sed -i 's/EA_POSTFIX_/EASTL_POSTFIX_/g'
grep -RiIl 'EA_FORCE_INLINE' | xargs sed -i 's/EA_FORCE_INLINE/EASTL_FORCE_INLINE/g'
grep -RiIl 'EA_NO_INLINE' | xargs sed -i 's/EA_NO_INLINE/EASTL_NO_INLINE/g'
grep -RiIl 'EA_NO_VTABLE' | xargs sed -i 's/EA_NO_VTABLE/EASTL_NO_VTABLE/g'
grep -RiIl 'EA_CLASS_NO_VTABLE' | xargs sed -i 's/EA_CLASS_NO_VTABLE/EASTL_CLASS_NO_VTABLE/g'
grep -RiIl 'EA_STRUCT_NO_VTABLE' | xargs sed -i 's/EA_STRUCT_NO_VTABLE/EASTL_STRUCT_NO_VTABLE/g'
grep -RiIl 'EA_PASCAL' | xargs sed -i 's/EA_PASCAL/EASTL_PASCAL/g'
grep -RiIl 'EA_SSE' | xargs sed -i 's/EA_SSE/EASTL_SSE/g'
grep -RiIl 'EA_IMPORT' | xargs sed -i 's/EA_IMPORT/EASTL_IMPORT/g'
grep -RiIl 'EA_EXPORT' | xargs sed -i 's/EA_EXPORT/EASTL_EXPORT/g'
grep -RiIl 'EA_PRAGMA_ONCE_SUPPORTED' | xargs sed -i 's/EA_PRAGMA_ONCE_SUPPORTED/EASTL_PRAGMA_ONCE_SUPPORTED/g'
grep -RiIl 'EA_OVERRIDE' | xargs sed -i 's/EA_OVERRIDE/EASTL_OVERRIDE/g'
grep -RiIl 'EA_SEALED' | xargs sed -i 's/EA_SEALED/EASTL_SEALED/g'
grep -RiIl 'EA_ABSTRACT' | xargs sed -i 's/EA_ABSTRACT/EASTL_ABSTRACT/g'
grep -RiIl 'EA_NOEXCEPT' | xargs sed -i 's/EA_NOEXCEPT/EASTL_NOEXCEPT/g'
grep -RiIl 'EA_CHAR16_NATIVE' | xargs sed -i 's/EA_CHAR16_NATIVE/EASTL_CHAR16_NATIVE/g'
grep -RiIl 'EA_CHAR32_NATIVE' | xargs sed -i 's/EA_CHAR32_NATIVE/EASTL_CHAR32_NATIVE/g'
grep -RiIl 'EA_CHAR16' | xargs sed -i 's/EA_CHAR16/EASTL_CHAR16/g'
grep -RiIl 'EA_CHAR32' | xargs sed -i 's/EA_CHAR32/EASTL_CHAR32/g'
grep -RiIl 'EAArrayCount' | xargs sed -i 's/EAArrayCount/EASTLArrayCount/g'
grep -RiIl 'EA_PREPROCESSOR_JOIN' | xargs sed -i 's/EA_PREPROCESSOR_JOIN/EASTL_PREPROCESSOR_JOIN/g'
grep -RiIl 'EA_COMPILER_CPP11_ENABLED' | xargs sed -i 's/EA_COMPILER_CPP11_ENABLED/EASTL_COMPILER_CPP11_ENABLED/g'

REM now change function names...
grep -RiIl 'push_back_uninitialized' | xargs sed -i 's/push_back_uninitialized/pushBackUninitialized/g'
grep -RiIl 'push_front_uninitialized' | xargs sed -i 's/push_front_uninitialized/pushFrontUninitialized/g'
grep -RiIl 'push_front' | xargs sed -i 's/push_front/pushFront/g'
grep -RiIl 'push_back' | xargs sed -i 's/push_back/pushBack/g'
grep -RiIl 'set_capacity' | xargs sed -i 's/set_capacity/setCapacity/g'
grep -RiIl 'pop_back' | xargs sed -i 's/pop_back/popBack/g'
grep -RiIl 'pop_front' | xargs sed -i 's/pop_front/popFront/g'
grep -RiIl 'remove_if' | xargs sed -i 's/remove_if/removeIf/g'
grep -RiIl 'validate_iterator' | xargs sed -i 's/validate_iterator/validateIterator/g'
grep -RiIl 'push_heap' | xargs sed -i 's/push_heap/pushHeap/g'
grep -RiIl 'pop_heap' | xargs sed -i 's/pop_heap/popHeap/g'
grep -RiIl 'make_heap' | xargs sed -i 's/make_heap/makeHeap/g'
grep -RiIl 'sort_heap' | xargs sed -i 's/sort_heap/sortHeap/g'
grep -RiIl 'remove_heap' | xargs sed -i 's/remove_heap/removeHeap/g'
grep -RiIl 'change_heap' | xargs sed -i 's/change_heap/changeHeap/g'
grep -RiIl 'is_heap' | xargs sed -i 's/is_heap/isHeap/g'
grep -RiIl 'get_default_allocator' | xargs sed -i 's/get_default_allocator/getDefaultAllocator/g'
grep -RiIl 'get_name' | xargs sed -i 's/get_name/getName/g'
grep -RiIl 'set_name' | xargs sed -i 's/set_name/setName/g'
grep -RiIl 'max_alt' | xargs sed -i 's/max_alt/maxAlt/g'
grep -RiIl 'min_alt' | xargs sed -i 's/min_alt/minAlt/g'
grep -RiIl 'min_element' | xargs sed -i 's/min_element/minElement/g'
grep -RiIl 'max_element' | xargs sed -i 's/max_element/maxElement/g'
grep -RiIl 'iter_swap' | xargs sed -i 's/iter_swap/iterSwap/g'
grep -RiIl 'swap_ranges' | xargs sed -i 's/swap_ranges/swapRanges/g'
grep -RiIl 'adjacent_find' | xargs sed -i 's/adjacent_find/adjacentFind/g'
grep -RiIl 'copy_backward' | xargs sed -i 's/copy_backward/copyBackward/g'
grep -RiIl 'count_if' | xargs sed -i 's/count_if/countIf/g'
grep -RiIl 'fill_n' | xargs sed -i 's/fill_n/fillN/g'
grep -RiIl 'find_if' | xargs sed -i 's/find_if/findIf/g'
grep -RiIl 'find_first_of' | xargs sed -i 's/find_first_of/findFirstOf/g'
grep -RiIl 'find_first_not_of' | xargs sed -i 's/find_first_not_of/findFirstNotOf/g'
grep -RiIl 'for_each' | xargs sed -i 's/for_each/forEach/g'
grep -RiIl 'generate_n' | xargs sed -i 's/generate_n/generateN/g'
grep -RiIl 'lexicographical_compare' | xargs sed -i 's/lexicographical_compare/lexicographicalCompare/g'
grep -RiIl 'lower_bound' | xargs sed -i 's/lower_bound/lowerBound/g'
grep -RiIl 'upper_bound' | xargs sed -i 's/upper_bound/upperBound/g'
grep -RiIl 'equal_range' | xargs sed -i 's/equal_range/equalRange/g'
grep -RiIl 'replace_if' | xargs sed -i 's/replace_if/replaceIf/g'
grep -RiIl 'remove_copy_if' | xargs sed -i 's/remove_copy_if/removeCopyIf/g'
grep -RiIl 'remove_copy' | xargs sed -i 's/remove_copy/removeCopy/g'
grep -RiIl 'replace_copy_if' | xargs sed -i 's/replace_copy_if/replaceCopyIf/g'
grep -RiIl 'replace_copy' | xargs sed -i 's/replace_copy/replaceCopy/g'
grep -RiIl 'reverse_copy' | xargs sed -i 's/reverse_copy/reverseCopy/g'
grep -RiIl 'search_n' | xargs sed -i 's/search_n/searchN/g'
grep -RiIl 'binary_search_i' | xargs sed -i 's/binary_search_i/binarySearchI/g'
grep -RiIl 'binary_search' | xargs sed -i 's/binary_search/binarySearch/g'
grep -RiIl 'find_end' | xargs sed -i 's/find_end/findEnd/g'
grep -RiIl 'set_difference' | xargs sed -i 's/set_difference/setDifference/g'
grep -RiIl 'find_first' | xargs sed -i 's/find_first/findFirst/g'
grep -RiIl 'find_next' | xargs sed -i 's/find_next/findNext/g'
grep -RiIl 'find_last' | xargs sed -i 's/find_last/findLast/g'
grep -RiIl 'find_prev' | xargs sed -i 's/find_prev/findPrev/g'
grep -RiIl 'get_allocator' | xargs sed -i 's/get_allocator/getAllocator/g'
grep -RiIl 'set_allocator' | xargs sed -i 's/set_allocator/setAllocator/g'
grep -RiIl 'get_flags' | xargs sed -i 's/get_flags/getFlags/g'
grep -RiIl 'set_flags' | xargs sed -i 's/set_flags/setFlags/g'
grep -RiIl 'to_ulong' | xargs sed -i 's/to_ulong/toUlong/g'
grep -RiIl 'fixed_allocator_with_overflow' | xargs sed -i 's/fixed_allocator_with_overflow/fixedAllocatorWithOverflow/g'
grep -RiIl 'fixed_allocator' | xargs sed -i 's/fixed_allocator/fixedAllocator/g'
grep -RiIl 'fixed_hash_map' | xargs sed -i 's/fixed_hash_map/fixedHashMap/g'
grep -RiIl 'fixed_hashtable_allocator' | xargs sed -i 's/fixed_hashtable_allocator/fixedHashtableAllocator/g'
grep -RiIl 'max_size' | xargs sed -i 's/max_size/maxSize/g'
grep -RiIl 'fixed_hash_multimap' | xargs sed -i 's/fixed_hash_multimap/fixedHashMultimap/g'
grep -RiIl 'hash_multimap' | xargs sed -i 's/hash_multimap/hashMultimap/g'
grep -RiIl 'fixed_swap' | xargs sed -i 's/fixed_swap/fixedSwap/g'
grep -RiIl 'hash_map' | xargs sed -i 's/hash_map/hashMap/g'
grep -RiIl 'EASTL/hashMap.h EASTL/hash_map' | xargs sed -i 's/hash_map/h/g'
grep -RiIl 'fixed_hash_set' | xargs sed -i 's/fixed_hash_set/fixedHashSet/g'
grep -RiIl 'hash_set' | xargs sed -i 's/hash_set/hashSet/g'
grep -RiIl 'EASTL/hashSet.h EASTL/hash_set' | xargs sed -i 's/hash_set/h/g'
grep -RiIl 'fixed_hash_multiset' | xargs sed -i 's/fixed_hash_multiset/fixedHashMultiset/g'
grep -RiIl 'hash_multiset' | xargs sed -i 's/hash_multiset/hashMultiset/g'
grep -RiIl 'fixed_list' | xargs sed -i 's/fixed_list/fixedList/g'
grep -RiIl 'has_overflowed' | xargs sed -i 's/has_overflowed/hasOverflowed/g'
grep -RiIl 'fixed_map' | xargs sed -i 's/fixed_map/fixedMap/g'
grep -RiIl 'fixed_multimap' | xargs sed -i 's/fixed_multimap/fixedMultimap/g'
grep -RiIl 'fixed_set' | xargs sed -i 's/fixed_set/fixedSet/g'
grep -RiIl 'EASTL/fixedSet.h EASTL/fixed_set' | xargs sed -i 's/fixed_set/h/g'
grep -RiIl 'fixed_multiset' | xargs sed -i 's/fixed_multiset/fixedMultiset/g'
grep -RiIl 'fixed_string' | xargs sed -i 's/fixed_string/fixedString/g'
grep -RiIl 'get_overflow_allocator' | xargs sed -i 's/get_overflow_allocator/getOverflowAllocator/g'
grep -RiIl 'set_overflow_allocator' | xargs sed -i 's/set_overflow_allocator/setOverflowAllocator/g'
grep -RiIl 'fixed_substring' | xargs sed -i 's/fixed_substring/fixedSubstring/g'
grep -RiIl 'fixed_vector' | xargs sed -i 's/fixed_vector/fixedVector/g'
grep -RiIl 'promote_heap' | xargs sed -i 's/promote_heap/promoteHeap/g'
grep -RiIl 'adjust_heap' | xargs sed -i 's/adjust_heap/adjustHeap/g'
grep -RiIl 'get_temporary_buffer' | xargs sed -i 's/get_temporary_buffer/getTemporaryBuffer/g'
grep -RiIl 'return_temporary_buffer' | xargs sed -i 's/return_temporary_buffer/returnTemporaryBuffer/g'
grep -RiIl 'uninitialized_move_start' | xargs sed -i 's/uninitialized_move_start/uninitializedMoveStart/g'
grep -RiIl 'uninitialized_move_commit' | xargs sed -i 's/uninitialized_move_commit/uninitializedMoveCommit/g'
grep -RiIl 'uninitialized_move_abort' | xargs sed -i 's/uninitialized_move_abort/uninitializedMoveAbort/g'
grep -RiIl 'uninitialized_move' | xargs sed -i 's/uninitialized_move/uninitializedMove/g'
grep -RiIl 'uninitialized_copy_ptr' | xargs sed -i 's/uninitialized_copy_ptr/uninitializedCopyPtr/g'
grep -RiIl 'uninitialized_copy_fill' | xargs sed -i 's/uninitialized_copy_fill/uninitializedCopyFill/g'
grep -RiIl 'uninitialized_copy_copy' | xargs sed -i 's/uninitialized_copy_copy/uninitializedCopyCopy/g'
grep -RiIl 'uninitialized_copy' | xargs sed -i 's/uninitialized_copy/uninitializedCopy/g'
grep -RiIl 'uninitialized_fill_ptr' | xargs sed -i 's/uninitialized_fill_ptr/uninitializedFillPtr/g'
grep -RiIl 'uninitialized_fillN_ptr' | xargs sed -i 's/uninitialized_fillN_ptr/uninitializedFillNPtr/g'
grep -RiIl 'uninitialized_fillN' | xargs sed -i 's/uninitialized_fillN/uninitializedFillN/g'
grep -RiIl 'uninitialized_fill_copy' | xargs sed -i 's/uninitialized_fill_copy/uninitializedFillCopy/g'
grep -RiIl 'uninitialized_fill' | xargs sed -i 's/uninitialized_fill/uninitializedFill/g'
grep -RiIl 'is_sorted' | xargs sed -i 's/is_sorted/isSorted/g'
grep -RiIl 'insertion_sort' | xargs sed -i 's/insertion_sort/insertionSort/g'
grep -RiIl 'shell_sort' | xargs sed -i 's/shell_sort/shellSort/g'
grep -RiIl 'heap_sort' | xargs sed -i 's/heap_sort/heapSort/g'
grep -RiIl 'merge_sort_buffer' | xargs sed -i 's/merge_sort_buffer/mergeSortBuffer/g'
grep -RiIl 'merge_sort' | xargs sed -i 's/merge_sort/mergeSort/g'
grep -RiIl 'quick_sort' | xargs sed -i 's/quick_sort/quickSort/g'
grep -RiIl 'get_partition' | xargs sed -i 's/get_partition/getPartition/g'
grep -RiIl 'partial_sort' | xargs sed -i 's/partial_sort/partialSort/g'
grep -RiIl 'nth_element' | xargs sed -i 's/nth_element/nthElement/g'
grep -RiIl 'stable_sort' | xargs sed -i 's/stable_sort/stableSort/g'
grep -RiIl 'extract_radix_key' | xargs sed -i 's/extract_radix_key/extractRadixKey/g'
grep -RiIl 'radix_sort' | xargs sed -i 's/radix_sort/radixSort/g'
grep -RiIl 'comb_sort' | xargs sed -i 's/comb_sort/combSort/g'
grep -RiIl 'bubble_sort' | xargs sed -i 's/bubble_sort/bubbleSort/g'
grep -RiIl 'selection_sort' | xargs sed -i 's/selection_sort/selectionSort/g'
grep -RiIl 'shaker_sort' | xargs sed -i 's/shaker_sort/shakerSort/g'
grep -RiIl 'bucket_sort' | xargs sed -i 's/bucket_sort/bucketSort/g'
grep -RiIl 'basic_string' | xargs sed -i 's/basic_string/basicString/g'
grep -RiIl 'force_size' | xargs sed -i 's/force_size/forceSize/g'
grep -RiIl 'append_sprintf_va_list' | xargs sed -i 's/append_sprintf_va_list/appendSprintfVaList/g'
grep -RiIl 'append_sprintf' | xargs sed -i 's/append_sprintf/appendSprintf/g'
grep -RiIl 'findLast_of' | xargs sed -i 's/findLast_of/findLastOf/g'
grep -RiIl 'findLast_not_of' | xargs sed -i 's/findLast_not_of/findLastNotOf/g'
grep -RiIl 'make_lower' | xargs sed -i 's/make_lower/makeLower/g'
grep -RiIl 'make_upper' | xargs sed -i 's/make_upper/makeUpper/g'
grep -RiIl 'sprintf_va_list' | xargs sed -i 's/sprintf_va_list/sprintfVaList/g'
grep -RiIl 'use_self' | xargs sed -i 's/use_self/useSelf/g'
grep -RiIl 'use_first' | xargs sed -i 's/use_first/useFirst/g'
grep -RiIl 'use_second' | xargs sed -i 's/use_second/useSecond/g'
grep -RiIl 'make_pair_ref' | xargs sed -i 's/make_pair_ref/makePairRef/g'
grep -RiIl 'make_pair' | xargs sed -i 's/make_pair/makePair/g'


grep -RiIl '<EASTL/' | xargs sed -i 's/<EASTL\//<eastl\//g'
grep -RiIl '<EABase/' | xargs sed -i 's/<EABase\//<eastl\/EABase\//g'
grep -RiIl 'eastl/fixedAllocator.h' | xargs sed -i 's/eastl\/fixedAllocator.h/eastl\/fixed_allocator.h/g'
