
#ifndef __EASTL_EXTRA_SCOPEGUARD_H__
#define __EASTL_EXTRA_SCOPEGUARD_H__

// This needs C++11!

// taken from Andrei Alexandrescu
//   slides here: https://skydrive.live.com/?cid=f1b8ff18a2aec5c5&id=F1B8FF18A2AEC5C5%211158

/* example:
 *  void fun() {
 *     char name[] = "/tmp/filexxx";
 *     auto fd = mkstemp(name);
 *     auto g1 = scopeGuard([] {
 *        fclose(fd);
 *        unlink(name);
 *     });
 *     auto buf = malloc(1024*1024);
 *     auto g2 = scopeGuard([] { free(buf); });
 *  }
 * or
 *  void fun() {
 *     ...
 *     // LOOK AT THE TRAILING ';' !!
 *     SCOPE_EXIT { fclose(fd); unlink(name); };
 *     ...
 *     SCOPE_EXIT { free(buf); };
 *  }
 */
template<class Function> class ScopeGuard {
   Function mFunction;
   bool mActive;
public:
   ScopeGuard(Function f)
      : mFunction(std::move(f))
      , mActive(true)
   { }
   ~ScopeGuard() {
      if(mActive)
         mFunction();
   }
   void dismiss() {
      mActive = false;
   }

   ScopeGuard() = delete;
   ScopeGuard(const ScopeGuard&) = delete;
   ScopeGuard& operator=(const ScopeGuard&) = delete;
   ScopeGuard(ScopeGuard &&rhs)
      : mFunction(std::move(rhs.mFunction))
      , mActive(rhs.mActive)
   {
      rhs.dismiss();
   }
};

//Type deduction
template<class Function> ScopeGuard<Function> scopeGuard(Function f) {
   return ScopeGuard<Function>(std::move(f));
}

namespace detail {
   enum class ScopeGuardOnExit {};
   template<typename Function> ScopeGuard<Function> operator+(ScopeGuardOnExit, Function &&fn) {
      return ScopeGuard<Function>(std::forward<Function>(fn));
   }
}

#define SCOPE_EXIT auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) = ::detail::ScopeGuardOnExit() + [&]()
#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1,s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
   #define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __COUNTER__)
#else
   #define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)
#endif

#endif   //#ifndef __EASTL_EXTRA_SCOPEGUARD_H__
