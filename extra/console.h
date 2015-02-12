/*  _______         __              
   |_     _|.-----.|  |_.-----.----.
    _|   |_ |     ||   _|  -__|   _|
   |_______||__|__||____|_____|__|  
       coded by Questor / Inter      */

#ifndef EASTL_EXTRA_CONSOLE_H_
#define EASTL_EXTRA_CONSOLE_H_

#include "eastl/types.h"

namespace eastl {

class console {
public:
   typedef enum {
      FG_Blue = 0x0001,               /*!< Blue Foreground */
      FG_Green = 0x0002,              /*!< Green Foreground */
      FG_Red = 0x0004,                /*!< Red Foreground */
      FG_Intensity = 0x0008,          /*!< Foreground intensity (makes the foreground colour a shade brighter) */
      BG_Blue = 0x0010,               /*!< Blue Background */
      BG_Green = 0x0020,              /*!< Green Background */
      BG_Red = 0x0040,                /*!< Red Background */
      BG_Intensity = 0x0080,          /*!< Background intensity (makes the foreground colour a shade brighter) */
      FG_Brown = 0x0000,                                      /*!< Brown Foreground (POSIX only) */
      FG_Magenta = FG_Blue | FG_Red,                          /*!< Magenta Foreground */
      FG_Cyan = FG_Blue | FG_Green,                           /*!< Cyan Foreground */
      BG_Brown = 0x0000,                                      /*!< Brown Background (POSIX only) */
      BG_Magenta = BG_Blue | BG_Red,                          /*!< Magenta Background */
      BG_Cyan = BG_Green | BG_Blue,                           /*!< Cyan Background */
      FG_Gray = FG_Blue | FG_Green | FG_Red,                  /*!< Gray Foreground */
      FG_White = FG_Blue | FG_Green | FG_Red | FG_Intensity,  /*!< White Foreground */
      BG_Gray = BG_Blue | BG_Green | BG_Red,                  /*!< Gray Background */
      BG_White = BG_Blue | BG_Green | BG_Red | BG_Intensity   /*!< White Background */
   } Color;

   console(const char *title);
   ~console();

   void setColor(int color);
   void clear();
   void flush();

protected:
   void *mConsoleHandle;
};

}  //namespace eastl

#endif
