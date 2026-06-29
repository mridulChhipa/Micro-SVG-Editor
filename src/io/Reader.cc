#include "src/io/Reader.h"

#include <stdexcept>

#include <QFile>

std::string Reader::ReadFile(const QString& path) {
  QFile content(path);
  if (!content.open(QIODevice::ReadOnly))
    throw std::runtime_error("Reader: cannot open " + path.toStdString());

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
  if (Eof()) throw std::out_of_range("Reader: past end");
  return data_[curr_];
}

char Reader::Next() {
  char c = Front();
  curr_++;
  return c;
}

const std::string& Reader::Path() const { return src_; }
