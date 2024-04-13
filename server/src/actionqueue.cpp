#include <actionqueue.h>

// Constructors

ActionQueue::ActionQueue(){
    m_queueSize = 0;
}

// Public functions

void ActionQueue::Add(void(*action)(), bool& highPriority){
    if (highPriority)
        m_queue.push_front(action);
    else
        m_queue.push_back(action);
    m_queueSize++;
}

void ActionQueue::Handle(){
    if (!m_queueSize)
        return;
    (m_queue.front())();
    m_queue.pop_front();
    m_queueSize--;
}

void ActionQueue::Clear(){
    m_queue.clear();
    m_queueSize = 0;
}