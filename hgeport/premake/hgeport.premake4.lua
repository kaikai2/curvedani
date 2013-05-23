solution "hgeport"
	configurations { "Debug", "Release" }
	language "C++"
	--  platforms({ "Native" })
	location ( "../build/" .. _ACTION)
	defines { "WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }

dofile  "../../curvedani/premake/curvedani_vcproj_premake4.lua"
dofile  "hgeport_vcproj_premake4.lua"
dofile  "hgeportdemo_vcproj_premake4.lua"
dofile  "hgeportConverter_vcproj_premake4.lua"
