project "hgeportdemo"
    kind "WindowedApp"
    location ("../build/" .. _ACTION)
    targetdir ("../bin")
    targetextension ".exe"
    defines { "_WINDOWS" }
    linkoptions { "/NODEFAULTLIB:libc" }
    links
    {
        "hgeport",
	"hge",
	"hgehelp",
	"curvedani",
	"winmm",
    }
    files
    {
      "../src/demo/**.cpp",
      "../src/demo/**.h",
      "../include/**.h",
    }
    excludes
    {
    	"libc.lib"
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
      "../lib/" .. _ACTION,
      "../../curvedani/lib/" .. _ACTION
    }
    flags
    {
      "NoPCH",
      "StaticRuntime",
      "WinMain"
    }
    configuration "Debug"
      defines { "_DEBUG" }
      targetname "hgeportdemo_d"
      flags
      {
      	"Symbols",
      }
    configuration "Release"
      defines { "NDEBUG" }
      targetname "hgeportdemo"
      flags
      {
	 "OptimizeSize",
      }
