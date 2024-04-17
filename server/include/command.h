#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

struct Command
{
    size_t argc; // Arguments count
    /**
     * Function to execute.
     * @param vector<string> passes arguments
     * @param output reference to output
    */
    void (*function)(std::vector<std::string>, std::string& output);
};

#endif