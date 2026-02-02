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

        addToolAction("Polygon", true, actionGroup, ":ui/icons/polygon.jpg", "Draw Polygon");
        addToolAction("Polyline", true, actionGroup);
        addToolAction("Text", true, actionGroup);
    }

public:
    LeftToolBar(const QString &title, QWidget *parent = nullptr) : QToolBar(title, parent)
    {
        // Toolbar setup can be done here
        setupToolbar();
    }
};
#endif