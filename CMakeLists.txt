cmake_minimum_required(VERSION 3.16)
project(xml_parser VERSION 0.1 LANGUAGES CXX)

# ---------------------------
# C++ standard
# ---------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------
# Enable Qt features
# ---------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---------------------------
# Find Qt6 Widgets
# ---------------------------
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ---------------------------
# GUI sources
# ---------------------------
set(GUI_SOURCES
    src/gui/main.cpp
    src/gui/mainwindow.cpp
)

set(GUI_HEADERS
    src/gui/mainwindow.h
)

set(UI_FILES
    src/gui/mainwindow.ui
)

# ---------------------------
# Backend sources
# ---------------------------
file(GLOB_RECURSE BACKEND_SOURCES
    src/xml/*.cpp
    src/tree/*.cpp
    src/io/*.cpp
    src/cli/*.cpp
    src/graph/*.cpp
    src/graph/user/*.cpp
)

# ---------------------------
# Define executable
# ---------------------------
# We use add_executable (not qt_add_executable) to avoid WinMain issues
add_executable(xml_editor
    ${GUI_SOURCES}
    ${GUI_HEADERS}
    ${UI_FILES}
    ${BACKEND_SOURCES}
)

# ---------------------------
# Include directories
# ---------------------------
target_include_directories(xml_editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---------------------------
# Link Qt6 Widgets
# ---------------------------
target_link_libraries(xml_editor PRIVATE Qt6::Widgets)

# ---------------------------
# Force console application (no WinMain)
# ---------------------------
set_target_properties(xml_editor PROPERTIES WIN32_EXECUTABLE FALSE)

# ---------------------------
# Copy resources folder to build
# ---------------------------
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
