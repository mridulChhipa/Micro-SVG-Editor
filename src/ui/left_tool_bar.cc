#include "src/ui/left_tool_bar.h"

namespace micro_svg_editor {

QAction* LeftToolBar::AddToolAction(QString name, bool is_toggleable,
                                    QActionGroup* action_group,
                                    QString icon_path, QString tooltip) {
  QAction* action;
  if (!icon_path.isEmpty()) {
    action = new QAction(QIcon(icon_path), tooltip, this);
  } else {
    action = new QAction(name, this);
  }
  if (is_toggleable) {
    action->setCheckable(true);
    action->setActionGroup(action_group);
  }

  this->addAction(action);
  return action;
}

void LeftToolBar::SetupToolbar(QActionGroup* action_group) {
  if (!action_group) {
    action_group = new QActionGroup(this);
    action_group->setExclusive(true);
  }

  QAction* rectangle_action = AddToolAction(
      "Rectangle", true, action_group, ":ui/icons/rectangle.jpg", "Rectangle");
  QAction* circle_action = AddToolAction("Circle", true, action_group,
                                         ":ui/icons/circle.jpg", "Circle");
  QAction* line_action =
      AddToolAction("Line", true, action_group, ":ui/icons/line.jpg", "Line");
  QAction* hexagon_action = AddToolAction("Hexagon", true, action_group,
                                          ":ui/icons/polygon.jpg", "Hexagon");
  QAction* freehand_action = AddToolAction("Freehand", true, action_group);
  QAction* text_action = AddToolAction("Text", true, action_group);

  connect(rectangle_action, &QAction::triggered, this,
          &LeftToolBar::ToolSelectionTriggered);
  connect(circle_action, &QAction::triggered, this,
          &LeftToolBar::ToolSelectionTriggered);
  connect(line_action, &QAction::triggered, this,
          &LeftToolBar::ToolSelectionTriggered);
  connect(hexagon_action, &QAction::triggered, this,
          &LeftToolBar::ToolSelectionTriggered);
  connect(freehand_action, &QAction::triggered, this,
          &LeftToolBar::ToolSelectionTriggered);
  connect(text_action, &QAction::triggered, this,
          &LeftToolBar::ToolSelectionTriggered);
}

void LeftToolBar::ToolSelectionTriggered() {
  QAction* action = qobject_cast<QAction*>(sender());
  if (action) {
    QString tool_name = action->text();
    emit toolSelected(tool_name);
  }
}

LeftToolBar::LeftToolBar(const QString& title, QWidget* parent,
                         QActionGroup* action_group)
    : QToolBar(title, parent) {
  SetupToolbar(action_group);
}

}  // namespace micro_svg_editor
