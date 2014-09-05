#ifndef DEBUG_H
#define DEBUG_H

extern bool verbose;

void debug(const char *format, ...) __attribute__((format(printf, 1, 2)));

#endif
