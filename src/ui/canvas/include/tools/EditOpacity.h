#ifndef EDIT_OPACITY_HPP
#define EDIT_OPACITY_HPP

#include <memory>
#include <QWidget>

inline void editOpacity(QWidget *parent, bool &edited, float &prop, std::string title)
{
  bool ok = false;
  double newFillOpacity = QInputDialog::getDouble(parent, QString::fromStdString(title), QString::fromStdString("Enter (decimal fraction)"), prop, 0, 1, 1, &ok, Qt::WindowFlags(), 0.01);

  if (ok)
  {
    prop = newFillOpacity;
    edited = true;
  }
}

#endif