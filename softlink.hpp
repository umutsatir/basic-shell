#ifndef SOFTLINK_HPP
#define SOFTLINK_HPP
#include "file.hpp"

#include <string>
#include <vector>

class SoftLink : public File
{
public:
    SoftLink(std::string n);
    void cat() const override;

    std::string getName() const override;
    std::string getType() const override;

private:
    std::unique_ptr<File> source;
    std::string linkName;
};

#endif