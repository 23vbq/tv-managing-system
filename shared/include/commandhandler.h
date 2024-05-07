#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#define CMD_H_VALID 1
#define CMD_H_CMDNOTFOUND 0
#define CMD_H_ARGC (-1)
#define CMD_H_AUTHERR (-2)

#include "command.h"
#include "authmanager.h"
#include "serversocket.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <syslog.h>

using namespace std;

class CommandHandler
{
public:
    static const string CMD_VALID; // String to output if command was valid
    static const string CMD_BAD; // String to output if command was invalid
private:
    unordered_map<string, Command> m_commands; // FIXME should it be on tree? - root->command->param
    vector<string> m_cmdbuffer; // Arguments vector buffer
    string m_output; // Output buffer
    int m_current_sd; // Socket descriptor that requested command

    /**
     * Removes new line and carriage return from end
    */
    void RemoveNewLineEnd(string&);
    /**
     * Gets arguments from command and outputs them into `m_cmdbuffer`
     * @return Count of arguments
    */
    size_t GetArgs(const string&);
public:
    /**
     * Adds command to handle
     * @param name Command name that will be used to invoke
     * @param command Command struct
    */
    void AddCommand(string, const Command&);
    /**
     * Handles provided command. Finds command by name and executes with arguments
     * @param command Command string
     * @param currentSd Socket descriptor that requested command
     * @return Return code (CMD_H_)
    */
    int Handle(string, int);
    /**
     * @return Output of last handled command
    */
    string GetOutput();
    /**
     * @return Socket descriptor from last handle
    */
    int GetCurrentSd();
};

#endif