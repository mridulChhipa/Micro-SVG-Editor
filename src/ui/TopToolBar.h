#ifndef TOP_TOOLBAR_HPP
#define TOP_TOOLBAR_HPP
#include <QToolBar>
#include <QActionGroup>
#include <QAction>

class TopToolBar : public QToolBar
{
  Q_OBJECT
private:
  QAction *addToolAction(QString name, bool isToggleable, QActionGroup *action_group, QString icon_path, QString tooltip);

  void setupToolbar(QActionGroup *action_group);

public:
  TopToolBar(const QString &title, QWidget *parent, QActionGroup *action_group);
private slots:
  void toolSelectionTriggered();

signals:
  void toolSelected(const QString &tool_name);
};
#endif