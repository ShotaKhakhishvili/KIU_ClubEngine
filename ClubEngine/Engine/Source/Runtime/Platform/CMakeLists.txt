file(GLOB_RECURSE CEPLATFORM_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CEPLATFORM_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CEPlatform ${CE_MODULE_TYPE}
    ${CEPLATFORM_PUBLIC}
    ${CEPLATFORM_PRIVATE}
)

add_library(ClubEngine::Platform ALIAS CEPlatform)

set_target_properties(CEPlatform PROPERTIES OUTPUT_NAME "ClubEngine-Platform")

target_include_directories(CEPlatform
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CEPlatform
    PUBLIC
        ClubEngine::Core
    PRIVATE
        ThirdParty::glfw
        ThirdParty::glad
)

target_compile_definitions(CEPlatform PRIVATE CEPLATFORM_BUILD)

ce_apply_warnings(CEPlatform)