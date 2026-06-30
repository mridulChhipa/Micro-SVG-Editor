#ifndef MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_HEADERS_H_
#define MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_HEADERS_H_

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <ranges>
#include <variant>
#include <vector>

#include <QImage>
#include <QInputDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRegularExpression>
#include <QResizeEvent>
#include <QWidget>

#include "src/model/circle.h"
#include "src/model/hexagon.h"
#include "src/model/line.h"
#include "src/model/path.h"
#include "src/model/polyline.h"
#include "src/model/rect.h"
#include "src/model/svg.h"
#include "src/model/text.h"
#include "src/ui/canvas/handle_type.h"

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_HEADERS_H_
