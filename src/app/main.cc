#include <QApplication>
#include "src/ui/AppWindow.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  // This is the window that the user will see
  AppWindow window;
  window.show();

  return app.exec();
}