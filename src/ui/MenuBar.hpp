// Menu Bar Setup
#ifndef MENU_BAR_SETUP_HPP
#define MENU_BAR_SETUP_HPP

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFont>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    MenuBar(QWidget *parent = nullptr) : QMenuBar(parent)
    {
        setupMenus();
    }

    void setupMenus()
    {
        QFont font("Segoe UI", 10);
        font.setStyleStrategy(QFont::PreferAntialias);
        this->setFont(font);
        this->setNativeMenuBar(false);

        QMenu *fileMenu = this->addMenu("File");
        QAction *newAction = fileMenu->addAction("New");
        QAction *openAction = fileMenu->addAction("Open");
        fileMenu->addSeparator();
        QAction *saveAction = fileMenu->addAction("Save");
        QAction *saveAsAction = fileMenu->addAction("Save As");
        fileMenu->addSeparator();
        QAction *clearAction = fileMenu->addAction("Clear Canvas");
        fileMenu->addAction("Exit", this, &QWidget::close);

        connect(openAction, &QAction::triggered, this, &MenuBar::openRequested);
        connect(saveAction, &QAction::triggered, this, &MenuBar::saveRequested);
        connect(saveAsAction, &QAction::triggered, this, &MenuBar::saveAsRequested);
        connect(clearAction, &QAction::triggered, this, &MenuBar::clearRequested);
        connect(newAction, &QAction::triggered, this, &MenuBar::newRequested);

        // Edit Menu
        QMenu *editMenu = this->addMenu("Edit");
        editMenu->addAction("Undo");
        editMenu->addAction("Redo");
        editMenu->addSeparator();
        editMenu->addAction("Cut");
        editMenu->addAction("Copy");
        editMenu->addAction("Paste");

        // View Menu
        QMenu *viewMenu = this->addMenu("View");
        viewMenu->addAction("Zoom In");
        viewMenu->addAction("Zoom Out");
        viewMenu->addSeparator();
        viewMenu->addAction("Reset Zoom");
        viewMenu->addAction("Fullscreen");
        viewMenu->addSeparator();
        viewMenu->addAction("Toggle Code / Design View");
    }

signals:
    void openRequested();
    void saveRequested();
    void saveAsRequested();
    void clearRequested();
    void newRequested();
};
#endif