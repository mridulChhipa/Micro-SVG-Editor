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
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>

#include <QScreen>
#include <QGuiApplication>

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

    void loadStyleSheet();

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

        connect(menuBar, &MenuBar::exitRequested, this, &QWidget::close);

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
#include "styles/LoadStyleSheet.hpp"

#endif