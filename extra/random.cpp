
#include "random.h"

double eastl::gauss() {
   static unsigned long seed = 61829450;
   double sum = 0;
   for(int i=0; i<3; ++i) {
      unsigned long hold = seed;
      seed ^= seed<<13; seed ^= seed>>17; seed ^= seed << 5;
      long r = hold+seed;
      sum += (double)r * (1.0/0x7fffffff);
   }
   return sum; //[-3.0;3.0]
}
