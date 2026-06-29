#ifndef MICRO_SVG_EDITOR_SRC_UI_APP_WINDOW_H_
#define MICRO_SVG_EDITOR_SRC_UI_APP_WINDOW_H_

#include <string>

#include "src/ui/AppWindowHeaders.h"

class AppWindow : public QMainWindow {
  // Tells the Meta-Object Compiler (MOC) to generate the extra code that backs
  // signals, slots and other Qt features.
  Q_OBJECT

 public:
  AppWindow();

 private slots:
  void OpenFile();
  void SaveCurrFile();
  void SaveAsFile();
  void OpenSampleFile();
  void ClearCanvas();
  void NewFile();

  void LoadStyleSheet();

 private:
  Canvas* canvas_;
  // Path of the currently opened file; avoids re-prompting for a location on
  // every save.
  std::string curr_file_path_;
  // A tool action group shared between both toolbars.
  QActionGroup* tool_action_group_;
};

#endif  // MICRO_SVG_EDITOR_SRC_UI_APP_WINDOW_H_
