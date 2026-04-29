file(GLOB_RECURSE CEPROJECT_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CEPROJECT_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CEProject ${CE_MODULE_TYPE}
    ${CEPROJECT_PUBLIC}
    ${CEPROJECT_PRIVATE}
)

add_library(ClubEngine::Project ALIAS CEProject)

set_target_properties(CEProject PROPERTIES OUTPUT_NAME "ClubEngine-Project")

target_include_directories(CEProject
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CEProject
    PUBLIC
        ClubEngine::Core
    PRIVATE
        ThirdParty::nlohmann
)

target_compile_definitions(CEProject PRIVATE CEPROJECT_BUILD)

ce_apply_warnings(CEProject)