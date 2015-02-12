/*  _______         __              
   |_     _|.-----.|  |_.-----.----.
    _|   |_ |     ||   _|  -__|   _|
   |_______||__|__||____|_____|__|  
       coded by Questor / Inter      */

#include "console.h"
using namespace eastl;

//on linux use openpty?

#ifdef TARGET_PLATFORM_WINDOWS
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#endif
#ifdef TARGET_PLATFORM_LINUX
#include <pty.h>
#endif

console::console(const char *title) {
#ifdef TARGET_PLATFORM_WINDOWS
   if(AllocConsole()) {
      int handle = _open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
      FILE *handleFile = _fdopen(handle, "w");
      *stdout = *handleFile;
      int i=setvbuf(stdout, NULL, _IONBF, 0);

      handle = _open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE), _O_TEXT);
      handleFile = _fdopen(handle, "w");
      *stderr = *handleFile;
      i = setvbuf(stdout, NULL, _IONBF, 0);

      handle = _open_osfhandle((intptr_t)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT);
      handleFile = _fdopen(handle, "r");
      *stdin = *handleFile;
      i = setvbuf(stdin, NULL, _IONBF, 0);
   }

   SMALL_RECT window_size = {0, 0, 79, 49};
	COORD buffer_size = {80, 50};
   mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(mConsoleHandle, TRUE, &window_size);
	SetConsoleScreenBufferSize(mConsoleHandle, buffer_size);

   SetConsoleTitleA(title);
#endif
}

console::~console() {
#ifdef TARGET_PLATFORM_WINDOWS
   FreeConsole();
#endif
}

void console::setColor(int color) {
#ifdef TARGET_PLATFORM_WINDOWS
   SetConsoleTextAttribute(mConsoleHandle, color);
#endif
}

void console::clear() {
#ifdef TARGET_PLATFORM_WINDOWS
   COORD coordScreen = { 0, 0 };
   DWORD cCharsWritten;
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   DWORD dwConSize;

   GetConsoleScreenBufferInfo ( mConsoleHandle, &csbi );
   dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
   FillConsoleOutputCharacter ( mConsoleHandle, TEXT ( ' ' ), dwConSize,
                                coordScreen, &cCharsWritten );
   GetConsoleScreenBufferInfo ( mConsoleHandle, &csbi );
   FillConsoleOutputAttribute ( mConsoleHandle, csbi.wAttributes, dwConSize,
                                coordScreen, &cCharsWritten );
   SetConsoleCursorPosition ( mConsoleHandle, coordScreen );
#endif
}

void console::flush() {
#ifdef TARGET_PLATFORM_WINDOWS
   fflush(stdin);
   fflush(stdout);
#endif
}

/*
22 int PDC_get_rows(void)
23	{
24	    CONSOLE_SCREEN_BUFFER_INFO scr;
28	    GetConsoleScreenBufferInfo(mConsoleHandle, &scr);
30	    return scr.srWindow.Bottom - scr.srWindow.Top + 1;
31	}
35	int PDC_get_buffer_rows(void)
36	{
37	    CONSOLE_SCREEN_BUFFER_INFO scr;
41	    GetConsoleScreenBufferInfo(mConsoleHandle, &scr);
43	    return scr.dwSize.Y;
44	}
48	int PDC_get_columns(void)
49	{
50	    CONSOLE_SCREEN_BUFFER_INFO scr;
54	    GetConsoleScreenBufferInfo(mConsoleHandle, &scr);
56	    return scr.srWindow.Right - scr.srWindow.Left + 1;
57	}

   CHAR_INFO character;
	COORD char_buffer_size = {1, 1};
	COORD char_pos = {0, 0};
	SMALL_RECT write_area = {0, 0, 0, 0};
	character.Char.AsciiChar = 'A';
	character.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY;	
   WriteConsoleOutput(mConsoleHandle, &character, char_buffer_size, char_pos, &write_area);
*/
