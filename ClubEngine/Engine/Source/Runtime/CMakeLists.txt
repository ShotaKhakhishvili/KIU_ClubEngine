add_subdirectory(Core)
add_subdirectory(Asset)
add_subdirectory(Render)
add_subdirectory(Engine)
add_subdirectory(Launch)

add_library(ClubRuntime INTERFACE)
add_library(ClubEngine::Runtime ALIAS ClubRuntime)

target_link_libraries(ClubRuntime INTERFACE
  ClubEngine::Core
  ClubEngine::Asset
  ClubEngine::Engine
  ClubEngine::Render
  ClubEngine::Launch
)
