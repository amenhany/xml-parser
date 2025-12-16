cmake_minimum_required(VERSION 3.16)

project(xml_gui VERSION 0.1 LANGUAGES CXX)

# Enable automatic processing of Qt UI, MOC, and RCC files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fix for noexcept static_assert errors in Qt6 headers
add_compile_options(-std=c++17 -fexceptions)

# Find Qt6 explicitly (Widgets module is required)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# List of project source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Create the Qt6 executable
qt_add_executable(xml_gui
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Link Qt6 Widgets module
target_link_libraries(xml_gui PRIVATE Qt6::Widgets)

# Windows-specific property for GUI application
set_target_properties(xml_gui PROPERTIES WIN32_EXECUTABLE TRUE)

# Optional: macOS bundle settings (only relevant if building on macOS)
if(Qt6_VERSION VERSION_LESS "6.1.0")
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.xml_gui)
endif()
set_target_properties(xml_gui PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
)

# Installation rules
include(GNUInstallDirs)
install(TARGETS xml_gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize Qt6 executable
qt_finalize_executable(xml_gui)
