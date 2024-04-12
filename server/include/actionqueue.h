#ifndef ACTION_QUEUE_H
#define ACTION_QUEUE_H

#include <vector>

using namespace std;

class ActionQueue{
private:
    vector<void(*)()> m_queue;
    size_t m_queueSize;
public:
    ActionQueue();

    void Add(void(*)(), bool&);
    void Handle();
    void Clear();
};

#endif