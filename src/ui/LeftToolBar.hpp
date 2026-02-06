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

    void setupToolbar()
    {
        QActionGroup *actionGroup = new QActionGroup(this);
        actionGroup->setExclusive(true);

        QAction *hexagonAction = addToolAction("Hexagon", true, actionGroup, ":ui/icons/polygon.jpg", "Hexagon");
        QAction *freehandAction = addToolAction("Freehand", true, actionGroup);
        QAction *textAction = addToolAction("Text", true, actionGroup);

        connect(hexagonAction, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
        connect(freehandAction, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
        connect(textAction, &QAction::triggered, this, &LeftToolBar::toolSelectionTriggered);
    }

public:
    LeftToolBar(const QString &title, QWidget *parent = nullptr) : QToolBar(title, parent)
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