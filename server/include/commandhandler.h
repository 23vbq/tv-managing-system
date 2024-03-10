#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>
#include <unordered_map>

using namespace std;

class CommandHandler
{
private:
    unordered_map<string, void (*)()> m_commands; // FIXME should it be on tree? - root->command->param
public:
    void AddCommand(string, void (*)());
    void Handle(string);
};

#endif