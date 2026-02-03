set(SRC_SOURCES
    app/main.cpp
    resources.qrc
)

set(SRC_HEADERS
    ui/AppWindow.hpp
    ui/canvas/Canvas.hpp
    ui/MenuBar.hpp
    ui/TopToolBar.hpp
    ui/LeftToolBar.hpp
)

add_executable(MicroSvgEditor
    ${SRC_SOURCES}
    ${SRC_HEADERS}
)

target_include_directories(MicroSvgEditor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

target_link_libraries(MicroSvgEditor
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
)

if(MICRO_SVG_EDITOR_ENABLE_WARNINGS)
	if(MSVC)
		target_compile_options(MicroSvgEditor PRIVATE /W4 /permissive- /EHsc)
	else()
		target_compile_options(MicroSvgEditor PRIVATE -Wall -Wextra -Wpedantic)
	endif()
endif()
