#include "src/ui/canvas/Canvas.h"

// My undo redo stores the whole svg and creates a deepcopy so that new operations don't do changes to ptr in stacks
void Canvas::undo()
{
  selected_shape = nullptr; // Deselect any shape when undoing
  if (!undo_stack.isEmpty())
  {
    redo_stack.push_back(svg.clone());
    svg = undo_stack.back();
    undo_stack.pop_back();
    update();
  }
}

void Canvas::redo()
{
  selected_shape = nullptr;
  if (!redo_stack.isEmpty())
  {
    undo_stack.push_back(svg.clone());
    svg = redo_stack.back();
    redo_stack.pop_back();
    update();
  }
}

void Canvas::cut()
{
  if (selected_shape != nullptr)
  {
    undo_stack.push_back(svg.clone());
    redo_stack.clear();

    copy();
    auto it = std::find(svg.objects.begin(), svg.objects.end(), selected_shape);
    if (it != svg.objects.end())
    {
      svg.objects.erase(it);
      selected_shape = nullptr;
    }
    update();
  }
}

void Canvas::copy()
{
  if (selected_shape != nullptr)
    clipboard_shape = selected_shape->clone(); // Deep copy of the selected shape
}