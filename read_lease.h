/* leases.h */
#ifndef _READ_LEASES_H_
#define _READ_LEASES_H_

struct lease {
	unsigned char chaddr[16];
	unsigned int yiaddr;
	unsigned int expires;
	char hostname[64];
};

#endif
