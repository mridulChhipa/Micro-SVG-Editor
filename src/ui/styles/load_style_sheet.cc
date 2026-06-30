#include "src/ui/app_window.h"

namespace micro_svg_editor {

void AppWindow::LoadStyleSheet() {
  QFile file(":ui/styles/style.qss");

  qDebug() << "Attempting reload from:" << QFileInfo(file).absoluteFilePath();

  if (file.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream stream(&file);
    qApp->setStyleSheet(stream.readAll());
    file.close();
    qDebug() << "Style Reloaded successfully!";
  } else {
    qDebug() << "Error: Could not find style.qss at" << file.fileName();
  }
}

}  // namespace micro_svg_editor
