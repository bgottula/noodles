#include "all.h"

bool verbose = false;

void debug(const char *format, ...)
{
	if (verbose)
	{
		va_list va;
		va_start(va, format);
		vfprintf(stderr, format, va);
		va_end(va);
	}
}

char *StrPool::alloc(size_t len)
{
	char *str = new char[len];
	
	lock_guard<mutex> lock(m_mutex);
	m_strs.push_back(str);
	
	return str;
}

void StrPool::clear(void)
{
	lock_guard<mutex> lock(m_mutex);
	
	while (!m_strs.empty())
	{
		delete[] m_strs.front();
		m_strs.pop_front();
	}
}
