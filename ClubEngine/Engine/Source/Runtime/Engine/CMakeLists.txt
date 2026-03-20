file(GLOB_RECURSE CEENGINE_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CEENGINE_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CEEngine ${CE_MODULE_TYPE}
  ${CEENGINE_PUBLIC}
  ${CEENGINE_PRIVATE}
)

add_library(ClubEngine::Engine ALIAS CEEngine)

set_target_properties(CEEngine PROPERTIES OUTPUT_NAME "ClubEngine-Engine")

target_include_directories(CEEngine
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_compile_definitions(CEEngine PRIVATE CEENGINE_BUILD)

ce_apply_warnings(CEEngine)
