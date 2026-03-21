add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Render)
add_subdirectory(Launch)

add_library(ClubRuntime INTERFACE)
add_library(ClubEngine::Runtime ALIAS ClubRuntime)

target_link_libraries(ClubRuntime INTERFACE
  ClubEngine::Core
  ClubEngine::Engine
  ClubEngine::Render
  ClubEngine::Launch
)
