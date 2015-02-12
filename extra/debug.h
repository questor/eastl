
#ifndef EASTL_DEBUG_H
#define EASTL_DEBUG_H

#include <eastl/internal/config.h>
#include <eastl/EAAssert/eaassert.h>

namespace eastl {
void printDebugString(const char *text, ...);
void fatalError(const char *text,...);
};

#endif
