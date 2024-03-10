#include "commandhandler.h"

void CommandHandler::AddCommand(string name, void(*function)()){
    m_commands[name] = function;
}
void CommandHandler::Handle(string name){
    if (m_commands.count(name) == 0)
        return;
    (m_commands[name])();
    // Inaczej. Muszę brać tego stringa jako cmd. Rozbić go na string[], a następnie to handlować.
    // To jest bardzo do przemyślenia, czy jakimś drzewkiem, czy tak?
}