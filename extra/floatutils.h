/*  _______         __              
   |_     _|.-----.|  |_.-----.----.
    _|   |_ |     ||   _|  -__|   _|
   |_______||__|__||____|_____|__|  
       coded by Questor / Inter      */

#ifndef EASTL_EXTRA_FLOATUTILS_H_
#define EASTL_EXTRA_FLOATUTILS_H_

#include "eastl/algorithm.h"  //min/max
#include "eastl/EAAssert/eaassert.h"

namespace eastl {

const float PI          = 3.14159265358979323846264338327950288f;
const float PI2         = 6.28318530717958647692528676655900577f;
const float PID2        = 1.57079632679489661923132169163975144f;
const float PI_SQR      = 9.86960440108935861883449099987615114f;

const float GOLDEN_RATIO = 1.618033988749894848f;

//look at http://realtimecollisiondetection.net/pubs/ (good float-reference!)
//       Decimal            next representable number
//         10.0             x + 0.000001
//        100.0             x + 0.000008
//      1,000.0             x + 0.000061
//     10,000.0             x + 0.000977
//    100,000.0             x + 0.007813
//  1,000,000.0             x + 0.0625
// 10,000,000.0             x + 1.0

//absolute tolerance comparison of x and y
//equal(float) = fabs(x-y) < EPSILON
inline bool floatAbsoluteEqual(const float x, const float y, const float EPSILON=0.000061f) {
   return fabs(x-y) < EPSILON;
}

//relative tolerance
//equal(float) = fabs(x-y) <= EPSILON*max(fabs(x), fabs(y))
inline bool floatRelativeEqual(const float x, const float y, const float EPSILON=0.000061f) {
   return fabs(x-y) <= EPSILON*max(fabs(x), fabs(y));
}

//combined
//equal(float) = fabs(x-y) <= EPSILON * max(1.0f, fabs(x), fabs(y))
inline bool floatCombinedEqual(const float x, const float y, const float EPSILON=0.000061f) {
   return fabs(x-y) <= EPSILON*max(1.0, max(fabs(x), fabs(y)));
}

// from http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
// Usable AlmostEqual function
inline bool almostEqual2sComplement(float A, float B, int maxUlps) {
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    EA_ASSERT(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
    int aInt = *(int*)&A;
    // Make aInt lexicographically ordered as a twos-complement int
    if (aInt < 0)
        aInt = 0x80000000 - aInt;
    // Make bInt lexicographically ordered as a twos-complement int
    int bInt = *(int*)&B;
    if (bInt < 0)
        bInt = 0x80000000 - bInt;
    int intDiff = abs(aInt - bInt);
    if (intDiff <= maxUlps)
        return true;
    return false;
}

//32bits:
inline float fastFabs32(float val) {
   int temp = (*((int*)&val)) & 0x7fffffff;
   return *((float*)&temp);
}

//64bits:
inline float fastFabs64(float val) {
   int temp = (*((int*)&val)+1) & 0x7fffffff;
   return *((float*)&temp);
}

//32bits:
inline double fastDabs32(double val) {
   int temp = (*((int*)&val)) & 0x7fffffff;
   return *((double*)&temp);
}
//64bits:
inline double fastDabs64(double val) {
   int temp = (*((int*)&val)+1) & 0x7fffffff;
   return *((double*)&temp);
}

inline bool lessThanZero32(float &f) {
   return ((*((unsigned int *) &(f))) > 0x80000000U);
}
inline bool lessOrEqualZero(float &f) {
   return ((*((int *) &(f))) <= 0);
}
inline bool greaterThanZero(float &f) {
   return ((*((int *) &(f))) > 0);
}
inline bool greaterOrEqualZero32(float &f) {
   return ((*((unsigned int *) &(f))) <= 0x80000000U);
}


// Fast Sin(http://www.devmaster.net/forums/showthread.php?t=5784&page=1)

//without accurate the maximum error ist 0.056 (3-term Taylor)
//with accurate the maximum error is 0.001 (4-term Taylor)
template <typename T, bool accurate>
T fastSin(const T& theta) {
   const T B = 4 / PI;
   const T C = -4 / PI_SQR;

   T y = B * theta + C * theta * abs(theta);

   if (accurate) {
      //Optimal values are Q = 0.775, P = 0.225 for the absolute error and 
      // Q = 0.782, P = 0.218 for the relative error
      //  const float Q = 0.775;
      const T P = 0.225;
      y = P * (y * abs(y) - y) + y;   // Q * y + P * y * abs(y)
   }
   return y;
}

//for cos: x -= (x > pi) & (2 * pi);
template <typename T, bool accurate> T fastCos(const T& theta) {
   T thetaNew = theta;
   if(theta > PI)
      thetaNew -= PI2;
   return fastSin<T, accurate>(thetaNew);
}

//and arcsine(http://www.devmaster.net/forums/showthread.php?t=5784&page=3):
const float asin4_params1[5]={   6.32559537178112e-05f,   9.97002719101181e-01f,   3.23729856176963e-02f,   3.89287300071597e-02f,   1.93549238398372e-01f};
const float asin4_params2[7]={   2.09625797161885e+01f,  -1.74835553411477e+02f,   6.13575281494908e+02f,  -1.14033116228467e+03f,   1.19159992307311e+03f,  -6.63957441058529e+02f,   1.54421991537526e+02f};
const float asin4_params3[4]={   1.57080010233116e+00f,  -1.41437401362252e+00f,   1.84777752400778e-03f,  -1.24625163381900e-01f};
const float asin4_split1=0.6f;
const float asin4_split2=0.925f;

inline float asin4(float x) {
  if (x<asin4_split1)
    return asin4_params1[0]+x*(asin4_params1[1]+x*(asin4_params1[2]+x*(asin4_params1[3]+x*(asin4_params1[4]))));
  if (x<asin4_split2)
    return asin4_params2[0]+x*(asin4_params2[1]+x*(asin4_params2[2]+x*(asin4_params2[3]+x*(asin4_params2[4]+x*(asin4_params2[5]+x*asin4_params2[6])))));
  float xx=sqrt(1-x);
  return asin4_params3[0]+xx*(asin4_params3[1]+xx*(asin4_params3[2]+xx*asin4_params3[3]));
}


////////////////////////////////////////////////////////////////////////////////////////
//Fast Float-to-Int (http://cbloomrants.blogspot.com/2009/01/01-17-09-float-to-int.html)
union DoubleAndInt64 {
   uint64_t intType;
   double doubleType;
};
static const double floatutil_xs_doublemagic = (6755399441055744.0); // 2^52 * 1.5
static const double floatutil_xs_doublemagicdelta = (1.5e-8);                         //almost .5f = .5f + 1e^(number of exp bit)
static const double floatutil_xs_doublemagicroundeps = (0.5f - floatutil_xs_doublemagicdelta);       //almost .5f = .5f - 1e^(number of exp bit)

#include <xmmintrin.h>

// ftoi_round : *banker* rounding!
/* Note that this is a "banker's round" not a normal arithmetic rounding where 0.5 always goes up or 
   down - 0.5 goes to the nearest *even* value. So 2.5 goes to 2.0 and 3.5 goes to 4.0 ; eg. 0.5's 
   go up half the time and down half the time. To be more precise, ftoi_round will actually round the 
   same way that bits that drop out of the bottom of the FPU registers during addition round. */
inline int ftoi_round(const double val) {
  DoubleAndInt64 dunion;
  dunion.doubleType = val + floatutil_xs_doublemagic;
  return (int) dunion.intType; // just cast to grab the bottom bits
}

inline int ftoi_trunc(const float f) {
  return _mm_cvtt_ss2si( _mm_set_ss( f ) );
}

inline int ftoi_round_sse(const float f) {
  return _mm_cvt_ss2si( _mm_set_ss( f ) );
}

inline int ftoi_floor(const double val) {
    return ftoi_round(val - floatutil_xs_doublemagicroundeps);
}

inline int ftoi_ceil(const double val) {
    return ftoi_round(val + floatutil_xs_doublemagicroundeps);
}

// ftoi_trunc_xs = Sree's truncate
inline int ftoi_trunc_xs(const double val) {
  return (val<0) ? ftoi_round(val+floatutil_xs_doublemagicroundeps) : 
                   ftoi_round(val-floatutil_xs_doublemagicroundeps);
}


}  //namespace istl

#endif
