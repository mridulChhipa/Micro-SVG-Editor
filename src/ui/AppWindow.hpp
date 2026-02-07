#ifndef APP_WINDOW_HPP
#define APP_WINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QApplication>
#include <QDir>      // Added for path debugging
#include <QFileInfo> // Added for path debugging
#include <QFileDialog>

#include "./canvas/Canvas.hpp"
#include "MenuBar.hpp"
#include "TopToolBar.hpp"
#include "LeftToolBar.hpp"

#include "../io/Reader.hpp"
#include "../io/Lexer.hpp"
#include "../io/Parser.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT
private:
    Canvas *drawingCanvas;
    std::string currentFilePath;
    QActionGroup *toolActionGroup;

private slots:
    void openFile();
    void saveCurrFile();
    void saveAsFile();
    void openSampleFile();
    void clearCanvas();
    void newFile();

    void loadStyleSheet()
    {
        QFile file(":ui/style.qss");

        qDebug() << "Attempting reload from:" << QFileInfo(file).absoluteFilePath();

        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream stream(&file);
            qApp->setStyleSheet(stream.readAll());
            file.close();
            qDebug() << "Style Reloaded successfully!";
        }
        else
        {
            qDebug() << "Error: Could not find style.qss at" << file.fileName();
        }
    }

public:
    AppWindow()
    {
        loadStyleSheet();

        toolActionGroup = new QActionGroup(this);
        toolActionGroup->setExclusive(true);

        QAction *manualReload = new QAction(this);
        manualReload->setShortcut(QKeySequence(Qt::Key_F5));
        connect(manualReload, &QAction::triggered, this, &AppWindow::loadStyleSheet);
        this->addAction(manualReload);

        setWindowTitle("Micro Svg Editor");
        resize(1280, 720);

        drawingCanvas = new Canvas(this);
        setCentralWidget(drawingCanvas);
        openSampleFile();

        // Menu Bar
        MenuBar *menuBar = new MenuBar(this);
        setMenuBar(menuBar);
        connect(menuBar, &MenuBar::openRequested, this, &AppWindow::openFile);
        connect(menuBar, &MenuBar::saveRequested, this, &AppWindow::saveCurrFile);
        connect(menuBar, &MenuBar::saveAsRequested, this, &AppWindow::saveAsFile);
        connect(menuBar, &MenuBar::clearRequested, this, &AppWindow::clearCanvas);
        connect(menuBar, &MenuBar::newRequested, this, &AppWindow::newFile);
        
        connect(menuBar, &MenuBar::undoRequested, drawingCanvas, &Canvas::undo);
        connect(menuBar, &MenuBar::redoRequested, drawingCanvas, &Canvas::redo);

        connect(menuBar, &MenuBar::cutRequested, drawingCanvas, &Canvas::cut);
        connect(menuBar, &MenuBar::copyRequested, drawingCanvas, &Canvas::copy);
        connect(menuBar, &MenuBar::pasteRequested, drawingCanvas, &Canvas::paste);

        TopToolBar *topToolBar = new TopToolBar("Top Tools", this, toolActionGroup);
        topToolBar->setObjectName("TopToolBar");
        topToolBar->setMovable(false);
        addToolBar(Qt::TopToolBarArea, topToolBar);
        connect(topToolBar, &TopToolBar::toolSelected, drawingCanvas, &Canvas::setCurrentTool);

        LeftToolBar *leftToolBar = new LeftToolBar("Tools", this, toolActionGroup);
        leftToolBar->setObjectName("LeftToolBar");
        leftToolBar->setMovable(false);
        addToolBar(Qt::LeftToolBarArea, leftToolBar);
        connect(leftToolBar, &LeftToolBar::toolSelected, drawingCanvas, &Canvas::setCurrentTool);
    }
};

#include "MenuBarOptions.hpp"

#endif