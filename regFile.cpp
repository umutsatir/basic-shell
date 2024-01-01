#include "regFile.hpp"

RegFile::RegFile(const std::string &n) : name(n) {}

std::vector<char> RegFile::cat() const
{
    return data;
}

std::vector<char>::const_iterator regFile::begin() const
{
    return data.begin();
}

std::vector<char>::const_iterator regFile::end() const
{
    return data.end();
}

std::string regFile::getName() const
{
    return name;
}

std::string regFile::getType() const
{
    return "regFile";
}
