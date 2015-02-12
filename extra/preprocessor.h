
#ifndef __EASTL_EXTRA_PREPROCESSOR_H__
#define __EASTL_EXTRA_PREPROCESSOR_H__


// Macro that offers run-time and preprocessor-time checks to know if some macros are defined
// Assumption: When existing, macros are defined to be 1
//
// Compiles and works both under GCC and MSVC (and expected to work under most other environments)
//
// Response to Linus Torvalds' G+ challenge: https://plus.google.com/u/0/102150693225130002912/posts/9gntjh57dXt
//
// from David Ramos
// kudos to Jean-Noel Avila for simplifying my initial implementation
// example:
// #define CONFIG_EXISTS 1
//   if(isPreprocessorEnabled(CONFIG_DOESNT_EXIST))
//      printf("Enabled! RUNTIME\n");
// #if isPreprocessorEnabled(CONFIG_EXISTS)
//   printf("Enabled! PREPROCESSOR\n");
// #endif

#define internal_preproc_donothing(...)
#define internal_preproc___open (
#define internal_preproc_close__ )
/* This macro below is so it works on GCC. In MSVC it would be enough with just
   doing "#define is_enabled1(x) (donothing __open helper_##x close__) + 0)" */
#define internal_preproc_macro_caller(macro, args) macro args
#define internal_preproc_helper_1 internal_preproc_close__ internal_preproc___open 1
#define internal_preproc_is_enabled1(x) (internal_preproc_macro_caller(internal_preproc_donothing, internal_preproc___open internal_preproc_helper_##x internal_preproc_close__) + 0)
#define isPreprocessorEnabled(x) internal_preproc_is_enabled1(x)



// http://www.codersnotes.com/notes/easy-preprocessor-defines
// #define BIG_ENDIAN EASTL_ON
// #if EASTL_USING(BIG_ENDIAN)
//   ...
// #endif
// The advantage here is that a macro is forced to either be defined to ON or OFF; if
// you don’t define it all you’ll get a compiler error.
#define EASTL_ON 2-
#define EASTL_OFF 1-
#define EASTL_USING(x) ( (x 0) == 2)


// by Stefan Reinalter
// http://molecularmusings.wordpress.com/2011/07/12/a-plethora-of-macros/
// http://molecularmusings.wordpress.com/2011/07/22/an-improved-assert/

/// allows member functions to be made abstract. uses nonstandard C++ extensions provided by MSVC
#define EASTL_ABSTRACT    abstract

/// marks member functions as being an override of a base class virtual function. uses nonstandard C++ extensions provided by MSVC
#define EASTL_OVERRIDE    override

/// allows classes and member functions to be made sealed. uses nonstandard C++ extensions provided by MSVC
#define EASTL_FINAL       sealed

/// allows to emit pragmas from within macros
#define EASTL_PRAGMA(pragma)    __pragma(pragma)

/// support for C99 restrict keyword
#define EASTL_RESTRICT    __restrict

/// tells the compiler that the return value (RV) of a function is an object that will not be aliased with any other pointers
#define EASTL_RESTRICT_RV    __declspec(restrict)

/// tells the compiler that a function call does not modify or reference visible global state and only modifies the memory pointed to directly by pointer parameters
#define EASTL_NO_ALIAS    __declspec(noalias)

/// forces a function to be inlined
#define EASTL_INLINE    __forceinline

/// tells the compiler to never inline a particular function
#define EASTL_NO_INLINE    __declspec(noinline)

/// passes optimization hints to the compiler
#define EASTL_HINT(hint)    __assume(hint)

/// used in switch-statements whose default-case can never be reached, resulting in more optimal code
#define EASTL_NO_SWITCH_DEFAULT    EASTL_HINT(0)

/// stringizes a string, even macros
#define EASTL_STRINGIZE_HELPER(token)    #token
#define EASTL_STRINGIZE(str)             EASTL_STRINGIZE_HELPER(str)

/// concatenates tokens, even when the tokens are macros themselves
#define EASTL_JOIN_HELPER_HELPER(_0, _1)		_0##_1
#define EASTL_JOIN_HELPER(_0, _1)				EASTL_JOIN_HELPER_HELPER(_0, _1)
#define EASTL_JOIN_IMPL(_0, _1)					EASTL_JOIN_HELPER(_0, _1)


#define EASTL_JOIN_2(_0, _1)																	EASTL_JOIN_IMPL(_0, _1)
#define EASTL_JOIN_3(_0, _1, _2)																EASTL_JOIN_2(EASTL_JOIN_2(_0, _1), _2)
#define EASTL_JOIN_4(_0, _1, _2, _3)															EASTL_JOIN_2(EASTL_JOIN_3(_0, _1, _2), _3)
#define EASTL_JOIN_5(_0, _1, _2, _3, _4)														EASTL_JOIN_2(EASTL_JOIN_4(_0, _1, _2, _3), _4)
#define EASTL_JOIN_6(_0, _1, _2, _3, _4, _5)													EASTL_JOIN_2(EASTL_JOIN_5(_0, _1, _2, _3, _4), _5)
#define EASTL_JOIN_7(_0, _1, _2, _3, _4, _5, _6)												EASTL_JOIN_2(EASTL_JOIN_6(_0, _1, _2, _3, _4, _5), _6)
#define EASTL_JOIN_8(_0, _1, _2, _3, _4, _5, _6, _7)											EASTL_JOIN_2(EASTL_JOIN_7(_0, _1, _2, _3, _4, _5, _6), _7)
#define EASTL_JOIN_9(_0, _1, _2, _3, _4, _5, _6, _7, _8)										EASTL_JOIN_2(EASTL_JOIN_8(_0, _1, _2, _3, _4, _5, _6, _7), _8)
#define EASTL_JOIN_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9)									EASTL_JOIN_2(EASTL_JOIN_9(_0, _1, _2, _3, _4, _5, _6, _7, _8), _9)
#define EASTL_JOIN_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10)								EASTL_JOIN_2(EASTL_JOIN_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9), _10)
#define EASTL_JOIN_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11)							EASTL_JOIN_2(EASTL_JOIN_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10), _11)
#define EASTL_JOIN_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12)					EASTL_JOIN_2(EASTL_JOIN_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11), _12)
#define EASTL_JOIN_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13)				EASTL_JOIN_2(EASTL_JOIN_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12), _13)
#define EASTL_JOIN_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14)			EASTL_JOIN_2(EASTL_JOIN_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13), _14)
#define EASTL_JOIN_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15)		EASTL_JOIN_2(EASTL_JOIN_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14), _15)

/// chooses a value based on a condition
#define EASTL_IF_0(t, f)			f
#define EASTL_IF_1(t, f)			t
#define EASTL_IF(cond, t, f)		EASTL_JOIN_2(EASTL_IF_, EASTL_TO_BOOL(cond))(t, f)


/// converts a condition into a boolean 0 (=false) or 1 (=true)
#define EASTL_TO_BOOL_0 0
#define EASTL_TO_BOOL_1 1
#define EASTL_TO_BOOL_2 1
#define EASTL_TO_BOOL_3 1
#define EASTL_TO_BOOL_4 1
#define EASTL_TO_BOOL_5 1
#define EASTL_TO_BOOL_6 1
#define EASTL_TO_BOOL_7 1
#define EASTL_TO_BOOL_8 1
#define EASTL_TO_BOOL_9 1
#define EASTL_TO_BOOL_10 1
#define EASTL_TO_BOOL_11 1
#define EASTL_TO_BOOL_12 1
#define EASTL_TO_BOOL_13 1
#define EASTL_TO_BOOL_14 1
#define EASTL_TO_BOOL_15 1
#define EASTL_TO_BOOL_16 1

#define EASTL_TO_BOOL(x)		EASTL_JOIN_2(EASTL_TO_BOOL_, x)

/// Returns 1 if the arguments to the variadic macro are separated by a comma, 0 otherwise.
#define EASTL_HAS_COMMA(...)							EASTL_HAS_COMMA_EVAL(EASTL_HAS_COMMA_ARGS(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))
#define EASTL_HAS_COMMA_EVAL(...)						__VA_ARGS__
#define EASTL_HAS_COMMA_ARGS(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16


/// Returns 1 if the argument list to the variadic macro is empty, 0 otherwise.
#define EASTL_IS_EMPTY(...)														\
   EASTL_HAS_COMMA                                                   \
   (                                                                 \
      EASTL_JOIN_5                                                   \
      (                                                              \
         EASTL_IS_EMPTY_CASE_,                                       \
         EASTL_HAS_COMMA(__VA_ARGS__),										   \
         EASTL_HAS_COMMA(EASTL_IS_EMPTY_BRACKET_TEST __VA_ARGS__),	\
         EASTL_HAS_COMMA(__VA_ARGS__ (~)),									\
         EASTL_HAS_COMMA(EASTL_IS_EMPTY_BRACKET_TEST __VA_ARGS__ (~))\
      )                                                              \
   )

#define EASTL_IS_EMPTY_CASE_0001			,
#define EASTL_IS_EMPTY_BRACKET_TEST(...)	,


// Example of EASTL_VA_NUM_ARGS
// #define MAX_2(a,b)   std::max(a,b)
// #define MAX_3(a,b,c) MAX_2(MAX_2(a,b),c)
// #define MAX(...)  EASTL_JOIN(MAX_, EASTL_VA_NUM_ARGS(__VA_ARGS__)) EASTL_PASS_VA(__VA_ARGS__)

// EASTL_VA_NUM_ARGS() is a very nifty macro to retrieve the number of arguments handed to a variable-argument macro.
// unfortunately, VS 2010 still has this preprocessor bug which treats a __VA_ARGS__ argument as being one single parameter:
// https://connect.microsoft.com/VisualStudio/feedback/details/521844/variadic-macro-treating-va-args-as-a-single-parameter-for-other-macros#details
#if _MSC_VER >= 1400
#	define EASTL_VA_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...)	N
#	define EASTL_VA_NUM_ARGS_REVERSE_SEQUENCE			16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
#	define EASTL_VA_NUM_ARGS_LEFT (
#	define EASTL_VA_NUM_ARGS_RIGHT )
#	define EASTL_VA_NUM_ARGS(...)						EASTL_VA_NUM_ARGS_HELPER EASTL_VA_NUM_ARGS_LEFT __VA_ARGS__, EASTL_VA_NUM_ARGS_REVERSE_SEQUENCE EASTL_VA_NUM_ARGS_RIGHT
#else
#	define EASTL_VA_NUM_ARGS(...)						EASTL_VA_NUM_ARGS_HELPER(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#	define EASTL_VA_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...)	N
#endif

// EASTL_NUM_ARGS correctly handles the case of 0 arguments
#define EASTL_NUM_ARGS(...)								EASTL_IF(EASTL_IS_EMPTY(__VA_ARGS__), 0, EASTL_VA_NUM_ARGS(__VA_ARGS__))


// EASTL_PASS_ARGS passes __VA_ARGS__ as multiple parameters to another macro, working around the following bug:
// https://connect.microsoft.com/VisualStudio/feedback/details/521844/variadic-macro-treating-va-args-as-a-single-parameter-for-other-macros#details
#if _MSC_VER >= 1400
#	define EASTL_PASS_ARGS_LEFT (
#	define EASTL_PASS_ARGS_RIGHT )
#	define EASTL_PASS_ARGS(...)							EASTL_PASS_ARGS_LEFT __VA_ARGS__ EASTL_PASS_ARGS_RIGHT
#else
#	define EASTL_PASS_ARGS(...)							(__VA_ARGS__)
#endif

/// Expand any number of arguments into a list of operations called with those arguments
#define EASTL_EXPAND_ARGS_0(op, empty)
#define EASTL_EXPAND_ARGS_1(op, a1)																			op(a1, 0)
#define EASTL_EXPAND_ARGS_2(op, a1, a2)																		op(a1, 0) op(a2, 1)
#define EASTL_EXPAND_ARGS_3(op, a1, a2, a3)																	op(a1, 0) op(a2, 1) op(a3, 2)
#define EASTL_EXPAND_ARGS_4(op, a1, a2, a3, a4)																op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3)
#define EASTL_EXPAND_ARGS_5(op, a1, a2, a3, a4, a5)															op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4)
#define EASTL_EXPAND_ARGS_6(op, a1, a2, a3, a4, a5, a6)														op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5)
#define EASTL_EXPAND_ARGS_7(op, a1, a2, a3, a4, a5, a6, a7)													op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6)
#define EASTL_EXPAND_ARGS_8(op, a1, a2, a3, a4, a5, a6, a7, a8)												op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7)
#define EASTL_EXPAND_ARGS_9(op, a1, a2, a3, a4, a5, a6, a7, a8, a9)											op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8)
#define EASTL_EXPAND_ARGS_10(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)									op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8) op(a10, 9)
#define EASTL_EXPAND_ARGS_11(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)								op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8) op(a10, 9) op(a11, 10)
#define EASTL_EXPAND_ARGS_12(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)							op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8) op(a10, 9) op(a11, 10) op(a12, 11)
#define EASTL_EXPAND_ARGS_13(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)					op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8) op(a10, 9) op(a11, 10) op(a12, 11) op(a13, 12)
#define EASTL_EXPAND_ARGS_14(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)				op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8) op(a10, 9) op(a11, 10) op(a12, 11) op(a13, 12) op(a14, 13)
#define EASTL_EXPAND_ARGS_15(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)			op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8) op(a10, 9) op(a11, 10) op(a12, 11) op(a13, 12) op(a14, 13) op(a15, 14)
#define EASTL_EXPAND_ARGS_16(op, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16)		op(a1, 0) op(a2, 1) op(a3, 2) op(a4, 3) op(a5, 4) op(a6, 5) op(a7, 6) op(a8, 7) op(a9, 8) op(a10, 9) op(a11, 10) op(a12, 11) op(a13, 12) op(a14, 13) op(a15, 14) op(a16, 15)

#define EASTL_EXPAND_ARGS(op, ...)		EASTL_JOIN_2(EASTL_EXPAND_ARGS_, EASTL_NUM_ARGS(__VA_ARGS__)) EASTL_PASS_ARGS(op, __VA_ARGS__)

/// Turns any legal C++ expression into nothing
#define EASTL_UNUSED_IMPL(symExpr, n)	, (void)sizeof(symExpr)
#define EASTL_UNUSED(...)					(void)sizeof(true) EASTL_EXPAND_ARGS EASTL_PASS_ARGS(EASTL_UNUSED_IMPL, __VA_ARGS__)


#endif   //#ifndef __EASTL_EXTRA_PREPROCESSOR_H__
