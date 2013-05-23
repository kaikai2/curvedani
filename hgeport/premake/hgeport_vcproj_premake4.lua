  project "hgeport"
    local hgeport = project()
    kind "StaticLib"
    name = "hgeport"
    location ("../build/" .. _ACTION)
    targetdir ("../lib/" .. _ACTION)
    targetextension ".lib"
    files
    {
      "../src/*.cpp",
      "../src/*.h",
      "../include/**.h",
    }
    excludes
    {
    }
    includedirs
    {
      "../include",
      "../../curvedani/include",
      "../third/hge181/include"
    }
    libdirs
    {
      "../lib",
      "../third/hge181/lib/vc"
    }
    flags
    {
      "NoPCH",
      "StaticRuntime",
      "WinMain"
    }
    defines { "_LIB" }
    configuration "Debug"
      defines { "_DEBUG" }
      targetname "hgeport_d"
      flags
      {
      	"Symbols", 
      }
    configuration "Release"
      defines { "NDEBUG" }
      targetname "hgeport"
      flags
      {
	 "OptimizeSize",
      }
