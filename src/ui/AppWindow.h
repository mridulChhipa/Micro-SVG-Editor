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
  AppWindow()
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

    /*
    This connection is very important as they connect our toolbars tools (like drawing rect and stuff)
    I had tried that for each tool a single signal with the tool_name as a parameter let you set the current tool
    to remove any redundancy thus saving lines :)
    connect(top_tb, &TopToolBar::toolSelected, canvas, &Canvas::setcurr_tool);
    */

    LeftToolBar *left_tb = new LeftToolBar("Tools", this, tool_action_group);
    left_tb->setObjectName("LeftToolBar");
    left_tb->setMovable(false);
    addToolBar(Qt::LeftToolBarArea, left_tb);
    connect(left_tb, &LeftToolBar::toolSelected, canvas, &Canvas::setcurr_tool);
  }
};

#include "MenuBarOptions.h"
#include "styles/LoadStyleSheet.h"

#endif