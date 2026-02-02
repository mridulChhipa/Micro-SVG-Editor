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

        addToolAction("Select", true, actionGroup);
        addToolAction("Move", true, actionGroup);
        addToolAction("Rectangle", true, actionGroup, ":ui/icons/rectangle.jpg", "Draw Rectangle");
        addToolAction("Circle", true, actionGroup, ":ui/icons/circle.jpg", "Draw Circle");
        addToolAction("Line", true, actionGroup, ":ui/icons/line.jpg", "Draw Line");

        addToolAction("Fill Color", false, actionGroup);
        addToolAction("Stroke Color", false, actionGroup);

        addAction("Delete");
    }

public:
    TopToolBar(const QString &title, QWidget *parent = nullptr) : QToolBar(title, parent)
    {
        // Toolbar setup can be done here
        setupToolbar();
    }
};
#endif