/*  _______         __              
   |_     _|.-----.|  |_.-----.----.
    _|   |_ |     ||   _|  -__|   _|
   |_______||__|__||____|_____|__|  
       coded by Questor / Inter      */

#include "eastl/extra/debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef TARGET_PLATFORM_WINDOWS
    #include <windows.h>
#endif

namespace eastl {

//----------------------------------------------------------------------------------
// printDebugString
//----------------------------------------------------------------------------------
void printDebugString(const char *text, ...) {
	static char textbuffer[2048];
	va_list args;
	va_start(args,text);
	vsprintf(textbuffer, text,args);
	va_end(args);

#ifdef TARGET_PLATFORM_WINDOWS
    static char textbuf2[2048];
    sprintf(textbuf2,"%s\n",textbuffer);
    OutputDebugStringA(textbuf2);
#else
    printf("%s\n",textbuffer);
#endif
}

//----------------------------------------------------------------------------------
// fatalError
//----------------------------------------------------------------------------------
// Fatal Error
// 22.09.2000
void fatalError(const char *text, ...) {
	static char textbuffer[2048];
	static char textbuf2[2048];
	va_list args;
	va_start(args,text);
	vsprintf(textbuffer, text,args);
	va_end(args);

    sprintf(textbuf2,"[FATAL] %s\n",textbuffer);
    printf(textbuf2);
#ifdef TARGET_PLATFORM_WINDOWS
    OutputDebugStringA(textbuf2);
#endif

   EASTL_DEBUG_BREAK();

   exit(1);			//TODO: clean shutdown
}

}  //namespace eastl
