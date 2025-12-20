cmake_minimum_required(VERSION 3.16)
project(xml_parser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(xml_gui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/mainwindow.ui
)


file(GLOB_RECURSE BACKEND_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/xml/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tree/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp
)

target_sources(xml_gui PRIVATE ${BACKEND_SOURCES})

target_include_directories(xml_gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xml_gui PRIVATE Qt6::Widgets)

set_target_properties(xml_gui PROPERTIES
    WIN32_EXECUTABLE TRUE
)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
