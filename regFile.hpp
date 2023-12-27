#define REGFILE_HPP
#ifdef REGFILE_HPP
#include "file.hpp"

#include <vector>
#include <string>

class RegFile : public File
{
public:
    RegFile(const std::string &name);

private:
    std::vector<char> data;
};

#endif