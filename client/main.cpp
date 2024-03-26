#include "logger.h"
#include "clientsocket_win.h"
#include "serializer.h"
#include "endpointsettings.h"
#include "windowmanager.h"

#include <iostream>
#include <vector>

// using namespace std;

Logger* m_log;
ClientSocket* m_clientsock;
WindowManager* m_WindowManager;

void CleanUp();

int main(int argc, char* argv[]){
    // Initialize logger
    try{
        m_log = new Logger("./test/log");
    } catch (const char* ex){
        std::cerr<<"[Exception] "<<ex<<'\n';
        CleanUp();
        //return 1;
    }
    // FIXME test
    /*cout<<"TEST";
    _sleep(1000);*/
    // Window
    // FIXME test
    m_WindowManager = new WindowManager();
    while (true){
        if (!m_WindowManager->ProcessMessage())
            break;
        Sleep(10);
    }
    // Initialize ClientSocket
    /*m_clientsock = new ClientSocket();
    m_clientsock->Connect("192.168.121.132", "5555");
    // FIXME test
    std::string testbuff;
    std::vector<EndpointSettings> eps;
    m_clientsock->Receive(testbuff);
    std::cout<<testbuff<<'\n';
    m_clientsock->Send("GETEPLS");
    m_clientsock->Receive(testbuff);
    Serializer sr(testbuff);
    size_t n = sr.DeserializeNext<size_t>();
    for (int i = 0; i < n; i++){
        m_clientsock->Send("GETEPSET " + sr.DeserializeNext());
        m_clientsock->Receive(testbuff);
        Serializer esr(testbuff);
        EndpointSettings epbuff{esr.DeserializeNext(), esr.DeserializeNext<bool>(), esr.DeserializeNext(), esr.DeserializeNext<unsigned int>()};
        eps.push_back(epbuff);
    }
    eps[1].dir = "/mnt/test/bardo";
    Serializer sr2;
    sr2.AddValue(eps[1].name);
    sr2.AddValue<bool>(eps[1].localcfg);
    sr2.AddValue(eps[1].dir);
    sr2.AddValue<unsigned int>(eps[1].showtime);
    m_clientsock->Send("SETEPSET " + eps[1].name + " " + sr2.Serialize());
    m_clientsock->Receive(testbuff);
    //EndpointSettings eps{sr.DeserializeNext(), sr.DeserializeNext<bool>(), sr.DeserializeNext(), sr.DeserializeNext<unsigned int>()};
    std::cout<<testbuff<<'\n';*/
    // Cleanup
    CleanUp();
    return 0;
}

void CleanUp(){
    delete m_WindowManager;
    delete m_clientsock;
    delete m_log;
}