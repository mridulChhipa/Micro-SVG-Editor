# Micro Vector Editor (SVG)

## Testing  Modules
Run the following commands to
- test the parser:  `g++ src/tests/test_parser.cpp -o test_parser && ./test_parser`
- test the lexer:  `g++ src/tests/test_lexer.cpp -o test_lexer && ./test_lexer`

## Environment Setup
- The following packages need to be installed:
    `qt6-base-dev`.
- Run 
    1. `sudo snap install cmake --classic` to install cmake. this may take a while.
    2. `sudo apt install qt6-base-dev`

## Building the Application (using Cmake)
- Make a build directory using `mkdir build` at the root folder of the project and enter it through `cd build`
- Next run the following commands in **./build** folder:
    ```
    cmake ..
    cmake --build .
    ```

## Fixing Rendering and Harware Acceleration Issues
- To use hardware acceleration :
    ```
    # 1. Force Mesa to use the D3D12 driver directly (Fixes Zink Error)
    export GALLIUM_DRIVER=d3d12

    # 2. Force Qt to use X11 (Stable)
    export QT_QPA_PLATFORM=xcb

    Optionally if you have NVIDIA-GPU and want to use it
    # 3. Force the D3D12 driver to pick your NVIDIA card
    export MESA_D3D12_DEFAULT_ADAPTER_NAME=NVIDIA
    ```
- You can check the rendering device via `glxinfo -B` and check `Device: D3D12`
    ```
    Extended renderer info (GLX_MESA_query_renderer):
    Vendor: Microsoft Corporation (0xffffffff)
        Device: D3D12 (NVIDIA GeForce RTX 3050 6GB Laptop GPU) (0xffffffff)
        Version: 24.0.5
    ```
    If it shows llvmpipe then acceleration is not working and some fixes are required
    ...
    
## **Note**
- When you will install libgl and mesa, then version 25.2.8 does not support gpu-acceleration due to which you may need to downgrade to version 24 of mesa. The following command will help you for the corresponding task `
sudo apt-get install --allow-downgrades libgl1-mesa-dri=24.0.5-1ubuntu1 libglx-mesa0=24.0.5-1ubuntu1 libglapi-mesa=24.0.5-1ubuntu1 libegl-mesa0=24.0.5-1ubuntu1 libgbm1=24.0.5-1ubuntu1
`
- 

## Design Decisions
- To make components as much reusable, the reader and lexer were implemented seprately from the parser.
- The app was split into the following four components :
    - The AppWindow: It is the main window which appears to the user when he opens the application. All the other three components are rendered on it.
    - The MenuBar: I have tried to keep it and style it so that it looks similar to the menubar of VS Code.
    - Toolbars: The tools related to drawing on the canvas are put in them and split into two (Left and Top) for future enhancements and updates.
    - The Canvas: It is the working area where all the objects are drawn. For convenience it is taken as the svg width and height parameters.
- A separate *style.qss* file was used in the early developments stages which was connected with a watcher too. This allowed reloading the ui simultaneously with saving changes in the style.qss even when the app is running without even having to build the binary/executable again and again for small changes in the styling.
- Since all shapes shared some common features like stroke, fill, opacity, etc. Therefore it was decided to inherit them from a simpler parent class of GraphicsObject.
- Connections, Slots and Signals were created to communicate between the menubars, toolbars and the canvas.
- To reduce complexity of undo-redo operations we have been adding the complete SVG and not the changes.
This assignment had a strong emphasis on ownership and scoping of variables. One of the places where did placed a major role was when we do undo-redo operations. Putting a SVG object in the stack creates a shallow copy which updates the SVG->objects because they are references and therefore even though we add a SVG object to the stack.
    - Question: Why is creation of a shallow copy is not needed when we add a new shape ? 
    - Answer: Because whenver I am adding a new shape, a new pointer is created therefore, since SVG is not a ptr, therefore its object vector has distinct elements though all the old and new ptrs still exist.
    - Therefore to handle this, a clone function was added to SVG and GraphicsObject (for deepcopy of SVG)
- The componets were made movable and resizable. The text size feature was not provided specifically instead, the resize operation would let you to change the text size.
- To prevent not defining objects again and again, std::variant was used so that editing objects becomes easy because the variant directly returns the variant of pointer of that specific type. This was done because many attributes such as x, y, r, points, etc. were used as shape specific attributes which would require to define object pointers again and again.

## Problems faces and solutions
- As and when I had started to split my program into components, the exit button stopped working and created a bug that just closed the menubar. So that happened because exit() on `this` closes this and when I had split it was refering to the menubar. So I had created an exit signal that call exit for the application through the canvas going upto appwindow.

## Current Bugs
- The select option works properly but without selecting the select option you won't be able to select just by clicking, you would need to drag it a bit.
- Currently handling transforms it a bit difficult specially in cases of translate.
- The handles of lines are also not properly created as of now;

## Usage
- Scroll Wheel works as zoom
- Undo redo are in edit tab in menubar at the top
- File opening and saving related commands are in file tab of the menu bar

## File Organization
```
MicroSvgEditor/
├── src/
│   ├── app/
│   │   └── main.cpp
│   ├── io/
│   │   ├── Lexer.h
│   │   ├── LexerCore.h
│   │   ├── LexerHelpers.h
│   │   ├── LexerScanners.h
│   │   ├── LexerSpecial.h
│   │   ├── Parser.h
│   │   ├── Reader.h
│   │   └── Token.h
│   ├── model/
│   │   ├── Circle.h
│   │   ├── GraphicsObject.h
│   │   ├── Hexagon.h
│   │   ├── Line.h
│   │   ├── Path.h
│   │   ├── Polyline.h
│   │   ├── Rect.h
│   │   ├── Svg.h
│   │   └── Text.h
│   ├── tests/
│   │   ├── test_lexer.cpp
│   │   └── test_parser.cpp
│   ├── ui/
│   │   ├── canvas/
│   │   │   ├── include/
│   │   │   │   ├── handlers/
│   │   │   │   │   ├── events/
│   │   │   │   │   │   ├── MouseMoveEvent.h
│   │   │   │   │   │   ├── MousePressEvent.h
│   │   │   │   │   │   ├── MouseReleaseEvent.h
│   │   │   │   │   │   ├── PaintEvent.h
│   │   │   │   │   │   └── WheelEvent.h
│   │   │   │   │   ├── CircleResizeHandler.h
│   │   │   │   │   └── HexagonResizeHandler.h
│   │   │   │   ├── tools/
│   │   │   │   │   ├── BorderRadiusEdit.h
│   │   │   │   │   ├── CanvasEdit.h
│   │   │   │   │   ├── EditOpacity.h
│   │   │   │   │   ├── StrokeEdit.h
│   │   │   │   │   └── TextEdit.h
│   │   │   │   ├── AddShapes.h
│   │   │   │   ├── BuildPaths.h
│   │   │   │   ├── DragAndResize.h
│   │   │   │   ├── ObjectCreation.h
│   │   │   │   ├── Paste.h
│   │   │   │   ├── Renderer.h
│   │   │   │   ├── RenderHandles.h
│   │   │   │   ├── StackOperations.h
│   │   │   │   └── Tools.h
│   │   │   ├── Canvas.h
│   │   │   ├── CanvasHeaders.h
│   │   │   ├── CanvasParts.h
│   │   │   └── HandleType.h
│   │   ├── icons/
│   │   │   ├── circle.jpg
│   │   │   ├── line.jpg
│   │   │   ├── polygon.jpg
│   │   │   └── rectangle.jpg
│   │   ├── styles/
│   │   │   ├── LoadStyleSheet.h
│   │   │   └── style.qss
│   │   ├── AppWindow.h
│   │   ├── AppWindowHeaders.h
│   │   ├── LeftToolBar.h
│   │   ├── MenuBar.h
│   │   ├── MenuBarOptions.h
│   │   └── TopToolBar.h
│   └── resources.qrc
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Scope of Improvements and Optimizations
- Use QuadTrees for optimization if needed in future
- Use Debouncer for optimizing mouse move events if needed in future
