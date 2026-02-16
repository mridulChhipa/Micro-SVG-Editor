#include "src/ui/AppWindow.h"

void AppWindow::openFile()
{
  // Shows a dialog box to choose which file to open
  QString file_path = QFileDialog::getOpenFileName(this, "Open SVG File", QDir::homePath(), "SVG Files (*.svg)");
  if (!file_path.isEmpty())
  {
    qDebug() << "File opened:" << file_path;

    // Parser parses the file and creates a SVG object
    Reader svg_reader = Reader(file_path);
    Lexer svg_lexer = Lexer(svg_reader);
    Parser svg_parser = Parser(svg_lexer);
    svg_parser.parse();
    // Once file has been parsed and svg object is created, send the parsed object to canvas and update the working file path
    canvas->updateCanvas(svg_parser.getSVG());
    curr_file_path = file_path.toStdString();
  }
}

void AppWindow::saveCurrFile()
{
  if (curr_file_path.empty())
  {
    // Shows a dialog box to choose file destination
    QString file_path = QFileDialog::getSaveFileName(this, "Save SVG File", QDir::homePath(), "SVG Files (*.svg)");
    if (!file_path.isEmpty())
      curr_file_path = file_path.toStdString();
    else
    {
      qDebug() << "Save cancelled.";
      return;
    }
  }
  qDebug() << "Saving to:" << QString::fromStdString(curr_file_path);
  QFile file(QString::fromStdString(curr_file_path));
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QTextStream out(&file);
    out << QString::fromStdString(canvas->currentCanvasToSVG());
    file.close();
    qDebug() << "File saved successfully.";
  }
  else
  {
    qDebug() << "Error: Could not open file for writing:" << QString::fromStdString(curr_file_path);
  }
}

void AppWindow::saveAsFile()
{
  QString file_path = QFileDialog::getSaveFileName(this, "Save SVG File As", QDir::homePath(), "SVG Files (*.svg)");
  if (!file_path.isEmpty())
  {
    curr_file_path = file_path.toStdString();
    qDebug() << "Saving to:" << file_path;
    QFile file(file_path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream out(&file);
      out << QString::fromStdString(canvas->currentCanvasToSVG()); // Maps to getSvg() and returns an xml
      file.close();
      qDebug() << "File saved successfully.";
    }
    else
      qDebug() << "Error: Could not open file for writing:" << file_path;
  }
}

void AppWindow::openSampleFile()
{
  QString file_path = ":testxmls/sample.svg";
  qDebug() << "Opening sample file:" << file_path;
  Reader svg_reader = Reader(file_path);
  Lexer svg_lexer = Lexer(svg_reader);
  Parser svg_parser = Parser(svg_lexer);
  svg_parser.parse();
  canvas->updateCanvas(svg_parser.getSVG());
}

void AppWindow::clearCanvas()
{
  canvas->clearCanvas();
}

void AppWindow::newFile()
{
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screen_geometry = screen->geometry();

  int width = screen_geometry.width();
  int height = screen_geometry.height();
  // Screen ke area ke 1 / 4 th area ki khali canvas
  canvas->updateCanvasSize(width / 2, height / 2);
  canvas->clearCanvas();
  curr_file_path.clear();
}