file(GLOB_RECURSE CEASSETIMPORT_PUBLIC  CONFIGURE_DEPENDS "Public/*.h" "Public/*.hpp")
file(GLOB_RECURSE CEASSETIMPORT_PRIVATE CONFIGURE_DEPENDS "Private/*.cpp" "Private/*.c")

add_library(CEAssetImport ${CE_MODULE_TYPE}
  ${CEASSETIMPORT_PUBLIC}
  ${CEASSETIMPORT_PRIVATE}
)

add_library(ClubEngine::AssetImport ALIAS CEAssetImport)

set_target_properties(CEAssetImport PROPERTIES OUTPUT_NAME "ClubEngine-AssetImport")

target_include_directories(CEAssetImport
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(CEAssetImport
  PUBLIC
    ClubEngine::RenderCore
  PRIVATE
    ClubEngine::Core
    ThirdParty::stb
)

target_compile_definitions(CEAssetImport PRIVATE CEASSETIMPORT_BUILD)

ce_apply_warnings(CEAssetImport)