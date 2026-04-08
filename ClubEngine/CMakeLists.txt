cmake_minimum_required(VERSION 3.20)
project(ClubEngine LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(CE_BUILD_SHARED_MODULES "Build runtime modules as shared libs (DLL/.so/.dylib)" ON)

if (CE_BUILD_SHARED_MODULES)
  set(CE_MODULE_TYPE SHARED)
else()
  set(CE_MODULE_TYPE STATIC)
endif()

function(ce_apply_warnings target)
  if (MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endfunction()

add_subdirectory(Engine)
add_subdirectory(Projects)
# add logger logic to the engine
# firstly, we need to create a logger class that can be used throughout the engine
# this logger class should be able to log messages to the console and to a file
# we can use spdlog for this, as it is a header-only library that is easy to integrate into our engine
# we can create a Logger class that wraps around spdlog and provides a simple interface for logging messages
# we can also create different log levels (e.g. info, warning, error) and allow the user to set the log level at runtime
# we can also create a log file that is rotated daily, so that we don't have to worry about log files getting too large 

#we should create a file called Logger.h in the Engine directory, and a file called Logger.cpp in the same directory at ./