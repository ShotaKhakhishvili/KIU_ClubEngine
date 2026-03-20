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
