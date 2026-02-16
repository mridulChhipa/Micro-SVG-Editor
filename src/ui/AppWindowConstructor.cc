#include "src/ui/AppWindow.h"

AppWindow::AppWindow()
{
  loadStyleSheet();

  tool_action_group = new QActionGroup(this);
  tool_action_group->setExclusive(true);

  setWindowTitle("Micro Svg Editor");

  resize(1280, 720);
  canvas = new Canvas(this);
  setCentralWidget(canvas);
  openSampleFile();

  // Menu Bar at the top having three lists
  MenuBar *menu_bar = new MenuBar(this);
  setMenuBar(menu_bar);

  // The connect function is provided by qt, lets you connect signals to another objects method
  // Here ex: connects menuBar openRequesed signal to AppWindows's openFile() method
  connect(menu_bar, &MenuBar::openRequested, this, &AppWindow::openFile);
  connect(menu_bar, &MenuBar::saveRequested, this, &AppWindow::saveCurrFile);
  connect(menu_bar, &MenuBar::saveAsRequested, this, &AppWindow::saveAsFile);
  connect(menu_bar, &MenuBar::clearRequested, this, &AppWindow::clearCanvas);
  connect(menu_bar, &MenuBar::newRequested, this, &AppWindow::newFile);

  connect(menu_bar, &MenuBar::undoRequested, canvas, &Canvas::undo);
  connect(menu_bar, &MenuBar::redoRequested, canvas, &Canvas::redo);

  connect(menu_bar, &MenuBar::cutRequested, canvas, &Canvas::cut);
  connect(menu_bar, &MenuBar::copyRequested, canvas, &Canvas::copy);
  connect(menu_bar, &MenuBar::pasteRequested, canvas, &Canvas::paste);

  connect(menu_bar, &MenuBar::exitRequested, this, &QWidget::close);

  connect(menu_bar, &MenuBar::zoomInRequested, canvas, &Canvas::zoom_in);
  connect(menu_bar, &MenuBar::zoomOutRequested, canvas, &Canvas::zoom_out);
  connect(menu_bar, &MenuBar::zoomResetRequested, canvas, &Canvas::zoom_reset);

  // ToolBars (QToolBar)
  TopToolBar *top_tb = new TopToolBar("Top Tools", this, tool_action_group);
  top_tb->setObjectName("TopToolBar"); // Used for styling each of the toolbar separately
  top_tb->setMovable(false);
  addToolBar(Qt::TopToolBarArea, top_tb);
  connect(top_tb, &TopToolBar::toolSelected, canvas, &Canvas::setcurr_tool);

  /*
  This connection is very important as they connect our toolbars tools (like drawing rect and stuff)
  I had tried that for each tool a single signal with the tool_name as a parameter let you set the current tool
  to remove any redundancy thus saving lines :)
  */

  LeftToolBar *left_tb = new LeftToolBar("Left Tools", this, tool_action_group);
  left_tb->setObjectName("LeftToolBar");
  left_tb->setMovable(false);
  addToolBar(Qt::LeftToolBarArea, left_tb);
  connect(left_tb, &LeftToolBar::toolSelected, canvas, &Canvas::setcurr_tool);
}