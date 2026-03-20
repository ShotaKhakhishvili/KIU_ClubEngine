# ---------------- GLFW (build from source) ----------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(GLFW)
add_library(ThirdParty::glfw ALIAS glfw)

# Prevent GLFW from including OpenGL headers (we use GLAD)
target_compile_definitions(glfw INTERFACE GLFW_INCLUDE_NONE)

# ---------------- GLM (header-only) ----------------
add_library(ThirdParty_glm INTERFACE)
add_library(ThirdParty::glm ALIAS ThirdParty_glm)

target_include_directories(ThirdParty_glm INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/glm
)

# ---------------- STB (header-only) ----------------
add_library(ThirdParty_stb INTERFACE)
add_library(ThirdParty::stb ALIAS ThirdParty_stb)

target_include_directories(ThirdParty_stb INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/stb
)

# ---------------- GLAD (compile glad.c) ----------------
add_library(ThirdParty_glad STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
)
add_library(ThirdParty::glad ALIAS ThirdParty_glad)

target_include_directories(ThirdParty_glad PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

set_target_properties(ThirdParty_glad PROPERTIES POSITION_INDEPENDENT_CODE ON)
