#define DIRECTORY_HPP
#ifdef DIRECTORY_HPP
#include "file.hpp"

class Directory : public File
{
public:
    Directory(const std::string &n);
    void addFile(std::unique_ptr<File> f);
    bool removeFile(const std::string &name);
    std::vector<char> cat() const override;
    std::vector<std::unique_ptr<File>>::const_iterator begin() const override;
    std::vector<std::unique_ptr<File>>::const_iterator end() const override;

    std::string getName() const override;
    std::string getType() const override;

private:
    std::string dirName;
    std::vector<std::unique_ptr<File>> allDir;
};

#endif