#ifndef DEBUG_H
#define DEBUG_H

extern bool verbose;

void debug(const char *format, ...) __attribute__((format(printf, 1, 2)));

class StrPool
{
public:
	char *alloc(size_t len);
	void clear(void);
	
private:
	mutex m_mutex;
	list<const char *> m_strs;
};

#endif
