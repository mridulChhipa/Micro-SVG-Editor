#include <QApplication>
#include "src/ui/app_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  // This is the window that the user will see
  micro_svg_editor::AppWindow window;
  window.show();

  return app.exec();
}