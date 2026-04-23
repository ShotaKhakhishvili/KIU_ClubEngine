add_subdirectory(Core)
add_subdirectory(CoreUObject)
add_subdirectory(Asset)
add_subdirectory(AssetImport)
add_subdirectory(RenderCore)
add_subdirectory(RHI.OpenGL)
add_subdirectory(Renderer)
add_subdirectory(Engine)
add_subdirectory(Launch)

add_library(ClubRuntime INTERFACE)
add_library(ClubEngine::Runtime ALIAS ClubRuntime)

target_link_libraries(ClubRuntime INTERFACE
  ClubEngine::Core
  ClubEngine::CoreUObject
  ClubEngine::Asset
  ClubEngine::RenderCore
  ClubEngine::RHIOpenGL
  ClubEngine::Renderer
  ClubEngine::Engine
  ClubEngine::Launch
)
