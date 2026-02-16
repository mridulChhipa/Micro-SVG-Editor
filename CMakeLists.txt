cmake_minimum_required(VERSION 4.1)
project(MicroSvgEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_REQUIRED_COMPONENTS Widgets)
find_package(Qt6 COMPONENTS ${QT_REQUIRED_COMPONENTS} QUIET)

if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 COMPONENTS ${QT_REQUIRED_COMPONENTS} REQUIRED)
    set(QT_VERSION_MAJOR 5)
endif()

message(STATUS "MicroSvgEditor: Using Qt Major Version: ${QT_VERSION_MAJOR}")

set(SRC_SOURCES
    src/app/main.cc
    src/resources.qrc
)

set(SRC_HEADERS
    src/ui/AppWindow.h
    src/ui/canvas/Canvas.h
    src/ui/MenuBar.h
    src/ui/TopToolBar.h
    src/ui/LeftToolBar.h
)

add_executable(MicroSvgEditor
    ${SRC_SOURCES}
    ${SRC_HEADERS}
)

target_include_directories(MicroSvgEditor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MicroSvgEditor
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
)