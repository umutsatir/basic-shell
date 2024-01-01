#define REGFILE_HPP
#ifdef REGFILE_HPP
#include "file.hpp"

#include <vector>
#include <string>

class RegFile : public File
{
public:
    RegFile(const std::string &n);
    std::vector<char> cat() const override;
    std::vector<char>::const_iterator begin() const override;
    std::vector<char>::const_iterator end() const override;

    std::string getName() const override;
    std::string getType() const override;

private:
    std::vector<char> data;
    std::string name;
};

#endif