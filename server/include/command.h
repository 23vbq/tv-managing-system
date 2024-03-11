#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

struct Command
{
    size_t argc; // Arguments count
    void (*function)(std::vector<std::string>, std::string& output); // Function to execute
};

#endif