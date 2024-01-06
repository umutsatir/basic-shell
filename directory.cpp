#include "directory.hpp"

Directory::Directory(const std::string &n) : File(n) {}

Directory::Directory(const Directory &d) : File(d.getName())
{
    for (const auto &file : d.allDir)
        allDir.push_back(std::make_unique<File>(*file));
}

Directory::Directory(Directory &&d) : File(std::move(d.getName())), allDir(std::move(d.allDir)) {}

void Directory::addFile(std::unique_ptr<File> f)
{
    if (isFound(f->getName()))
    {
        std::cout << "File or directory already exists." << std::endl;
        return;
    }
    allDir.push_back(std::move(f));
}

bool Directory::isFound(const std::string &name)
{
    for (const auto &elem : allDir)
    {
        if (elem->getName() == name)
            return true;
    }
    return false;
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

void Directory::cat() const
{
    std::cout << "Unable to get data because it is a directory." << std::endl;
}

std::vector<std::unique_ptr<File>>::const_iterator Directory::begin() const
{
    return allDir.begin();
}

std::vector<std::unique_ptr<File>>::const_iterator Directory::end() const
{
    return allDir.end();
}

std::string Directory::getType() const
{
    return "Directory";
}