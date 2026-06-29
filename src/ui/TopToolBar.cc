#include "src/ui/TopToolBar.h"

QAction* TopToolBar::AddToolAction(QString name, bool is_toggleable,
                                   QActionGroup* action_group = nullptr,
                                   QString icon_path = "",
                                   QString tooltip = "") {
  QAction* action;
  if (!icon_path.isEmpty())
    action = new QAction(QIcon(icon_path), tooltip, this);
  else
    action = new QAction(name, this);
  if (is_toggleable) {
    action->setCheckable(true);
    action->setActionGroup(action_group);
  }

  this->addAction(action);
  return action;
}

void TopToolBar::SetupToolbar(QActionGroup* action_group = nullptr) {
  if (!action_group) {
    action_group = new QActionGroup(this);
    action_group->setExclusive(true);
  }

  QAction* select_action = AddToolAction("Select", true, action_group);

  QAction* fill_color_action = AddToolAction("Fill Color", false, action_group);
  QAction* stroke_action = AddToolAction("Stroke", false, action_group);
  QAction* fill_opacity = AddToolAction("Fill Opacity", false, action_group);
  QAction* stroke_opacity =
      AddToolAction("Stroke Opacity", false, action_group);
  QAction* opacity_action = AddToolAction("Opacity", false, action_group);

  QAction* text_edit_action = AddToolAction("Text Edit", false, action_group);
  QAction* border_radius_action =
      AddToolAction("Border Radius", false, action_group);
  QAction* rotate_action = AddToolAction("Transform", false, action_group);

  QAction* canvas_dimensions_action =
      AddToolAction("Canvas Dimensions", false, action_group);

  connect(border_radius_action, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);
  connect(canvas_dimensions_action, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);

  connect(select_action, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);

  connect(fill_color_action, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);
  connect(stroke_action, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);
  connect(text_edit_action, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);

  QAction* delete_action = AddToolAction("Delete", false, action_group);
  connect(delete_action, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);

  connect(fill_opacity, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);
  connect(rotate_action, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);
  connect(stroke_opacity, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);
  connect(opacity_action, &QAction::triggered, this,
          &TopToolBar::ToolSelectionTriggered);
}

void TopToolBar::ToolSelectionTriggered() {
  QAction* action = qobject_cast<QAction*>(sender());
  if (action) {
    QString tool_name = action->text();
    emit toolSelected(tool_name);
  }
}

TopToolBar::TopToolBar(const QString& title, QWidget* parent = nullptr,
                       QActionGroup* action_group = nullptr)
    : QToolBar(title, parent) {
  SetupToolbar(action_group);
}
