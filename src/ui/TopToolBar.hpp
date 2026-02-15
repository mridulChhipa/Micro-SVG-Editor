#ifndef TOP_TOOLBAR_HPP
#define TOP_TOOLBAR_HPP
#include <QToolBar>
#include <QActionGroup>
#include <QAction>

class TopToolBar : public QToolBar
{
  Q_OBJECT
private:
  QAction *addToolAction(QString name, bool isToggleable, QActionGroup *action_group = nullptr, QString icon_path = "", QString tooltip = "")
  {
    QAction *action;
    if (!icon_path.isEmpty())
      action = new QAction(QIcon(icon_path), tooltip, this);
    else
      action = new QAction(name, this);
    if (isToggleable)
    {
      action->setCheckable(true);
      action->setActionGroup(action_group);
    }

    this->addAction(action);
    return action;
  };

  void setupToolbar(QActionGroup *action_group = nullptr)
  {
    if (!action_group)
    {
      action_group = new QActionGroup(this);
      action_group->setExclusive(true);
    }

    QAction *selectAction = addToolAction("Select", true, action_group);

    QAction *fillColorAction = addToolAction("Fill Color", false, action_group);
    QAction *strokeAction = addToolAction("Stroke", false, action_group);
    QAction *fillOpacity = addToolAction("Fill Opacity", false, action_group);
    QAction *strokeOpacity = addToolAction("Stroke Opacity", false, action_group);
    QAction *opacityAction = addToolAction("Opacity", false, action_group);

    QAction *textEditAction = addToolAction("Text Edit", false, action_group);
    QAction *borderRadiusAction = addToolAction("Border Radius", false, action_group);
    QAction *rotateAction = addToolAction("Transform", false, action_group);

    QAction *canvasDimensionsAction = addToolAction("Canvas Dimensions", false, action_group);

    connect(borderRadiusAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
    connect(canvasDimensionsAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

    connect(selectAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

    connect(fillColorAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
    connect(strokeAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
    connect(textEditAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

    QAction *deleteAction = addToolAction("Delete", false, action_group);
    connect(deleteAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

    connect(fillOpacity, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
    connect(rotateAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
    connect(strokeOpacity, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
    connect(opacityAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
  }

public:
  TopToolBar(const QString &title, QWidget *parent = nullptr, QActionGroup *action_group = nullptr) : QToolBar(title, parent)
  {
    setupToolbar(action_group);
  }
private slots:
  void toolSelectionTriggered()
  {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
      QString tool_name = action->text();
      emit toolSelected(tool_name);
    }
  }

signals:
  void toolSelected(const QString &tool_name);
};
#endif