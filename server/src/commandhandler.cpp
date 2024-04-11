#include "commandhandler.h"

// Static variables

const string CommandHandler::CMD_VALID = "OK\r\n";
const string CommandHandler::CMD_BAD = "BAD\r\n";

// Private functions

void CommandHandler::RemoveNewLineEnd(string& command){
    size_t n = command.length();
    if (command.find('\n', n - 1) == string::npos)
        return;
    command = command.substr(0, n - 1);
}

size_t CommandHandler::GetArgs(const string &command){
    m_cmdbuffer.clear();
    size_t sPos = 0, ePos, n = 0;
    while ((sPos = command.find('\2', sPos)) != string::npos){
        if ((ePos = command.find('\3', ++sPos)) == string::npos)
            break;
        if (sPos >= ePos)
            break;
        m_cmdbuffer.push_back(command.substr(sPos, ePos - sPos));
        sPos = ePos + 1;
        n++;
        syslog(LOG_DEBUG, "Command arg: %s", &(m_cmdbuffer.back())[0]);
    }
    return n;
}

// Public functions

void CommandHandler::AddCommand(string name, const Command& command){
    m_commands[name] = command;
}
bool CommandHandler::Handle(string command, int currentSd){
    RemoveNewLineEnd(command);
    // Getting command
    size_t eCmdPos = command.find(' ');
    /*if (eCmdPos == string::npos)
        return false;*/
    string cmdName = command.substr(0, eCmdPos);
    if (m_commands.count(cmdName) == 0)
        return false;
    Command *cmdPtr = &m_commands[cmdName];
    syslog(LOG_INFO, "Requested command '%s'", &cmdName[0]);
    // Getting arguments
    size_t argc = GetArgs(command.substr(eCmdPos + 1));
    if (cmdPtr->argc != argc)
        return false;
    m_current_sd = currentSd;
    cmdPtr->function(m_cmdbuffer, m_output);
    return true;
}
string CommandHandler::GetOutput(){
    return m_output;
}
int CommandHandler::GetCurrentSd(){
    return m_current_sd;
}