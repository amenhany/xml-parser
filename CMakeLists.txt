cmake_minimum_required(VERSION 3.16)

project(xml_editor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# --- Core library ---
file(GLOB_RECURSE CORE_SOURCES core/src/*.cpp)
add_library(core STATIC ${CORE_SOURCES})
target_include_directories(core PUBLIC core/include gui/include)

# --- GUI sources ---
set(GUI_SOURCES
    gui/src/main.cpp
    gui/src/gui.cpp
    gui/src/mainwindow.cpp
    gui/include/gui.h
    gui/include/mainwindow.h
    gui/src/mainwindow.ui
)

# --- Executable ---
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(xml_editor
        MANUAL_FINALIZATION
        ${GUI_SOURCES}
    )
else()
    add_executable(xml_editor ${GUI_SOURCES})
endif()

# Include GUI headers
target_include_directories(xml_editor PRIVATE gui/include)

# --- Link libraries ---
target_link_libraries(xml_editor PRIVATE core Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(xml_editor)
endif()
