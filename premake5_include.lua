-- To compile VTFParser with a garrysmod_common project

project("VTFParser")
kind("StaticLib")
language("C++")
targetdir("premakeout/%{cfg.buildcfg}")

files({ "**.h", "**.cpp" })

filter("configurations:ReleaseWithSymbols")
defines("NDEBUG")
symbols("On")

filter("configurations:Release")
defines("NDEBUG")
optimize("On")
