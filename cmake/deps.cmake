function(SetupDeps)
  set(imgui_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/imgui)
  if (NOT EXISTS ${imgui_SOURCE_DIR})
    execute_process(COMMAND git clone https://github.com/ocornut/imgui -b docking --depth 1 ${imgui_SOURCE_DIR})
  endif()

  # IMGUI TARGET
  file(GLOB imgui_sources ${imgui_SOURCE_DIR}/*.cpp)
  file(GLOB imgui_misc_sources ${imgui_SOURCE_DIR}/misc/cpp/*.cpp)
  set(imgui_backend_sources ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)
  add_library(imgui STATIC ${imgui_sources} ${imgui_misc_sources} ${imgui_backend_sources})
  target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}/backends ${imgui_SOURCE_DIR})

  # Override compiler flags for imgui target
  target_compile_options(imgui PRIVATE
      $<$<CONFIG:Debug>: -O2>  # Force optimization level 2 even in Debug
      $<$<CONFIG:Debug>: -DNDEBUG>  # Define NDEBUG in Debug
      $<$<NOT:$<CONFIG:Debug>>: -O2>  # Use O2 for other build types
  )

endfunction()

