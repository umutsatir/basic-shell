#ifndef FILE_HPP
#define FILE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class File
{
public:
    File(const std::string &n) : name(n) {}
    virtual std::string getName() const { return name; }
    virtual std::string getType() const { return "File"; }
    virtual void cat() const { return; }
    void setName(const std::string &n) { name = n; }

protected:
    std::string name;
};

#endif