#include "all.h"


int Noodle::pop(void)
{
	int sample = m_queue.front();
    m_queue.pop();
    return sample;
}
