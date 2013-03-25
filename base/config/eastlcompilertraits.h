/*
Copyright (C) 2009 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*-----------------------------------------------------------------------------
 * config/eastlcompilertraits.h
 *
 * Copyright (c) 2002 - 2005 Electronic Arts Inc. All rights reserved.
 * Maintained by Paul Pedriana, Maxis
 *
 *-----------------------------------------------------------------------------
 * Currently supported defines include:
 *    TARGET_COMPILER_HAS_C99_TYPES
 *    TARGET_COMPILER_HAS_CHAR_16_32
 *    TARGET_COMPILER_IS_ANSIC
 *    TARGET_COMPILER_IS_C99
 *    TARGET_COMPILER_IS_CPLUSPLUS
 *    TARGET_COMPILER_IS_CPLUSPLUS_11_ENABLED
 *    TARGET_COMPILER_HAS_MOVE_SEMANTICS
 *    TARGET_COMPILER_MANAGED_CPP
 *
 *    EASTL_ALIGN_OF()
 *    EASTL_ALIGN() / EASTL_PREFIX_ALIGN() / EASTL_POSTFIX_ALIGN()
 *    EASTL_ALIGNED()
 *    EASTL_PACKED()
 *
 *    EASTL_LIKELY()
 *    EASTL_UNLIKELY()
 *    EASTL_INIT_PRIORITY()
 *    EASTL_MAY_ALIAS()
 *    EASTL_ASSUME()
 *    EASTL_PURE
 *    EASTL_WEAK
 *
 *    EASTL_WCHAR_T_NON_NATIVE
 *    EASTL_WCHAR_SIZE = <n bytes>
 *
 *    EASTL_RESTRICT
 *    EASTL_DEPRECATED   / EASTL_PREFIX_DEPRECATED   / EASTL_POSTFIX_DEPRECATED
 *    EASTL_FORCE_INLINE / EASTL_PREFIX_FORCE_INLINE / EASTL_POSTFIX_FORCE_INLINE
 *    EASTL_NO_INLINE    / EASTL_PREFIX_NO_INLINE    / EASTL_POSTFIX_NO_INLINE
 *    EASTL_NO_VTABLE    / EASTL_CLASS_NO_VTABLE     / EASTL_STRUCT_NO_VTABLE
 *    EASTL_PASCAL
 *    EASTL_PASCAL_FUNC()
 *    EASTL_SSE = [0 | 1]
 *    EASTL_IMPORT
 *    EASTL_EXPORT
 *    EASTL_PRAGMA_ONCE_SUPPORTED
 *    EASTL_OVERRIDE
 *    EASTL_SEALED
 *    EASTL_ABSTRACT
 *
 *  Todo:
 *    Find a way to reliably detect wchar_t size at preprocessor time and 
 *    implement it below for EASTL_WCHAR_SIZE.
 *
 *  Todo:
 *    Find out how to support EASTL_PASCAL and EASTL_PASCAL_FUNC for systems in 
 *    which it hasn't yet been found out for.
 *---------------------------------------------------------------------------*/


#ifndef INCLUDED_eacompilertraits_H
#define INCLUDED_eacompilertraits_H

    #ifndef INCLUDED_eaplatform_H
        #include "base/config/targetplatform.h"
    #endif

    #ifndef INCLUDED_eacompiler_H
        #include "base/config/targetcompiler.h"
    #endif

    // Metrowerks uses #defines in its core C header files to define 
    // the kind of information we need below (e.g. C99 compatibility)
    #if defined(__MWERKS__)   
        // Defining the following causes C99 compilers to enable the macros 
        // associated with the defines. The C99 standard specifies that you 
        // should define these as such.
        #ifndef __STDC_LIMIT_MACROS
            #define __STDC_LIMIT_MACROS
        #endif

        #ifndef __STDC_CONSTANT_MACROS
            #define __STDC_CONSTANT_MACROS
        #endif

        #include <stddef.h>
    #endif

    #if defined(__SNC__) || defined(TARGET_PLATFORM_PS3) || defined(__S3E__)
        #ifndef __STDC_LIMIT_MACROS
            #define __STDC_LIMIT_MACROS
        #endif

        #ifndef __STDC_CONSTANT_MACROS
            #define __STDC_CONSTANT_MACROS
        #endif

        #include <stdint.h>

        #if !defined(TARGET_COMPILER_HAS_INTTYPES)
            #if !defined(__S3E__)
                #define TARGET_COMPILER_HAS_INTTYPES
            #endif
        #endif
    #endif

    // Determine if this compiler is ANSI C compliant and if it is C99 compliant.
    #if defined(__STDC__)
        #define TARGET_COMPILER_IS_ANSIC    // The compiler claims to be ANSI C

        // Is the compiler a C99 compiler or equivalent?
        // From ISO/IEC 9899:1999:
        //    6.10.8 Predefined macro names
        //    __STDC_VERSION__ The integer constant 199901L. (150)
        //
        //    150) This macro was not specified in ISO/IEC 9899:1990 and was 
        //    specified as 199409L in ISO/IEC 9899/AMD1:1995. The intention 
        //    is that this will remain an integer constant of type long int 
        //    that is increased with each revision of this International Standard.
        //
        #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
            #define TARGET_COMPILER_IS_C99
        #endif
    #endif

    // Some compilers (e.g. GCC) define __USE_ISOC99 if they are not 
    // strictly C99 compilers (or are simply C++ compilers) but are set 
    // to use C99 functionality. Metrowerks defines _MSL_C99 as 1 in 
    // this case, but 0 otherwise.
    #if (defined(__USE_ISOC99) || (defined(_MSL_C99) && (_MSL_C99 == 1))) && !defined(TARGET_COMPILER_IS_C99)
        #define TARGET_COMPILER_IS_C99
    #endif
 
    // Metrowerks defines C99 types (e.g. intptr_t) instrinsically when in C99 mode (-lang C99 on the command line).
    #if (defined(_MSL_C99) && (_MSL_C99 == 1))
        #define TARGET_COMPILER_HAS_C99_TYPES
    #endif

    #if defined(__GNUC__) 
        #if (((__GNUC__ * 100) + __GNUC_MINOR__) >= 302) // Also, GCC defines _HAS_C9X.
            #define TARGET_COMPILER_HAS_C99_TYPES // The compiler is not necessarily a C99 compiler, but it defines C99 types.
            
            #ifndef __STDC_LIMIT_MACROS
                #define __STDC_LIMIT_MACROS
            #endif
            
            #ifndef __STDC_CONSTANT_MACROS
                #define __STDC_CONSTANT_MACROS    // This tells the GCC compiler that we want it to use its native C99 types.
            #endif
        #endif
        #if (defined(__GXX_EXPERIMENTAL_CXX0X__) && !defined(TARGET_COMPILER_IS_CPLUSPLUS_11_ENALBED))
            #define TARGET_COMPILER_IS_CPLUSPLUS_11_ENALBED
            #define TARGET_COMPILER_HAS_MOVE_SEMANTICS
        #endif
        #define EASTL_GNU_VERSION_NUM (__GNUC__*100)+__GNUC_MINOR__
        #ifdef TARGET_COMPILER_IS_CPLUSPLUS_11_ENABLED
            #if EASTL_GNU_VERSION_NUM >= 404
                #define TARGET_COMPILER_HAS_CHAR_16_32
                #define TARGET_COMPILER_HAS_VARIADIC_TEMPLATES
            #endif
        #endif

    #endif

    #if defined(__clang__)
        #if (defined(__GXXC_EXPERIMENTAL_CXX0X__) && !defined(TARGET_COMPILER_IS_CPLUSPLUS_11_ENABLED))
            #define TARGET_COMPILER_IS_CPLUSPLUS_11_ENALBED
            #define TARGET_COMPILER_HAS_MOVE_SEMANTICS
        #endif
        #define EASTL_CLANG_VERSION_NUM (__clang_major__*10)+__clang_minor__
        #if EASTL_CLANG_VERSION_NUM >= 29
            #define TARGET_COMPILER_HAS_C99_TYPES
        #endif
        #ifdef TARGET_COMPILER_IS_CPLUSPLUS_11_ENALBED
            #if EASTL_CLANG_VERSION_NUM >= 29
                #define TARGET_COMPILER_HAS_CHAR_16_32
                #define TARGET_COMPILER_HAS_VARIADIC_TEMPLATES
            #endif
        #endif
        #ifndef __STDC_LIMIT_MACROS
            #define __STDC_LIMIT_MACROS
        #endif
        #ifndef __STDC_CONSTANT_MACROS
            #define __STDC_CONSTANT_MACROS          // This tells the GCC compiler that we want it to use its native C99 types.
        #endif
    #endif

    #if defined(_MSC_VER)
        #if(_MSC_VER >= 1600)
            #define TARGET_COMPILER_IS_CPLUSPLUS_11_ENALBED
            #define TARGET_COMPILER_HAS_MOVE_SEMANTICS
        #endif
    #endif

    #ifdef  __cplusplus
        #define TARGET_COMPILER_IS_CPLUSPLUS
        #if (__cplusplus > 199711L)
            #define TARGET_COMPILER_IS_CPLUSPLUS_11_ENALBED
        #endif
    #endif


    // ------------------------------------------------------------------------
    // TARGET_COMPILER_MANAGED_CPP
    // Defined if this is being compiled with Managed C++ extensions
    #ifdef TARGET_COMPILER_MSVC
        #if TARGET_COMPILER_VERSION >= 1300
            #ifdef _MANAGED
                #define TARGET_COMPILER_MANAGED_CPP
            #endif
        #endif
    #endif


    // ------------------------------------------------------------------------
    // alignment expressions
    //
    // Here we define
    //    EASTL_ALIGN_OF(type)         // Returns size_t.
    //    EASTL_ALIGN(n)               // Used as a prefix. n is byte alignment, with being a power of two. Most of the time you can use this and avoid using EASTL_PREFIX_ALIGN/EASTL_POSTFIX_ALIGN.
    //    EASTL_PREFIX_ALIGN(n)        // n is byte alignment, with being a power of two. You should need this only for unusual compilers.
    //    EASTL_POSTFIX_ALIGN(n)       // Valid values for n are 1, 2, 4, 8, etc. You should need this only for unusual compilers.
    //    EASTL_ALIGNED(t, v, n)       // Type, variable, alignment. Used to align an instance. You should need this only for unusual compilers.
    //    EASTL_PACKED                 // Specifies that the given structure be packed (and not have its members aligned).
    //
    // Example usage:
    //    size_t x = EASTL_ALIGN_OF(int);                                  Non-aligned equivalents.        Meaning
    //    EASTL_PREFIX_ALIGN(8) int x = 5;                                 int x = 5;                      Align x on 8 for compilers that require prefix attributes. Can just use EASTL_ALIGN instead.
    //    EASTL_ALIGN(8) int x;                                            int x;                          Align x on 8 for compilers that allow prefix attributes.
    //    int x EASTL_POSTFIX_ALIGN(8);                                    int x;                          Align x on 8 for compilers that require postfix attributes.
    //    int x EASTL_POSTFIX_ALIGN(8) = 5;                                int x = 5;                      Align x on 8 for compilers that require postfix attributes.
    //    int x EASTL_POSTFIX_ALIGN(8)(5);                                 int x(5);                       Align x on 8 for compilers that require postfix attributes.
    //    struct EASTL_PREFIX_ALIGN(8) X { int x; } EASTL_POSTFIX_ALIGN(8);   struct X { int x; };            Define X as a struct which is aligned on 8 when used.
    //    EASTL_ALIGNED(int, x, 8) = 5;                                    int x = 5;                      Align x on 8.
    //    EASTL_ALIGNED(int, x, 16)(5);                                    int x(5);                       Align x on 16.
    //    EASTL_ALIGNED(int, x[3], 16);                                    int x[3];                       Align x array on 16.
    //    EASTL_ALIGNED(int, x[3], 16) = { 1, 2, 3 };                      int x[3] = { 1, 2, 3 };         Align x array on 16.
    //    int x[3] EASTL_PACKED;                                           int x[3];                       Pack the 3 ints of the x array. GCC doesn't seem to support packing of int arrays.
    //    struct EASTL_ALIGN(32) X { int x; int y; };                      struct X { int x; };            Define A as a struct which is aligned on 32 when used.
    //    EASTL_ALIGN(32) struct X { int x; int y; } Z;                    struct X { int x; } Z;          Define A as a struct, and align the instance Z on 32.
    //    struct X { int x EASTL_PACKED; int y EASTL_PACKED; };               struct X { int x; int y; };     Pack the x and y members of struct X.
    //    struct X { int x; int y; } EASTL_PACKED;                         struct X { int x; int y; };     Pack the members of struct X.
    //    typedef EASTL_ALIGNED(int, int16, 16); int16 n16;                typedef int int16; int16 n16;   Define int16 as an int which is aligned on 16.
    //    typedef EASTL_ALIGNED(X, X16, 16); X16 x16;                      typedef X X16; X16 x16;         Define X16 as an X which is aligned on 16.

    // SNC (EDG) intends to be compatible with GCC but has a bug whereby it 
    // fails to support calling a constructor in an aligned declaration when 
    // using postfix alignment attributes. Prefix works for alignment, but does not align
    // the size like postfix does.  Prefix also fails on templates.  So gcc style post fix
    // is still used, but the user will need to use EASTL_POSTFIX_ALIGN before the constructor parameters.
    // this note by Paul and Frank
    #if defined(TARGET_COMPILER_SN) && defined(__GNUC__)  // If using the SN compiler in GCC compatibility mode...
        #define EASTL_ALIGN_OF(type) ((size_t)__alignof__(type))
        #define EASTL_ALIGN(n) __attribute__((aligned(n)))
        #define EASTL_PREFIX_ALIGN(n) 
        #define EASTL_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
        #define EASTL_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
        #define EASTL_PACKED __attribute__((packed))

    // GCC 2.x doesn't support prefix attributes.
    #elif defined(__GNUC__) && (__GNUC__ < 3)
        #define EASTL_ALIGN_OF(type) ((size_t)__alignof__(type))
        #define EASTL_ALIGN(n)
        #define EASTL_PREFIX_ALIGN(n)
        #define EASTL_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
        #define EASTL_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
        #define EASTL_PACKED __attribute__((packed))

    // GCC 3.x+ and IBM C support prefix attributes.
    #elif (defined(__GNUC__) && (__GNUC__ >= 3)) || defined(__xlC__)
        #define EASTL_ALIGN_OF(type) ((size_t)__alignof__(type))
        #define EASTL_ALIGN(n) __attribute__((aligned(n)))
        #define EASTL_PREFIX_ALIGN(n)
        #define EASTL_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
        #define EASTL_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
        #define EASTL_PACKED __attribute__((packed))

    // Clang
    #elif defined(TARGET_COMPILER_CLANG)
        //checks for clang-supported attributes
        #if __has_attribute(aligned)
            #define CLANG_ALIGNED(n) __attribute__((aligned(n)))
        #else
            #define CLANG_ALIGNED(n)
        #endif

        #if __has_attribute(packed)
            #define CLANG_PACKED __attribute__((packed))
        #else
            #define CLANG_PACKED
        #endif

        #define EA_ALIGN_OF(type) ((size_t)__alignof__(type))
        #define EA_ALIGN(n) CLANG_ALIGNED(n)
        #define EA_PREFIX_ALIGN(n)
        #define EA_POSTFIX_ALIGN(n) CLANG_ALIGNED(n)
        #define EA_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
        #define EA_PACKED CLANG_PACKED

        //make sure we get no macro naming conflicts
        #undef CLANG_ALIGNED
        #undef CLANG_PACKED

    // Metrowerks supports prefix attributes.
    // Metrowerks does not support packed alignment attributes.
    #elif defined(TARGET_COMPILER_METROWERKS)
        #define EASTL_ALIGN_OF(type) ((size_t)__alignof__(type))
        #define EASTL_ALIGN(n) __attribute__((aligned(n)))
        #define EASTL_PREFIX_ALIGN(n)
        #define EASTL_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
        #define EASTL_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
        #define EASTL_PACKED

    // Microsoft supports prefix alignment via __declspec, but the alignment value must be a literal number, not just a constant expression.
    // Contrary to VC7.x and earlier documentation, __declspec(align) works on stack variables. VC8+ (VS2005+) documents correctly.
    // Microsoft does not support packed alignment attributes; you must use #pragma pack.
    #elif defined(TARGET_COMPILER_INTEL) || defined(TARGET_PLATFORM_XBOX) || (defined(TARGET_COMPILER_MSVC) && (TARGET_COMPILER_VERSION >= 1300))
        #define EASTL_ALIGN_OF(type) ((size_t)__alignof(type))
        #define EASTL_ALIGN(n) __declspec(align(n))
        #define EASTL_PREFIX_ALIGN(n) __declspec(align(n))
        #define EASTL_POSTFIX_ALIGN(n)
        #define EASTL_ALIGNED(variable_type, variable, n) __declspec(align(n)) variable_type variable
        #define EASTL_PACKED

    // Arm brand compiler
    #elif defined(__ARMCC_VERSION)
        #define EASTL_ALIGN_OF(type) ((size_t)__ALIGNOF__(type))
        #define EASTL_ALIGN(n) __align(n)
        #define EASTL_PREFIX_ALIGN(n) __align(n)
        #define EASTL_POSTFIX_ALIGN(n)
        #define EASTL_ALIGNED(variable_type, variable, n) __align(n) variable_type variable
        #define EASTL_PACKED __packed

    #else // Unusual compilers
        // There is nothing we can do about some of these. This is not as bad a problem as it seems.
        // If the given platform/compiler doesn't support alignment specifications, then it's somewhat
        // likely that alignment doesn't matter for that platform. Otherwise they would have defined 
        // functionality to manipulate alignment.
        #define EASTL_ALIGN(n)
        #define EASTL_PREFIX_ALIGN(n)
        #define EASTL_POSTFIX_ALIGN(n)
        #define EASTL_ALIGNED(variable_type, variable, n) variable_type variable
        #define EASTL_PACKED

        #ifdef __cplusplus
            template <typename T> struct EAAlignOf1 { enum { s = sizeof (T), value = s ^ (s & (s - 1)) }; };
            template <typename T> struct EAAlignOf2;
            template <int size_diff> struct helper { template <typename T> struct Val { enum { value = size_diff }; }; };
            template <> struct helper<0> { template <typename T> struct Val { enum { value = EAAlignOf2<T>::value }; }; };
            template <typename T> struct EAAlignOf2 { struct Big { T x; char c; };
            enum { diff = sizeof (Big) - sizeof (T), value = helper<diff>::template Val<Big>::value }; };
            template <typename T> struct EAAlignof3 { enum { x = EAAlignOf2<T>::value, y = EAAlignOf1<T>::value, value = x < y ? x : y }; };
            #define EASTL_ALIGN_OF(type) ((size_t)EAAlignof3<type>::value)

        #else
            // C implementation of EASTL_ALIGN_OF
            // This implementation works for most cases, but doesn't directly work 
            // for types such as function pointer declarations. To work with those
            // types you need to typedef the type and then use the typedef in EASTL_ALIGN_OF.
            #define EASTL_ALIGN_OF(type) ((size_t)offsetof(struct { char c; type m; }, m))
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_LIKELY / EASTL_UNLIKELY
    //
    // Defined as a macro which gives a hint to the compiler for branch
    // prediction. GCC gives you the ability to manually give a hint to 
    // the compiler about the result of a comparison, though it's often
    // best to compile shipping code with profiling feedback under both
    // GCC (-fprofile-arcs) and VC++ (/LTCG:PGO, etc.). However, there 
    // are times when you feel very sure that a boolean expression will
    // usually evaluate to either true or false and can help the compiler
    // by using an explicity directive...
    //
    // Example usage:
    //    if(EASTL_LIKELY(a == 0)) // Tell the compiler that a will usually equal 0.
    //       { ... }
    //
    // Example usage:
    //    if(EASTL_UNLIKELY(a == 0)) // Tell the compiler that a will usually not equal 0.
    //       { ... }
    //
    #ifndef EASTL_LIKELY
        #if (defined(__GNUC__) && (__GNUC__ >= 3)) || \
            (defined(__clang__) && __has_builtin(__builtin_expect)) || \
            defined(__MWERKS__) // Metrowerks supports __builtin_expect, but with some platforms (e.g. Wii) it appears to ignore it.
            #if defined(__cplusplus)
                #define EASTL_LIKELY(x)   __builtin_expect(!!(x), true)
                #define EASTL_UNLIKELY(x) __builtin_expect(!!(x), false) 
            #else
                #define EASTL_LIKELY(x)   __builtin_expect(!!(x), 1)
                #define EASTL_UNLIKELY(x) __builtin_expect(!!(x), 0) 
            #endif
        #else
            #define EASTL_LIKELY(x)   (x)
            #define EASTL_UNLIKELY(x) (x)
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_INIT_PRIORITY
    //
    // This is simply a wrapper for the GCC init_priority attribute that allows 
    // multiplatform code to be easier to read. This attribute doesn't apply
    // to VC++ because VC++ uses file-level pragmas to control init ordering.
    //
    // Example usage:
    //     SomeClass gSomeClass EASTL_INIT_PRIORITY(2000);
    //
    #if !defined(EASTL_INIT_PRIORITY)
        #if defined(__GNUC__)
            #define EASTL_INIT_PRIORITY(x)  __attribute__ ((init_priority (x)))
        #else
            #define EASTL_INIT_PRIORITY(x)
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_MAY_ALIAS
    //
    // Defined as a macro that wraps the GCC may_alias attribute. This attribute
    // has no significance for VC++ because VC++ doesn't support the concept of 
    // strict aliasing. Users should avoid writing code that breaks strict 
    // aliasing rules; EASTL_MAY_ALIAS is for cases with no alternative.
    //
    // Example usage:
    //    void* EASTL_MAY_ALIAS gPtr = NULL;
    //
    // Example usage:
    //    typedef void* EASTL_MAY_ALIAS pvoid_may_alias;
    //    pvoid_may_alias gPtr = NULL;
    //
    #if defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 303)
        #define EASTL_MAY_ALIAS __attribute__((__may_alias__))
    #elif defined(TARGET_COMPILER_CLANG) && __has_attribute(__may_alias__)
        #define EASTL_MAY_ALIAS __attribute__((__may_alias__))
    #else
        #define EASTL_MAY_ALIAS
    #endif


    // ------------------------------------------------------------------------
    // EASTL_ASSUME
    //
    // This acts the same as the VC++ __assume directive and is implemented 
    // simply as a wrapper around it to allow portable usage of it and to take
    // advantage of it if and when it appears in other compilers.
    //
    // Example usage:
    //    void Function(int a) {
    //       switch(a) {
    //          case 1:
    //             DoSomething(1);
    //             break;
    //          case 2:
    //             DoSomething(-1);
    //             break;
    //          default:
    //             EASTL_ASSUME(0); // This tells the optimizer that the default cannot be reached.
    //       }
    //    }
    //
    #ifndef EASTL_ASSUME
        #if defined(_MSC_VER) && (_MSC_VER >= 1300) // If VC7.0 and later (including XBox, and XBox 360)...
            #define EASTL_ASSUME(x) __assume(x)
        #else
            #define EASTL_ASSUME(x)
        #endif
    #endif



    // ------------------------------------------------------------------------
    // EASTL_PURE
    // 
    // This acts the same as the GCC __attribute__ ((pure)) directive and is
    // implemented simply as a wrapper around it to allow portable usage of 
    // it and to take advantage of it if and when it appears in other compilers.
    //
    // A "pure" function is one that has no effects except its return value and 
    // its return value is a function of only the function's parameters or 
    // non-volatile global variables. Any parameter or global variable access 
    // must be read-only. Loop optimization and subexpression elimination can be 
    // applied to such functions. A common example is strlen(): Given identical 
    // inputs, the function's return value (its only effect) is invariant across 
    // multiple invocations and thus can be pulled out of a loop and called but once.
    //
    // Example usage:
    //    EASTL_PURE void Function();
    //
    #ifndef EASTL_PURE
        #if defined(TARGET_COMPILER_GNUC)
            #define EASTL_PURE __attribute__((pure))
        #elif defined(__ARMCC_VERSION)  // Arm brand compiler for ARM CPU
            #define EASTL_PURE __pure
        #elif defined(TARGET_COMPILER_CLANG) && __has_attribute(pure)
            #define EASTL_PURE __attribute__((pure))
        #else
            #define EASTL_PURE
        #endif
    #endif



    // ------------------------------------------------------------------------
    // EASTL_WEAK
    // EASTL_WEAK_SUPPORTED -- defined as 0 or 1.
    // 
    // GCC
    // The weak attribute causes the declaration to be emitted as a weak
    // symbol rather than a global. This is primarily useful in defining
    // library functions which can be overridden in user code, though it
    // can also be used with non-function declarations.
    //
    // VC++
    // At link time, if multiple definitions of a COMDAT are seen, the linker 
    // picks one and discards the rest. If the linker option /OPT:REF 
    // is selected, then COMDAT elimination will occur to remove all the 
    // unreferenced data items in the linker output.
    //
    // Example usage:
    //    EASTL_WEAK void Function();
    //
    #ifndef EASTL_WEAK
        #if defined(_MSC_VER) && (_MSC_VER >= 1300) // If VC7.0 and later (including XBox)...
            #define EASTL_WEAK __declspec(selectany)
            #define EASTL_WEAK_SUPPORTED 1
        #elif defined(_MSC_VER) || (defined(__GNUC__) && defined(__CYGWIN__))
            #define EASTL_WEAK
            #define EASTL_WEAK_SUPPORTED 0
        #elif defined(__ARMCC_VERSION)  // Arm brand compiler for ARM CPU
            #define EASTL_WEAK __weak
            #define EASTL_WEAK_SUPPORTED 1
        #else                           // GCC and IBM compilers, others.
            #define EASTL_WEAK __attribute__((weak))
            #define EASTL_WEAK_SUPPORTED 1
        #endif
    #endif



    // ------------------------------------------------------------------------
    // wchar_t
    // Here we define:
    //    EASTL_WCHAR_T_NON_NATIVE
    //    EASTL_WCHAR_SIZE = <sizeof(wchar_t)>
    //
    #ifndef EASTL_WCHAR_T_NON_NATIVE
        // Compilers that always implement wchar_t as native include:
        //     COMEAU, new SN, and other EDG-based compilers.
        //     GCC
        //     Borland
        //     SunPro
        //     IBM Visual Age
        #if defined(TARGET_COMPILER_INTEL)
            #if (TARGET_COMPILER_VERSION < 700)
                #define EASTL_WCHAR_T_NON_NATIVE 1
            #else
                #if (!defined(_WCHAR_T_DEFINED) && !defined(_WCHAR_T))
                    #define EASTL_WCHAR_T_NON_NATIVE 1
                #endif
            #endif
        #elif defined(TARGET_COMPILER_MSVC) || defined(TARGET_COMPILER_BORLAND)
            #ifndef _NATIVE_WCHAR_T_DEFINED
                #define EASTL_WCHAR_T_NON_NATIVE 1
            #endif
        #elif defined(TARGET_COMPILER_METROWERKS)
            #if !__option(wchar_type)
                #define EASTL_WCHAR_T_NON_NATIVE 1
            #endif
        #elif defined(__SNC__) && !defined(__cplusplus) // If compiling C under SNC...
            #define EASTL_WCHAR_T_NON_NATIVE 1
        #endif
    #endif

    #ifndef EASTL_WCHAR_SIZE // If the user hasn't specified that it is a given size...
        #if defined(__WCHAR_MAX__) // GCC defines this for most platforms.
            #if (__WCHAR_MAX__ == 2147483647) || (__WCHAR_MAX__ == 4294967295)
                #define EASTL_WCHAR_SIZE 4
            #elif (__WCHAR_MAX__ == 32767) || (__WCHAR_MAX__ == 65535)
                #define EASTL_WCHAR_SIZE 2
            #elif (__WCHAR_MAX__ == 127) || (__WCHAR_MAX__ == 255)
                #define EASTL_WCHAR_SIZE 1
            #else
                #define EASTL_WCHAR_SIZE 4
            #endif
        #elif defined(WCHAR_MAX) // The SN and Arm compilers define this.
            #if (WCHAR_MAX == 2147483647) || (WCHAR_MAX == 4294967295)
                #define EASTL_WCHAR_SIZE 4
            #elif (WCHAR_MAX == 32767) || (WCHAR_MAX == 65535)
                #define EASTL_WCHAR_SIZE 2
            #elif (WCHAR_MAX == 127) || (WCHAR_MAX == 255)
                #define EASTL_WCHAR_SIZE 1
            #else
                #define EASTL_WCHAR_SIZE 4
            #endif
        #elif defined(_WCMAX) // The SN and Arm compilers define this.
            #if (_WCMAX == 2147483647) || (_WCMAX == 4294967295)
                #define EASTL_WCHAR_SIZE 4
            #elif (_WCMAX == 32767) || (_WCMAX == 65535)
                #define EASTL_WCHAR_SIZE 2
            #elif (_WCMAX == 127) || (_WCMAX == 255)
                #define EASTL_WCHAR_SIZE 1
            #else
                #define EASTL_WCHAR_SIZE 4
            #endif
        #elif defined(TARGET_PLATFORM_UNIX) || defined(TARGET_PLATFORM_PS3) || defined(TARGET_PLATFORM_PS3_SPU)
            // It is standard on Unix to have wchar_t be int32_t or uint32_t.
            // All versions of GNUC default to a 32 bit wchar_t, but has been used 
            // with the -fshort-wchar GCC command line option to force it to 16 bit.
            // If you know that the compiler is set to use a wchar_t of other than 
            // the default, you need to manually define EASTL_WCHAR_SIZE for the build.
            #define EASTL_WCHAR_SIZE 4
        #else
            // It is standard on Windows to have wchar_t be uint16_t.
            // Metrowerks and the new EDG-based SN compilers define wchar_t 
            // as uint16_t. Given that there is currently no known way to tell at preprocessor 
            // time what the size of wchar_t is, we declare it to be 2.
            // If you have EASTL_WCHAR_SIZE != sizeof(wchar_t), then your
            // code might not be broken, but it also won't work with wchar libraries
            // and data from other parts of EA. Under GCC, you can force wchar_t 
            // to two bytes with the -fshort-wchar compiler argument.
            #define EASTL_WCHAR_SIZE 2
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_RESTRICT
    // 
    // The C99 standard defines a new keyword, restrict, which allows for the 
    // improvement of code generation regarding memory usage. Compilers can
    // generate significantly faster code when you are able to use restrict.
    // 
    // Example usage:
    //    void DoSomething(char* EASTL_RESTRICT p1, char* EASTL_RESTRICT p2);
    //
    #ifndef EASTL_RESTRICT
        #if defined(TARGET_COMPILER_MSVC) && (TARGET_COMPILER_VERSION >= 1400) // If VC8 (VS2005) or later...
            #define EASTL_RESTRICT __restrict
        #elif defined(TARGET_COMPILER_GNUC)
            #define EASTL_RESTRICT __restrict // GCC defines 'restrict' (as opposed to __restrict) in C99 mode only.
        #elif defined(__ARMCC_VERSION)
            #define EASTL_RESTRICT __restrict
        #elif defined(TARGET_COMPILER_CLANG)
            #define EA_RESTRICT __restrict
        #elif defined(__MWERKS__)
            #if __option(c99)
                #define EASTL_RESTRICT restrict
            #else
                #define EASTL_RESTRICT
            #endif
        #elif defined(TARGET_COMPILER_IS_C99)
            #define EASTL_RESTRICT restrict
        #else
            // If the compiler didn't support restricted pointers, defining EASTL_RESTRICT 
            // away would result in compiling and running fine but you just wouldn't 
            // the same level of optimization. On the other hand, all the major compilers 
            // support restricted pointers.
            #define EASTL_RESTRICT
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_DEPRECATED            // Used as a prefix.
    // EASTL_PREFIX_DEPRECATED     // You should need this only for unusual compilers.
    // EASTL_POSTFIX_DEPRECATED    // You should need this only for unusual compilers.
    // 
    // Example usage:
    //    EASTL_DEPRECATED void Function();
    //
    // or for maximum portability:
    //    EASTL_PREFIX_DEPRECATED void Function() EASTL_POSTFIX_DEPRECATED;
    //
    #ifndef EASTL_DEPRECATED
        #if defined(TARGET_COMPILER_MSVC) && (TARGET_COMPILER_VERSION > 1300) // If VC7 (VS2003) or later...
            #define EASTL_DEPRECATED __declspec(deprecated)
        #elif defined(TARGET_COMPILER_MSVC)
            #define EASTL_DEPRECATED 
        #else
            #define EASTL_DEPRECATED __attribute__((deprecated))
        #endif
    #endif

    #ifndef EASTL_PREFIX_DEPRECATED
        #if defined(TARGET_COMPILER_MSVC) && (TARGET_COMPILER_VERSION > 1300) // If VC7 (VS2003) or later...
            #define EASTL_PREFIX_DEPRECATED __declspec(deprecated)
            #define EASTL_POSTFIX_DEPRECATED
        #elif defined(TARGET_COMPILER_MSVC)
            #define EASTL_PREFIX_DEPRECATED
            #define EASTL_POSTFIX_DEPRECATED
        #else
            #define EASTL_PREFIX_DEPRECATED
            #define EASTL_POSTFIX_DEPRECATED __attribute__((deprecated))
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_FORCE_INLINE              // Used as a prefix.
    // EASTL_PREFIX_FORCE_INLINE       // You should need this only for unusual compilers.
    // EASTL_POSTFIX_FORCE_INLINE      // You should need this only for unusual compilers.
    //
    // Example usage:
    //     EASTL_FORCE_INLINE void Foo();                                // Implementation elsewhere.
    //     EASTL_PREFIX_FORCE_INLINE void Foo() EASTL_POSTFIX_FORCE_INLINE; // Implementation elsewhere.
    //
    // Note that when the prefix version of this function is used, it replaces
    // the regular C++ 'inline' statement. Thus you should not use both the 
    // C++ inline statement and this macro with the same function declaration.
    //
    // To force inline usage under GCC 3.1+, you use this:
    //    inline void Foo() __attribute__((always_inline));
    //       or
    //    inline __attribute__((always_inline)) void Foo();
    //
    // The CodeWarrior compiler doesn't have the concept of forcing inlining per function.
    // 
    #ifndef EASTL_FORCE_INLINE
        #if defined(TARGET_COMPILER_MSVC)
            #define EASTL_FORCE_INLINE __forceinline
        #elif defined(TARGET_COMPILER_GNUC) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 301)
            #if defined(__cplusplus)
                #define EASTL_FORCE_INLINE inline __attribute__((always_inline))
            #else
                #define EASTL_FORCE_INLINE __inline__ __attribute__((always_inline))
            #endif
        #elif defined(TARGET_COMPILER_CLANG) && __has_attribute(always_inline)
            #if defined(__cplusplus)
                #define EASTL_FORCE_INLINE inline __attribute__((always_inline))
            #else
                #define EASTL_FORCE_INLINE __inline__ __attribute__((always_inline))
            #endif
        #else
            #if defined(__cplusplus)
                #define EASTL_FORCE_INLINE inline
            #else
                #define EASTL_FORCE_INLINE __inline
            #endif
        #endif
    #endif

    #if defined(TARGET_COMPILER_SN) && defined(TARGET_PLATFORM_PS3) // SN's implementation of always_inline is broken and sometimes fails to link the function.
        #define EASTL_PREFIX_FORCE_INLINE  inline
        #define EASTL_POSTFIX_FORCE_INLINE 
    #elif defined(TARGET_COMPILER_GNUC) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 301)
        #define EASTL_PREFIX_FORCE_INLINE  inline
        #define EASTL_POSTFIX_FORCE_INLINE __attribute__((always_inline))
    #else
        #define EASTL_PREFIX_FORCE_INLINE  inline
        #define EASTL_POSTFIX_FORCE_INLINE 
    #endif


    // ------------------------------------------------------------------------
    // EASTL_NO_INLINE             // Used as a prefix. 
    // EASTL_PREFIX_NO_INLINE      // You should need this only for unusual compilers.
    // EASTL_POSTFIX_NO_INLINE     // You should need this only for unusual compilers.
    //
    // Example usage:
    //     EASTL_NO_INLINE        void Foo();                       // Implementation elsewhere.
    //     EASTL_PREFIX_NO_INLINE void Foo() EASTL_POSTFIX_NO_INLINE;  // Implementation elsewhere.
    //
    // That this declaration is incompatbile with C++ 'inline' and any
    // variant of EASTL_FORCE_INLINE.
    //
    // To disable inline usage under VC++ priof to VS2005, you need to use this:
    //    #pragma inline_depth(0) // Disable inlining.
    //    void Foo() { ... }
    //    #pragma inline_depth()  // Restore to default.
    //
    // Since there is no easy way to disable inlining on a function-by-function
    // basis in VC++ prior to VS2005, the best strategy is to write platform-specific 
    // #ifdefs in the code or to disable inlining for a given module and enable 
    // functions individually with EASTL_FORCE_INLINE. 
    // 
    #ifndef EASTL_NO_INLINE
        #if defined(TARGET_COMPILER_MSVC) && (TARGET_COMPILER_VERSION >= 1400) // If VC8 (VS2005) or later...
            #define EASTL_NO_INLINE __declspec(noinline)
        #elif defined(TARGET_COMPILER_MSVC)
            #define EASTL_NO_INLINE
        #else
            #define EASTL_NO_INLINE __attribute__((noinline))
        #endif
    #endif

    #if defined(TARGET_COMPILER_MSVC) && (TARGET_COMPILER_VERSION >= 1400) // If VC8 (VS2005) or later...
        #define EASTL_PREFIX_NO_INLINE  __declspec(noinline)
        #define EASTL_POSTFIX_NO_INLINE
    #elif defined(TARGET_COMPILER_MSVC)
        #define EASTL_PREFIX_NO_INLINE
        #define EASTL_POSTFIX_NO_INLINE
    #else
        #define EASTL_PREFIX_NO_INLINE
        #define EASTL_POSTFIX_NO_INLINE __attribute__((noinline))
    #endif


    // ------------------------------------------------------------------------
    // EASTL_NO_VTABLE
    //
    // Example usage:
    //     class EASTL_NO_VTABLE X {
    //        virtual void InterfaceFunction();
    //     };
    // 
    //     EASTL_CLASS_NO_VTABLE(X) {
    //        virtual void InterfaceFunction();
    //     };
    //
    #ifdef TARGET_COMPILER_MSVC
        #define EASTL_NO_VTABLE           __declspec(novtable)
        #define EASTL_CLASS_NO_VTABLE(x)  class __declspec(novtable) x
        #define EASTL_STRUCT_NO_VTABLE(x) struct __declspec(novtable) x
    #else
        #define EASTL_NO_VTABLE
        #define EASTL_CLASS_NO_VTABLE(x)  class x
        #define EASTL_STRUCT_NO_VTABLE(x) struct x
    #endif


    // ------------------------------------------------------------------------
    // EASTL_PASCAL
    //
    // Also known on PC platforms as stdcall.
    // This convention causes the compiler to assume that the called function 
    // will pop off the stack space used to pass arguments, unless it takes a 
    // variable number of arguments. 
    //
    // Example usage:
    //    this:
    //       void DoNothing(int x);
    //       void DoNothing(int x){}
    //    would be written as this:
    //       void EASTL_PASCAL_FUNC(DoNothing(int x));
    //       void EASTL_PASCAL_FUNC(DoNothing(int x)){}
    // 
    #ifndef EASTL_PASCAL
        #if defined(TARGET_COMPILER_MSVC)
            #define EASTL_PASCAL __stdcall
        #elif defined(TARGET_COMPILER_GNUC) && defined(TARGET_PROCESSOR_X86)
            #define EASTL_PASCAL __attribute__((stdcall))
        #elif defined(TARGET_COMPILER_METROWERKS) && defined(TARGET_PLATFORM_WINDOWS)
            // You need to make sure you have the Metrowerks "ANSI keywords only' 
            // compilation option disabled for the pascal keyword to work.
            #define EASTL_PASCAL   pascal
        #else
            // Some compilers simply don't support pascal calling convention.
            // As a result, there isn't an issue here, since the specification of 
            // pascal calling convention is for the purpose of disambiguating the
            // calling convention that is applied.
            #define EASTL_PASCAL
        #endif
    #endif

    #ifndef EASTL_PASCAL_FUNC
        #if defined(TARGET_COMPILER_MSVC)
            #define EASTL_PASCAL_FUNC(funcname_and_paramlist)    __stdcall funcname_and_paramlist
        #elif defined(TARGET_COMPILER_GNUC) && defined(TARGET_PROCESSOR_X86)
            #define EASTL_PASCAL_FUNC(funcname_and_paramlist)    __attribute__((stdcall)) funcname_and_paramlist
        #elif defined(TARGET_COMPILER_METROWERKS) && defined(TARGET_PLATFORM_WINDOWS)
            #define EASTL_PASCAL_FUNC(funcname_and_paramlist)    pascal funcname_and_paramlist
        #else
            #define EASTL_PASCAL_FUNC(funcname_and_paramlist)    funcname_and_paramlist
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_SSE
    // Visual C Processor Packs define _MSC_FULL_VER and are needed for SSE
    // Intel C also has SSE support.
    // EASTL_SSE is used to select FPU or SSE versions in hw_select.inl
    #ifndef EASTL_SSE
        #if defined(TARGET_COMPILER_GNUC) || defined(TARGET_COMPILER_CLANG)
            #if defined(__SSE2__)
                #define EASTL_SSE 2
            #elif defined(__SSE__) && __SSE__
                #define EASTL_SSE 1
            #else
                #define EASTL_SSE 0
            #endif
        #elif defined(TARGET_PROCESSOR_X86) && defined(_MSC_FULL_VER) && !defined(__NOSSE__) && defined(_M_IX86_FP)
            #define EASTL_SSE _M_IX86_FP 
        #elif defined(TARGET_PROCESSOR_X86) && defined(TARGET_COMPILER_INTEL) && !defined(__NOSSE__)
            #define EASTL_SSE 1
        #else
            #define EASTL_SSE 0
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_IMPORT
    // import declaration specification
    // specifies that the declared symbol is imported from another dynamic library.
    #ifndef EASTL_IMPORT
        #if defined(TARGET_COMPILER_MSVC)
            #define EASTL_IMPORT __declspec(dllimport)
        #else
            #define EASTL_IMPORT
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_EXPORT
    // export declaration specification
    // specifies that the declared symbol is exported from the current dynamic library.
    // this is not the same as the C++ export keyword.
    #ifndef EASTL_EXPORT
        #if defined(TARGET_COMPILER_MSVC)
            #define EASTL_EXPORT __declspec(dllexport)
        #else
            #define EASTL_EXPORT
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_PRAGMA_ONCE_SUPPORTED
    // 
    // This is a wrapper for the #pragma once preprocessor directive.
    // It allows for some compilers (in particular VC++) to implement signifcantly
    // faster include file preprocessing. #pragma once can be used to replace 
    // header include guards or to augment them. However, #pragma once isn't 
    // necessarily supported by all compilers and isn't guaranteed to be so in
    // the future, so using #pragma once to replace traditional include guards 
    // is not strictly portable. Note that a direct #define for #pragma once is
    // impossible with VC++, due to limitations, but can be done with other 
    // compilers/preprocessors via _Pragma("once").
    // 
    // Example usage (which includes traditional header guards for portability):
    //    #ifndef SOMEPACKAGE_SOMEHEADER_H
    //    #define SOMEPACKAGE_SOMEHEADER_H
    //
    //    #if defined(EASTL_PRAGMA_ONCE_SUPPORTED)
    //        #pragma once
    //    #endif
    //
    //    <user code> 
    //
    //    #endif
    //
    #if defined(_MSC_VER) || defined(__MWERKS__) || defined(__GNUC__) || defined(__SNC__) || defined(__ICC) || defined(__ICL) || defined(__clang__)
        #define EASTL_PRAGMA_ONCE_SUPPORTED 1
    #endif


    // ------------------------------------------------------------------------
    // EASTL_OVERRIDE
    // 
    //  See http://msdn.microsoft.com/en-us/library/41w3sh1c.aspx for more information.
    // 
    #ifndef EASTL_OVERRIDE
        #if defined(TARGET_COMPILER_MSVC) && (TARGET_COMPILER_VERSION >= 1400) // VS2005 (VC8) and later
            #define EASTL_OVERRIDE override
        #else
            #define EASTL_OVERRIDE 
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_SEALED
    // 
    // See http://msdn.microsoft.com/en-us/library/49k3w2fx%28VS.71%29.aspx for more information.
    // 
    #ifndef EASTL_SEALED
        #if defined(TARGET_COMPILER_MSVC) && (TARGET_COMPILER_VERSION >= 1400) // VS2005 (VC8) and later
            #define EASTL_SEALED sealed
        #else
            #define EASTL_SEALED 
        #endif
    #endif


    // ------------------------------------------------------------------------
    // EASTL_ABSTRACT
    // 
    // See http://msdn.microsoft.com/en-us/library/49k3w2fx%28VS.71%29.aspx for more information.
    // 
    #ifndef EASTL_ABSTRACT
        #if defined(TARGET_COMPILER_MSVC) && (TARGET_COMPILER_VERSION >= 1400) // VS2005 (VC8) and later
            #define EASTL_ABSTRACT abstract
        #else
            #define EASTL_ABSTRACT 
        #endif
    #endif


#endif // Header include guard










