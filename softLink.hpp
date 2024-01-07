#ifndef SOFTLINK_HPP
#define SOFTLINK_HPP
#include "file.hpp"
#include "regFile.hpp"

#include <string>
#include <vector>

class SoftLink : public File
{
public:
    SoftLink(std::string s, std::string n);
    SoftLink(const SoftLink &other);
    void cat() const override;

    std::string getName() const override;
    std::string getType() const override;
    std::string getSourceName() const;

private:
    std::string sourceName;
};

#endif