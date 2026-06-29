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

#include "src/model/Circle.h"
#include "src/model/Hexagon.h"
#include "src/model/Line.h"
#include "src/model/Path.h"
#include "src/model/Polyline.h"
#include "src/model/Rect.h"
#include "src/model/SVG.h"
#include "src/model/Text.h"
#include "src/ui/canvas/HandleType.h"

#endif  // MICRO_SVG_EDITOR_SRC_UI_CANVAS_CANVAS_HEADERS_H_
