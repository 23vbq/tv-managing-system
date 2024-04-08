#include "commandhandler.h"

// Static variables

const string CommandHandler::CMD_VALID = "OK\r\n";
const string CommandHandler::CMD_BAD = "BAD\r\n";

// Private functions

size_t CommandHandler::SplitCommand(vector<string>& cmdarray, string cmd){
    cmdarray.clear();
    // size_t ePos = cmd.find(' ');
    size_t ePos, fPos = 0;
    size_t n = 1;
    while ((ePos = cmd.find(' ', fPos)) != string::npos)
    {
        cmdarray.push_back(cmd.substr(fPos, ePos - fPos));
        fPos = ePos + 1;
        n++;
    }
    cmdarray.push_back(cmd.substr(fPos));
    return n;
}
void CommandHandler::RemoveNewLineEnd(string& command){
    size_t n = command.length();
    if (command.find('\n', n - 1) == string::npos)
        return;
    command = command.substr(0, n - 1);
}

// Public functions

void CommandHandler::AddCommand(string name, const Command& command){
    m_commands[name] = command;
}
bool CommandHandler::Handle(string command, int currentSd){
    RemoveNewLineEnd(command);
    size_t n = SplitCommand(m_cmdbuffer, command); // FIXME cannot handle if argument has spaces
    if (!n)
        return false;
    if (m_commands.count(m_cmdbuffer[0]) == 0)
        return false;
    m_current_sd = currentSd;
    Command *cmdPtr = &m_commands[m_cmdbuffer[0]];
    if (cmdPtr->argc != n - 1)
        return false;
    m_cmdbuffer.erase(m_cmdbuffer.begin());
    cmdPtr->function(m_cmdbuffer, m_output);
    return true;
    // Inaczej. Muszę brać tego stringa jako cmd. Rozbić go na string[], a następnie to handlować.
    // To jest bardzo do przemyślenia, czy jakimś drzewkiem, czy tak?
}
string CommandHandler::GetOutput(){
    return m_output;
}
int CommandHandler::GetCurrentSd(){
    return m_current_sd;
}