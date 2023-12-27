#define SOFTLINK_HPP
#ifdef SOFTLINK_HPP
#include "file.hpp"

#include <string>
#include <vector>

class SoftLink : public File
{
public:
    SoftLink(const std::string &n, const std::string &s) : name(n), source(std::make_unique<File>(s)) {}
    std::vector<char> read() const { return this->source->read(); }

private:
    std::unique_ptr<File> source;
    std::string name;
    std::string sourceName;
};

#endif