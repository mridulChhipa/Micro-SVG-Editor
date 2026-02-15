#ifndef CANVAS_EDIT_HPP
#define CANVAS_EDIT_HPP

#include <memory>
#include <QWidget>

inline void canvasEdit(Canvas &canvas, QWidget *parent, bool &edited)
{
  bool okWidth, okHeight;
  int newWidth = QInputDialog::getInt(parent, "Canvas Width",
                                      "Enter canvas width:",
                                      static_cast<int>(canvas.svg.width), 1, 5000, 1, &okWidth);
  int newHeight = QInputDialog::getInt(parent, "Canvas Height",
                                       "Enter canvas height:",
                                       static_cast<int>(canvas.svg.height), 1, 5000, 1, &okHeight);
  if (okWidth && okHeight)
  {
    canvas.svg.width = newWidth;
    canvas.svg.height = newHeight;
    edited = true;
  }

  canvas.curr_tool = "";
}

#endif