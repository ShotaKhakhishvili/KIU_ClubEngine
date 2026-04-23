file(GLOB_RECURSE CERENDERCORE_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CERENDERCORE_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CERenderCore ${CE_MODULE_TYPE}
  ${CERENDERCORE_PUBLIC}
  ${CERENDERCORE_PRIVATE}
)

add_library(ClubEngine::RenderCore ALIAS CERenderCore)

set_target_properties(CERenderCore PROPERTIES OUTPUT_NAME "ClubEngine-RenderCore")

target_include_directories(CERenderCore
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CERenderCore
  PUBLIC
    ClubEngine::Core
)

target_compile_definitions(CERenderCore PRIVATE CERENDERCORE_BUILD)

ce_apply_warnings(CERenderCore)