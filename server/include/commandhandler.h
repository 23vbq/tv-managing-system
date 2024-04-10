#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "command.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <syslog.h>

using namespace std;

class CommandHandler
{
public:
    static const string CMD_VALID;
    static const string CMD_BAD;
private:
    unordered_map<string, Command> m_commands; // FIXME should it be on tree? - root->command->param
    vector<string> m_cmdbuffer;
    string m_output;
    int m_current_sd;

    size_t SplitCommand(vector<string>&, string);
    void RemoveNewLineEnd(string&);
    size_t GetArgs(const string&);
public:

    void AddCommand(string, const Command&);
    bool Handle(string, int);
    string GetOutput();
    int GetCurrentSd();
};

#endif