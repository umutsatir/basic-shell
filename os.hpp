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
    void saveProgram();
    void loadProgram();
    void runCommand(const std::string &command);
    std::vector<std::string> seperateCommand(const std::string &command) const;
};

#endif