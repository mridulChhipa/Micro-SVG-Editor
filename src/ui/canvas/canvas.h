#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_H_

#include "src/ui/canvas/canvas_headers.h"  // All Qt and other includes.
#include "src/ui/canvas/controllers/clipboard.h"
#include "src/ui/canvas/controllers/history.h"
#include "src/ui/canvas/controllers/viewport.h"
#include "src/ui/tools/tool_registry.h"

namespace micro_svg_editor {

class Canvas : public QWidget {
  Q_OBJECT

 public:
  explicit Canvas(QWidget* parent);

  // Used to update the canvas when a file is parsed and opened.
  void UpdateCanvas(SVG new_svg);
  void UpdateCanvasSize(int w, int h);
  std::string CurrentCanvasToSvg();
  void ClearCanvas();
  void SetCurrentTool(const QString& tool_name);
  void Undo();
  void Redo();
  void Cut();
  void Copy();
  void Paste();
  void ZoomIn();
  void ZoomOut();
  void ZoomReset();

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

 private:
  void ApplyDrag(const QPoint& delta);
  void ApplyResize(const QPoint& delta, HandleType handle);
  void AddShapeToCanvas(const std::string& shape_type, QPointF location);
  // Converts the in-progress freehand stroke (current_path_) into a Path.
  std::unique_ptr<Path> BuildFreehandPath();

  // Matches svg coordinates to a translated / shifted canvas.
  QPointF ToCanvasCoordinates(QPointF point);

  // The live document and a non-owning pointer to the current selection (which
  // is owned by svg_.objects()).
  SVG svg_;
  GraphicsObject* selected_shape_ = nullptr;
  QString curr_tool_;
  Viewport viewport_;

  bool dragging_{false};
  bool is_resizing_{false};
  QPoint last_point_;
  HandleType curr_handle_{HandleType::kNone};
  bool is_drawing_ = false;
  QPainterPath current_path_;
  History history_;
  Clipboard clipboard_;
  ToolRegistry tool_registry_;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_H_
