#ifndef MICRO_SVG_EDITOR_SRC_UI_TOP_TOOL_BAR_H_
#define MICRO_SVG_EDITOR_SRC_UI_TOP_TOOL_BAR_H_

#include <QAction>
#include <QActionGroup>
#include <QToolBar>

class TopToolBar : public QToolBar {
  Q_OBJECT

 public:
  TopToolBar(const QString& title, QWidget* parent, QActionGroup* action_group);

 private slots:
  void ToolSelectionTriggered();

 signals:
  void toolSelected(const QString& tool_name);

 private:
  QAction* AddToolAction(QString name, bool is_toggleable,
                         QActionGroup* action_group, QString icon_path,
                         QString tooltip);
  void SetupToolbar(QActionGroup* action_group);
};

#endif  // MICRO_SVG_EDITOR_SRC_UI_TOP_TOOL_BAR_H_
