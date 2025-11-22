cmake_minimum_required(VERSION 3.15)
project(ClubEngine LANGUAGES C CXX)

# ==========================================================
#  GENERAL CONFIG
# ==========================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================================================
#  SOURCE FILES (ENGINE + GAME TOGETHER)
# ==========================================================
file(GLOB_RECURSE CLUBENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Private/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Public/*.h"
)

# Explicitly add glad.c (required for gladLoadGL)
set(GLAD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/Private/glad.c")
if (NOT EXISTS ${GLAD_SRC})
    message(FATAL_ERROR "GLAD source not found at: ${GLAD_SRC}")
endif()

add_executable(${PROJECT_NAME}
        ${CLUBENGINE_SOURCES}
        ${GLAD_SRC}
)

# ==========================================================
#  INCLUDE PATHS
# ==========================================================
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Public
        ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/include
)

# Add macro for build identification
target_compile_definitions(${PROJECT_NAME} PRIVATE CLUBENGINE_BUILD)

# ==========================================================
#  LINK LIBRARIES
# ==========================================================
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/lib)

if (WIN32)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE glfw3 opengl32 user32 gdi32
    )

    # Force console subsystem for MinGW (so main() is used instead of WinMain)
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
#  WARNINGS
# ==========================================================
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

