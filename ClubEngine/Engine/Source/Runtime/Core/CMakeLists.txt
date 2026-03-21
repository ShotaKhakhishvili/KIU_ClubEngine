file(GLOB_RECURSE CECORE_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CECORE_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CECore ${CE_MODULE_TYPE}
    ${CECORE_PUBLIC}
    ${CECORE_PRIVATE}
)

add_library(ClubEngine::Core ALIAS CECore)

set_target_properties(CECore PROPERTIES OUTPUT_NAME "ClubEngine-Core")

target_include_directories(CECore
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_compile_definitions(CECore PRIVATE CECORE_BUILD)

ce_apply_warnings(CECore)