cmake_minimum_required(VERSION 3.15)
project(ClubEngine LANGUAGES C CXX)

# ==========================================================
#  GENERAL CONFIG
# ==========================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================================================
#  SOURCE FILES
# ==========================================================
file(GLOB_RECURSE CLUBENGINE_CPP
        "${CMAKE_SOURCE_DIR}/src/Private/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/Private/*.c"
)

file(GLOB_RECURSE CLUBENGINE_HEADERS
        "${CMAKE_SOURCE_DIR}/src/Public/*.h"
)

add_executable(${PROJECT_NAME}
        ${CLUBENGINE_CPP}
        ${CLUBENGINE_HEADERS}
        src/Private/VBO.cpp
        src/Public/VBO.h
        src/Private/EBO.cpp
        src/Public/EBO.h
        src/Private/VAO.cpp
        src/Public/VAO.h
)

# ==========================================================
#  INCLUDE DIRECTORIES
# ==========================================================
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src/Public
        ${CMAKE_SOURCE_DIR}/Libraries/include
)

# ==========================================================
#  LINK LIBRARIES
# ==========================================================
target_link_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}/Libraries/lib
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE glfw3 opengl32 user32 gdi32
    )

    # Force console subsystem for MinGW (so main() runs instead of WinMain)
    if(MINGW)
        target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-subsystem,console")
    endif()

elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_FOUNDATION_LIBRARY CoreFoundation)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE glfw3
            ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${CORE_FOUNDATION_LIBRARY} ${OPENGL_LIBRARY}
    )

elseif(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE glfw3 OpenGL::GL ${X11_LIBRARIES} Threads::Threads dl m
    )
endif()

# ==========================================================
#  SHADER COPYING (IMPORTANT!)
# ==========================================================
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/Private/Shaders
        ${CMAKE_CURRENT_BINARY_DIR}/Shaders
)

# ==========================================================
#  WARNINGS
# ==========================================================
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ==========================================================
#  BUILD FLAG
# ==========================================================
target_compile_definitions(${PROJECT_NAME} PRIVATE CLUBENGINE_BUILD)
