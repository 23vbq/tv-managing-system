#include "logger.h"
#include "clientsocket_win.h"

#include <iostream>

// using namespace std;

Logger* m_log;

int main(int argc, char* argv[]){
    // Initialize logger
    // Logger log("test/log");
    m_log = new Logger("test/log");
    // FIXME test
    /*cout<<"TEST";
    _sleep(1000);*/
    // Window
    // Initialize ClientSocket
    ClientSocket cs;
    cs.Connect();
    std::string testbuff;
    cs.Receive(testbuff);
    std::cout<<testbuff<<'\n';
    cs.Send("TEST kot 8");
    cs.Receive(testbuff);
    std::cout<<testbuff<<'\n';
    // Cleanup
    delete m_log;
    return 0;
}