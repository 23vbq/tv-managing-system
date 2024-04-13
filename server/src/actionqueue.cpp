#include <actionqueue.h>

// Constructors

ActionQueue::ActionQueue(){
    m_queueSize = 0;
}

// Public functions

void ActionQueue::Handle(){
    if (!m_queueSize)
        return;
    //(m_queue.front())();
    // FIXME test
    syslog(LOG_CRIT, "%i", *((int*)m_queue.front().obj));
    m_queue.pop_front();
    m_queueSize--;
}

void ActionQueue::Clear(){
    m_queue.clear();
    m_queueSize = 0;
}