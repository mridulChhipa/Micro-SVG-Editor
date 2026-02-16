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
  std::string readFile(const QString &path);

public:
  Reader() {};
  Reader(const QString &path);

  void open(const QString &path);
  bool eof() const;
  char front() const;

  // Can create with name next because we are not using namespace std
  char next();
  const std::string &path() const;

private:
  std::string data;
  std::string src;
  int curr = 0;
};

#endif