#ifndef MICRO_SVG_EDITOR_SRC_UI_LEFT_TOOL_BAR_H_
#define MICRO_SVG_EDITOR_SRC_UI_LEFT_TOOL_BAR_H_

#include <QAction>
#include <QActionGroup>
#include <QToolBar>

class LeftToolBar : public QToolBar {
  Q_OBJECT

 public:
  LeftToolBar(const QString& title, QWidget* parent,
              QActionGroup* action_group);

 private slots:
  // Triggers a common signal for all the different tools, each with its own
  // tool_name.
  void ToolSelectionTriggered();

 signals:
  // Emitted with the corresponding tool_name.
  void toolSelected(const QString& tool_name);

 private:
  QAction* AddToolAction(QString name, bool is_toggleable,
                         QActionGroup* action_group, QString icon_path,
                         QString tooltip);
  void SetupToolbar(QActionGroup* action_group);
};

#endif  // MICRO_SVG_EDITOR_SRC_UI_LEFT_TOOL_BAR_H_
