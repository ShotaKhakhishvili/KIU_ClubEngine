add_subdirectory(Engine)
add_subdirectory(Launch)

add_library(ClubRuntime INTERFACE)
add_library(ClubEngine::Runtime ALIAS ClubRuntime)

target_link_libraries(ClubRuntime INTERFACE
  ClubEngine::Engine
  ClubEngine::Launch
)
