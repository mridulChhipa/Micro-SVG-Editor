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
        QAction *exitAction = fileMenu->addAction("Exit");

        connect(openAction, &QAction::triggered, this, &MenuBar::openRequested);
        connect(saveAction, &QAction::triggered, this, &MenuBar::saveRequested);
        connect(saveAsAction, &QAction::triggered, this, &MenuBar::saveAsRequested);
        connect(clearAction, &QAction::triggered, this, &MenuBar::clearRequested);
        connect(newAction, &QAction::triggered, this, &MenuBar::newRequested);
        connect(exitAction, &QAction::triggered, this, &MenuBar::exitRequested);

        // Edit Menu
        QMenu *editMenu = this->addMenu("Edit");
        QAction *undoAction = editMenu->addAction("Undo");
        QAction *redoAction = editMenu->addAction("Redo");
        editMenu->addSeparator();
        QAction *cutAction = editMenu->addAction("Cut");
        QAction *copyAction = editMenu->addAction("Copy");
        QAction *pasteAction = editMenu->addAction("Paste");

        connect(undoAction, &QAction::triggered, this, &MenuBar::undoRequested);
        connect(redoAction, &QAction::triggered, this, &MenuBar::redoRequested);
        connect(cutAction, &QAction::triggered, this, &MenuBar::cutRequested);
        connect(copyAction, &QAction::triggered, this, &MenuBar::copyRequested);
        connect(pasteAction, &QAction::triggered, this, &MenuBar::pasteRequested);

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
    
    void undoRequested();
    void redoRequested();

    void cutRequested();
    void copyRequested();
    void pasteRequested();

    void exitRequested();
};
#endif