#include "softLink.hpp"

SoftLink::SoftLink(std::string s, std::string n) : File(n), sourceName(s) {}

SoftLink::SoftLink(const SoftLink &other) : File(other), sourceName(other.sourceName) {}

void SoftLink::cat() const
{
    std::cout << "An iterator is an object that has the ability to access items from its collection one at a time and keep track of the current position." << std::endl;
}

std::string SoftLink::getName() const
{
    return name + " " + sourceName;
}

std::string SoftLink::getType() const
{
    return "SoftLink";
}

std::string SoftLink::getSourceName() const
{
    return sourceName;
}