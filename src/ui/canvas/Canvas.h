#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "src/ui/canvas/CanvasHeaders.h" // All Qt and other includes
using ShapeVariant = std::variant<std::shared_ptr<Rect>, std::shared_ptr<Circle>, std::shared_ptr<Line>, std::shared_ptr<Polyline>, std::shared_ptr<Path>, std::shared_ptr<Hexagon>, std::shared_ptr<Text>>;
class Canvas : public QWidget
{
  Q_OBJECT
private:
  bool dragging{false};
  bool is_resizing{false};
  QPoint last_point;
  HandleType curr_handle{HandleType::None};
  QRectF start_rect;
  QVector<SVG> undo_stack;
  QVector<SVG> redo_stack;
  QVector<SVG> temp_stack; // Used for undo / redo while dragging and resizing
  bool isPerformingUndoRedo{false};
  bool is_drawing = false;
  QPainterPath current_path;
  GraphicsObjectPtr clipboard_shape{nullptr};
  const int handle_size = 8;
  const int adjust = 20;
  double zoom_factor = 1;
  // Shape variants to access varying members directly (Reference: BeQuant.dev C++ Track)
  std::optional<ShapeVariant> toShapeVariant(const GraphicsObjectPtr &obj);
  // Drawing related functions
  QTransform findTransform(const GraphicsObjectPtr &obj);
  void createObject(const GraphicsObjectPtr &obj, QPainterPath &path, QPen &pen);
  void applyDashArray(QPen &pen, const std::string &dasharray);
  void createBrush(const GraphicsObjectPtr &obj, QPainter &painter);
  void drawSVG(QPainter &painter);
  void applyDrag(const QPoint &delta);
  void applyResize(const QPoint &delta, HandleType handle);
  template <typename PointContainer>
  void buildPointPath(QPainterPath &path, const PointContainer &points, bool close);
  void buildShapePath(QPainterPath &path, const std::shared_ptr<Rect> &s);
  void buildShapePath(QPainterPath &path, const std::shared_ptr<Circle> &s);
  void buildShapePath(QPainterPath &path, const std::shared_ptr<Line> &s);
  void buildShapePath(QPainterPath &path, const std::shared_ptr<Polyline> &s);
  void buildShapePath(QPainterPath &path, const std::shared_ptr<Path> &s);
  void buildShapePath(QPainterPath &path, const std::shared_ptr<Hexagon> &s);
  void buildShapePath(QPainterPath &path, const std::shared_ptr<Text> &s);
  void addShapeToCanvas(const std::string, QPointF location);
  // This function is required to match the coordinates of svg to a translated / shifted canvas
  QPointF toCanvasCoordinates(QPointF point)
  {
    point.setX(point.x() / zoom_factor - x_offset);
    point.setY(point.y() / zoom_factor - y_offset);
    return point;
  }
  // Methods for Rendering and Hit-testing handles
  QRectF renderHandle(const QRectF &obj, HandleType handle_type);
  HandleType hitTestHandles(const QRectF &obj, const QPointF &point);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

public:
  // Names self sufficient for there meaning
  GraphicsObjectPtr selected_shape{nullptr};
  QString curr_tool{""};
  SVG svg;
  float x_offset{0};
  float y_offset{0};
  explicit Canvas(QWidget *parent = nullptr) : QWidget(parent), dragging(false)
  {
    undo_stack.clear();
    redo_stack.clear();
  }
  void updateCanvas(const SVG &newSvg); // Used to update canvas when a file is parsed and opened
  void updateCanvasSize(int w, int h);
  std::string currentCanvasToSVG() { return svg.toSVG(); }
  void clearCanvas();
  void setcurr_tool(const QString &tool_name);
  void undo();
  void redo();
  void cut();
  void copy();
  void paste();
  void zoom_in();
  void zoom_out();
  void zoom_reset();
};

#include "src/ui/canvas/CanvasParts.h" // Forward includes
#endif