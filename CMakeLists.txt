# CMakeLists.txt : CMake project for wos, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("WOS")

find_package(SDL2 REQUIRED)
# Add a shared module - modules are intended to be imported at runtime.
# - This is where you add the source files
add_executable(wos_exe src/main.cpp src/wos.cpp)

target_include_directories(wos_exe PRIVATE src/ /usr/local/lib/c++/ ${SDL2_INCLUDE_DIRS})
target_link_libraries(wos_exe ${SDL2_LIBRARIES})