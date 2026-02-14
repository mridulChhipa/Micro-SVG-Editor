#ifndef TOP_TOOLBAR_HPP
#define TOP_TOOLBAR_HPP
#include <QToolBar>
#include <QActionGroup>
#include <QAction>

class TopToolBar : public QToolBar
{
    Q_OBJECT
private:
    QAction *addToolAction(QString name, bool isToggleable, QActionGroup *actionGroup = nullptr, QString icon_path = "", QString tooltip = "")
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
        if (isToggleable)
        {
            action->setCheckable(true);
            action->setActionGroup(actionGroup);
        }

        this->addAction(action);
        return action;
    };

    void setupToolbar(QActionGroup *actionGroup = nullptr)
    {
        if (!actionGroup)
        {
            actionGroup = new QActionGroup(this);
            actionGroup->setExclusive(true);
        }

        QAction *selectAction = addToolAction("Select", true, actionGroup);

        QAction *fillColorAction = addToolAction("Fill Color", false, actionGroup);
        QAction *strokeAction = addToolAction("Stroke", false, actionGroup);
        QAction *fillOpacity = addToolAction("Fill Opacity", false, actionGroup);
        QAction *strokeOpacity = addToolAction("Stroke Opacity", false, actionGroup);
        QAction *opacityAction = addToolAction("Opacity", false, actionGroup);

        QAction *textEditAction = addToolAction("Text Edit", false, actionGroup);
        QAction *borderRadiusAction = addToolAction("Border Radius", false, actionGroup);
        QAction *rotateAction = addToolAction("Transform", false, actionGroup);

        QAction *canvasDimensionsAction = addToolAction("Canvas Dimensions", false, actionGroup);

        connect(borderRadiusAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(canvasDimensionsAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

        connect(selectAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

        connect(fillColorAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(strokeAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(textEditAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

        QAction *deleteAction = addToolAction("Delete", false, actionGroup);
        connect(deleteAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

        connect(fillOpacity, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(rotateAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(strokeOpacity, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(opacityAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
    }

public:
    TopToolBar(const QString &title, QWidget *parent = nullptr, QActionGroup *actionGroup = nullptr) : QToolBar(title, parent)
    {
        // Toolbar setup can be done here
        setupToolbar(actionGroup);
    }
private slots:
    void toolSelectionTriggered()
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
        {
            QString toolName = action->text();
            emit toolSelected(toolName);
        }
    }

signals:
    void toolSelected(const QString &toolName);
};
#endif