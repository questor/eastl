cd ..
extra\fart -r *.h,*.cpp EA_COMPILER_GNUC TARGET_COMPILER_GNUC
extra\fart -r *.h,*.cpp EA_COMPILER_ARM TARGET_COMPILER_ARM
extra\fart -r *.h,*.cpp EA_COMPILER_EDG TARGET_COMPILER_EDG
extra\fart -r *.h,*.cpp EA_COMPILER_SN TARGET_COMPILER_SN
extra\fart -r *.h,*.cpp EA_COMPILER_MSVC TARGET_COMPILER_MSVC
extra\fart -r *.h,*.cpp EA_COMPILER_METROWERKS TARGET_COMPILER_METROWERKS
extra\fart -r *.h,*.cpp EA_COMPILER_INTEL TARGET_COMPILER_INTEL
extra\fart -r *.h,*.cpp EA_COMPILER_BORLANDC TARGET_COMPILER_BORLANDC
extra\fart -r *.h,*.cpp EA_COMPILER_IBM TARGET_COMPILER_IBM
extra\fart -r *.h,*.cpp EA_COMPILER_VERSION TARGET_COMPILER_VERSION
extra\fart -r *.h,*.cpp EA_COMPILER_NAME TARGET_COMPILER_NAME
extra\fart -r *.h,*.cpp EA_COMPILER_STRING TARGET_COMPILER_STRING
extra\fart -r *.h,*.cpp EA_COMPILER_NO_STATIC_CONSTANTS TARGET_COMPILER_NO_STATIC_CONSTANTS
extra\fart -r *.h,*.cpp EA_COMPILER_NO_TEMPLATE_SPECIALIZATION TARGET_COMPILER_NO_TEMPLATE_SPECIALIZATION
extra\fart -r *.h,*.cpp EA_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION TARGET_COMPILER_NO_TEMPLATE_PARTIAL_SPECIALIZATION
extra\fart -r *.h,*.cpp EA_COMPILER_NO_MEMBER_TEMPLATES TARGET_COMPILER_NO_MEMBER_TEMPLATES
extra\fart -r *.h,*.cpp EA_COMPILER_NO_MEMBER_TEMPLATE_SPECIALIZATION TARGET_COMPILER_NO_MEMBER_TEMPLATE_SPECIALIZATION
extra\fart -r *.h,*.cpp EA_COMPILER_NO_TEMPLATE_TEMPLATES TARGET_COMPILER_NO_TEMPLATE_TEMPLATES
extra\fart -r *.h,*.cpp EA_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS TARGET_COMPILER_NO_MEMBER_TEMPLATE_FRIENDS
extra\fart -r *.h,*.cpp EA_COMPILER_NO_VOID_RETURNS TARGET_COMPILER_NO_VOID_RETURNS
extra\fart -r *.h,*.cpp EA_COMPILER_NO_COVARIANT_RETURN_TYPE TARGET_COMPILER_NO_COVARIANT_RETURN_TYPE
extra\fart -r *.h,*.cpp EA_COMPILER_NO_DEDUCED_TYPENAME TARGET_COMPILER_NO_DEDUCED_TYPENAME
extra\fart -r *.h,*.cpp EA_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP TARGET_COMPILER_NO_ARGUMENT_DEPENDENT_LOOKUP
extra\fart -r *.h,*.cpp EA_COMPILER_NO_EXCEPTION_STD_NAMESPACE TARGET_COMPILER_NO_EXCEPTION_STD_NAMESPACE
extra\fart -r *.h,*.cpp EA_COMPILER_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS TARGET_COMPILER_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
extra\fart -r *.h,*.cpp EA_COMPILER_NO_RTTI TARGET_COMPILER_NO_RTTI
extra\fart -r *.h,*.cpp EA_COMPILER_NO_EXCEPTIONS TARGET_COMPILER_NO_EXCEPTIONS
extra\fart -r *.h,*.cpp EA_COMPILER_NO_UNWIND TARGET_COMPILER_NO_UNWIND
extra\fart -r *.h,*.cpp EA_COMPILER_NO_STANDARD_CPP_LIBRARY TARGET_COMPILER_NO_STANDARD_CPP_LIBRARY
extra\fart -r *.h,*.cpp EA_COMPILER_NO_STATIC_VARIABLE_INIT TARGET_COMPILER_NO_STATIC_VARIABLE_INIT
extra\fart -r *.h,*.cpp EA_COMPILER_NO_STATIC_FUNCTION_INIT TARGET_COMPILER_NO_STATIC_FUNCTION_INIT
extra\fart -r *.h,*.cpp EA_COMPILER_NO_MEMBER_FUNCTION_SPECIALIZATION TARGET_COMPILER_NO_MEMBER_FUNCTION_SPECIALIZATION
extra\fart -r *.h,*.cpp EA_COMPILER_PARTIAL_TEMPLATE_SPECIALIZATION TARGET_COMPILER_PARTIAL_TEMPLATE_SPECIALIZATION
extra\fart -r *.h,*.cpp EA_COMPILER_TEMPLATE_SPECIALIZATION TARGET_COMPILER_TEMPLATE_SPECIALIZATION
extra\fart -r *.h,*.cpp EA_COMPILER_MEMBER_TEMPLATES TARGET_COMPILER_MEMBER_TEMPLATES
extra\fart -r *.h,*.cpp EA_COMPILER_MEMBER_TEMPLATE_SPECIALIZATION TARGET_COMPILER_MEMBER_TEMPLATE_SPECIALIZATION


extra\fart -r *.h,*.cpp EA_PLATFORM_ TARGET_PLATFORM_
extra\fart -r *.h,*.cpp EA_PROCESSOR_ TARGET_PROCESSOR_
extra\fart -r *.h,*.cpp EA_SYSTEM_ TARGET_SYSTEM_
extra\fart -r *.h,*.cpp EA_ASM_STYLE_ TARGET_ASM_STYLE_
extra\fart -r *.h,*.cpp EA_POSIX_THREADS_AVAILABLE TARGET_POSIX_THREADS_AVAILABLE

extra\fart -r *.h,*.cpp EA_COMPILER TARGET_COMPILER

extra\fart -r *.h,*.cpp eacompiler.h targetcompiler.h
extra\fart -r *.h,*.cpp eaplatform.h targetplatform.h
extra\fart -r *.h,*.cpp eacompilertraits.h eastlcompilertraits.h

cd eabase
ren earesult.h result.h
ren eabase.h base.h
cd config
ren eacompiler.h targetcompiler.h
ren eaplatform.h targetplatform.h
ren eacompilertraits.h eastlcompilertraits.h
cd ../..

ren eabase base

extra\fart -r *.h,*.cpp EA_ALIGN_OF EASTL_ALIGN_OF
extra\fart -r *.h,*.cpp EA_ALIGN EASTL_ALIGN
extra\fart -r *.h,*.cpp EA_PACKED EASTL_PACKED
extra\fart -r *.h,*.cpp EA_LIKELY EASTL_LIKELY
extra\fart -r *.h,*.cpp EA_UNLIKELY EASTL_UNLIKELY
extra\fart -r *.h,*.cpp EA_INIT_PRIORITY EASTL_INIT_PRIORITY
extra\fart -r *.h,*.cpp EA_MAY_ALIAS EASTL_MAY_ALIAS
extra\fart -r *.h,*.cpp EA_ASSUME EASTL_ASSUME
extra\fart -r *.h,*.cpp EA_PURE EASTL_PURE
extra\fart -r *.h,*.cpp EA_WEAK EASTL_WEAK
extra\fart -r *.h,*.cpp EA_WCHAR_T_NON_NATIVE EASTL_WCHAR_T_NON_NATIVE
extra\fart -r *.h,*.cpp EA_WCHAR_SIZE EASTL_WCHAR_SIZE
extra\fart -r *.h,*.cpp EA_RESTRICT EASTL_RESTRICT
extra\fart -r *.h,*.cpp EA_DEPRECATED EASTL_DEPRECATED
extra\fart -r *.h,*.cpp EA_PREFIX_ EASTL_PREFIX_
extra\fart -r *.h,*.cpp EA_POSTFIX_ EASTL_POSTFIX_
extra\fart -r *.h,*.cpp EA_FORCE_INLINE EASTL_FORCE_INLINE
extra\fart -r *.h,*.cpp EA_NO_INLINE EASTL_NO_INLINE
extra\fart -r *.h,*.cpp EA_NO_VTABLE EASTL_NO_VTABLE
extra\fart -r *.h,*.cpp EA_CLASS_NO_VTABLE EASTL_CLASS_NO_VTABLE
extra\fart -r *.h,*.cpp EA_STRUCT_NO_VTABLE EASTL_STRUCT_NO_VTABLE
extra\fart -r *.h,*.cpp EA_PASCAL EASTL_PASCAL
extra\fart -r *.h,*.cpp EA_SSE EASTL_SSE
extra\fart -r *.h,*.cpp EA_IMPORT EASTL_IMPORT
extra\fart -r *.h,*.cpp EA_EXPORT EASTL_EXPORT
extra\fart -r *.h,*.cpp EA_PRAGMA_ONCE_SUPPORTED EASTL_PRAGMA_ONCE_SUPPORTED
extra\fart -r *.h,*.cpp EA_OVERRIDE EASTL_OVERRIDE
extra\fart -r *.h,*.cpp EA_SEALED EASTL_SEALED
extra\fart -r *.h,*.cpp EA_ABSTRACT EASTL_ABSTRACT


extra\fart -r *.h,*.cpp EABase base
extra\fart -r *.h,*.cpp INCLUDED_eabase_H INCLUDED_base_H
extra\fart -r *.h,*.cpp base/eabase.h base/base.h

extra\fart -r *.h,*.cpp INCLUDED_earesult_H INCLUDED_result_H



extra\fart -r *.h,*.cpp EA_CHAR16_NATIVE EASTL_CHAR16_NATIVE
extra\fart -r *.h,*.cpp EA_CHAR32_NATIVE EASTL_CHAR32_NATIVE
extra\fart -r *.h,*.cpp EA_CHAR16 EASTL_CHAR16
extra\fart -r *.h,*.cpp EA_CHAR32 EASTL_CHAR32

extra\fart -r *.h,*.cpp EAArrayCount EASTLArrayCount
extra\fart -r *.h,*.cpp EABASE_STATIC_ASSERT_ENABLED CONFIG_STATIC_ASSERT_ENABLED
extra\fart -r *.h,*.cpp EA_PREPROCESSOR_JOIN EASTL_PREPROCESSOR_JOIN

extra\fart -r *.h,*.cpp EA_SSE EASTL_SSE
extra\fart -r *.h,*.cpp EA_SSE EASTL_SSE
extra\fart -r *.h,*.cpp EA_SSE EASTL_SSE

REM now change function names...
extra\fart -r *.h,*.cpp push_back_uninitialized pushBackUninitialized
extra\fart -r *.h,*.cpp push_front_uninitialized pushFrontUninitialized
extra\fart -r *.h,*.cpp push_front pushFront
extra\fart -r *.h,*.cpp push_back pushBack
extra\fart -r *.h,*.cpp set_capacity setCapacity
extra\fart -r *.h,*.cpp pop_back popBack
extra\fart -r *.h,*.cpp pop_front popFront
extra\fart -r *.h,*.cpp remove_if removeIf
extra\fart -r *.h,*.cpp validate_iterator validateIterator
extra\fart -r *.h,*.cpp push_heap pushHeap
extra\fart -r *.h,*.cpp pop_heap popHeap
extra\fart -r *.h,*.cpp make_heap makeHeap
extra\fart -r *.h,*.cpp sort_heap sortHeap
extra\fart -r *.h,*.cpp remove_heap removeHeap
extra\fart -r *.h,*.cpp change_heap changeHeap
extra\fart -r *.h,*.cpp is_heap isHeap

extra\fart -r *.h,*.cpp get_default_allocator getDefaultAllocator
extra\fart -r *.h,*.cpp get_name getName
extra\fart -r *.h,*.cpp set_name setName
extra\fart -r *.h,*.cpp max_alt maxAlt
extra\fart -r *.h,*.cpp min_alt minAlt
extra\fart -r *.h,*.cpp min_element minElement
extra\fart -r *.h,*.cpp max_element maxElement
extra\fart -r *.h,*.cpp iter_swap iterSwap
extra\fart -r *.h,*.cpp swap_ranges swapRanges
extra\fart -r *.h,*.cpp adjacent_find adjacentFind
extra\fart -r *.h,*.cpp copy_backward copyBackward
extra\fart -r *.h,*.cpp count_if countIf
extra\fart -r *.h,*.cpp fill_n fillN
extra\fart -r *.h,*.cpp find_if findIf
extra\fart -r *.h,*.cpp find_first_of findFirstOf
extra\fart -r *.h,*.cpp find_first_not_of findFirstNotOf
extra\fart -r *.h,*.cpp for_each forEach
extra\fart -r *.h,*.cpp generate_n generateN
extra\fart -r *.h,*.cpp lexicographical_compare lexicographicalCompare
extra\fart -r *.h,*.cpp lower_bound lowerBound
extra\fart -r *.h,*.cpp upper_bound upperBound
extra\fart -r *.h,*.cpp equal_range equalRange
extra\fart -r *.h,*.cpp replace_if replaceIf
extra\fart -r *.h,*.cpp remove_copy_if removeCopyIf
extra\fart -r *.h,*.cpp remove_copy removeCopy
extra\fart -r *.h,*.cpp replace_copy_if replaceCopyIf
extra\fart -r *.h,*.cpp replace_copy replaceCopy
extra\fart -r *.h,*.cpp reverse_copy reverseCopy
extra\fart -r *.h,*.cpp search_n searchN
extra\fart -r *.h,*.cpp binary_search_i binarySearchI
extra\fart -r *.h,*.cpp binary_search binarySearch
extra\fart -r *.h,*.cpp find_end findEnd
extra\fart -r *.h,*.cpp set_difference setDifference
extra\fart -r *.h,*.cpp find_first findFirst
extra\fart -r *.h,*.cpp find_next findNext
extra\fart -r *.h,*.cpp find_last findLast
extra\fart -r *.h,*.cpp find_prev findPrev
extra\fart -r *.h,*.cpp get_allocator getAllocator
extra\fart -r *.h,*.cpp set_allocator setAllocator
extra\fart -r *.h,*.cpp get_flags getFlags
extra\fart -r *.h,*.cpp set_flags setFlags
extra\fart -r *.h,*.cpp to_ulong toUlong
extra\fart -r *.h,*.cpp fixed_allocator_with_overflow fixedAllocatorWithOverflow
extra\fart -r *.h,*.cpp fixed_allocator fixedAllocator
extra\fart -r *.h,*.cpp fixed_hash_map fixedHashMap
extra\fart -r *.h,*.cpp fixed_hashtable_allocator fixedHashtableAllocator
extra\fart -r *.h,*.cpp max_size maxSize
extra\fart -r *.h,*.cpp fixed_hash_multimap fixedHashMultimap
extra\fart -r *.h,*.cpp hash_multimap hashMultimap
extra\fart -r *.h,*.cpp fixed_swap fixedSwap
extra\fart -r *.h,*.cpp hash_map hashMap
extra\fart -r *.h,*.cpp EASTL/hashMap.h EASTL/hash_map.h
extra\fart -r *.h,*.cpp fixed_hash_set fixedHashSet
extra\fart -r *.h,*.cpp hash_set hashSet
extra\fart -r *.h,*.cpp EASTL/hashSet.h EASTL/hash_set.h
extra\fart -r *.h,*.cpp fixed_hash_multiset fixedHashMultiset
extra\fart -r *.h,*.cpp hash_multiset hashMultiset
extra\fart -r *.h,*.cpp fixed_list fixedList
extra\fart -r *.h,*.cpp has_overflowed hasOverflowed
extra\fart -r *.h,*.cpp fixed_map fixedMap
extra\fart -r *.h,*.cpp fixed_multimap fixedMultimap
extra\fart -r *.h,*.cpp fixed_set fixedSet
extra\fart -r *.h,*.cpp EASTL/fixedSet.h EASTL/fixed_set.h
extra\fart -r *.h,*.cpp fixed_multiset fixedMultiset
extra\fart -r *.h,*.cpp fixed_string fixedString
extra\fart -r *.h,*.cpp get_overflow_allocator getOverflowAllocator
extra\fart -r *.h,*.cpp set_overflow_allocator setOverflowAllocator
extra\fart -r *.h,*.cpp fixed_substring fixedSubstring
extra\fart -r *.h,*.cpp fixed_vector fixedVector




extra\fart -r *.h,*.cpp promote_heap promoteHeap
extra\fart -r *.h,*.cpp adjust_heap adjustHeap
extra\fart -r *.h,*.cpp get_temporary_buffer getTemporaryBuffer
extra\fart -r *.h,*.cpp return_temporary_buffer returnTemporaryBuffer
extra\fart -r *.h,*.cpp uninitialized_move_start uninitializedMoveStart
extra\fart -r *.h,*.cpp uninitialized_move_commit uninitializedMoveCommit
extra\fart -r *.h,*.cpp uninitialized_move_abort uninitializedMoveAbort
extra\fart -r *.h,*.cpp uninitialized_move uninitializedMove
extra\fart -r *.h,*.cpp uninitialized_copy_ptr uninitializedCopyPtr
extra\fart -r *.h,*.cpp uninitialized_copy_fill uninitializedCopyFill
extra\fart -r *.h,*.cpp uninitialized_copy_copy uninitializedCopyCopy
extra\fart -r *.h,*.cpp uninitialized_copy uninitializedCopy
extra\fart -r *.h,*.cpp uninitialized_fill_ptr uninitializedFillPtr
extra\fart -r *.h,*.cpp uninitialized_fillN_ptr uninitializedFillNPtr
extra\fart -r *.h,*.cpp uninitialized_fillN uninitializedFillN
extra\fart -r *.h,*.cpp uninitialized_fill_copy uninitializedFillCopy
extra\fart -r *.h,*.cpp uninitialized_fill uninitializedFill
extra\fart -r *.h,*.cpp is_sorted isSorted
extra\fart -r *.h,*.cpp insertion_sort insertionSort
extra\fart -r *.h,*.cpp shell_sort shellSort
extra\fart -r *.h,*.cpp heap_sort heapSort
extra\fart -r *.h,*.cpp merge_sort_buffer mergeSortBuffer
extra\fart -r *.h,*.cpp merge_sort mergeSort
extra\fart -r *.h,*.cpp quick_sort quickSort
extra\fart -r *.h,*.cpp get_partition getPartition
extra\fart -r *.h,*.cpp partial_sort partialSort
extra\fart -r *.h,*.cpp nth_element nthElement
extra\fart -r *.h,*.cpp stable_sort stableSort
extra\fart -r *.h,*.cpp extract_radix_key extractRadixKey
extra\fart -r *.h,*.cpp radix_sort radixSort
extra\fart -r *.h,*.cpp comb_sort combSort
extra\fart -r *.h,*.cpp bubble_sort bubbleSort
extra\fart -r *.h,*.cpp selection_sort selectionSort
extra\fart -r *.h,*.cpp shaker_sort shakerSort
extra\fart -r *.h,*.cpp bucket_sort bucketSort
extra\fart -r *.h,*.cpp basic_string basicString
extra\fart -r *.h,*.cpp force_size forceSize
extra\fart -r *.h,*.cpp append_sprintf_va_list appendSprintfVaList
extra\fart -r *.h,*.cpp append_sprintf appendSprintf
extra\fart -r *.h,*.cpp findLast_of findLastOf
extra\fart -r *.h,*.cpp findLast_not_of findLastNotOf
extra\fart -r *.h,*.cpp make_lower makeLower
extra\fart -r *.h,*.cpp make_upper makeUpper
extra\fart -r *.h,*.cpp sprintf_va_list sprintfVaList
extra\fart -r *.h,*.cpp use_self useSelf
extra\fart -r *.h,*.cpp use_first useFirst
extra\fart -r *.h,*.cpp use_second useSecond
extra\fart -r *.h,*.cpp make_pair_ref makePairRef
extra\fart -r *.h,*.cpp make_pair makePair
