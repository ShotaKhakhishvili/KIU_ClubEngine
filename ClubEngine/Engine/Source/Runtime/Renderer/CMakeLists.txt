file(GLOB_RECURSE CERENDERER_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CERENDERER_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CERenderer ${CE_MODULE_TYPE}
  ${CERENDERER_PUBLIC}
  ${CERENDERER_PRIVATE}
)

add_library(ClubEngine::Renderer ALIAS CERenderer)

set_target_properties(CERenderer PROPERTIES OUTPUT_NAME "ClubEngine-Renderer")

target_include_directories(CERenderer
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CERenderer
  PRIVATE
    ClubEngine::Core
    ClubEngine::Asset
    ClubEngine::RHIOpenGL
    ClubEngine::RenderCore
)

target_compile_definitions(CERenderer PRIVATE CERENDERER_BUILD)

ce_apply_warnings(CERenderer)