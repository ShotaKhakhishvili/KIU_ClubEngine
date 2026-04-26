file(GLOB_RECURSE CEASSET_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CEASSET_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CEAsset ${CE_MODULE_TYPE}
    ${CEASSET_PUBLIC}
    ${CEASSET_PRIVATE}
)

add_library(ClubEngine::Asset ALIAS CEAsset)

set_target_properties(CEAsset PROPERTIES OUTPUT_NAME "ClubEngine-Asset")

target_include_directories(CEAsset
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CEAsset
    PUBLIC
    ClubEngine::CoreUObject
    ClubEngine::RenderCore
    PRIVATE
    ClubEngine::AssetImport
)

target_compile_definitions(CEAsset PRIVATE CEASSET_BUILD)

ce_apply_warnings(CEAsset)