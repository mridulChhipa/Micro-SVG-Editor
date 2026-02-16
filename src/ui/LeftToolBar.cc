#include "src/ui/LeftToolBar.h"

QAction *LeftToolBar::addToolAction(QString name, bool is_toggleable, QActionGroup *action_group = nullptr, QString icon_path = "", QString tooltip = "")
{
  QAction *action;
  if (!icon_path.isEmpty())
  {
    action = new QAction(QIcon(icon_path), tooltip, this);
  }
  else
  {
    action = new QAction(name, this);
  }
  if (is_toggleable)
  {
    action->setCheckable(true);
    action->setActionGroup(action_group);
  }

  this->addAction(action);
  return action;
};

void LeftToolBar::setupToolbar(QActionGroup *action_group = nullptr)
{
  if (!action_group)
  {
    action_group = new QActionGroup(this);
    action_group->setExclusive(true);
  }

  QAction *rectangle_action = addToolAction("Rectangle", true, action_group, ":ui/icons/rectangle.jpg", "Rectangle");
  QAction *circle_action = addToolAction("Circle", true, action_group, ":ui/icons/circle.jpg", "Circle");
  QAction *line_action = addToolAction("Line", true, action_group, ":ui/icons/line.jpg", "Line");
  QAction *hexagon_action = addToolAction("Hexagon", true, action_group, ":ui/icons/polygon.jpg", "Hexagon");
  QAction *freehand_action = addToolAction("Freehand", true, action_group);
  QAction *text_action = addToolAction("Text", true, action_group);

  connect(rectangle_action, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
  connect(circle_action, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
  connect(line_action, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
  connect(hexagon_action, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
  connect(freehand_action, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
  connect(text_action, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
}

void LeftToolBar::toolSelectionTriggered()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (action)
  {
    QString tool_name = action->text();
    emit toolSelected(tool_name);
  }
}

LeftToolBar::LeftToolBar(const QString &title, QWidget *parent = nullptr, QActionGroup *action_group = nullptr) : QToolBar(title, parent)
{
  setupToolbar(action_group);
}