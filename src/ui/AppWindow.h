#ifndef APP_WINDOW_HPP
#define APP_WINDOW_HPP

// All includes placed in a single file (one more is there too at the bottom of this file)
#include "src/ui/AppWindowHeaders.h"

class AppWindow : public QMainWindow
{
  /*
  Tells the Meta-Object Compiler (MOC) to generate additional C++ code to handle
  signals and slots and some other Qt required features
  */
  Q_OBJECT
private:
  Canvas *canvas;
  std::string curr_file_path;      // Path of currently opened file, prevent selecting again and again the location when saving
  QActionGroup *tool_action_group; // A tool action group shared between both the toolbars

private slots:
  // These method names are themselves meaningful enough
  void openFile();
  void saveCurrFile();
  void saveAsFile();
  void openSampleFile();
  void clearCanvas();
  void newFile();

  void loadStyleSheet();

public:
  AppWindow();
};
#endif