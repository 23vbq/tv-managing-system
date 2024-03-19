#include "logger.h"
#include "clientsocket_win.h"
#include "serializer.h"
#include "endpointsettings.h"

#include <iostream>

// using namespace std;

Logger* m_log;
ClientSocket* m_clientsock;

int main(int argc, char* argv[]){
    // Initialize logger
    // Logger log("test/log");
    m_log = new Logger("test/log");
    // FIXME test
    /*cout<<"TEST";
    _sleep(1000);*/
    // Window
    // Initialize ClientSocket
    m_clientsock = new ClientSocket();
    m_clientsock->Connect("192.168.121.132", "5555");
    // FIXME test
    std::string testbuff;
    m_clientsock->Receive(testbuff);
    std::cout<<testbuff<<'\n';
    m_clientsock->Send("GETEPSET Testowytv");
    m_clientsock->Receive(testbuff);
    Serializer sr(testbuff);
    EndpointSettings eps{sr.DeserializeNext(), sr.DeserializeNext<bool>(), sr.DeserializeNext(), sr.DeserializeNext<unsigned int>()};
    std::cout<<testbuff<<'\n';
    // Cleanup
    delete m_clientsock;
    delete m_log;
    return 0;
}