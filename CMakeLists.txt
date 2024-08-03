cmake_minimum_required(VERSION 3.20)

project("VTFParser" CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(VTFParser
        "VTFParser.cpp"
        "FileFormat/Parser.cpp"
        "DXTn/DXT1.cpp"
        "DXTn/DXT3.cpp"
        "DXTn/DXT5.cpp"
)
