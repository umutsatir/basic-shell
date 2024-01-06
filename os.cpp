#include "os.hpp"

#include <iostream>
#include <sstream>

OS::OS() : root(std::make_unique<Directory>("root")), parent(nullptr)
{
    loadProgram();
}

OS::~OS()
{
    saveProgram();
}

void OS::run()
{
    while (true)
    {
        std::string command;
        std::cout << "MyShell" << std::endl
                  << "> ";
        std::getline(std::cin, command);

        if (command == "exit")
        {
            saveProgram();
            break;
        }

        runCommand(command);
        std::cout << std::endl;
    }
}

void OS::saveProgram()
{
    return;
}

void OS::loadProgram()
{
    return;
}

void OS::runCommand(const std::string &command)
{
    std::vector<std::string> seperatedCommands = seperateCommand(command);
    if (seperatedCommands[0] == "ls")
    {
        if (seperatedCommands.size() == 2 && seperatedCommands[1] == "-R")
            recursive_ls(root->begin(), root->end());
        else if (seperatedCommands.size() == 1)
            ls();
    }
    else if (seperatedCommands[0] == "cd")
    {
        if (seperatedCommands.size() == 2)
            cd(seperatedCommands[1]);
    }
    else if (seperatedCommands[0] == "cp")
    {
        if (seperatedCommands.size() == 3)
            cp(seperatedCommands[1], seperatedCommands[2]);
    }
    else if (seperatedCommands[0] == "mkdir")
    {
        if (seperatedCommands.size() == 2)
            mkdir(seperatedCommands[1]);
    }
    else if (seperatedCommands[0] == "touch")
    {
        if (seperatedCommands.size() == 2)
            touch(seperatedCommands[1]);
    }
    else if (seperatedCommands[0] == "rm")
    {
        if (seperatedCommands.size() == 2)
            rm(seperatedCommands[1]);
    }
    else if (seperatedCommands[0] == "rmdir")
    {
        if (seperatedCommands.size() == 2)
            rmdir(seperatedCommands[1]);
    }
    else if (seperatedCommands[0] == "link")
    {
        if (seperatedCommands.size() == 3)
            link(seperatedCommands[1], seperatedCommands[2]);
    }
    else if (seperatedCommands[0] == "cat")
    {
        if (seperatedCommands.size() == 2)
            cat(seperatedCommands[1]);
    }
    else
        std::cout << "Command not found." << std::endl;
}

std::vector<std::string> OS::seperateCommand(const std::string &command) const
{
    std::vector<std::string> seperatedCommands;

    std::stringstream ss(command);
    std::string word;
    while (getline(ss, word, ' '))
    {
        seperatedCommands.push_back(word);
    }
    return seperatedCommands;
}

void OS::recursive_ls(std::vector<std::unique_ptr<File>>::const_iterator iter, std::vector<std::unique_ptr<File>>::const_iterator end) const
{
    if (iter == end)
        return;
    std::cout << (*iter)->getType() << "     " << (*iter)->getName() << std::endl;
    recursive_ls(iter + 1, end);
}

void OS::ls() const
{
    for (auto iter = root->begin(); iter != root->end(); ++iter)
        std::cout << (*iter)->getType() << "     " << (*iter)->getName() << std::endl;
}

void OS::cd(const std::string &dirName) // !! check if this is working
{
    if (dirName == ".")
        return;
    if (dirName == ".." && parent != nullptr)
    {
        parent = root.get();
        root = std::make_unique<Directory>(*parent);
        return;
    }

    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if ((*iter)->getName() == dirName && (*iter)->getType() == "Directory")
        {
            parent = root.get(); // get function returns the stored pointer, so we can store it in parent variable
            root = std::make_unique<Directory>(*iter);
            return;
        }
    }

    std::cout << "Directory not found." << std::endl;
    return;
}

void OS::cp(const std::string &destName, const std::string &source)
{
    return;
}

void OS::mkdir(const std::string &dirName)
{
    std::unique_ptr<File> newDir = std::make_unique<Directory>(dirName);
    root->addFile(std::move(newDir));
}

void OS::touch(const std::string &dirName)
{
    std::unique_ptr<File> newDir = std::make_unique<RegFile>(dirName);
    root->addFile(std::move(newDir));
}

void OS::cat(const std::string &name) const
{
    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if ((*iter)->getName() == name)
        {
            (*iter)->cat();
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}

void OS::rm(const std::string &name)
{
    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if ((*iter)->getName() == name && (*iter)->getType() == "regFile")
        {
            root->removeFile((*iter)->getName());
            return;
        }
    }
    std::cout << "File not found." << std::endl;
}

void OS::rmdir(const std::string &name)
{
    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if ((*iter)->getName() == name && (*iter)->getType() == "Directory")
        {
            root->removeFile((*iter)->getName());
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}

void OS::link(const std::string &destName, const std::string &source)
{
    return;
}

Directory *OS::getParentDir() const
{
    return parent;
}