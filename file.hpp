#define FILE_HPP
#ifdef FILE_HPP

#include <string>

class File
{
public:
    virtual ~File() {}

    virtual std::string getName() const = 0;
    virtual std::string getType() const = 0;
    virtual std::vector<char> read() const = 0;
    virtual bool write(const std::vector<char> &data) = 0;
};

#endif