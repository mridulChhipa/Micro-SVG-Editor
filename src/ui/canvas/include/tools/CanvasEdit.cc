#include "src/ui/canvas/Canvas.h"

void Canvas::canvasEdit(bool &edited)
{
  bool w_enter, okHeight;
  int newWidth = QInputDialog::getInt(this, "Canvas Width",
                                      "Enter canvas width:",
                                      static_cast<int>(svg.width), 1, 5000, 1, &w_enter);
  int newHeight = QInputDialog::getInt(this, "Canvas Height",
                                       "Enter canvas height:",
                                       static_cast<int>(svg.height), 1, 5000, 1, &okHeight);
  if (w_enter && okHeight)
  {
    svg.width = newWidth;
    svg.height = newHeight;
    edited = true;
  }

  curr_tool = "";
}
