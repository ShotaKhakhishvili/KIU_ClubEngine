file(GLOB_RECURSE CELAUNCH_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CELAUNCH_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c" "Private/*.h" "Private/*.hpp")

add_library(CELaunch ${CE_MODULE_TYPE}
  ${CELAUNCH_PUBLIC}
  ${CELAUNCH_PRIVATE}
)

add_library(ClubEngine::Launch ALIAS CELaunch)

set_target_properties(CELaunch PROPERTIES OUTPUT_NAME "ClubEngine-Launch")

target_include_directories(CELaunch
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Public
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CELaunch
  PUBLIC
    ClubEngine::Engine
)

if (WIN32)
  target_link_libraries(CELaunch PUBLIC opengl32)
endif()

target_compile_definitions(CELaunch PRIVATE CELAUNCH_BUILD)

ce_apply_warnings(CELaunch)


