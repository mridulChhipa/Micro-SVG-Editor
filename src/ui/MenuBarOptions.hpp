#ifndef MENU_BAR_OPTIONS_HPP
#define MENU_BAR_OPTIONS_HPP

inline void AppWindow::openFile()
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

inline void AppWindow::saveCurrFile()
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

inline void AppWindow::saveAsFile()
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

inline void AppWindow::openSampleFile()
{
    QString filePath = ":testxmls/sample.svg";

    qDebug() << "Opening sample file:" << filePath;
    Reader svgReader = Reader(filePath);
    Lexer svgLexer = Lexer(svgReader);
    Parser svgParser = Parser(svgLexer);
    svgParser.parse();
    drawingCanvas->updateCanvas(svgParser.getSVG());
}

inline void AppWindow::clearCanvas()
{
    drawingCanvas->clearCanvas();
}

inline void AppWindow::newFile()
{
    drawingCanvas->clearCanvas();
    currentFilePath.clear();
}

#endif