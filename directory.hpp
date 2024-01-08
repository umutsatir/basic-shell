#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP
#include "file.hpp"
#include <algorithm>

class Directory : public File
{
public:
    Directory(const std::string &n);
    Directory(const Directory &d);
    Directory(Directory &&d);
    void addFile(std::unique_ptr<File> f);
    bool removeFile(const std::string &name);
    void cat() const override;
    std::vector<std::unique_ptr<File>>::const_iterator begin() const;
    std::vector<std::unique_ptr<File>>::const_iterator end() const;

    std::string getType() const override;
    Directory &operator=(Directory &&d) noexcept;
    Directory &operator=(const Directory &d) = delete;

private:
    std::vector<std::unique_ptr<File>> allDir;
    bool isFound(const std::string &name);
};

#endif