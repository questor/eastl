/*  _______         __              
   |_     _|.-----.|  |_.-----.----.
    _|   |_ |     ||   _|  -__|   _|
   |_______||__|__||____|_____|__|  
       coded by Questor / Inter      */

/* Some notes about generators (http://cbloomrants.blogspot.com/2009/02/02-19-09-two-code-gems.html):
   - First of all, with a lot of random number generators, the low bits are the worst, faster and better is:
         unsigned int rand_count(int size) {
             unsigned int r = rand();
             return (unsigned int) ( ( (uint64) r * size) >> 32 );
         }
      which throw away the lower bits...

*/

#ifndef EASTL_EXTRA_RANDOM_H__
#define EASTL_EXTRA_RANDOM_H__

#include "eastl/types.h"
#include "eastl/extra/debug.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

namespace eastl {

// randMod returns in [0, range-1] taken from http://cbloomrants.blogspot.com/2009/02/02-19-09-two-code-gems.html
template<class Random> uint32_t randMod(Random &randGen, uint32_t range ) {
    ASSERT( range > 0 ); // infinite loop

    uint32_t mask = range | (range>>1) | (range>>2) | (range>>3);
    mask = mask | (mask>>4);
    mask = mask | (mask>>8);
    mask = mask | (mask>>16);
    // bsr would be faster

    for(;;) {
        uint32_t rand32 = randGen.rand();    //32bit-random-value
        uint32_t randInMask = rand32 & mask;
        ASSERT( randInMask < range*2 );
        // > 50% chance of passing the test so iterations should be rare
        if ( randInMask < range )
            return randInMask;
    }
}

template<class Random> uint32_t randMod2(Random &randGen, uint32_t startRange, uint32_t endRange) {
   double v = (static_cast<double>(randGen.rand())) / RAND_MAX;
   return static_cast<long>(v*(endRange-startRange+1)+startRange);
}

template<class Random> void twoGauss(Random &randGen, float &val1, float &val2) {
   float x1, x2, w;
   do {
      x1 = 2.0f * randGen.randf() - 1.0f;
      x2 = 2.0f * randGen.randf() - 1.0f;
      w = x1*x1+x2*x2;
   } while (w >= 1.0f);
   w = sqrt((-2.0f*log(w))/w);
   val1 = x1*w;
   val2 = x2*w;
}

double gauss();

template<class Random> double gauss(Random &randGen) {
   double sum = 0;
   for(int i=0; i<3; ++i) {
      float r = randGen.randf();
      sum += (double)r * (1.0/0x7fffffff);
   }
   return sum; //[-3.0;3.0]
}

/**
 * Linear Congruential Random Number Generator (approx cycle 2^^32)
        value = a*value+c
   values for a and c:
   214013    2531011
   17405     10395331
   214013    13737667
   69069     1
 */
class LCRG {
public:
   /// constructor with defaultseed of 42
   LCRG() { mCurrentValue = 42; }
   /// constructor with specified start-seed
   /// \param seed   value to seed the random number generator
   LCRG(int seed) { mCurrentValue = seed; }

   /// set seed-value of the random number generator
   /// \param seed   value to seed the random number generator
   void srand(int seed) {
      mCurrentValue = seed;
   }
   /// generate 15bit random number value
   /// \return random number in the range 0..+32767 (only positiv numbers, 15bit)
   int rand16bit() {
      mCurrentValue = mCurrentValue * 214013L + 2531011L;
      return (mCurrentValue>>16) & 0x7fff;
   }
   /// generate 32bit random number value
   /// \return random number in the range -2^31..2^31
   int rand() {
      mCurrentValue = mCurrentValue * 214013L + 2531011L;
      return mCurrentValue;
   }
private:
   int mCurrentValue;
};

/**
 * Linear Congruential Random Number Generator (approx cycle 2^^32) for Floatingpoint Values
 */
class LCRGF {
public:
   /// constructor with defaultseed of 1
   LCRGF() { mCurrentValue = 1; }
   /// constructor with specified start-seed
   /// \param seed   value to seed the random number generator; the seed MUST not be 0!
   LCRGF(int seed) {
      ASSERT(seed != 0);
      mCurrentValue = seed;
   }    //seed MUST not be 0!

   /// set seed-value of the random number generator
   /// \param seed   value to seed the random number generator
   void srand(int seed) {
      mCurrentValue = seed;
   }

   /// generator random number
   /// based on this: http://code4k.blogspot.com/2009/10/random-float-number-using-x86-asm-code.html#more
   /// \return random number in the range (-1..+1]
   float randf1to1() {
      mCurrentValue *= 16807;
      return ((float)mCurrentValue) / (float)0x80000000;
   }

   /// generator random number
   /// taken from iq/rgba(http://rgba.scenesp.org/articles/sfrand/sfrand.htm)
   /// \return random number in the range [0..+1)
   float randf() {
      unsigned int a;
      mCurrentValue *= 16807;
      a = (mCurrentValue&0x007fffff) | 0x3F800000;  //force exp to 127 (value = sign * 2^(exp-127)*mantisa)
      return( *((float*)&a) - 1.0f );
   }

   void gaussRandom(float &out1, float &out2) {
      float x1, x2, w;
      do {
         x1 = 2.0f * randf() - 1.0f;
         x2 = 2.0f * randf() - 1.0f;
         w = x1*x1 + x2*x2;
      } while(w>=1.0);
      w = (float)sqrt((-2.0*log(w))/w);
      out1 = x1 * w;    //are both gaussian values
      out2 = x2 * w;
   }
private:
   int mCurrentValue;
};

/**
 * LFSR113 returning 32bit-value(period 2^^113)
 */
class LFSR113 {
public:
   /// constructor with defaultseedvalues
   LFSR113() {
      z1 = 631303693;      //seedvalues taken from random.org :)
      z2 = 8740621;
      z3 = 766854529;
      z4 = 14162150;
   }
   /// constructor with specified seedvalues
   /// the seedvalues MUST satisfy:
   /// z1>1, z2>7, z3>15, z4>127
   LFSR113(unsigned long s1, unsigned long s2, unsigned long s3, unsigned long s4) {
      //the seed MUST satifsy:
      // z1>1, z2>7, z3>15, z4>127
      ASSERT(z1 > 1);
      ASSERT(z2 > 7);
      ASSERT(z3 > 15);
      ASSERT(z4 > 127);
      z1 = s1; z2 = s2; z3 = s3; z4 = s4;
   }
   /// generate 32bit random number value
   /// \return random number in the range 0..2^32
   unsigned long rand() {
      unsigned long b;
      b  = (((z1<<6)^z1) >> 13);
      z1 = (((z1 & 4294967294)<<18)^b);
      b  = (((z2<<2)^z2) >> 27);
      z2 = (((z2 & 4294967288)<<2)^b);
      b  = (((z3<<13)^z3) >> 21);
      z3 = (((z3 & 4294967280)<<7)^b);
      b  = (((z4 << 3) ^z4) >> 12);
      z4 = (((z4 & 4294967168) << 13)^b);
      return (z1^z2^z3^z4);
   }
private:
   unsigned long z1,z2,z3,z4;
};

/**
 * WELL512 from Chris Lomont (period 2^^512)
 */
class WELL512 {
public:
   /// constructor using defaultseedvalues with values taken from random.org
   WELL512() {
      //init should also reset this to 0
      mIndex = 0;
      // initialize state to random bits
      mState[0]  = 627819223;
      mState[1]  = 486455989;
      mState[2]  = 323780391;
      mState[3]  = 489561707;
      mState[4]  = 548171288;
      mState[5]  = 78882334;
      mState[6]  = 716095697;
      mState[7]  = 645356953;
      mState[8]  = 165938953;
      mState[9]  = 818970504;
      mState[10] = 52273416;
      mState[11] = 468842599;
      mState[12] = 300629288;
      mState[13] = 252556277;
      mState[14] = 71504501;
      mState[15] = 527382159;
   }

   /// generate 32bit random number value
   /// \return random number in the range 0..2^32
   unsigned long rand() {
      unsigned long a,b,c,d;
      a = mState[mIndex];
      c = mState[(mIndex+13)&15];
      b = a^c^(a<<16)^(c<<15);
      c = mState[(mIndex+9)&15];
      c ^= (c>>11);
      a = mState[mIndex] = b^c;
      d = a^((a<<5)&0xDA442D20UL);
      mIndex = (mIndex+15) & 15;
      a = mState[mIndex];
      mState[mIndex] = a^b^d^(a<<2)^(b<<18)^(c<<28);
      return mState[mIndex];
   }
private:
   unsigned int mIndex;
   unsigned long mState[16];
};

}  //namespace eastl

#endif
