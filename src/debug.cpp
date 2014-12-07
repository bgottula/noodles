#include "all.h"

bool verbose = false;
mutex debug_mutex;

void debug(const char *format, ...)
{
	if (!verbose) return;
	
	const int buf_size = 4096;
	char *buf = new char[buf_size];
	
	va_list va;
	va_start(va, format);
	int result = vsnprintf(buf, buf_size, format, va);
	va_end(va);
	
	assert(result >= 0);
	assert(result < buf_size);
	
	/* only lock when we actually dump the string to stderr */
	{
		lock_guard<mutex> lock(debug_mutex);
		
		fputs(buf, stderr);
		fflush(stderr);
	}
	
	delete[] buf;
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
#elif defined(_MSC_VER)
const char *demangle(const char *mangled)
{
	const char *demangled = mangled;
	
	/* remove 'class ' prefix that MSVC puts on type_info::name strings */
	if (strncmp(mangled, "class ", 6) == 0)
	{
		demangled += 6;
	}
	
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

char *StringPool::alloc(size_t len)
{
	char *str = new char[len];
	
	lock_guard<mutex> lock(m_mutex);
	
	m_strs.push_back(str);
	return str;
}

void StringPool::clear(void)
{
	lock_guard<mutex> lock(m_mutex);
	
	while (!m_strs.empty())
	{
		delete[] m_strs.front();
		m_strs.pop_front();
	}
}
