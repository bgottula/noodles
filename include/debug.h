#ifndef DEBUG_H
#define DEBUG_H

#define AT_RST  "\e[0m"  // reset all attributes
#define AT_BLD  "\e[1m"  // bold
#define AT_ULI  "\e[4m"  // underline
#define AT_BLK  "\e[5m"  // blink
#define AT_REV  "\e[7m"  // reverse
#define AT_NBLD "\e[22m" // no bold
#define AT_NULI "\e[24m" // no underline
#define AT_NBLK "\e[25m" // no blink
#define AT_NREV "\e[27m" // no reverse

#define FG_BLK "\e[30m"
#define FG_RED "\e[31m"
#define FG_GRN "\e[32m"
#define FG_YLW "\e[33m"
#define FG_BLU "\e[34m"
#define FG_MGT "\e[35m"
#define FG_CYN "\e[36m"
#define FG_WHT "\e[37m"
#define FG_DEF "\e[39m"

#define BG_BLK "\e[40m"
#define BG_RED "\e[41m"
#define BG_GRN "\e[42m"
#define BG_YLW "\e[43m"
#define BG_BLU "\e[44m"
#define BG_MGT "\e[45m"
#define BG_CYN "\e[46m"
#define BG_WHT "\e[47m"
#define BG_DEF "\e[49m"



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
