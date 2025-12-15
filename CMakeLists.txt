cmake_minimum_required(VERSION 3.15)
project(ClubEngine LANGUAGES C CXX)

# ==========================================================
#  LANGUAGE STANDARD
# ==========================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================================================
#  SOURCE FILES
# ==========================================================
file(GLOB_RECURSE CLUBENGINE_SOURCES
        "${CMAKE_SOURCE_DIR}/src/Private/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/Private/*.c"
        "${CMAKE_SOURCE_DIR}/src/Public/*.h"
)

add_executable(${PROJECT_NAME}
        ${CLUBENGINE_SOURCES}
        src/Public/Texture.h
        src/Private/Texture.cpp
)

# ==========================================================
#  COMPILE DEFINITIONS
# ==========================================================
# Used by Path::ProjectRoot()
target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        PROJECT_DIR="${CMAKE_SOURCE_DIR}"
        CLUBENGINE_BUILD
)

# ==========================================================
#  INCLUDE DIRECTORIES
# ==========================================================
# Only headers that should be included by engine code
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Public
        ${CMAKE_SOURCE_DIR}/Libraries/include
)

# ==========================================================
#  LINK DIRECTORIES
# ==========================================================
target_link_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/Libraries/lib
)

# ==========================================================
#  PLATFORM LIBRARIES
# ==========================================================
if (WIN32)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            glfw3
            opengl32
            user32
            gdi32
    )

    if (MINGW)
        target_link_options(${PROJECT_NAME}
                PRIVATE "-Wl,-subsystem,console"
        )
    endif()

elseif (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(CORE_FOUNDATION_LIBRARY CoreFoundation REQUIRED)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)

    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            glfw3
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${CORE_FOUNDATION_LIBRARY}
            ${OPENGL_LIBRARY}
    )

elseif (UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)

    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            glfw3
            OpenGL::GL
            ${X11_LIBRARIES}
            Threads::Threads
            dl
            m
    )
endif()

# ==========================================================
#  WARNINGS
# ==========================================================
if (MSVC)
    target_compile_options(${PROJECT_NAME}
            PRIVATE /W4 /permissive-
    )
else()
    target_compile_options(${PROJECT_NAME}
            PRIVATE -Wall -Wextra -Wpedantic
    )
endif()
