file(GLOB_RECURSE GAME1_SANDBOX_PUBLIC CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Public/*.hpp"
)

file(GLOB_RECURSE GAME1_SANDBOX_PRIVATE CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.c"
)

add_library(Game1Sandbox
  ${GAME1_SANDBOX_PUBLIC}
  ${GAME1_SANDBOX_PRIVATE}
)

target_include_directories(Game1Sandbox
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(Game1Sandbox
  PUBLIC
    ClubEngine::Runtime
)

ce_apply_warnings(Game1Sandbox)