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
        i = 1;
        j = 1;
        hadCR = false;
    }

    bool eof() const { return curr >= static_cast<int>(data.size()); }

    char top() const
    {
        if (eof())
            throw std::out_of_range("Reader: past end");
        return data[curr];
    }

    char next()
    {
        char c = top();
        curr++;
        if (c == '\r')
        {
            i++;
            j = 1;
            hadCR = true;
            return c;
        }
        if (c == '\n')
        {
            if (!hadCR)
            {
                i++;
                j = 1;
            }
            hadCR = false;
            return c;
        }

        j++;
        hadCR = false;
        return c;
    }

    int row() const { return i; }
    int col() const { return j; }
    const std::string &path() const { return src; }

private:
    std::string data;
    std::string src;
    int curr = 0;
    int i = 1;
    int j = 1;
    bool hadCR = false;
};

#endif