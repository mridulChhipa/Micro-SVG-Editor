#ifndef STACK_OPERATIONS_HPP
#define STACK_OPERATIONS_HPP

inline void Canvas::undo()
{
    selected_shape = nullptr; // Deselect any shape when undoing
    if (!undoStack.isEmpty())
    {
        std::cout << "Undoing... Current stack size: " << undoStack.size() << std::endl;
        redoStack.push_back(svg); // Save current state for redo
        // std::cout << undoStack.back().toSVG(); // Debug: Print SVG of last state
        svg = undoStack.back(); // Revert to last state
        undoStack.pop_back();   // Remove last state
        update();
    }
}

inline void Canvas::redo()
{
    selected_shape = nullptr; // Deselect any shape when redoing
    if (!redoStack.isEmpty())
    {
        std::cout << "Redoing... Current stack size: " << redoStack.size() << std::endl;
        undoStack.push_back(svg); // Save current state for undo
        // std::cout << redoStack.back().toSVG(); // Debug: Print SVG of last state
        svg = redoStack.back();   // Revert to next state
        redoStack.pop_back();     // Remove last state
        update();                 // Refresh canvas
    }
}

#endif