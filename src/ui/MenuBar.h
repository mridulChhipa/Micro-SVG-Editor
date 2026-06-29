#ifndef MICRO_SVG_EDITOR_SRC_UI_MENU_BAR_H_
#define MICRO_SVG_EDITOR_SRC_UI_MENU_BAR_H_

#include <QAction>
#include <QFont>
#include <QMenu>
#include <QMenuBar>

class MenuBar : public QMenuBar {
  Q_OBJECT

 public:
  explicit MenuBar(QWidget* parent);
  void SetupMenus();

 signals:
  void openRequested();
  void saveRequested();
  void saveAsRequested();
  void clearRequested();
  void newRequested();

  void undoRequested();
  void redoRequested();

  void cutRequested();
  void copyRequested();
  void pasteRequested();

  void exitRequested();

  void zoomInRequested();
  void zoomOutRequested();
  void zoomResetRequested();
};

#endif  // MICRO_SVG_EDITOR_SRC_UI_MENU_BAR_H_
