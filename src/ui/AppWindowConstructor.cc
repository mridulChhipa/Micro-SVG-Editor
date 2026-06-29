#include "src/ui/AppWindow.h"

AppWindow::AppWindow() {
  LoadStyleSheet();

  tool_action_group_ = new QActionGroup(this);
  tool_action_group_->setExclusive(true);

  setWindowTitle("Micro Svg Editor");

  resize(1280, 720);
  canvas_ = new Canvas(this);
  setCentralWidget(canvas_);
  OpenSampleFile();

  // Menu Bar at the top having three lists
  MenuBar* menu_bar = new MenuBar(this);
  setMenuBar(menu_bar);

  // The connect function is provided by qt, lets you connect signals to another
  // objects method Here ex: connects menuBar openRequesed signal to
  // AppWindows's OpenFile() method
  connect(menu_bar, &MenuBar::openRequested, this, &AppWindow::OpenFile);
  connect(menu_bar, &MenuBar::saveRequested, this, &AppWindow::SaveCurrFile);
  connect(menu_bar, &MenuBar::saveAsRequested, this, &AppWindow::SaveAsFile);
  connect(menu_bar, &MenuBar::clearRequested, this, &AppWindow::ClearCanvas);
  connect(menu_bar, &MenuBar::newRequested, this, &AppWindow::NewFile);

  connect(menu_bar, &MenuBar::undoRequested, canvas_, &Canvas::Undo);
  connect(menu_bar, &MenuBar::redoRequested, canvas_, &Canvas::Redo);

  connect(menu_bar, &MenuBar::cutRequested, canvas_, &Canvas::Cut);
  connect(menu_bar, &MenuBar::copyRequested, canvas_, &Canvas::Copy);
  connect(menu_bar, &MenuBar::pasteRequested, canvas_, &Canvas::Paste);

  connect(menu_bar, &MenuBar::exitRequested, this, &QWidget::close);

  connect(menu_bar, &MenuBar::zoomInRequested, canvas_, &Canvas::ZoomIn);
  connect(menu_bar, &MenuBar::zoomOutRequested, canvas_, &Canvas::ZoomOut);
  connect(menu_bar, &MenuBar::zoomResetRequested, canvas_, &Canvas::ZoomReset);

  // ToolBars (QToolBar)
  TopToolBar* top_tb = new TopToolBar("Top Tools", this, tool_action_group_);
  top_tb->setObjectName(
      "TopToolBar");  // Used for styling each of the toolbar separately
  top_tb->setMovable(false);
  addToolBar(Qt::TopToolBarArea, top_tb);
  connect(top_tb, &TopToolBar::toolSelected, canvas_, &Canvas::SetCurrentTool);

  /*
  This connection is very important as they connect our toolbars tools (like
  drawing rect and stuff) I had tried that for each tool a single signal with
  the tool_name as a parameter let you set the current tool to remove any
  redundancy thus saving lines :)
  */

  LeftToolBar* left_tb =
      new LeftToolBar("Left Tools", this, tool_action_group_);
  left_tb->setObjectName("LeftToolBar");
  left_tb->setMovable(false);
  addToolBar(Qt::LeftToolBarArea, left_tb);
  connect(left_tb, &LeftToolBar::toolSelected, canvas_,
          &Canvas::SetCurrentTool);
}