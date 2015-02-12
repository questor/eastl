/*  _______         __              
   |_     _|.-----.|  |_.-----.----.
    _|   |_ |     ||   _|  -__|   _|
   |_______||__|__||____|_____|__|  
       coded by Questor / Inter      */

/* Based on sources from these:
 * Copyright (c) 2010-2012 Ivan Voras <ivoras@freebsd.org>
 * Copyright (c) 2012 Tim Hartrick <tim@edgecast.com> */

#ifndef __EASTL_EXTRAS_FIXEDPOINT_H__
#define __EASTL_EXTRAS_FIXEDPOINT_H__

#include <math.h>
#include "eastl/string.h"

namespace eastl {

#include "eastl/types.h"

//TODO: rounding and save div
template<typename NormalType, typename MulType, int fractBits> class FixedPoint {
public:
   FixedPoint() {}

   FixedPoint genConstant(float val) {
      return val * (float)(1<<fractBits) + (val>=0? 0.5f : -0.5f);
   }
   FixedPoint genConstant(int val) {
      return val << fractBits;
   }

   FixedPoint(NormalType value) {
      mValue = value << fractBits;
   }

   FixedPoint(float val) {
      set(val);
   }

   void set(float val) {
      val *= (float)(1<<fractBits);
      val += (val>=0)?0.5f:-0.5f;
      mValue = (NormalType)val;
   }
   void set(NormalType val) {
      mValue = val << fractBits;
   }
   FixedPoint &operator++() {
      mValue += (1 << fractBits);
      return *this;
   }
   FixedPoint &operator--() {
      mValue -= (1 << fractBits);
      return *this;
   }
   FixedPoint &operator=(FixedPoint val) {
      mValue = val.mValue;
      return *this;
   }
   FixedPoint &operator=(float val) {
      set(val);
      return *this;
   }
   bool operator==(FixedPoint val) {
      return val.mValue == mValue;
   }
   operator float() {
      return mValue >> fractBits;
   }
   operator int() {
      return mValue >> fractBits;
   }
   FixedPoint operator+(FixedPoint val) {
      NormalType result = mValue + val.mValue;
      FixedPoint ret;
      ret.mValue = result;
      return ret;
   }
   FixedPoint &operator+=(FixedPoint val) {
      mValue += val.mValue;
      return *this;
   }
   FixedPoint operator-(FixedPoint val) {
      NormalType result = mValue - val.mValue;
      FixedPoint ret;
      ret.mValue = result;
      return ret;
   }
   FixedPoint &operator-=(FixedPoint val) {
      mValue -= val.mValue;
      return *this;
   }
   FixedPoint operator*(FixedPoint val) {
      NormalType result = (NormalType)(((MulType)mValue*val.mValue) << fractBits);
      FixedPoint ret;
      ret.mValue = result;
      return ret;
   }
   FixedPoint &operator*=(FixedPoint val) {
      mValue = (NormalType)(((MulType)mValue*val.mValue) << fractBits);
      return *this;
   }
   FixedPoint operator*(NormalType val) {
      int32_t result = mValue*val;
      FixedPoint ret;
      ret.mValue = result;
      return ret;
   }
   FixedPoint &operator*=(NormalType val) {
      mValue = mValue*val;
      return *this;
   }
   FixedPoint operator/(FixedPoint val) {
      return internalDiv(val);
   }
   FixedPoint &operator/=(FixedPoint val) {
      mValue = (NormalType)(((MulType)mValue << fractBits)/(MulType)val.mValue);
      return *this;
   }
protected:
   NormalType internalDiv(NormalType val1, NormalType val2) {
      return (NormalType)(((MulType)val1 << fractBits)/(MulType)val2);
   }
public:
   eastl::string convertToStr() {
      char tmp[32];
      char *ptr = &tmp[0];

      NormalType val = mValue;
      int maxDec = 2;
      if(sizeof(NormalType) == 64)
         maxDec = 10;
      if(val < 0) {
         *(ptr++) = '-';
         val *= -1;
      }

      char tmp2[12];
      char *ptr2 = &tmp2[0];
      int whole = val >> fractBits;
      do {
         *(ptr2++) = '0' + whole % 10;
         whole /= 10;
      } while(whole != 0);
      XASSERT(ptr2 < &tmp2[0]+sizeof(tmp2), "buffer overwrite, increase tmp2 buffer!");
      while(ptr2 != &tmp2[0])
         *(ptr++) = *(--ptr2);

      *(ptr++) = '.';

      NormalType mask = (1 << fractBits)-1;
      int fract = val;
      while(maxDec != 0) {
         fract = (fract&mask)*10;
         *(ptr++) = '0' + (fract>>fractBits)%10;
         --maxDec;
      }
      *(ptr) = '\0';
      XASSERT(ptr < &tmp[0]+sizeof(tmp), "buffer overwrite, increase tmp buffer!");
      return eastl::string(tmp);
   }

   FixedPoint sqrt() {
      if(mValue < 0)
         return genConstant(-1);
      if(mValue == (NormalType)genConstant(0) || mValue == (NormalType)genConstant(1))
         return mValue;
      bool invert = false;
      int iter = fractBits;
      if(mValue < (NormalType)genConstant(1) && mValue > 6) {
         invert = true;
         mValue = internalDiv(genConstant(1), mValue);
      }
      if(mValue > (NormalType)genConstant(1)) {
         NormalType s = mValue;
         iter = 0;
         while(s > 0) {
            s >>= 2;
            iter++;
         }
      }
      // Newton's iteration
      NormalType l = (mValue>>1)+1;
      for(int i=0; i<iter; ++i) {
         l = (l + internalDiv(mValue, l)) >> 1;
      }
      if(invert)
         mValue = internalDiv(genConstant(1), l);
      else
         mValue = l;
      return mValue;
   }

protected:
   NormalType mValue;
};

//fastsin for fixed-point(http://www.devmaster.net/forums/showthread.php?t=5784&page=2)
/*fp16 ne3d_sintp_02pi(fp16 angle)
{
  fp16 Xi2,Xi3,Yi,Yi2,Yi3,Yi4;

  angle = angle >> 1;
  Xi2 = ((angle >> 1) * (angle >> 2)) >> 13;
  Xi3 = (6640 * Xi2) >> 12;
  Yi =  (((20860 * angle) >> 13) - Xi3) >> 1;
  Yi2 = (Yi * Yi) >> 14;
  Yi3 = (14746 * Yi2) >> 16;
  Yi4 = (25395 * Yi ) >> 14;
  Yi = Yi4 + Yi3;

  return Yi;
}*/

typedef FixedPoint<int32_t, int64_t, 16> FixedPoint32;
//typedef FixedPoint<int64_t, int128_t> FixedPoint64;

}  //namespace eastl

#endif   // __EASTL_EXTRAS_FIXEDPOINT_H__
