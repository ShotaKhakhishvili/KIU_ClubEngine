cmake_minimum_required(VERSION 3.20)

project(ClubEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CLUBENGINE_VERSION "${PROJECT_VERSION}" CACHE INTERNAL "ClubEngine version")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    set(CE_PLATFORM_NAME "Win64")
elseif(UNIX)
    set(CE_PLATFORM_NAME "Linux64")
else()
    set(CE_PLATFORM_NAME "UnknownPlatform")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CE_OUTPUT_ROOT
    "${CMAKE_SOURCE_DIR}/Binaries/${CE_PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/${PROJECT_VERSION}"
)

set(CE_INTERMEDIATE_ROOT
    "${CMAKE_SOURCE_DIR}/Intermediate/${CE_PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/${PROJECT_VERSION}"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CE_OUTPUT_ROOT}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CE_OUTPUT_ROOT}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CE_INTERMEDIATE_ROOT}")

option(CE_BUILD_SHARED_MODULES "Build runtime modules as shared libs (DLL/.so/.dylib)" ON)

if(CE_BUILD_SHARED_MODULES)
    set(CE_MODULE_TYPE SHARED)
else()
    set(CE_MODULE_TYPE STATIC)
endif()

function(ce_apply_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_subdirectory(Engine)