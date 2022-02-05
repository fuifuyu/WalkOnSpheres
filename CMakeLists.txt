# CMakeLists.txt : CMake project for wos, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("WOS")

find_package(SDL2 REQUIRED)

SET ( PDE_CORE_HEADER 
    src/core/pde.h
    src/core/boundary.h
)
FILE ( GLOB PDE_SOURCE
  src/boundary/*
  src/pde/*
)

INCLUDE_DIRECTORIES ( src )
INCLUDE_DIRECTORIES ( src/core )

add_library(pde STATIC
    ${PDE_SOURCE}
)
target_include_directories(pde INTERFACE
    ${PDE_CORE_HEADER}
)

add_executable(wos_exe src/main.cpp src/wos.cpp src/window.cpp)
target_include_directories(wos_exe PRIVATE 
    ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(wos_exe ${SDL2_LIBRARIES} pde)

SOURCE_GROUP (core REGULAR_EXPRESSION src/core/.*)
SOURCE_GROUP (boundary REGULAR_EXPRESSION src/boundary/.*)
