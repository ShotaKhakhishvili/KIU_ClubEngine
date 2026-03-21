file(GLOB_RECURSE CERENDER_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CERENDER_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CERender ${CE_MODULE_TYPE}
  ${CERENDER_PUBLIC}
  ${CERENDER_PRIVATE}
)

add_library(ClubEngine::Render ALIAS CERender)

set_target_properties(CERender PROPERTIES OUTPUT_NAME "ClubEngine-Render")

target_include_directories(CERender
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CERender
  PUBLIC
    ClubEngine::Core
)

target_compile_definitions(CERender PRIVATE CERENDER_BUILD)

#if (WIN32)
  #target_link_libraries(CERender PUBLIC opengl32)
#endif()

ce_apply_warnings(CERender)