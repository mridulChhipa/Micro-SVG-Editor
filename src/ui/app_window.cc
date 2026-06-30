#include "src/ui/app_window.h"

namespace micro_svg_editor {

void AppWindow::OpenFile() {
  // Shows a dialog box to choose which file to open
  QString file_path = QFileDialog::getOpenFileName(
      this, "Open SVG File", QDir::homePath(), "SVG Files (*.svg)");
  if (!file_path.isEmpty()) {
    qDebug() << "File opened:" << file_path;

    // Load via the injected loader, then send the document to the canvas and
    // record the working file path.
    canvas_->UpdateCanvas(loader_.Load(file_path));
    curr_file_path_ = file_path.toStdString();
  }
}

void AppWindow::SaveCurrFile() {
  if (curr_file_path_.empty()) {
    // Shows a dialog box to choose file destination
    QString file_path = QFileDialog::getSaveFileName(
        this, "Save SVG File", QDir::homePath(), "SVG Files (*.svg)");
    if (!file_path.isEmpty())
      curr_file_path_ = file_path.toStdString();
    else {
      qDebug() << "Save cancelled.";
      return;
    }
  }
  qDebug() << "Saving to:" << QString::fromStdString(curr_file_path_);
  QFile file(QString::fromStdString(curr_file_path_));
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out << QString::fromStdString(canvas_->CurrentCanvasToSvg());
    file.close();
    qDebug() << "File saved successfully.";
  } else {
    qDebug() << "Error: Could not open file for writing:"
             << QString::fromStdString(curr_file_path_);
  }
}

void AppWindow::SaveAsFile() {
  QString file_path = QFileDialog::getSaveFileName(
      this, "Save SVG File As", QDir::homePath(), "SVG Files (*.svg)");
  if (!file_path.isEmpty()) {
    curr_file_path_ = file_path.toStdString();
    qDebug() << "Saving to:" << file_path;
    QFile file(file_path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream out(&file);
      out << QString::fromStdString(
          canvas_->CurrentCanvasToSvg());  // Maps to GetSvg() and returns XML.
      file.close();
      qDebug() << "File saved successfully.";
    } else
      qDebug() << "Error: Could not open file for writing:" << file_path;
  }
}

void AppWindow::OpenSampleFile() {
  QString file_path = ":testxmls/sample.svg";
  qDebug() << "Opening sample file:" << file_path;
  canvas_->UpdateCanvas(loader_.Load(file_path));
}

void AppWindow::ClearCanvas() { canvas_->ClearCanvas(); }

void AppWindow::NewFile() {
  QScreen* screen = QGuiApplication::primaryScreen();
  QRect screen_geometry = screen->geometry();

  int width = screen_geometry.width();
  int height = screen_geometry.height();
  // Start with a blank canvas covering a quarter of the screen area.
  canvas_->UpdateCanvasSize(width / 2, height / 2);
  canvas_->ClearCanvas();
  curr_file_path_.clear();
}

}  // namespace micro_svg_editor
