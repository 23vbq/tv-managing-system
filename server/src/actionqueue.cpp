#include <actionqueue.h>

// Constructors

ActionQueue::ActionQueue(){
    m_queueSize = 0;
}

// Public functions

void ActionQueue::Add(void(*action)(), bool& highPriority){
    if (highPriority)
        m_queue.insert(m_queue.begin(), action);
    else
        m_queue.push_back(action);
    m_queueSize++;
}