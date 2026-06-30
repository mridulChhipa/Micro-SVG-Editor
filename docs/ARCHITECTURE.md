# Architecture

Micro-SVG-Editor is organised into three layers with a strict one-way
dependency direction. Keeping to these rules is what makes the codebase testable
and open to extension; the `check` build target enforces the most important one.

```
ui  ───▶  io  ───▶  model
```

- **`src/model`** — the pure, **framework-free** domain. The `SVG` document and
  the `GraphicsObject` hierarchy (`Rect`, `Circle`, `Line`, `Polyline`, `Path`,
  `Hexagon`, `Text`). **No Qt, no io, no ui includes here.**
- **`src/io`** — turning bytes into the model and back: `Reader → Lexer →
  Parser`, the `ShapeFactory`, and `SvgLoader` (behind the `ILoader` seam). May
  use the model and Qt; must not depend on ui.
- **`src/ui`** — the Qt views/controllers: `AppWindow`, `Canvas`, toolbars,
  menubar, and the canvas collaborators.

## Key design rules (the SOLID conventions)

- **One class, one responsibility.** `Canvas` is a thin view; its former
  responsibilities live in collaborators: `Viewport` (pan/zoom), `History`
  (undo/redo snapshots), `Clipboard`, `ShapeRenderer` (painting), and the
  `selection` helpers (hit-testing/picking). Geometry shared by rendering and
  hit-testing lives in `shape_geometry` / `handle_geometry`.
- **Type-specific behaviour goes in a visitor, never an RTTI `dynamic_cast` or a
  `Type()` string tag.** The base `GraphicsObject` interface is minimal
  (`Accept`, `Translate`, `ToSvg`, `Clone`). Per-type behaviour is a
  `ShapeVisitor`: `PathBuilder` (rendering geometry), `ResizeVisitor`,
  `FontApplier`, `BorderRadiusVisitor`, `TextEditor`. Derive partial visitors
  from `DefaultShapeVisitor` and override only the shapes you care about.
- **Tools are Command/Strategy objects in a registry.** Each editing tool is a
  `ToolAction` registered in `ToolRegistry`; `Canvas::SetCurrentTool` is a
  lookup, not an `if/else` chain.
- **Shapes are created through `ShapeFactory`** (keyed by SVG tag), so the parser
  depends on an abstraction, not on concrete shape classes.
- **Depend on abstractions at the seams.** `AppWindow` is given an `ILoader`
  (constructor injection); it does not construct the `Reader/Lexer/Parser`
  pipeline itself.

## How to add things (open/closed in practice)

- **A new shape:** add the `model/` class (+ its `Accept`), add a `Visit`
  overload to `ShapeVisitor`, handle it in the visitors that care, and register
  it in `ShapeFactory`. No edits to parser/canvas dispatch logic.
- **A new editing tool:** add a `ToolAction` subclass and register it in
  `ToolRegistry`. No edits to `SetCurrentTool`.

## Conventions & enforcement

- Google C++ Style; formatting via the repo `.clang-format`
  (`clang-format --Werror`), built with `-Wall -Wextra` (zero warnings).
- **Layer check:** `cmake --build build --target check` runs
  `scripts/check_layers.sh`, failing the build if `src/model` gains a Qt/io/ui
  dependency or `src/io` depends on ui.

## Known, deliberate deviations
- Undo/redo uses whole-document **snapshots** (`History`) rather than the Command
  pattern — already SRP/OCP-clean and far lower risk for this snapshot-friendly
  domain.
- Model/shape data members are public-via-accessors data carriers; classes keep
  public accessors rather than full getter/setter encapsulation everywhere.
- `GraphicsObject::PreferStrokeHitTest()` remains a one-line capability query
  (a bool); visitor-ising it would be net-negative.
- The `src/ui/canvas/include/` directory holds `.cc` files (historical name).
