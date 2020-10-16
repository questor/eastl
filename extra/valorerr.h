
#pragma once

#include "debug.h"

#define STRICT_MODE 0

template<typename T> class ValOrErr {
public:
   ValOrErr()
      : mIsInErrorState(false)
      , mErrorIsHandled(false)
      , mErrorString("no error")
   {}

   ~ValOrErr() {
#if STRICT_MODE == 0
      if(mErrorIsHandled == false) {
#else
      if((mIsInErrorState == true) && (mErrorIsHandled == false)) {
#endif
         assert(false, "error not checked!");
      }
   }

   void enterErrorState(const char *errorString) {
      if(mIsInErrorState) {
         assert(false, "signaling an error and already in error state!");
      }
      mIsInErrorState = true;
      mErrorIsHandled = false;
      mErrorString = errorString;
   }

   bool getErrorState() {
      mErrorIsHandled = true;
      return mIsInErrorState;
   }

   const char* getErrorString() {
      return mErrorString;
   }

   void setValue(const T &value) {
      mValue = value;
   }

   const T& valueOrDie() const {
      assert(mIsInErrorState == false, "Error set but value requested");
      return mValue;
   }

protected:
   T mValue;

   bool mIsInErrorState;
   bool mErrorIsHandled;
   const char* mErrorString;
};

