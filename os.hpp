#ifndef OS_HPP
#define OS_HPP
#include "directory.hpp"
#include "regFile.hpp"
#include "softLink.hpp"

class OS
{
public:
    OS();
    ~OS();
    void run();

private:
    // variables
    std::unique_ptr<Directory> rootPtr;
    Directory *root;
    std::vector<Directory *> dirStack;
    long int size = 0;
    // other functions
    std::vector<std::string> seperateCommand(const std::string &command, const char &chr) const;
    void runCommand(const std::string &command);
    Directory *cd_nonsave(const std::string &dirName);
    long int getPoints(Directory *_root);
    // load/save functions
    void saveProgram();
    void loadProgram();
    void saveDirectory(std::ofstream &outFile, const Directory *dir, bool isDir);
    void loadDirectory(std::ifstream &inFile, Directory *parentDir);
    // commands
    void recursive_ls(std::vector<std::unique_ptr<File>>::const_iterator iter, std::vector<std::unique_ptr<File>>::const_iterator end) const;
    void ls() const;
    void cd(const std::string &dirName);
    void cp(const std::string &destName, const std::string &source);
    void cp_directory(Directory *sourceDir, Directory *destDir);
    void mkdir(const std::string &dirName);
    void cat(const std::string &name) const;
    void rm(const std::string &name);
    void rmdir(const std::string &name);
    void link(const std::string &destName, const std::string &source);
    void touch(const std::string &dirName);
};

#endif