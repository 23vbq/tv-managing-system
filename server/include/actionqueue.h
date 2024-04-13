#ifndef ACTION_QUEUE_H
#define ACTION_QUEUE_H

#include <deque>
#include <syslog.h>

using namespace std;

class ActionQueue{
public:
    struct ActionQueueObject{
        void* obj;
    };
private:
    deque<ActionQueueObject> m_queue;
    size_t m_queueSize;
public:
    ActionQueue();

    template <typename T>
    void Add(T* objectPtr, const bool& highPriority){ //, void(T::*&&action)()
        ActionQueueObject aqo{objectPtr};
        if (highPriority)
            m_queue.push_front(aqo);
        else
            m_queue.push_back(aqo);
        m_queueSize++;
    }
    void Handle();
    void Clear();
};

#endif