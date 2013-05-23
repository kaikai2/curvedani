-- target 是预定义变量,表示编译环境的名字
project.path = "../build/" .. target
project.name = "hgeport"
project.bindir = "../bin"
project.libdir = "../lib"

-----------------------------
-- HGE PORT lib package
-----------------------------
package = newpackage()
packageHGEport = package

package.path = "../build/" .. target
package.kind = "lib"
package.name = "hgeport"
package.language = "c++"
package.libdir = "../../lib/" .. target
package.bindir = package.libdir

package.config["Debug"].objdir = "./Debug/hgeport"
package.config["Debug"].target = package.name .. "_d"
package.config["Release"].objdir = "./Release/hgeport"
package.config["Release"].target = package.name

if (target == "vs2005") then
	package.defines = { "_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE" }
end
if (windows) then
	package.config["Debug"].defines = { "WIN32", "_DEBUG", "_LIB" }
	package.config["Release"].defines = { "WIN32", "NDEBUG", "_LIB" }
else
	package.config["Debug"].defines = { "_DEBUG", "_LIB" }
	package.config["Release"].defines = { "NDEBUG", "_LIB" }
end

package.config["Release"].buildflags = {"optimize-speed", "no-symbols" }
package.buildflags = {"extra-warnings", "static-runtime", "no-exceptions", "no-rtti" }
package.includepaths = { "../../include", "../../../curvedani/include", "../../../hge/include" }
package.libpaths = { "../../lib", "../../../hge/lib/vc" }

package.files = {
  matchfiles("../../include/*.h", "../../src/*.cpp")
}

-----------------------------
-- HGE PORT demo package
-----------------------------
package = newpackage()

package.path = "../build/" .. target
package.kind = "winexe"
package.name = "hgeport demo"
package.language = "c++"
package.bindir = "../../bin"

package.config["Debug"].objdir = "./Debug/demo"
package.config["Debug"].target = "hgeport_demo_d"
package.config["Release"].objdir = "./Release/demo"
package.config["Release"].target = "hgeport_demo"

if (windows) then
	package.config["Debug"].defines = { "WIN32", "_DEBUG", "_CONSOLE" }
	package.config["Release"].defines = { "WIN32", "NDEBUG", "_CONSOLE" }
else
	package.config["Debug"].defines = { "_DEBUG", "_CONSOLE" }
	package.config["Release"].defines = { "NDEBUG", "_CONSOLE" }
end

package.linkoptions ={ "/NODEFAULTLIB:libc" }
package.config["Release"].buildflags = {"optimize-speed", "no-symbols" }
package.buildflags = {"no-main", "extra-warnings", "static-runtime", "no-exceptions", "no-rtti" }
package.links = { packageHGEport.name }
package.config["Debug"].links = { "hge", "hgehelp", "curvedani_d" }
package.config["Release"].links = { "hge", "hgehelp", "curvedani" }
package.includepaths = { "../../include", "../../../curvedani/include", "../../../hge/include" }
package.libpaths = { "../../lib", "../../../curvedani/lib/" .. target, "../../../hge/lib/vc"  }

package.files = {
  matchfiles("../../include/*.h", "../../src/demo/*.cpp")
}

-----------------------------
-- HGE PORT converter package
-----------------------------
package = newpackage()

package.path = "../build/" .. target
package.kind = "exe"
package.name = "hgeport converter"
package.language = "c++"
package.bindir = "../../bin"

package.config["Debug"].objdir = "./Debug/converter"
package.config["Debug"].target = "converter_d"
package.config["Release"].objdir = "./Release/converter"
package.config["Release"].target = "converter"

if (target == "vs2005") then
	package.defines = { "_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE" }
end
if (windows) then
	package.config["Debug"].defines = { "WIN32", "_DEBUG", "_CONSOLE" }
	package.config["Release"].defines = { "WIN32", "NDEBUG", "_CONSOLE" }
else
	package.config["Debug"].defines = { "_DEBUG", "_CONSOLE" }
	package.config["Release"].defines = { "NDEBUG", "_CONSOLE" }
end

package.linkoptions ={ "/NODEFAULTLIB:libc" }
package.config["Release"].buildflags = {"optimize-speed", "no-symbols" }
package.buildflags = {"extra-warnings", "static-runtime", "no-exceptions", "no-rtti" }
package.links = { packageHGEport.name }
package.config["Debug"].links = { "hge", "hgehelp", "curvedani_d" }
package.config["Release"].links = { "hge", "hgehelp", "curvedani" }
package.includepaths = { "../../include", "../../../curvedani/include" }
package.libpaths = { "../../lib", "../../../curvedani/lib/" .. target }

package.files = {
  matchfiles("../../include/*.h", "../../src/converter/*.cpp")
}
