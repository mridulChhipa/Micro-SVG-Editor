#include "src/model/path.h"

#include <cctype>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "src/model/shape_visitor.h"

namespace micro_svg_editor {

Path::Path(const std::unordered_map<std::string, std::string>& attributes) {
  InitialiseStyle(attributes);
  if (attributes.count("d")) {
    std::istringstream ss(attributes.at("d"));
    char command;
    float x, y;
    while (ss >> command) {
      std::vector<std::pair<float, float>> points;

      // Use a loop that checks if the next character is a digit or a sign.
      // This prevents the inner loop from consuming the next command letter.
      while (ss >> std::ws && (std::isdigit(ss.peek()) || ss.peek() == '-' ||
                               ss.peek() == '.')) {
        if (ss >> x >> y) {
          points.emplace_back(x, y);
        } else {
          ss.clear();
          break;
        }
      }

      commands_.emplace_back(command, points);

      // If the inner loop failed to find a number, the stream might be in a
      // fail state. We clear it so the outer loop can read the next 'command'.
      ss.clear();
    }
  }
}

void Path::Accept(ShapeVisitor& visitor) { visitor.Visit(*this); }

void Path::Translate(double dx, double dy) {
  for (auto& [_, points] : commands_)
    for (auto& [x, y] : points) {
      x += dx;
      y += dy;
    }
}

void Path::Resize(int, int, bool, bool, bool, bool) {}

std::string Path::ToSvg() const {
  std::ostringstream ss;
  ss << "<path" << PrintStyle() << " d=\"";
  for (const auto& [command, points] : commands_) {
    ss << command << " ";
    for (const auto& [x, y] : points) {
      ss << x << " " << y << " ";
    }
  }
  ss << "\"/>";
  return ss.str();
}

std::unique_ptr<GraphicsObject> Path::Clone() const {
  auto copy = std::make_unique<Path>();
  copy->commands_ = commands_;
  copy->set_fill(fill());
  copy->set_stroke(stroke());
  copy->set_stroke_width(stroke_width());
  copy->set_stroke_opacity(stroke_opacity());
  copy->set_fill_opacity(fill_opacity());
  copy->set_opacity(opacity());
  copy->set_stroke_linecap(stroke_linecap());
  copy->set_stroke_linejoin(stroke_linejoin());
  copy->set_stroke_dasharray(stroke_dasharray());
  copy->set_transform(transform());
  copy->set_visibility(visibility());
  return copy;
}

}  // namespace micro_svg_editor
