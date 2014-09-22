#ifndef ENDPOINT_H
#define ENDPOINT_H

/* forward declaration so we can use Block * pointers */
class Block;

struct Endpoint
{
	Block *block;
	const char *port;
};

#endif
