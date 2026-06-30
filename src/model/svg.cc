#include "src/model/svg.h"

#include <sstream>
#include <utility>

namespace micro_svg_editor {

std::string SVG::ToSvg() const {
  std::ostringstream ss;
  ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  ss << "<svg width=\"" << width_ << "\" height=\"" << height_ << "\" ";
  ss << "xmlns=\"" << xmlns_ << "\">\n";
  for (const auto& obj : objects_) ss << "  " << obj->ToSvg() << "\n";
  ss << "</svg>\n";
  return ss.str();
}

void SVG::Add(GraphicsObjectPtr obj) { objects_.push_back(std::move(obj)); }

void SVG::Remove(std::size_t idx) {
  if (idx < objects_.size()) objects_.erase(objects_.begin() + idx);
}

void SVG::Clear() { objects_.clear(); }

SVG SVG::Clone() const {
  SVG copy;
  copy.width_ = width_;
  copy.height_ = height_;
  copy.xmlns_ = xmlns_;
  copy.view_box_ = view_box_;
  for (const auto& obj : objects_)
    copy.objects_.push_back(obj->Clone());  // Deep copy each owned object.
  return copy;
}

}  // namespace micro_svg_editor
