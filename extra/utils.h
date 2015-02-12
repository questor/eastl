
#ifndef EASTL_EXTRA_UTILS_H_
#define EASTL_EXTRA_UTILS_H_

namespace eastl {

   //defined in crtdefs.h
   #ifndef _UNALIGNED
      #if defined(_M_X64) || defined(_M_ARM)
         #define _UNALIGNED __unaligned
      #else
         #define _UNALIGNED
      #endif
   #endif

   #if !defined(countof)
      #if !defined(__cplusplus)
         #define countof(_Array) (sizeof(_Array) / sizeof(0[_Array]))
      #else
      extern "C++"
         {
            // http://www.reedbeta.com/blog/2013/07/10/cpp-compile-time-array-size/
            template <typename T, int N> char(&countof_helper(T(&)[N]))[N];
            #define countof(x) (sizeof(countof_helper(x)))
         }
      #endif
   #endif

   // taken from nedmalloc
/*
explanation of the locality parameter(http://lwn.net/Articles/255364/)
enum _mm_hint
{
  _MM_HINT_T0 = 3,
  _MM_HINT_T1 = 2,
  _MM_HINT_T2 = 1,
  _MM_HINT_NTA = 0
};
The different hints to be used with the _mm_prefetch intrinsic are implementation defined. That 
means each processor version can implement them (slightly) differently. What can generally be said 
is that _MM_HINT_T0 fetches data to all levels of the cache for inclusive caches and to the lowest 
level cache for exclusive caches. If the data item is in a higher level cache it is loaded into 
L1d. The _MM_HINT_T1 hint pulls the data into L2 and not into L1d. If there is an L3 cache the 
_MM_HINT_T2 hints can do something similar for it. These are details, though, which are weakly 
specified and need to be verified for the actual processor in use. In general, if the data is to 
be used right away using _MM_HINT_T0 is the right thing to do. Of course this requires that the L1d 
cache size is large enough to hold all the prefetched data. If the size of the immediately used 
working set is too large, prefetching everything into L1d is a bad idea and the other two hints 
should be used.

The fourth hint, _MM_HINT_NTA, is special in that it allows telling the processor to treat the 
prefetched cache line specially. NTA stands for non-temporal aligned which we already explained 
in Section 6.1. The program tells the processor that polluting caches with this data should be 
avoided as much as possible since the data is only used for a short time. The processor can therefore, 
upon loading, avoid reading the data into the lower level caches for inclusive cache implementations. 
When the data is evicted from L1d the data need not be pushed into L2 or higher but, instead, can be 
written directly to memory. There might be other tricks the processor designers can deploy if this 
hint is given. The programmer must be careful using this hint: if the immediate working set size is 
too large and forces eviction of a cache line loaded with the NTA hint, reloading from memory will 
occur.
*/

   #if (defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)))
     #define PREFETCHCACHELINE(addr, rw, locality) __builtin_prefetch((const void *)(addr), (rw), (locality))
     #define STRUCTUREALIGNMENT(alignment)         __attribute__ ((aligned(alignment)))
   #elif (defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64)))
     #define PREFETCHCACHELINE(addr, rw, locality) _mm_prefetch((const char *)(addr), (locality))
     #define STRUCTUREALIGNMENT(alignment)         __declspec(align(alignment))
   #endif

};

#endif
    
