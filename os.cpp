#include "os.hpp"

#include <iostream>
#include <sstream>
#include <exception>
#include <fstream>

OS::OS() : rootPtr(std::make_unique<Directory>("root")), root(rootPtr.get())
{
    loadProgram();
}

OS::~OS()
{
    saveProgram();
    delete root;
}

void OS::run()
{
    while (true)
    {
        size = 0;
        getPoints(rootPtr.get());
        if (size / 1000000 >= 10)
        {
            std::cout << "Disk size is full. Program will be terminated." << std::endl;
            break;
        }
        std::string command;
        std::cout << "MyShell" << std::endl
                  << "Current size: " << size << " Bytes" << std::endl
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

long int OS::getPoints(Directory *_root)
{
    for (auto iter = _root->begin(); iter != _root->end(); ++iter)
    {
        size += sizeof(*(iter->get()));
        if ((*iter)->getType() == "Directory")
            size += getPoints(dynamic_cast<Directory *>(iter->get()));
    }
    return size;
}

void OS::saveProgram()
{
    std::ofstream outFile("os_state.txt", std::ios::out);

    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    if (!dirStack.empty())
        saveDirectory(outFile, *(dirStack.begin()), false);
    else
        saveDirectory(outFile, root, false);

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

    loadDirectory(inFile, root);

    inFile.close();
}

void OS::saveDirectory(std::ofstream &outFile, const Directory *dir, bool isDir)
{
    for (auto iter = dir->begin(); iter != dir->end(); ++iter)
    {
        outFile << (*iter)->getType() << " " << (*iter)->getName();

        if ((*iter)->getType() == "SoftLink")
            outFile << " " << dynamic_cast<SoftLink *>(iter->get())->getSourceName();

        outFile << std::endl;

        if ((*iter)->getType() == "Directory")
            saveDirectory(outFile, dynamic_cast<Directory *>((*iter).get()), true);
    }

    if (isDir)
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
            std::string name;
            if (entry == "Directory")
            {
                inFile >> name;
                auto newDir = std::make_unique<Directory>(name);
                parentDir->addFile(std::move(newDir));
                auto iter = parentDir->end();
                --iter; // selecting the last element of the allDir
                loadDirectory(inFile, dynamic_cast<Directory *>(iter->get()));
            }
            else if (entry == "regFile")
            {
                inFile >> name;
                auto newFile = std::make_unique<RegFile>(name);
                parentDir->addFile(std::move(newFile));
            }
            else
            {
                inFile >> name;
                std::string sourceName;
                inFile >> sourceName;
                auto newFile = std::make_unique<SoftLink>(sourceName, name);
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
        else
            std::cout << "ERROR: Parameters are not correct." << std::endl;
    }
    else if (seperatedCommands[0] == "cd")
    {
        if (seperatedCommands.size() == 2)
            cd(seperatedCommands[1]);
        else
            std::cout << "ERROR: Parameters are not correct." << std::endl;
    }
    else if (seperatedCommands[0] == "cp")
    {
        if (seperatedCommands.size() == 3)
            cp(seperatedCommands[1], seperatedCommands[2]);
        else
            std::cout << "ERROR: Parameters are not correct." << std::endl;
    }
    else if (seperatedCommands[0] == "mkdir")
    {
        if (seperatedCommands.size() == 2)
            mkdir(seperatedCommands[1]);
        else
            std::cout << "ERROR: Parameters are not correct." << std::endl;
    }
    else if (seperatedCommands[0] == "touch")
    {
        if (seperatedCommands.size() == 2)
            touch(seperatedCommands[1]);
        else
            std::cout << "ERROR: Parameters are not correct." << std::endl;
    }
    else if (seperatedCommands[0] == "rm")
    {
        if (seperatedCommands.size() == 2)
            rm(seperatedCommands[1]);
        else
            std::cout << "ERROR: Parameters are not correct." << std::endl;
    }
    else if (seperatedCommands[0] == "rmdir")
    {
        if (seperatedCommands.size() == 2)
            rmdir(seperatedCommands[1]);
        else
            std::cout << "ERROR: Parameters are not correct." << std::endl;
    }
    else if (seperatedCommands[0] == "link")
    {
        if (seperatedCommands.size() == 3)
            link(seperatedCommands[1], seperatedCommands[2]);
        else
            std::cout << "ERROR: Parameters are not correct." << std::endl;
    }
    else if (seperatedCommands[0] == "cat")
    {
        if (seperatedCommands.size() == 2)
            cat(seperatedCommands[1]);
        else
            std::cout << "ERROR: Parameters are not correct." << std::endl;
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
        root = dirStack.back();
        dirStack.pop_back();
        return;
    }

    for (auto iter = root->begin(); iter != root->end(); ++iter)
    {
        if ((*iter)->getName() == dirName && (*iter)->getType() == "Directory")
        {
            dirStack.push_back(root);
            Directory *currDir = dynamic_cast<Directory *>((*iter).get());
            root = currDir;
            return;
        }
    }

    std::cout << "Directory not found." << std::endl;
}

Directory *OS::cd_nonsave(const std::string &dirName)
{
    if (dirName == "." || dirName.empty())
        return root;
    if (dirName == "..")
    {
        if (dirStack.empty())
        {
            std::cout << "You are already in the root directory." << std::endl;
            return root;
        }
        return dirStack.back();
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
    return root;
}

void OS::cp(const std::string &source, const std::string &dirName)
{

    Directory *sourceDir = cd_nonsave(source);
    Directory *destDir = cd_nonsave(dirName);
    if (sourceDir->getType() == "Directory")
    {
        std::unique_ptr<Directory> newDir;
        if (source == "." || source.empty())
            newDir = std::make_unique<Directory>(*root);
        else
            newDir = std::make_unique<Directory>(source);
        Directory *newDirPtr = newDir.get();
        cp_directory(sourceDir, newDirPtr);
        destDir->addFile(std::move(newDir));
        return;
    }
    else if (sourceDir->getType() == "regFile")
    {
        RegFile *currFile = dynamic_cast<RegFile *>(sourceDir);
        sourceDir->addFile(std::move(std::make_unique<RegFile>(*currFile)));
        return;
    }
    std::cout << "File or directory not found." << std::endl;
    return;
}

void OS::cp_directory(Directory *sourceDir, Directory *destDir)
{
    for (auto iter = sourceDir->begin(); iter != sourceDir->end(); ++iter)
    {
        if ((*iter)->getType() == "Directory")
        {
            destDir->addFile(std::move(std::make_unique<Directory>((*iter)->getName()))); // copying the directory
            auto destDirIter = destDir->end();
            --destDirIter;
            if (destDirIter->get())
                cp_directory(dynamic_cast<Directory *>(iter->get()), dynamic_cast<Directory *>(destDirIter->get())); // copying the contents of the directory}
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
    std::cout << "File not found." << std::endl;
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
        if ((*iter)->getName() == source && (*iter)->getType() == "regFile")
        {
            std::unique_ptr<SoftLink> ptr = std::make_unique<SoftLink>(source, destName);
            root->addFile(std::move(ptr));
            return;
        }
    }
    std::cout << "File not found." << std::endl;
    return;
}