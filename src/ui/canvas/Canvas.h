#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_H_

#include "src/ui/canvas/CanvasHeaders.h"  // All Qt and other includes.

using ShapeVariant =
    std::variant<std::shared_ptr<Rect>, std::shared_ptr<Circle>,
                 std::shared_ptr<Line>, std::shared_ptr<Polyline>,
                 std::shared_ptr<Path>, std::shared_ptr<Hexagon>,
                 std::shared_ptr<Text>>;

class Canvas : public QWidget {
  Q_OBJECT

 public:
  GraphicsObjectPtr selected_shape_{nullptr};
  QString curr_tool_{""};
  SVG svg_;
  float x_offset_{0};
  float y_offset_{0};

  explicit Canvas(QWidget* parent);

  // Used to update the canvas when a file is parsed and opened.
  void UpdateCanvas(const SVG& new_svg);
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
  // Shape variants to access varying members directly.
  std::optional<ShapeVariant> ToShapeVariant(const GraphicsObjectPtr& obj);

  // Drawing related functions.
  QTransform FindTransform(const GraphicsObjectPtr& obj);
  void CreateObject(const GraphicsObjectPtr& obj, QPainterPath& path,
                    QPen& pen);
  void ApplyDashArray(QPen& pen, const std::string& dasharray);
  void CreateBrush(const GraphicsObjectPtr& obj, QPainter& painter);
  void DrawSvg(QPainter& painter);
  void ApplyDrag(const QPoint& delta);
  void ApplyResize(const QPoint& delta, HandleType handle);
  template <typename PointContainer>
  void BuildPointPath(QPainterPath& path, const PointContainer& points,
                      bool close);
  void BuildShapePath(QPainterPath& path, const std::shared_ptr<Rect>& s);
  void BuildShapePath(QPainterPath& path, const std::shared_ptr<Circle>& s);
  void BuildShapePath(QPainterPath& path, const std::shared_ptr<Line>& s);
  void BuildShapePath(QPainterPath& path, const std::shared_ptr<Polyline>& s);
  void BuildShapePath(QPainterPath& path, const std::shared_ptr<Path>& s);
  void BuildShapePath(QPainterPath& path, const std::shared_ptr<Hexagon>& s);
  void BuildShapePath(QPainterPath& path, const std::shared_ptr<Text>& s);
  void AddShapeToCanvas(const std::string name, QPointF location);

  // Matches svg coordinates to a translated / shifted canvas.
  QPointF ToCanvasCoordinates(QPointF point);

  // Methods for rendering and hit-testing handles.
  QRectF RenderHandle(const QRectF& obj, HandleType handle_type);
  HandleType HitTestHandles(const QRectF& obj, const QPointF& point);

  void StrokeEdit(bool& edited);
  void EditBorderRadius(bool& edited);
  void CanvasEdit(bool& edited);
  void EditOpacity(bool& edited, float& prop, std::string title);
  void TextEdit(bool& edited);

  void CircleResizeHandler(std::shared_ptr<Circle> s, int dx, int dy,
                           bool affects_left, bool affects_right,
                           bool affects_top, bool affects_bottom);
  void HexagonResizeHandler(std::shared_ptr<Hexagon> s, int dx, int dy,
                            bool affects_left, bool affects_right,
                            bool affects_top, bool affects_bottom);

  bool dragging_{false};
  bool is_resizing_{false};
  QPoint last_point_;
  HandleType curr_handle_{HandleType::kNone};
  QRectF start_rect_;
  QVector<SVG> undo_stack_;
  QVector<SVG> redo_stack_;
  // Used for undo / redo while dragging and resizing.
  QVector<SVG> temp_stack_;
  bool is_performing_undo_redo_{false};
  bool is_drawing_ = false;
  QPainterPath current_path_;
  GraphicsObjectPtr clipboard_shape_{nullptr};
  static constexpr int kHandleSize = 8;
  static constexpr int kAdjust = 20;
  double zoom_factor_ = 1;
};

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_H_
