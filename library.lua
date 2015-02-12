
require "tundra.syntax.glob"
require "tundra.syntax.files"
local native = require "tundra.native"

StaticLibrary {
   Name = "eastl",
   Sources = {
      Glob {
         Dir = _G.LIBROOT_EASTL,
         Extensions = { ".h", ".cpp" },
      },
   },
   Env = {
      CPPDEFS = {
        {Config="win32-*-*";     "EA_PLATFORM_WINDOWS"},
        {Config="linux_x86-*-*"; "EA_PLATFORM_LINUX"},
      },
      CPPPATH = {
         "extlibs",
      },
   },
   Propagate = {
      Env = {
         CPPDEFS = {
           {Config="win32-*-*";     "EA_PLATFORM_WINDOWS"},
           {Config="linux_x86-*-*"; "EA_PLATFORM_LINUX"},
         },
      },
   },
}

