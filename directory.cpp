#include "directory.hpp"

Directory::Directory(const std::string &n) : dirName(n) {}

void Directory::addFile(std::unique_ptr<File> f)
{
    allDir.push_back(f);
}

bool Directory::removeFile(const std::string &name)
{
    for (const auto &item : allDir)
    {
        if (item->getName() == name)
        {
            auto iter = std::find(allDir.begin(), allDir.end(), item);
            allDir.erase(iter);
            return true;
        }
    }
    return false;
}

std::vector<char> Directory::cat() const
{
    std::vector<char> concat;

    for (const auto &file : allDir)
    {
        std::vector<char> fileData = file->cat();
        concat.insert(concat.end(), fileData.begin(), fileData.end()); // adding the text at the end of the concat variable
    }
    return concat;
}

std::vector<std::unique_ptr<File>>::const_iterator Directory::begin() const
{
    return allDir.begin();
}

std::vector<std::unique_ptr<File>>::const_iterator Directory::end() const
{
    return allDir.end();
}

std::string Directory::getName() const
{
    return dirName;
}

std::string Directory::getType() const
{
    return "Directory";
}