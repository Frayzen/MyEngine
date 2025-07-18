include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(SetupDeps)

  # For each dependency, see if it's
  # already been provided to us by a parent project
  # find_package(Python3 COMPONENTS Interpreter REQUIRED)

  # execute_process(COMMAND ${Python3_EXECUTABLE} -m venv "${CMAKE_CURRENT_BINARY_DIR}")
  # set(ENV{VIRTUAL_ENV} "${CMAKE_CURRENT_BINARY_DIR}")
  # set(Python3_FIND_VIRTUALENV FIRST)
  # unset(Python3_EXECUTABLE)
  # find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
  # execute_process(COMMAND ${Python3_EXECUTABLE} -m pip install ${_pip_args} jinja2)

  if(NOT TARGET glm)
    cpmaddpackage("gh:g-truc/glm#1.0.1")
  endif()

  if (NOT TARGET GLFW::glfw)
    cpmaddpackage(
      NAME assimp
      GIT_REPOSITORY https://github.com/assimp/assimp.git
      GIT_TAG master
      OPTIONS
        "BUILD_SHARED_LIBS OFF"
        "ASSIMP_BUILD_TESTS OFF"
        "ASSIMP_INJECT_DEBUG_POSTFIX OFF"
        "ASSIMP_INSTALL OFF"
    )
  endif()
  if (NOT TARGET GLFW::glfw)
    cpmaddpackage("gh:glfw/glfw#3.4")
  endif()

  CPMAddPackage(
    NAME glad
    GITHUB_REPOSITORY Dav1dde/glad
    VERSION 2.0.6
    DOWNLOAD_ONLY
  )
  list(APPEND CMAKE_MODULE_PATH ${glad_SOURCE_DIR}/cmake)
  include(GladConfig)
  add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
  set(GLAD_LIBRARY glad_gl_core_33)
  # https://github.com/Dav1dde/glad/wiki/C#generating-during-build-processcmake/
  glad_add_library(${GLAD_LIBRARY} REPRODUCIBLE API gl:core=3.3)

  if (NOT TARGET imgui)
    cpmaddpackage("gh:ocornut/imgui#docking")

    # IMGUI TARGET
    file(GLOB imgui_sources ${imgui_SOURCE_DIR}/*.cpp)
    set(imgui_backend_sources ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)
    add_library(imgui STATIC ${imgui_sources} ${imgui_backend_sources})
    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}/backends ${imgui_SOURCE_DIR})
    target_link_libraries(imgui PUBLIC glfw ${GLAD_LIBRARY} freetype)

    # Override compiler flags for imgui target
    target_compile_options(imgui PRIVATE
        $<$<CONFIG:Debug>: -O2>  # Force optimization level 2 even in Debug
        $<$<CONFIG:Debug>: -DNDEBUG>  # Define NDEBUG in Debug
        $<$<NOT:$<CONFIG:Debug>>: -O2>  # Use O2 for other build types
    )

  endif()
endfunction()

