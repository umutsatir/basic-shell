#include "os.hpp"

#include <iostream>
#include <sstream>

OS::OS() : root(std::make_unique<Directory>("root"))
{
    loadProgam();
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
    }
}

void OS::saveProgram()
{
}

void OS::loadProgram()
{
}

void OS::runCommand(const std::string &command)
{
    std::vector<std::string> seperatedCommands = seperateCommand(command);
    // !! check if vector has enough members!!
    switch (seperatedCommands[0])
    {
    case "ls":
        if (seperatedCommands[1] == "-R")
            recursive_ls();
        else
            ls();
        break;
    case "cd":
        cd();
        break;
    case "cp":
        cp();
        break;
    case "mkdir":
        mkdir();
        break;
    case "rm":
        rm();
        break;
    case "link":
        link();
        break;
    case "cat":
        cat();
        break;
    default:
        std::cout << "Command not found." << std::endl;
        break;
    }
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

void OS::recursive_ls(std::unique_ptr<File> end) const
{ //!! will be edited
    if (root == end)
        return;
    if (root->getType() == "Directory")
    {
        end = root->end();
        recursive_ls(root->begin());
    }
    std::cout << root->getName() << std::endl;
    recursive_ls(++root);
}

void OS::ls() const
{
    while ()
}

void OS::cd(const std::string &dirName)
{
    if (dirName == ".")
        return;
    if (dirName == "..")
        continue; //!! edit this
    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if (iter->getName() == dirName && iter->getType() == "Directory")
        {
            root = iter;
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}

void OS::cp()
{
}

void OS::mkdir(const std::string &dirName)
{
    std::unique_ptr<Directory> newDir = std::make_unique<Directory>(dirName);
    root->addFile(newDir);
}

void OS::cat(const std::string &name) const
{
    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if (iter->getName() == name && iter->getType() == "regFile")
        {
            std::cout << iter->cat() << std::endl; //!! check this
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}

void OS::rm(const std::string &name)
{
    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if (iter->getName() == name)
        {
            root->removeFile(iter);
            return;
        }
    }
    std::cout << "Directory not found." << std::endl;
}