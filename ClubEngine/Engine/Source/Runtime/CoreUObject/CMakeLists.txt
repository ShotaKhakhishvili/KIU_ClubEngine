file(GLOB_RECURSE CECOREUOBJECT_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CECOREUOBJECT_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CECoreUObject ${CE_MODULE_TYPE}
    ${CECOREUOBJECT_PUBLIC}
    ${CECOREUOBJECT_PRIVATE}
)

add_library(ClubEngine::CoreUObject ALIAS CECoreUObject)

set_target_properties(CECoreUObject PROPERTIES OUTPUT_NAME "ClubEngine-CoreUObject")

target_include_directories(CECoreUObject
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CECoreUObject
    PUBLIC
    ClubEngine::Core
)

target_compile_definitions(CECoreUObject PRIVATE CECOREUOBJECT_BUILD)

ce_apply_warnings(CECoreUObject)