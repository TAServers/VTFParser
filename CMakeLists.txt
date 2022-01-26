cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project ("VTFParser")

add_executable (VTFParser "VTFParser.cpp" "VTFParser.h" "FileFormat/Enums.h" "FileFormat/Structs.h" "FileFormat/Parser.cpp" "FileFormat/Parser.h" "DXTn/DXTn.h" "DXTn/DXT1.cpp" "DXTn/DXT3.cpp" "DXTn/DXT5.cpp")
