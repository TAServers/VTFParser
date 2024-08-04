cmake_minimum_required(VERSION 3.20)

project("VTFParser" CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(VTFParser
        src/dxt/dxt1.cpp
        src/dxt/dxt3.cpp
        src/dxt/dxt5.cpp
        src/colours.hpp
        src/helpers/check-bounds.hpp
        src/helpers/offset-data-view.cpp
        src/errors.hpp
        src/dxt/structs.hpp
        src/vtf.cpp
        src/vtf.hpp
        src/file-format-objects/header.hpp
        src/file-format-objects/enums.hpp
        VTFParser.hpp
)
