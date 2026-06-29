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
    src/io/Reader.cc
    src/io/Lexer.cc
    src/io/Parser.cc
    src/io/ParserHelpers.cc
    src/io/LexerCore.cc
    src/io/LexerHelpers.cc
    src/io/LexerScanners.cc
    src/io/LexerSpecial.cc
    src/model/GraphicsObject.cc
    src/model/Circle.cc
    src/model/Hexagon.cc
    src/model/Line.cc
    src/model/Path.cc
    src/model/Polyline.cc
    src/model/Rect.cc
    src/model/SVG.cc
    src/model/Text.cc
    src/app/main.cc
    src/ui/canvas/Canvas.cc
    src/ui/styles/LoadStyleSheet.cc
    src/ui/canvas/include/tools/StrokeEdit.cc
    src/ui/canvas/include/tools/EditOpacity.cc
    src/ui/canvas/include/tools/CanvasEdit.cc
    src/ui/canvas/include/tools/BorderRadiusEdit.cc
    src/ui/canvas/include/tools/TextEdit.cc
    src/ui/canvas/include/handlers/CircleResizeHandler.cc
    src/ui/canvas/include/handlers/HexagonResizeHandler.cc
    src/ui/canvas/include/handlers/events/MouseMoveEvent.cc
    src/ui/canvas/include/handlers/events/MousePressEvent.cc
    src/ui/canvas/include/handlers/events/MouseReleaseEvent.cc
    src/ui/canvas/include/handlers/events/PaintEvent.cc
    src/ui/canvas/include/handlers/events/WheelEvent.cc
    src/ui/canvas/include/AddShapes.cc
    src/ui/canvas/include/BuildPaths.cc
    src/ui/canvas/include/DragAndResize.cc
    src/ui/canvas/include/ObjectCreation.cc
    src/ui/canvas/include/Paste.cc
    src/ui/canvas/include/RenderHandles.cc
    src/ui/canvas/include/Renderer.cc
    src/ui/canvas/include/StackOperations.cc
    src/ui/canvas/include/Tools.cc
    src/ui/AppWindowConstructor.cc
    src/ui/AppWindow.cc
    src/ui/MenuBar.cc
    src/ui/LeftToolBar.cc
    src/ui/TopToolBar.cc
    src/resources.qrc
)

set(SRC_HEADERS
    src/io/Reader.h
    src/io/Lexer.h
    src/io/Parser.h
    src/model/GraphicsObject.h
    src/model/Circle.h
    src/model/Hexagon.h
    src/model/Line.h
    src/model/Path.h
    src/model/Polyline.h
    src/model/Rect.h
    src/model/SVG.h
    src/model/Text.h
    src/ui/canvas/Canvas.h
    src/ui/AppWindow.h
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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MicroSvgEditor PRIVATE -Wall -Wextra)
endif()