#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "command.h"

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class CommandHandler
{
private:
    unordered_map<string, Command> m_commands; // FIXME should it be on tree? - root->command->param
    vector<string> m_cmdbuffer;

    size_t SplitCommand(vector<string>&, string);
public:
    void AddCommand(string, Command&);
    bool Handle(string);
};

#endif