#include "all.h"

void Noodle::push(int sample)
{
	assert(m_queue.size() <= m_max);
	m_queue.push_back(sample);
}

int Noodle::pop(void)
{
	int sample = m_queue.front();
	m_queue.pop_front();
	return sample;
}

int Noodle::peek(size_t where)
{
#warning TODO
	// TODO
}
