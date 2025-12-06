cmake_minimum_required(VERSION 3.16)

project(xml_editor LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add include directory using modern target_include_directories
# (PUBLIC = visible to this target and anything linking to it)
add_executable(xml_editor)

target_include_directories(xml_editor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Collect all .cpp files recursively
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

target_sources(xml_editor PRIVATE ${SOURCES})