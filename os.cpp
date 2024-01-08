#include "os.hpp"

#include <iostream>
#include <sstream>
#include <exception>
#include <fstream>

OS::OS() : root(std::make_unique<Directory>("root"))
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
            break;
        }

        runCommand(command);
        std::cout << std::endl;
    }
}

void OS::saveProgram()
{
    std::ofstream outFile("os_state.txt", std::ios::out);

    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    if (dirStack.empty())
        saveDirectory(outFile, dirStack.begin()->get());
    else
        saveDirectory(outFile, root.get());

    outFile.close();
}

void OS::loadProgram()
{
    std::ifstream inFile("os_state.txt", std::ios::in);

    if (!inFile.is_open())
    {
        std::cerr << "Error: Could not open file for reading. Starting with a fresh state." << std::endl;
        return;
    }

    // clear root
    root = std::make_unique<Directory>("root");

    loadDirectory(inFile, root.get());

    inFile.close();
}

void OS::saveDirectory(std::ofstream &outFile, const Directory *dir)
{
    for (auto iter = dir->begin(); iter != dir->end(); ++iter)
    {
        if ((*iter)->getType() == "Directory")
            saveDirectory(outFile, dynamic_cast<Directory *>((*iter).get()));
        outFile << (*iter)->getType() << " " << (*iter)->getName() << std::endl;
    }

    outFile << "END_DIR" << std::endl;
}

void OS::loadDirectory(std::ifstream &inFile, Directory *parentDir)
{
    std::string entry;
    while (inFile >> entry)
    {
        if (entry == "END_DIR")
            return;
        else
        {
            if (entry == "Directory")
            {
                std::string dirName;
                inFile >> dirName;
                auto newDir = std::make_unique<Directory>(dirName);
                parentDir->addFile(std::move(newDir));
                loadDirectory(inFile, dynamic_cast<Directory *>((--(root->end()))->get())); // get the last element of the allDir array
            }
            else if (entry == "regFile")
            {
                std::string name;
                inFile >> name;
                auto newFile = std::make_unique<RegFile>(name);
                parentDir->addFile(std::move(newFile));
            }
            else
            {
                std::string concatName;
                inFile >> concatName;
                std::vector<std::string> names = seperateCommand(concatName, ' ');
                auto newFile = std::make_unique<SoftLink>(names[0], names[1]);
                parentDir->addFile(std::move(newFile));
            }
        }
    }
}

void OS::runCommand(const std::string &command)
{
    std::vector<std::string> seperatedCommands = seperateCommand(command, ' ');
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

std::vector<std::string> OS::seperateCommand(const std::string &command, const char &chr) const
{
    std::vector<std::string> seperatedCommands;

    std::stringstream ss(command);
    std::string word;
    while (getline(ss, word, chr))
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

void OS::cd(const std::string &dirName)
{
    if (dirName == "." || dirName.empty())
        return;
    if (dirName == "..")
    {
        if (dirStack.empty())
        {
            std::cout << "You are already in the root directory." << std::endl;
            return;
        }
        root = std::move(dirStack.back());
        dirStack.pop_back();
        return;
    }

    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if ((*iter)->getName() == dirName && (*iter)->getType() == "Directory")
        {
            dirStack.push_back(std::move(root));
            Directory *currDir = dynamic_cast<Directory *>((*iter).get());
            root = std::make_unique<Directory>(*currDir);
            return;
        }
    }

    std::cout << "Directory not found." << std::endl;
}

Directory *OS::cd_nonsave(const std::string &dirName)
{
    if (dirName == "." || dirName.empty())
        return root.get();
    if (dirName == "..")
    {
        if (dirStack.empty())
        {
            std::cout << "You are already in the root directory." << std::endl;
            return root.get();
        }
        return dirStack.back().get();
    }

    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if ((*iter)->getName() == dirName && (*iter)->getType() == "Directory")
        {
            Directory *currDir = dynamic_cast<Directory *>((*iter).get());
            return currDir;
        }
    }

    std::cout << "Directory not found." << std::endl;
    return root.get();
}

void OS::cp(const std::string &source, const std::string &dirName)
{
    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if ((*iter)->getName() == source)
        {
            Directory *sourceDir = cd_nonsave(dirName);
            if ((*iter)->getType() == "Directory")
            {
                std::unique_ptr<Directory> destDir;
                if (source == "." || source.empty())
                    destDir = std::make_unique<Directory>(*(root.get()));
                else
                    destDir = std::make_unique<Directory>(source);

                for (auto iter = root->begin(); iter != root->end(); ++iter)
                {
                    if ((*iter)->getType() == "Directory")
                    {
                        Directory *currDir = dynamic_cast<Directory *>(((*iter).get()));
                        destDir->addFile(std::move(std::make_unique<Directory>(*currDir)));
                    }
                    else if ((*iter)->getType() == "regFile")
                    {
                        RegFile *currFile = dynamic_cast<RegFile *>((*iter).get());
                        destDir->addFile(std::move(std::make_unique<RegFile>(*currFile)));
                    }
                    else
                    {
                        SoftLink *currLink = dynamic_cast<SoftLink *>((*iter).get());
                        destDir->addFile(std::move(std::make_unique<SoftLink>(*currLink)));
                    }
                }
                sourceDir->addFile(std::move(destDir));
            }
            else if ((*iter)->getType() == "regFile")
            {
                RegFile *currFile = dynamic_cast<RegFile *>((*iter).get());
                sourceDir->addFile(std::move(std::make_unique<RegFile>(*currFile)));
                return;
            }
        }
    }
    std::cout << "File or directory not found." << std::endl;
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
        if ((*iter)->getName() == name && (*iter)->getType() != "Directory")
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

void OS::link(const std::string &source, const std::string &destName)
{
    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if ((*iter)->getName() == source)
        {
            std::unique_ptr<SoftLink> ptr = std::make_unique<SoftLink>(source, destName);
            root->addFile(std::move(ptr));
            return;
        }
    }
    std::cout << "File or directory not found." << std::endl;
    return;
}