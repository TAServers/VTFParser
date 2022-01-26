workspace("VTFParser")
configurations({"Debug", "Release"})

project("VTFParser")
kind("StaticLib")
language("C++")
targetdir("premakeout/%{cfg.buildcfg}")

files({ "**.h", "**.cpp" })

filter("configurations:Debug")
defines("DEBUG")
symbols("On")

filter("configurations:Release")
defines("NDEBUG")
optimize("On")
