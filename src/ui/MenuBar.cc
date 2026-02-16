#include "src/ui/MenuBar.h"

MenuBar::MenuBar(QWidget *parent = nullptr) : QMenuBar(parent)
{
  setupMenus();
}

void MenuBar::setupMenus()
{
  QFont font("Segoe UI", 10);
  font.setStyleStrategy(QFont::PreferAntialias);
  this->setFont(font);
  this->setNativeMenuBar(false);

  QMenu *file_menu = this->addMenu("File");
  QAction *new_action = file_menu->addAction("New");
  QAction *open_action = file_menu->addAction("Open");
  file_menu->addSeparator();
  QAction *save_action = file_menu->addAction("Save");
  QAction *save_asAction = file_menu->addAction("Save As");
  file_menu->addSeparator();
  QAction *clear_action = file_menu->addAction("Clear Canvas");
  QAction *exit_action = file_menu->addAction("Exit");

  connect(open_action, &QAction::triggered, this, &MenuBar::openRequested);
  connect(save_action, &QAction::triggered, this, &MenuBar::saveRequested);
  connect(save_asAction, &QAction::triggered, this, &MenuBar::saveAsRequested);
  connect(clear_action, &QAction::triggered, this, &MenuBar::clearRequested);
  connect(new_action, &QAction::triggered, this, &MenuBar::newRequested);
  connect(exit_action, &QAction::triggered, this, &MenuBar::exitRequested);

  // Edit Menu
  QMenu *edit_menu = this->addMenu("Edit");
  QAction *undo_action = edit_menu->addAction("Undo");
  QAction *redo_action = edit_menu->addAction("Redo");
  edit_menu->addSeparator();
  QAction *cut_action = edit_menu->addAction("Cut");
  QAction *copy_action = edit_menu->addAction("Copy");
  QAction *paste_action = edit_menu->addAction("Paste");

  connect(undo_action, &QAction::triggered, this, &MenuBar::undoRequested);
  connect(redo_action, &QAction::triggered, this, &MenuBar::redoRequested);
  connect(cut_action, &QAction::triggered, this, &MenuBar::cutRequested);
  connect(copy_action, &QAction::triggered, this, &MenuBar::copyRequested);
  connect(paste_action, &QAction::triggered, this, &MenuBar::pasteRequested);

  // View Menu
  QMenu *view_menu = this->addMenu("View");
  QAction *zoom_in = view_menu->addAction("Zoom In");
  QAction *zoom_out = view_menu->addAction("Zoom Out");
  QAction *zoom_reset = view_menu->addAction("Reset Zoom");

  connect(zoom_in, &QAction::triggered, this, &MenuBar::zoomInRequested);
  connect(zoom_out, &QAction::triggered, this, &MenuBar::zoomOutRequested);
  connect(zoom_reset, &QAction::triggered, this, &MenuBar::zoomResetRequested);
}