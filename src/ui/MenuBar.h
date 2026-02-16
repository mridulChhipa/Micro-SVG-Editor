#ifndef MENU_BAR_SETUP_HPP
#define MENU_BAR_SETUP_HPP

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFont>

class MenuBar : public QMenuBar
{
  Q_OBJECT
public:
  MenuBar(QWidget *parent);
  void setupMenus();

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
#endif