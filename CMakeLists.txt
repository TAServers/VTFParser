cmake_minimum_required(VERSION 3.20)

project("VTFParser" CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(VTFParser
        src/helpers/check-bounds.hpp
        src/errors.hpp
        src/vtf.cpp
        src/vtf.hpp
        src/file-format-objects/header.hpp
        src/file-format-objects/enums.hpp
        VTFParser.hpp
)
