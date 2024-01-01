#define SOFTLINK_HPP
#ifdef SOFTLINK_HPP
#include "file.hpp"

#include <string>
#include <vector>

class SoftLink : public File
{
public:
    SoftLink(const std::string &n, const std::string &s);
    std::vector<char> cat() const override;
    std::vector<std::string>::const_iterator begin() const override;
    std::vector<std::string>::const_iterator end() const override;

    std::string getName() const override;
    std::string getType() const override;

private:
    std::unique_ptr<File> source;
    std::string linkName;
};

#endif