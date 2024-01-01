#include "softLink.hpp"

SoftLink::SoftLink(const std::string &n, const std::string &s) : linkName(n), source(std::make_unique<File>(s)) {}

std::vector<char> SoftLink::cat() const
{
    return source->cat();
}

std::vector<std::string>::const_iterator SoftLink::begin() const
{
    return source->begin();
}

std::vector<std::string>::const_iterator SoftLink::end() const
{
    return source->end();
}

std::string SoftLink::getName() const
{
    return linkName;
}

std::string SoftLink::getType() const
{
    return "SoftLink";
}