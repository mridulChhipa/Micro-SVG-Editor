#include "src/ui/AppWindow.h"

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
