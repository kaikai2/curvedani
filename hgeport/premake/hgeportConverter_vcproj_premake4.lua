project "hgeportConverter"
    kind "ConsoleApp"
    location ("../build/" .. _ACTION)
    targetdir ("../bin")
    targetextension ".exe"
    defines { "_CONSOLE" }
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
      "../src/converter/**.cpp",
      "../src/converter/**.h",
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
      "../lib/" .. _ACTION,
      "../../curvedani/lib/" .. _ACTION,
      "../third/hge181/lib/vc"
    }
    flags
    {
      "NoPCH",
      "StaticRuntime",
      "WinMain"
    }
    configuration "Debug"
      defines { "_DEBUG" }
      targetname "hgeportcvt_d"
      flags
      {
      	"Symbols", 
      }
    configuration "Release"
      defines { "NDEBUG" }
      targetname "hgeportcvt"
      flags
      {
	 "OptimizeSize",
      }
