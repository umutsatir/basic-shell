#define FILE_HPP
#ifdef FILE_HPP

#include <string>
#include <vector>

class File
{
public:
    virtual ~File() {}

    virtual std::string getName() const = 0;
    virtual std::string getType() const = 0;
    virtual std::vector<char> cat() const = 0;

    // iterators
    virtual std::vector<std::string>::const_iterator begin() const = 0;
    virtual std::vector<std::string>::const_iterator end() const = 0;

private:
    std::string name;
};

#endif