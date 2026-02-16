#include "src/io/Reader.h"

std::string Reader::readFile(const QString &path)
{
  QFile content(path);
  if (!content.open(QIODevice::ReadOnly))
    throw std::runtime_error("Reader: cannot open " + path.toStdString());

  return content.readAll().toStdString();
}

Reader::Reader(const QString &path) { open(path); }

void Reader::open(const QString &path)
{
  data = readFile(path);
  src = path.toStdString();
  curr = 0;
}

bool Reader::eof() const { return curr >= static_cast<int>(data.size()); }

char Reader::front() const
{
  if (eof())
    throw std::out_of_range("Reader: past end");
  return data[curr];
}

// Can create with name next because we are not using namespace std
char Reader::next()
{
  char c = front();
  curr++;

  return c;
}

const std::string &Reader::path() const
{
  return src;
}
