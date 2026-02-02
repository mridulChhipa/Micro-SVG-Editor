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

#include "Canvas.hpp"
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

private slots:
    void openFile()
    {
        // Implementation for opening a file
        QString filePath = QFileDialog::getOpenFileName(this, "Open SVG File", QDir::homePath(), "SVG Files (*.svg)");
        if (!filePath.isEmpty())
        {
            qDebug() << "File opened:" << filePath;
            Reader svgReader = Reader(filePath.toStdString());
            Lexer svgLexer = Lexer(svgReader);
            Parser svgParser = Parser(svgLexer);
            svgParser.parse();
            drawingCanvas->updateCanvas(svgParser.getSVG());
        }
    }
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

        QAction *manualReload = new QAction(this);
        manualReload->setShortcut(QKeySequence(Qt::Key_F5));
        connect(manualReload, &QAction::triggered, this, &AppWindow::loadStyleSheet);
        this->addAction(manualReload);

        setWindowTitle("Micro Svg Editor");
        resize(1280, 720);

        drawingCanvas = new Canvas(this);
        setCentralWidget(drawingCanvas);

        // Menu Bar
        MenuBar *menuBar = new MenuBar(this);
        setMenuBar(menuBar);
        connect(menuBar, &MenuBar::openRequested, this, &AppWindow::openFile);
        // Toolbars
        TopToolBar *topToolBar = new TopToolBar("Top Tools", this);
        topToolBar->setObjectName("TopToolBar");
        addToolBar(Qt::TopToolBarArea, topToolBar);

        LeftToolBar *leftToolBar = new LeftToolBar("Tools", this);
        leftToolBar->setObjectName("LeftToolBar");
        addToolBar(Qt::LeftToolBarArea, leftToolBar);
    }
};

#endif