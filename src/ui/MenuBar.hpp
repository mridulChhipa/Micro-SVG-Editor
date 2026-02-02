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
        fileMenu->addAction("New");
        QAction *openAction = fileMenu->addAction("Open");

        connect(openAction, &QAction::triggered, this, &MenuBar::openRequested);

        fileMenu->addSeparator();
        fileMenu->addAction("Save");
        fileMenu->addAction("Save As");
        fileMenu->addSeparator();
        fileMenu->addAction("Clear");
        fileMenu->addAction("Exit", this, &QWidget::close);

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
};
#endif