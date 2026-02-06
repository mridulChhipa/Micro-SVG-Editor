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

    void setupToolbar()
    {
        QActionGroup *actionGroup = new QActionGroup(this);
        actionGroup->setExclusive(true);

        QAction *selectAction = addToolAction("Select", true, actionGroup);
        QAction *moveAction = addToolAction("Move", true, actionGroup);
        QAction *rectangleAction = addToolAction("Rectangle", true, actionGroup, ":ui/icons/rectangle.jpg", "Rectangle");
        QAction *circleAction = addToolAction("Circle", true, actionGroup, ":ui/icons/circle.jpg", "Circle");
        QAction *lineAction = addToolAction("Line", true, actionGroup, ":ui/icons/line.jpg", "Line");

        QAction *fillColorAction = addToolAction("Fill Color", false, actionGroup);
        QAction *strokeAction = addToolAction("Stroke", false, actionGroup);
        QAction *textEditAction = addToolAction("Text Edit", false, actionGroup);

        connect(selectAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(moveAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(rectangleAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(circleAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(lineAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

        connect(fillColorAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(strokeAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
        connect(textEditAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);

        QAction *deleteAction = addToolAction("Delete", false, actionGroup);
        connect(deleteAction, &QAction::triggered, this, &TopToolBar::toolSelectionTriggered);
    }

public:
    TopToolBar(const QString &title, QWidget *parent = nullptr) : QToolBar(title, parent)
    {
        // Toolbar setup can be done here
        setupToolbar();
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