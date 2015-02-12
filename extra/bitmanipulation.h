
#ifndef EASTL_EXTRA_BITMANIPULATION_H_
#define EASTL_EXTRA_BITMANIPULATION_H_

namespace eastl {
   /*
   #define STR_ME( X ) ( # X )
   namespace GameFlowType {
      enum Enum {
         Invalid,
         Run,
         Exit,
         Restart,
         Restore
      };
      const char* ToStr[] = {
         STR_ME( Invalid ),
         STR_ME( Run ),
         STR_ME( Exit ),
         STR_ME( Restart ),
         STR_ME( Restore )
      };
   };
   */

   /*
   class Blub { ... };
   typedef Blub PlatformDevice;

   extern Blub gGlobalBlub;
   PlatformDevice &getDevice() {
      return gGlobalBlub;
   }

      PlatformDevice &device=getDevice();
      device.init(...)

   ////////////////////////////////////////

   class Base { ... }
   class Blub : public Base { ... }
   class Bla : public Base { ... }
   typedef Base PlatformDevice;

   //must be initialized base on settings..
   extern Base *gGlobalDevice;
   PlatformDevice &getDevice() {
      return *gGlobalDevice;
   }

      PlatformDevice &device=getDevice();
      device.init(...)

   Enforcing Interfaces:
   #ifdef _DEBUG
      #define RENDER_INTERFACE(class) private class
   #else
      class StubClass {};
      #define RENDER_INTERFACE(class) private StubClass
   #endif

   class Blub : RENDER_INTERFACE(BlubBase) { ... }
   */

   //average of integers that do not overlow:
   inline int intAverage(int x, int y) {
      return (x&y)+((x^y)>>1);
   }

   //integer divide with ceiling
   inline int intCeiling(int a, int b) {
      return (a+b-1)/b;
   }

   //integer divide with round to nearest
   inline int intRoundToNearest(int a, int b) {
      return (a+(b>>1))/b;
   }

   //integer min (32bit)
   inline int intMin(int x, int y) {
      return x+(((y-x)>>(32-1))&(y-x));
   }
   //integer max (32bit)
   inline int intMax(int x, int y) {
      return x-(((x-y)>>(32-1))&(x-y));
   }

   inline int reverseBits(unsigned int x) {
      unsigned int y = 0x55555555;
      x = (((x >> 1) & y) | ((x & y) << 1));
      y = 0x33333333;
      x = (((x >> 2) & y) | ((x & y) << 2));
      y = 0x0f0f0f0f;
      x = (((x >> 4) & y) | ((x & y) << 4));
      y = 0x00ff00ff;
      x = (((x >> 8) & y) | ((x & y) << 8));
      return((x >> 16) | (x << 16));
   }

   inline bool isPowerOf2(const int number) {
      return (number & (number - 1)) == 0;
   }

   int getNextPowerOf2(int number);

};

#endif
