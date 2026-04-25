file(GLOB_RECURSE CERHIOPENGL_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CERHIOPENGL_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CERHIOpenGL ${CE_MODULE_TYPE}
  ${CERHIOPENGL_PUBLIC}
  ${CERHIOPENGL_PRIVATE}
)

add_library(ClubEngine::RHIOpenGL ALIAS CERHIOpenGL)

set_target_properties(CERHIOpenGL PROPERTIES OUTPUT_NAME "ClubEngine-RHI-OpenGL")

target_include_directories(CERHIOpenGL
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CERHIOpenGL
  PUBLIC
    ClubEngine::RenderCore
  PRIVATE
    ClubEngine::Core
    ThirdParty::glad
    ThirdParty::glm
)

target_compile_definitions(CERHIOpenGL PRIVATE CERHIOPENGL_BUILD)

if (WIN32)
  target_link_libraries(CERHIOpenGL PRIVATE opengl32)
endif()

ce_apply_warnings(CERHIOpenGL)