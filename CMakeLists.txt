cmake_minimum_required(VERSION 3.22)
project(demo_drm_reader)

set(CMAKE_CXX_STANDARD 20)

add_executable(demo_drm_reader
    main.cpp
    drm_reader.hpp drm_reader.cpp
)
