#include "src/io/reader.h"

#include <QDebug>
#include <QFile>

namespace micro_svg_editor {

std::string Reader::ReadFile(const QString& path) {
  QFile content(path);
  if (!content.open(QIODevice::ReadOnly)) {
    qWarning() << "Reader: cannot open" << path;
    return std::string();
  }

  return content.readAll().toStdString();
}

Reader::Reader(const QString& path) { Open(path); }

void Reader::Open(const QString& path) {
  data_ = ReadFile(path);
  src_ = path.toStdString();
  curr_ = 0;
}

bool Reader::Eof() const { return curr_ >= static_cast<int>(data_.size()); }

char Reader::Front() const {
  if (Eof()) return '\0';
  return data_[curr_];
}

char Reader::Next() {
  char c = Front();
  curr_++;
  return c;
}

const std::string& Reader::Path() const { return src_; }

}  // namespace micro_svg_editor
