#ifndef REGFILE_HPP
#define REGFILE_HPP
#include "file.hpp"

#include <vector>
#include <string>

class RegFile : public File
{
public:
    RegFile(const std::string &n);
    void cat() const override;

    std::string getType() const override;
    std::string getData() const;

private:
    std::string data;
};

#endif