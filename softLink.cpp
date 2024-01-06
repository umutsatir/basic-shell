#include "softLink.hpp"

SoftLink::SoftLink(std::string n) : File(n), source(std::make_unique<File>(n)) {}

void SoftLink::cat() const
{
    return source->cat();
}

std::string SoftLink::getName() const
{
    return linkName;
}

std::string SoftLink::getType() const
{
    return "SoftLink";
}