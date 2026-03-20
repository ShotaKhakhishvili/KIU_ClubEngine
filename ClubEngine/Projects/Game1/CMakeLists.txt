add_subdirectory(Source/Sandbox)

add_executable(Game1
  Source/Entry.cpp
)

target_link_libraries(Game1
  PRIVATE
    Game1Sandbox
)

# Optional: warnings 
ce_apply_warnings(Game1)
