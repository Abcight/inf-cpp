cmake_minimum_required(VERSION 3.15)
project(engine-2d VERSION 1.0.0 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type defaults to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O3)
    endif()
endif()

# Include directories:
# - System packages (GLFW, Lua, OpenAL, libsndfile) are found via find_package/pkg-config
# - Bundled header-only libraries (GLM, sol2, GLAD) in include/
# - Project headers in include/engine/
include_directories(
    ${CMAKE_SOURCE_DIR}/include/engine     # Project headers
    ${CMAKE_SOURCE_DIR}/include            # Bundled third-party headers (for #include <glm/...>)
    ${LUA_INCLUDE_DIRS}                    # Lua system headers
    ${SNDFILE_INCLUDE_DIRS}                # libsndfile system headers
)

# Find required system packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)

# Find Lua using pkg-config
pkg_check_modules(LUA REQUIRED lua>=5.4)

# OpenAL and libsndfile are required for audio
find_package(PkgConfig REQUIRED)
find_library(OPENAL_LIB NAMES openal)
pkg_check_modules(SNDFILE REQUIRED sndfile>=1.0)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

# Header files (all in include/engine/ directory)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/include/engine/*.h"
)

# Create executable
set(TARGET_NAME engine2d)
add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${TARGET_NAME}
    PRIVATE
        ${OPENGL_LIBRARIES}
        glfw
        ${LUA_LIBRARIES}
        ${OPENAL_LIB}
        ${SNDFILE_LIBRARIES}
)

# Link system libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(${TARGET_NAME} PRIVATE
        dl
        pthread
        m
    )
endif()

# Install target (optional)
install(TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION bin
)