# Micro Vector Editor (SVG)

## Testing  Modules
Run the following commands to
- test the parser:  `g++ src/tests/test_parser.cpp -o test_parser && ./test_parser`
- test the lexer:  `g++ src/tests/test_lexer.cpp -o test_lexer && ./test_lexer`

## Environment Setup
- The following packages need to be installed:
    `qt6-base`.
- Need to create a *symbolic link* of `style.qss`.

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
    # 1. Force Qt to use X11 (Stable)
    export QT_QPA_PLATFORM=xcb

    # 2. Force Mesa to use the D3D12 driver directly (Fixes Zink Error)
    export GALLIUM_DRIVER=d3d12

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
    
- **Note** When you will install libgl and mesa, then version 25.2.8 of mesa does not support gpu-acceleration due to which you may need to downgrade to version 24 of mesa. The following command will help you for the corresponding task `
sudo apt-get install --allow-downgrades libgl1-mesa-dri=24.0.5-1ubuntu1 libglx-mesa0=24.0.5-1ubuntu1 libglapi-mesa=24.0.5-1ubuntu1 libegl-mesa0=24.0.5-1ubuntu1 libgbm1=24.0.5-1ubuntu1
`

# Design Decisions
- This assignment had a strong emphasis on ownership and scoping of variables. One of the places where did placed a major role was when we do undo-redo operations. Putting a SVG object in the stack creates a shallow copy which updates the SVG->objects because they are references and therefore even though we add a SVG object to the stack.
    - Question: Why is creation of a shallow copy is not needed when we add a new shape ? 
    - Answer: Because whenver I am adding a new shape, a new pointer is created therefore, since SVG is not a ptr, therefore its object vector has distinct elements though all the old and new ptrs still exist.
    - Therefore to handle this, a clone function was added to SVG and GraphicsObject (for deepcopy of SVG)

- Connections and signals were used to connect various components of the UI (eg. Canvas, MenuBar, TopToolBar, etc)
- To reduce complexity of undo-redo operations we have been adding the complete SVG and not the changes.
- To make components as much reusable, the reader and lexer were implemented seprately from the parser.

## Potential Scopes for Optimization
- Use QuadTrees for optimization if needed in future
- Use Debouncer for optimizing mouse move events if needed in future
