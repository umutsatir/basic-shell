#include "regFile.hpp"

RegFile::RegFile(const std::string &n) : File(n)
{
    data = "An iterator is an object that has the ability to access items from its collection one at a time and keep track of the current position.";
}

void RegFile::cat() const
{
    std::cout << data << std::endl;
}

std::string RegFile::getType() const
{
    return "regFile";
}