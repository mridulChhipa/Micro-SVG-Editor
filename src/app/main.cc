#include <QApplication>
#include "src/ui/AppWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  AppWindow window;
  window.show();

  return app.exec();
}