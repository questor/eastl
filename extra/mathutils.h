
#ifndef EASTL_EXTRA_MATHUTILS_H_
#define EASTL_EXTRA_MATHUTILS_H_

namespace eastl {

template<typename T> int signum(T val) {  //return -1, 0 or +1
   return (val > T(0)) - (val < T(0));
}
//faster variant: #define sign1(x) (( x >> 31 ) | ( (unsigned int)( -x ) >> 31 ))

// Exponential moving average
// http://en.wikipedia.org/wiki/Moving_average
// http://excu.se/2011/07/low-pass-filtering-statistics/
template<typename T=float> class ExpMovingAverage {
public:
   ExpMovingAverage(int numberTimePeriods = 19) {
      // The coefficient a represents the degree of weighting decrease, a constant smoothing
      // factor between 0 and 1. A higher a discounts older observations faster. Alternatively,
      // a may be expressed in terms of N time periods, where a = 2/(N+1). For example,
      // N = 19 is equivalent to a = 0.1. The half-life of the weights (the interval over which
      // the weights decrease by a factor of two) is approximately N/2.8854 (within 1% if N > 5).
      mAlpha = 2.0 / (numberTimePeriods+1);
      mAverageValue = 0;
   }
   void addValue(T value) {
      // S(n) = alpha * x(n)+(1-alpha)*S(n-1)
      mAverageValue = mAlpha*value+(1-mAlpha)*mAverageValue;
   }
protected:
   T mAverageValue;
   T mAlpha;
};


// PID controller
// http://book.opensourceproject.org.cn/embedded/cmpcmatlab/opensource/6502/ddu0019.html
// http://excu.se/2011/08/animation-using-closed-loop-control/
template<typename T=float> class PIDController {
public:
   PIDController(T P, T I, T D, T errorThresh)
      : mP(P)
      , mI(I)
      , mD(D)
      , mErrorThresh(errorThresh)
      , mIntegral(0)
      , mStarted(false)
   {
   }

   T update(T setPoint, T processPoint, T timeStep) {
      T error = setPoint - processPoint;
      if(fabs(error) < mErrorThresh) {
         mIntegral += timeStep*error;
      }
      T deriv;
      if(!mStarted) {
         mStarted = true;
         deriv = 0;
      } else {
         deriv = (error - mPrevError)*(1.0/timeStep);
      }

      mPrevError = error;

      return mP*(error+mI*mIntegral+mD*deriv);
   }
protected:
   T mP, mI, mD, mErrorThresh;
   T mTimeStep;
   T mIntegral;
   T mPrevError;
   bool mStarted;
};

//PID controller without timestep, but much faster
template<typename T=float> class SimplePIDController {
public:
   SimplePIDController(T P, T I, T D, T currentValue)
      : mP(P)
      , mI(I)
      , mD(D)
      , mIErr(0)
      , mPrevError(0)
      , mCurrentValue(currentValue)
   {
   }

   void newSetPoint(T setPoint) {
      mSetPoint = setPoint;
   }

   T update() {
      T error = mSetPoint - mCurrentValue;

      //DeadZone: if(fabs(error)>threshold) error=0.0;

      mIErr += mPrevError;
      T DErr = error-mPrevError;

      mPrevError = error;

      mCurrentValue = mP*error + mI*mIErr + mD*DErr;
      return mCurrentValue;
   }

protected:
   T mSetPoint, mCurrentValue;
   T mP, mI, mD;
   T mIErr;
   T mPrevError;
};
}; //namespace eastl

#endif   //#ifndef EASTL_EXTRA_MATHUTILS_H_
