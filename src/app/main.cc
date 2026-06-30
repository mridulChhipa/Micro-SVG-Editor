#include <QApplication>

#include "src/io/svg_loader.h"
#include "src/ui/app_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  // Compose the dependencies and inject the loader into the window.
  micro_svg_editor::SvgLoader loader;
  micro_svg_editor::AppWindow window(loader);
  window.show();

  return app.exec();
}
