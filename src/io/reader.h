#ifndef MICRO_SVG_EDITOR_SRC_IO_READER_H_
#define MICRO_SVG_EDITOR_SRC_IO_READER_H_

#include <string>

#include <QString>

namespace micro_svg_editor {

class Reader {
 public:
  Reader() = default;
  explicit Reader(const QString& path);

  void Open(const QString& path);
  bool Eof() const;
  char Front() const;

  // Named Next (not next) since we are not relying on std naming here.
  char Next();
  const std::string& Path() const;

 private:
  std::string ReadFile(const QString& path);

  std::string data_;
  std::string src_;
  int curr_ = 0;
};

}  // namespace micro_svg_editor

#endif  // MICRO_SVG_EDITOR_SRC_IO_READER_H_
