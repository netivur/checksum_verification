cmake_minimum_required(VERSION 3.15)

project(csvp VERSION 0.1.1.1)
add_library(csvp_compiler_flags INTERFACE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_features(csvp_compiler_flags INTERFACE cxx_std_20)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(csvp_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>")

add_executable(csvp maincsvp.cpp)

target_include_directories(csvp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(csvp PUBLIC "${PROJECT_BINARY_DIR}")#"${PROJECT_SOURSE_DIR}/servicefiles")
install(TARGETS csvp DESTINATION bin)
