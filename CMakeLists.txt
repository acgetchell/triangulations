cmake_minimum_required(VERSION 3.0.0)
project(Triangulations VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CONFIGURATION_TYPES
    "Release"
    "Debug"
    "RelWithDebInfo"
    CACHE STRING ""
    FORCE)

add_executable(Triangulations main.cpp)
target_compile_features(Triangulations PRIVATE cxx_std_17)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
