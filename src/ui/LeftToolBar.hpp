#ifndef LEFT_TOOLBAR_HPP
#define LEFT_TOOLBAR_HPP
#include <QToolBar>
#include <QActionGroup>
#include <QAction>

class LeftToolBar : public QToolBar
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

        QAction *rectangleAction = addToolAction("Rectangle", true, actionGroup, ":ui/icons/rectangle.jpg", "Rectangle");
        QAction *circleAction = addToolAction("Circle", true, actionGroup, ":ui/icons/circle.jpg", "Circle");
        QAction *lineAction = addToolAction("Line", true, actionGroup, ":ui/icons/line.jpg", "Line");
        QAction *hexagonAction = addToolAction("Hexagon", true, actionGroup, ":ui/icons/polygon.jpg", "Hexagon");
        QAction *freehandAction = addToolAction("Freehand", true, actionGroup);
        QAction *textAction = addToolAction("Text", true, actionGroup);

        connect(rectangleAction, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
        connect(circleAction, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
        connect(lineAction, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
        connect(hexagonAction, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
        connect(freehandAction, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
        connect(textAction, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);

    }

public:
    LeftToolBar(const QString &title, QWidget *parent = nullptr, QActionGroup *actionGroup = nullptr) : QToolBar(title, parent)
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