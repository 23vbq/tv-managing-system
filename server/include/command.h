#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

struct Command
{
    size_t argc; // Arguments count
    void (*function)(std::vector<std::string>); // Function to execute

    // Command(size_t, void (*)(std::vector<std::string>));
};


#endif