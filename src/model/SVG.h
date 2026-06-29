#ifndef MICRO_SVG_EDITOR_SRC_MODEL_SVG_H_
#define MICRO_SVG_EDITOR_SRC_MODEL_SVG_H_

#include <cstddef>
#include <string>
#include <vector>

#include "src/model/GraphicsObject.h"

class SVG {
 public:
  float width{800}, height{600};
  std::string xmlns{"http://www.w3.org/2000/svg"};
  std::vector<float> viewBox;  // x, y, width, height

  std::vector<GraphicsObjectPtr> objects;

  std::string ToSvg() const;

  void Add(GraphicsObjectPtr obj);
  void Remove(std::size_t idx);
  void Clear();

  SVG Clone() const;
};

#endif  // MICRO_SVG_EDITOR_SRC_MODEL_SVG_H_
