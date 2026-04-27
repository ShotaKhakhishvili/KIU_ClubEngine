add_executable(AssetImporter
    main.cpp
)

target_link_libraries(AssetImporter
    PRIVATE
        ClubEngine::AssetImport
        ClubEngine::Core
)

ce_apply_warnings(AssetImporter)