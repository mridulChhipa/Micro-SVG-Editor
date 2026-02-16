#include "src/ui/canvas/Canvas.h"

void Canvas::editBorderRadius(bool &edited)
{
  bool ok;
  int newRadius = QInputDialog::getInt(this, "Border Radius",
                                       "Enter border radius:",
                                       0, 0, 100, 1, &ok);
  if (ok)
  {
    if (selected_shape->type() == "rect")
    {
      auto rect = std::dynamic_pointer_cast<Rect>(*std::find(svg.objects.begin(), svg.objects.end(), selected_shape));
      if (rect)
      {
        rect->rx = newRadius;
        rect->ry = newRadius;
        edited = true;
      }
    }
  }

  curr_tool = "";
}
