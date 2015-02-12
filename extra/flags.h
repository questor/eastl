
#ifndef __EASTL_EXTRA_FLAGS_H__
#define __EASTL_EXTRA_FLAGS_H__

#include "EASTL/internal/config.h"
#include "eastl/extra/preprocessor.h"

// by Stefan Reinalter
// http://molecularmusings.wordpress.com/2011/08/23/flags-on-steroids/

//Example:
// EASTL_DECLARE_FLAGS(PlayerState, Dazed, Stunned, Killed, Floating);
// PlayerState state;
// state.set(PlayerState::Dazed);
// state.remove(PlayerState::Dazed);
// PlayerFlags::Description desc = {};
// printf("flags is %s\n", state.toString(desc));

namespace eastl {

template <class T> class Flags {
  typedef typename T::Enum Enum;
  typedef typename T::Bits Bits;

public:
  typedef char Description[512];

  inline Flags(void)
    : mFlags(0)
  { }

  inline explicit Flags(Enum flag)
    : mFlags(flag)
  { }

  inline void set(Enum flag) {
    mFlags |= flag;
  }

  inline void remove(Enum flag) {
    mFlags &= ~flag;
  }

  inline void clear(void) {
    mFlags = 0;
  }

  inline bool isSet(Enum flag) const {
    return ((mFlags & flag) != 0);
  }

  inline bool isAnySet(void) const {
    return (mFlags != 0);
  }

  inline bool areAllSet(void) const {
    return (mFlags == ((1ull << T::Count) - 1u));
  }

  const char* toString(Description& description) const {
    int offset = 0;
    for (size_t i=0; i<T::Count; ++i) {
      if ((mFlags & (1u << i)) != 0) {
        offset += _snprintf_s(description + offset, sizeof(description) - offset, _TRUNCATE, "%s, ", T::ToString(1u << i));
      }
    }
    // remove the trailing comma, if any
    if (offset > 1)
      description[offset-2] = 0;

    return description;
  }

  inline Flags operator|(Flags other) const {
    return Flags(mFlags | other.mFlags);
  }

  inline Flags& operator|=(Flags other) {
    mFlags |= other.mFlags;
    return *this;
  }

private:
  inline explicit Flags(uint32_t flags)
    : mFlags(flags)
  { }

  union {
    uint32_t mFlags;
    Bits mBits;
  };
};

#define EASTL_DECLARE_FLAGS_ENUM(name, n)         name = (1u << n),
#define EASTL_DECLARE_FLAGS_BITS(name, n)         uint32_t name : 1;
#define EASTL_DECLARE_FLAGS_TO_STRING(name, n)    case name: return EASTL_STRINGIZE(name);

#define EASTL_DECLARE_FLAGS(name, ...)         \
   struct name {      \
     static const size_t Count = EASTL_VA_NUM_ARGS(__VA_ARGS__);        \
     static_assert(Count != 0, "empty enum found"); \
     enum Enum {        \
       EASTL_EXPAND_ARGS EASTL_PASS_ARGS(EASTL_DECLARE_FLAGS_ENUM, __VA_ARGS__)         \
     };         \
     struct Bits {        \
       EASTL_EXPAND_ARGS EASTL_PASS_ARGS(EASTL_DECLARE_FLAGS_BITS, __VA_ARGS__)         \
     };         \
     static const char* ToString(size_t value) { \
       switch (value) { \
         EASTL_EXPAND_ARGS EASTL_PASS_ARGS(EASTL_DECLARE_FLAGS_TO_STRING, __VA_ARGS__)         \
         default: \
           EASTL_NO_SWITCH_DEFAULT; \
       } \
     }   \
   };    \
   inline eastl::Flags<name> operator|(name::Enum lhs, name::Enum rhs) {         \
     return (eastl::Flags<name>(lhs) | eastl::Flags<name>(rhs));         \
   }

}  //namespace eastl

#endif //#ifndef __EASTL_EXTRA_FLAGS_H__
