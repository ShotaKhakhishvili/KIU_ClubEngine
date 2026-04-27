cmake_minimum_required(VERSION 3.20)

project([PROJ_NAME] LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED ENV{CLUBENGINE_ROOT})
    message(FATAL_ERROR "CLUBENGINE_ROOT is not set. Run club-init first.")
endif()

add_subdirectory(
    "$ENV{CLUBENGINE_ROOT}"
    "${CMAKE_BINARY_DIR}/ClubEngine"
)

file(GLOB_RECURSE [PROJ_NAME]_PUBLIC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/[PROJ_NAME]/Public/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/[PROJ_NAME]/Public/*.hpp"
)

file(GLOB_RECURSE [PROJ_NAME]_PRIVATE CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/[PROJ_NAME]/Private/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/[PROJ_NAME]/Private/*.c"
)

add_executable([PROJ_NAME]
    Source/Entry.cpp
    ${[PROJ_NAME]_PUBLIC}
    ${[PROJ_NAME]_PRIVATE}
)

target_include_directories([PROJ_NAME]
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/[PROJ_NAME]/Public
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/[PROJ_NAME]/Private
)

target_link_libraries([PROJ_NAME]
    PRIVATE
        ClubEngine::Launch
)

ce_apply_warnings([PROJ_NAME])