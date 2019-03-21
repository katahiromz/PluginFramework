# CMakeLists.txt --- CMake project settings
#    ex) cmake -G "Visual Studio 9 2008"
#    ex) cmake -DCMAKE_BUILD_TYPE=Release -G "MSYS Makefiles"
##############################################################################

# CMake minimum version
cmake_minimum_required(VERSION 2.4)

if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# enable testing
enable_testing()

# project name and languages
project(PluginFramework C CXX)

# check build type
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# make uppercase string of build type
string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)

# check build type again
if (NOT uppercase_CMAKE_BUILD_TYPE MATCHES "^(DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL)$")
    message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

## set output directory (build/)
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if (WIN32)
    # enable Win32 resource
    enable_language(RC)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # using Clang
        set(CMAKE_C_FLAGS "-static")
        set(CMAKE_CXX_FLAGS "-static")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # using GCC
        set(CMAKE_C_FLAGS "-static -static-libgcc -static-libstdc++")
        set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")
    elseif (MSVC)
        # replace "/MD" with "/MT" (building without runtime DLLs)
        set(CompilerFlags
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        foreach(CompilerFlags ${CompilerFlags})
            string(REPLACE "/MD" "/MT" ${CompilerFlags} "${${CompilerFlags}}")
        endforeach()
    endif()
endif()

##############################################################################

# options
option(PF_USE_UNICODE "Use Unicode for PluginFramework" OFF)

# Plugin1.plugin
add_library(Plugin1 SHARED Plugin1.c Plugin1.def)
target_compile_definitions(Plugin1 PRIVATE -DPLUGIN_BUILD)
set_target_properties(Plugin1 PROPERTIES OUTPUT_NAME "Plugin1.plugin")
set_target_properties(Plugin1 PROPERTIES PREFIX "")
set_target_properties(Plugin1 PROPERTIES SUFFIX "")
if (PF_USE_UNICODE)
    target_compile_definitions(Plugin1 PRIVATE -DUNICODE -D_UNICODE)
endif()

# Plugin2.plugin
add_library(Plugin2 SHARED Plugin2.cpp Plugin2.def)
target_compile_definitions(Plugin2 PRIVATE -DPLUGIN_BUILD)
set_target_properties(Plugin2 PROPERTIES OUTPUT_NAME "Plugin2.plugin")
set_target_properties(Plugin2 PROPERTIES PREFIX "")
set_target_properties(Plugin2 PROPERTIES SUFFIX "")
if (PF_USE_UNICODE)
    target_compile_definitions(Plugin2 PRIVATE -DUNICODE -D_UNICODE)
endif()

# libPluginFramework.a
add_library(PluginFramework STATIC PluginFramework.cpp)
if (PF_USE_UNICODE)
    target_compile_definitions(PluginFramework PRIVATE -DUNICODE -D_UNICODE)
endif()

# executable
add_executable(PluginTest PluginTest.cpp)
target_link_libraries(PluginTest PRIVATE PluginFramework shlwapi)
if (PF_USE_UNICODE)
    target_compile_definitions(PluginTest PRIVATE -DUNICODE -D_UNICODE)
endif()

# test
add_test(NAME PluginTest
         COMMAND $<TARGET_FILE:PluginTest>)

##############################################################################
