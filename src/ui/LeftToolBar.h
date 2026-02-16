#ifndef LEFT_TOOLBAR_HPP
#define LEFT_TOOLBAR_HPP

#include <QToolBar>
#include <QActionGroup>
#include <QAction>

class LeftToolBar : public QToolBar
{
  Q_OBJECT

private:
  QAction *addToolAction(QString name, bool is_toggleable, QActionGroup *action_group, QString icon_path, QString tooltip);
  void setupToolbar(QActionGroup *action_group);

public:
  LeftToolBar(const QString &title, QWidget *parent, QActionGroup *action_group);

private slots:
  // This trigers a common signal for all different tools with different tool_name
  void toolSelectionTriggered();

signals:
  // This signal is emitted with corresponding tool_name
  void toolSelected(const QString &tool_name);
};
#endif