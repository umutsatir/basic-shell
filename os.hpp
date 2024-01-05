#define OS_HPP
#ifdef OS_HPP
#include "file.hpp"
#include "directory.hpp"

class OS : public File
{
public:
    OS();
    ~OS();
    void run();

private:
    std::unique_ptr<Directory> root;
    std::vector<std::string> seperateCommand(const std::string &command) const;
    void runCommand(const std::string &command);
    void saveProgram();
    void loadProgram();
    void recursive_ls(std::unique_ptr<File> root);
};

#endif