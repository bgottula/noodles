#include "all.h"

void QNoodle::push(int sample)
{
	assert(m_queue.size() <= m_max);
	m_queue.push_back(sample);
}

int QNoodle::pop(void)
{
	int sample = m_queue.front();
	m_queue.pop_front();
	return sample;
}

int QNoodle::peek(size_t where)
{
#warning TODO
	// TODO
}
