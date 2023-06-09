cmake_minimum_required(VERSION 3.0)

# Sets project name
project(GeometryTrash LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Sets C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn off GLFW docs, tests, examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/External/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/External/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/External/glm)

# Specifies .cpp and .h files to be passed to the compiler
add_executable(${PROJECT_NAME}
    main.cpp
    window.cpp
    core.cpp
    player.cpp
    level.cpp
    gametextures.cpp
    wavetrail.cpp
    collision.cpp
    audiosystem.cpp
    song.cpp

    window.h
    core.h
    shaderloader.h
    debug.h
    player.h
    level.h
    colliders.h
    gametextures.h
    wavetrail.h
    collision.h
    settings.h
    audiosystem.h
    song.h

    External/stb_image.h
    External/stb_image_write.h

    External/miniaudio.h
)

# Specifies libraries to be linked (Qt components, glew, etc)
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm
)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

# Set this flag to silence warnings on Windows
if (MSVC OR MSYS OR MINGW)
  set(CMAKE_CXX_FLAGS "-Wno-volatile")
endif()

# Set this flag to silence warnings on MacOS
if (APPLE)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated-volatile")
endif()
