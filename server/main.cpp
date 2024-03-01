#include <syslog.h>
#include <chrono>
#include <thread>

const char* DAEMONNAME = "tv-managing-server";

using namespace std;

int main(int argc, char* argv[]){
    openlog(DAEMONNAME, LOG_CONS | LOG_PID, LOG_USER);
    syslog(LOG_INFO, "Starting server daemon");
    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
        syslog(LOG_NOTICE, "Test message");
    }
    syslog(LOG_INFO, "Daemon exited");
    return 0;
}