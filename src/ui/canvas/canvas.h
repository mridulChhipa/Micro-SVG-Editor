#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_H_

#include "src/ui/canvas/canvas_headers.h"  // All Qt and other includes.

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
  // Drawing related functions.
  QTransform FindTransform(GraphicsObject* obj);
  void CreateObject(GraphicsObject* obj, QPainterPath& path, QPen& pen);
  void ApplyDashArray(QPen& pen, const std::string& dasharray);
  void CreateBrush(GraphicsObject* obj, QPainter& painter);
  void DrawSvg(QPainter& painter);
  void ApplyDrag(const QPoint& delta);
  void ApplyResize(const QPoint& delta, HandleType handle);
  void AddShapeToCanvas(const std::string& shape_type, QPointF location);
  // Converts the in-progress freehand stroke (current_path_) into a Path.
  std::unique_ptr<Path> BuildFreehandPath();

  // Matches svg coordinates to a translated / shifted canvas.
  QPointF ToCanvasCoordinates(QPointF point);

  // Methods for rendering and hit-testing handles.
  QRectF RenderHandle(const QRectF& obj, HandleType handle_type);
  HandleType HitTestHandles(const QRectF& obj, const QPointF& point);

  void DeleteSelectedShape(bool& edited);
  void EditFillColor(bool& edited);
  void StrokeEdit(bool& edited);
  void EditBorderRadius(bool& edited);
  void CanvasEdit(bool& edited);
  void EditOpacity(const std::string& title, float& prop, bool& edited);
  void TextEdit(bool& edited);

  // The live document and a non-owning pointer to the current selection (which
  // is owned by svg_.objects()).
  SVG svg_;
  GraphicsObject* selected_shape_ = nullptr;
  QString curr_tool_;
  float x_offset_{0};
  float y_offset_{0};

  bool dragging_{false};
  bool is_resizing_{false};
  QPoint last_point_;
  HandleType curr_handle_{HandleType::kNone};
  QRectF start_rect_;
  std::vector<SVG> undo_stack_;
  std::vector<SVG> redo_stack_;
  // Used for undo / redo while dragging and resizing.
  std::vector<SVG> temp_stack_;
  bool is_performing_undo_redo_{false};
  bool is_drawing_ = false;
  QPainterPath current_path_;
  std::unique_ptr<GraphicsObject> clipboard_shape_;
  static constexpr int kHandleSize = 8;
  static constexpr int kAdjust = 20;
  double zoom_factor_ = 1;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_H_
