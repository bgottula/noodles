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

#if defined(__GNUC__)
const char *demangle(const char *mangled)
{
	static AutoFree strs;
	
	int status;
	
	char *demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
	assert(status == 0);
	assert(demangled != nullptr);
	
	/* keep track of the string so we can free() it later */
	strs.push(demangled);
	
	return demangled;
}
#endif

void AutoFree::push(void *p)
{
	lock_guard<mutex> lock(m_mutex);
	
	m_ptrs.push_back(p);
}

void AutoFree::clear(void)
{
	lock_guard<mutex> lock(m_mutex);
	
	while (!m_ptrs.empty())
	{
		free(m_ptrs.front());
		m_ptrs.pop_front();
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
