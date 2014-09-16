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
