
#include "eastl/extra/bitmanipulation.h"

int eastl::getNextPowerOf2(int number) {
   //Warning: result if number = 0 is 0! (not 1)
   --number;
   number |= number >> 1;
   number |= number >> 2;
   number |= number >> 4;
   number |= number >> 8;
   number |= number >> 16;
#ifdef _ENABLE_64BIT
   number |= number >> 16;
   number |= number >> 16;
#endif
   return number+1;
}
