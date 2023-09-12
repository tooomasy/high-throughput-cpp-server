cmake_minimum_required(VERSION 3.12)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Define custom CMake variables
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0") # Debug build with no optimization
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") # Release build with -O3 optimization

project(
    CppServer
    VERSION 0.0.1
    LANGUAGES CXX
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # add_compile_options(-Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror)
    add_compile_options(-Wall -Wextra -Wshadow -Wpedantic -Werror)
endif()

set(CMAKE_CXX_FLAGS_DEBUG_INIT "")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3")

add_subdirectory(src)

include(CTest)
add_subdirectory(test)