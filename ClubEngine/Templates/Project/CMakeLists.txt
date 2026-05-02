cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0144 NEW)
project([PROJ_NAME] LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ClubEngine REQUIRED
    PATHS "${CLUBENGINE_ROOT}/Binaries/${CE_PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/${CE_ENGINE_VERSION}"
    NO_DEFAULT_PATH
)

file(GLOB_RECURSE [PROJ_NAME]_PUBLIC  CONFIGURE_DEPENDS "Source/[PROJ_NAME]/Public/*.h")
file(GLOB_RECURSE [PROJ_NAME]_PRIVATE CONFIGURE_DEPENDS "Source/[PROJ_NAME]/Private/*.cpp")

add_executable([PROJ_NAME]
    Source/Entry.cpp
    ${[PROJ_NAME]_PUBLIC}
    ${[PROJ_NAME]_PRIVATE}
)

target_include_directories([PROJ_NAME] PRIVATE
    Source/[PROJ_NAME]/Public
    Source/[PROJ_NAME]/Private
)

target_link_libraries([PROJ_NAME] PRIVATE
    ClubEngine::Launch
)

add_custom_command(TARGET [PROJ_NAME] POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CLUBENGINE_ROOT}/Binaries/${CE_PLATFORM_NAME}/${CMAKE_BUILD_TYPE}/${CE_ENGINE_VERSION}"
        "$<TARGET_FILE_DIR:[PROJ_NAME]>"
)