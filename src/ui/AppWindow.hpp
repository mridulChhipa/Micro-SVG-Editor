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

private slots:
    void openFile()
    {
        QString filePath = QFileDialog::getOpenFileName(this, "Open SVG File", QDir::homePath(), "SVG Files (*.svg)");
        if (!filePath.isEmpty())
        {
            qDebug() << "File opened:" << filePath;
            Reader svgReader = Reader(filePath);
            Lexer svgLexer = Lexer(svgReader);
            Parser svgParser = Parser(svgLexer);
            svgParser.parse();
            drawingCanvas->updateCanvas(svgParser.getSVG());
            currentFilePath = filePath.toStdString();
        }
    }

    void saveCurrFile()
    {
        if (currentFilePath.empty())
        {
            QString filePath = QFileDialog::getSaveFileName(this, "Save SVG File", QDir::homePath(), "SVG Files (*.svg)");
            if (!filePath.isEmpty())
            {
                currentFilePath = filePath.toStdString();
            }
            else
            {
                qDebug() << "Save cancelled.";
                return;
            }
        }

        qDebug() << "Saving to:" << QString::fromStdString(currentFilePath);

        QFile file(QString::fromStdString(currentFilePath));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << QString::fromStdString(drawingCanvas->currentCanvasToSVG());
            file.close();
            qDebug() << "File saved successfully.";
        }
        else
        {
            qDebug() << "Error: Could not open file for writing:" << QString::fromStdString(currentFilePath);
        }
    }

    void saveAsFile()
    {
        QString filePath = QFileDialog::getSaveFileName(this, "Save SVG File As", QDir::homePath(), "SVG Files (*.svg)");

        if (!filePath.isEmpty())
        {
            currentFilePath = filePath.toStdString();

            qDebug() << "Saving to:" << filePath;

            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                out << QString::fromStdString(drawingCanvas->currentCanvasToSVG());
                file.close();
                qDebug() << "File saved successfully.";
            }
            else
            {
                qDebug() << "Error: Could not open file for writing:" << filePath;
            }
        }
    }

    void openSampleFile(const QString &samplePath = "")
    {
        QString filePath = samplePath;
        if (filePath.isEmpty())
            filePath = ":testxmls/sample.svg";

        qDebug() << "Opening sample file:" << filePath;
        Reader svgReader = Reader(filePath);
        Lexer svgLexer = Lexer(svgReader);
        Parser svgParser = Parser(svgLexer);
        svgParser.parse();
        drawingCanvas->updateCanvas(svgParser.getSVG());
    }

    void clearCanvas()
    {
        drawingCanvas->clearCanvas();
    }

    void newFile()
    {
        drawingCanvas->clearCanvas();
        currentFilePath.clear();
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
        openSampleFile();

        // Menu Bar
        MenuBar *menuBar = new MenuBar(this);
        setMenuBar(menuBar);
        connect(menuBar, &MenuBar::openRequested, this, &AppWindow::openFile);
        connect(menuBar, &MenuBar::saveRequested, this, &AppWindow::saveCurrFile);
        connect(menuBar, &MenuBar::saveAsRequested, this, &AppWindow::saveAsFile);
        connect(menuBar, &MenuBar::clearRequested, this, &AppWindow::clearCanvas);
        connect(menuBar, &MenuBar::newRequested, this, &AppWindow::newFile);
        // Toolbars
        TopToolBar *topToolBar = new TopToolBar("Top Tools", this);
        topToolBar->setObjectName("TopToolBar");
        addToolBar(Qt::TopToolBarArea, topToolBar);

        connect(topToolBar, &TopToolBar::toolSelected, drawingCanvas, &Canvas::setCurrentTool);

        LeftToolBar *leftToolBar = new LeftToolBar("Tools", this);
        leftToolBar->setObjectName("LeftToolBar");
        addToolBar(Qt::LeftToolBarArea, leftToolBar);

        connect(leftToolBar, &LeftToolBar::toolSelected, drawingCanvas, &Canvas::setCurrentTool);
    }
};

#endif