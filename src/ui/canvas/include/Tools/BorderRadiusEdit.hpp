#ifndef BORDER_RADIUS_HPP
#define BORDER_RADIUS_HPP

#include <memory>
#include <QWidget>

inline void editBorderRadius(Canvas &canvas, QWidget *parent, bool &edited)
{
  bool ok;
  int newRadius = QInputDialog::getInt(parent, "Border Radius",
                                       "Enter border radius:",
                                       0, 0, 100, 1, &ok);
  if (ok)
  {
    if (canvas.selected_shape->type() == "rect")
    {
      auto rect = std::dynamic_pointer_cast<Rect>(*std::find(canvas.svg.objects.begin(), canvas.svg.objects.end(), canvas.selected_shape));
      if (rect)
      {
        rect->rx = newRadius;
        rect->ry = newRadius;
        edited = true;
        canvas.update();
      }
    }
  }

  canvas.curr_tool = "";
}

#endif