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
    std::unique_ptr<Directory> root;
    std::vector<std::unique_ptr<Directory>> dirStack;
    // other functions
    std::vector<std::string> seperateCommand(const std::string &command) const;
    void runCommand(const std::string &command);
    // load/save functions
    void saveProgram();
    void loadProgram();
    // commands
    void recursive_ls(std::vector<std::unique_ptr<File>>::const_iterator iter, std::vector<std::unique_ptr<File>>::const_iterator end) const;
    void ls() const;
    void cd(const std::string &dirName);
    void cp(const std::string &destName, const std::string &source);
    void mkdir(const std::string &dirName);
    void cat(const std::string &name) const;
    void rm(const std::string &name);
    void rmdir(const std::string &name);
    void link(const std::string &destName, const std::string &source);
    void touch(const std::string &dirName);
};

#endif