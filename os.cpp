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

void OS::recursive_ls(std::unique_ptr<File> root) const
{
    for (auto iter = root.begin(); iter != root.end(); ++iter)
    {
        if (iter->getType == "Directory")
            recursive_ls(iter);
        cout << iter->dirName << endl;
    }
}