
#ifndef EASTL_DEBUG_H
#define EASTL_DEBUG_H

#include <eastl/internal/config.h>

namespace eastl {
void printDebugString(char *text, ...);
void fatalError(char *text,...);
};

#endif
