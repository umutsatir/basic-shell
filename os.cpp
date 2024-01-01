#include "os.hpp"

#include <iostream>
#include <sstream>

OS::OS() : root(std::make_unique<Directory>("root"))
{
    loadProgam();
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
}