#ifndef READER_HPP
#define READER_HPP

#include <cstddef>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

#include <QString>
#include <QFile>

class Reader
{
  std::string readFile(const QString &path)
  {
    QFile content(path);
    if (!content.open(QIODevice::ReadOnly))
      throw std::runtime_error("Reader: cannot open " + path.toStdString());

    return content.readAll().toStdString();
  }

public:
  Reader() = default;
  Reader(const QString &path) { open(path); }

  void open(const QString &path)
  {
    data = readFile(path);
    src = path.toStdString();
    curr = 0;
  }

  bool eof() const { return curr >= static_cast<int>(data.size()); }

  char front() const
  {
    if (eof())
      throw std::out_of_range("Reader: past end");
    return data[curr];
  }

  // Can create with name next because we are not using namespace std
  char next()
  {
    char c = front();
    curr++;

    return c;
  }

  const std::string &path() const { return src; }

private:
  std::string data;
  std::string src;
  int curr = 0;
};

#endif