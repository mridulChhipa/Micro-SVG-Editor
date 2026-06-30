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
    src/io/reader.cc
    src/io/lexer.cc
    src/io/parser.cc
    src/io/parser_helpers.cc
    src/io/lexer_core.cc
    src/io/lexer_helpers.cc
    src/io/lexer_scanners.cc
    src/io/lexer_special.cc
    src/model/graphics_object.cc
    src/model/circle.cc
    src/model/hexagon.cc
    src/model/line.cc
    src/model/path.cc
    src/model/polyline.cc
    src/model/rect.cc
    src/model/svg.cc
    src/model/text.cc
    src/app/main.cc
    src/ui/canvas/canvas.cc
    src/ui/styles/load_style_sheet.cc
    src/ui/canvas/include/tools/stroke_edit.cc
    src/ui/canvas/include/tools/edit_opacity.cc
    src/ui/canvas/include/tools/canvas_edit.cc
    src/ui/canvas/include/tools/border_radius_edit.cc
    src/ui/canvas/include/tools/text_edit.cc
    src/ui/canvas/include/handlers/events/mouse_move_event.cc
    src/ui/canvas/include/handlers/events/mouse_press_event.cc
    src/ui/canvas/include/handlers/events/mouse_release_event.cc
    src/ui/canvas/include/handlers/events/paint_event.cc
    src/ui/canvas/include/handlers/events/wheel_event.cc
    src/ui/canvas/include/add_shapes.cc
    src/ui/canvas/include/path_builder.cc
    src/ui/canvas/include/drag_and_resize.cc
    src/ui/canvas/include/object_creation.cc
    src/ui/canvas/include/paste.cc
    src/ui/canvas/include/render_handles.cc
    src/ui/canvas/include/renderer.cc
    src/ui/canvas/include/stack_operations.cc
    src/ui/canvas/include/tools.cc
    src/ui/app_window_constructor.cc
    src/ui/app_window.cc
    src/ui/menu_bar.cc
    src/ui/left_tool_bar.cc
    src/ui/top_tool_bar.cc
    src/resources.qrc
)

set(SRC_HEADERS
    src/io/reader.h
    src/io/lexer.h
    src/io/parser.h
    src/model/graphics_object.h
    src/model/shape_visitor.h
    src/model/circle.h
    src/model/hexagon.h
    src/model/line.h
    src/model/path.h
    src/model/polyline.h
    src/model/rect.h
    src/model/svg.h
    src/model/text.h
    src/ui/canvas/canvas.h
    src/ui/canvas/include/path_builder.h
    src/ui/app_window.h
    src/ui/menu_bar.h
    src/ui/top_tool_bar.h
    src/ui/left_tool_bar.h
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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MicroSvgEditor PRIVATE -Wall -Wextra)
endif()