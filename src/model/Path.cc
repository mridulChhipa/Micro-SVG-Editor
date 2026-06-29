#include "src/model/Path.h"

#include <cctype>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

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

      commands.emplace_back(command, points);

      // If the inner loop failed to find a number, the stream might be in a
      // fail state. We clear it so the outer loop can read the next 'command'.
      ss.clear();
    }
  }
}

std::string Path::Type() const { return "path"; }

std::string Path::ToSvg() const {
  std::ostringstream ss;
  ss << "<path" << PrintStyle() << " d=\"";
  for (const auto& [command, points] : commands) {
    ss << command << " ";
    for (const auto& [x, y] : points) {
      ss << x << " " << y << " ";
    }
  }
  ss << "\"/>";
  return ss.str();
}

std::shared_ptr<GraphicsObject> Path::Clone() const {
  auto copy = std::make_shared<Path>();
  copy->commands = commands;
  copy->fill = fill;
  copy->stroke = stroke;
  copy->stroke_width = stroke_width;
  copy->stroke_opacity = stroke_opacity;
  copy->fill_opacity = fill_opacity;
  copy->opacity = opacity;
  copy->stroke_linecap = stroke_linecap;
  copy->stroke_linejoin = stroke_linejoin;
  copy->stroke_dasharray = stroke_dasharray;
  copy->transform = transform;
  copy->visibility = visibility;
  return copy;
}
