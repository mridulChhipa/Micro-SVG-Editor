# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

Micro-SVG-Editor is a Qt desktop application (a COP290 course assignment) that opens, edits, renders, and saves a subset of SVG. It is written in C++20 against Qt6 (with a Qt5 fallback) and built with CMake.

## Build & Run

```bash
mkdir build && cd build
cmake ..
cmake --build .
./MicroSvgEditor
```

`CMAKE_AUTOMOC`, `AUTORCC`, and `AUTOUIC` are on — any class with `Q_OBJECT` is moc-processed automatically, and `src/resources.qrc` is compiled in. When you add a new `.cc`/`.h`, you must register it in `SRC_SOURCES`/`SRC_HEADERS` in [CMakeLists.txt](CMakeLists.txt); there is no glob.

Dependencies: `qt6-base-dev` (Widgets) and `cmake`.

On WSL2 the app falls back to software rendering (llvmpipe) unless you force a GPU path before launching:
```bash
export GALLIUM_DRIVER=d3d12
export QT_QPA_PLATFORM=xcb
```
See [README.md](README.md) for the full hardware-acceleration / Mesa-downgrade notes.

### Build gotcha: missing `sample.svg`

`.gitignore` ignores `*.svg`, but [src/resources.qrc](src/resources.qrc) references `testxmls/sample.svg` and `AppWindow::openSampleFile()` loads `:testxmls/sample.svg` on startup. That file is **not committed and is not present in a fresh checkout**, so AUTORCC (and startup) will fail until you create `src/testxmls/sample.svg` with any valid SVG document.

## Tests

There is currently **no test suite in the tree**. The testing commands in [README.md](README.md) (`g++ src/tests/test_parser.cpp ...`) are stale — `src/tests/` and those `.cpp` files no longer exist. Don't rely on them. If you add tests, prefer wiring them into the CMake build.

## Architecture

The app is three layers: an SVG **I/O pipeline** (`src/io`), a **document model** (`src/model`), and a **Qt UI** (`src/ui`). The UI owns a `SVG` document and renders/edits it; the I/O layer translates between `.svg` text and the model.

### I/O pipeline (`src/io`) — text ↔ model

Linear pipeline, each stage wraps the previous by reference:

```
Reader (chars) → Lexer (Tokens) → Parser (SVG model)
```

- [Reader](src/io/Reader.h) feeds characters from a file.
- [Lexer](src/io/Lexer.h) produces `Token`s ([Token.h](src/io/Token.h)). Its implementation is intentionally split across `LexerCore.cc`, `LexerHelpers.cc`, `LexerScanners.cc`, `LexerSpecial.cc` — all define `Lexer::` methods declared in the single `Lexer.h`.
- [Parser](src/io/Parser.h) consumes tokens and builds an `SVG`. `Parser::addTagName` ([ParserHelpers.cc](src/io/ParserHelpers.cc)) is the tag dispatch table — **this is the one place to extend when adding a new shape tag** (it maps `"rect"`, `"circle"`, `"polygon"→Hexagon`, etc. to `std::make_shared<Shape>(attributes)`).

The reverse direction is `SVG::toSVG()` → each `GraphicsObject::toSVG()`, used when saving.

### Model (`src/model`)

[GraphicsObject](src/model/GraphicsObject.h) is the abstract base for every shape. It holds the shared presentation attributes (`fill`, `stroke`, `opacity`, `transform`, …) and declares three pure-virtual methods every shape must implement: `type()`, `toSVG()`, and **`clone()`** (deep copy — load-bearing for undo/redo, see below). `using GraphicsObjectPtr = std::shared_ptr<GraphicsObject>`.

Concrete shapes: `Rect`, `Circle`, `Line`, `Polyline`, `Path`, `Hexagon`, `Text`. Each is constructed from an `unordered_map<string,string>` of XML attributes and calls `initialiseStyle()` for the common attributes.

[SVG](src/model/SVG.h) is the document: dimensions/`viewBox` plus `std::vector<GraphicsObjectPtr> objects`, with `add/remove/clear` and `clone()` (deep-copies every object).

**Adding a new shape** requires touching: a `model/Shape.{h,cc}` pair, the `Parser::addTagName` dispatch, the `ShapeVariant` typedef + `Canvas::toShapeVariant`, a `Canvas::buildShapePath` overload, and `CMakeLists.txt`.

### UI (`src/ui`)

[AppWindow](src/ui/AppWindow.h) (`QMainWindow`) is the root. It owns one `Canvas` (central widget), a `MenuBar`, and two `QToolBar`s (`TopToolBar`, `LeftToolBar`). Its implementation is split: `AppWindow.cc` (file open/save/new slots), `AppWindowConstructor.cc` (construction + all signal wiring), `LoadStyleSheet.cc`.

**All cross-widget communication is via Qt signals/slots, wired in `AppWindow::AppWindow()`.** The menu/toolbars never touch the canvas directly — e.g. `MenuBar` emits `openRequested`/`undoRequested`/… ([MenuBar.h](src/ui/MenuBar.h)) and toolbars emit a single `toolSelected(QString)` carrying the tool name. The canvas slot `Canvas::setcurr_tool(QString)` ([Tools.cc](src/ui/canvas/include/Tools.cc)) is the central dispatcher that maps a tool name string to the corresponding edit action.

Styling: a VS Code-like [style.qss](src/ui/styles/style.qss) loaded from the Qt resource (`:ui/styles/style.qss`) by `loadStyleSheet()`.

### Canvas — the core, split across many files

[Canvas](src/ui/canvas/Canvas.h) (`QWidget`) holds the live `SVG`, the current selection, zoom/offset state, the clipboard, and the undo/redo/temp stacks. The single `Canvas` class is declared in `Canvas.h` but its methods are **implemented across many `.cc` files under [src/ui/canvas/include/](src/ui/canvas/include/)**.

> Naming caveat: the directory is named `include/` but it contains **implementation `.cc` files, not headers**. Every file there starts with `#include "src/ui/canvas/Canvas.h"` and defines `Canvas::` methods. They are grouped by concern:
> - `handlers/events/` — the `QWidget` overrides (`MousePress/Move/ReleaseEvent`, `PaintEvent`, `WheelEvent`).
> - `tools/` — interactive edits (`StrokeEdit`, `EditOpacity`, `BorderRadiusEdit`, `TextEdit`, `CanvasEdit`).
> - `handlers/` — shape-specific resize math (`CircleResizeHandler`, `HexagonResizeHandler`).
> - top level — `Renderer`/`BuildPaths` (model→`QPainterPath`), `RenderHandles`, `DragAndResize`, `AddShapes`/`ObjectCreation`, `Paste`, `StackOperations`, `Tools`.

When editing a `Canvas` method, find it by concern in these files, not in `Canvas.cc` (which holds only the ctor and a few coordinate/lifecycle helpers).

### Two patterns to know

- **`ShapeVariant` + `toShapeVariant()`** ([Canvas.h](src/ui/canvas/Canvas.h), [Canvas.cc](src/ui/canvas/Canvas.cc)): a `std::variant` of `shared_ptr<ConcreteShape>`. `toShapeVariant()` does the `dynamic_pointer_cast` once based on `type()`, so type-specific code (e.g. accessing `x`/`r`/`points`) can `std::visit` instead of re-casting everywhere.

- **Undo/redo stores whole-document snapshots, not diffs** ([StackOperations.cc](src/ui/canvas/include/StackOperations.cc)): `undo_stack`/`redo_stack` are `QVector<SVG>`. Before a mutation, the canvas pushes `svg.clone()` and clears the redo stack. `clone()` (deep copy) is **required** — a plain `SVG` copy shares the `shared_ptr` objects, so without cloning, later edits would mutate the snapshot in the stack. `temp_stack` is used to coalesce a drag/resize gesture into a single undo entry.
