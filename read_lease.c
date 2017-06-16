#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "read_lease.h"

#define ARGV_MAX_LEN 100
#define IPADDR_MAX_LEN 15

static void usage(void)
{
	fprintf(stderr, "Usage: readlease [-p leases-file-path]\n");
}

static void print_hex(unsigned char *data, int len)
{
	int mod = len % 16;
	int i = len / 16;
	int j;
	int k;

	for (j = 0; j < 16; j++) {
		printf("%02x ", j);
	}
	printf("\n");
	for (j = 0; j < 16 * 2 + 15; j++) {
		printf("-");
	}
	printf("\n");

	for (j = 0; j < i; j++) {
		for (k = 0; k < 16; k++) {
			printf("%02x ", data[j * 16 + k]);
		}
		printf("\n");
	}

	for (k = 0; k < mod; k++) {
		printf("%02x ", data[j * 16 + k]);
	}
	printf("\n");
}

static void print_ipaddr(unsigned int ipaddr)
{
	char dst[16] = {0};

	if (!(inet_ntop(AF_INET, (const void *)&ipaddr, dst, IPADDR_MAX_LEN))) {
		fprintf(stderr, "Unable convert the ip address\n");
		return;
	}
	printf("IP address: %s\n", dst);
}

static void print_macaddr(unsigned char *pmac)
{
	char dst[18] = {0};
	sprintf(dst, "%02X:%02X:%02X:%02X:%02X:%02X",\
		*pmac,*(pmac+1),*(pmac+2),*(pmac+3),*(pmac+4),*(pmac+5));
	printf("\nMac address: %s\n", dst);
}

static int check_options(int argc, char *argv[], char *user_opt)
{
	int ret_opt = -1;
	while ((ret_opt = getopt(argc, argv, "p:")) != -1) {
		switch (ret_opt) {
			case 'p':
				strncpy(user_opt, optarg, ARGV_MAX_LEN);
				break;
			default:
				break;
		}
	}

	if (strlen(user_opt)) {
		return 0;
	}
	return -1;
}

int main(int argc, char *argv[])
{
	struct lease tmp_lease;
	char user_opt[ARGV_MAX_LEN] = {0};
	FILE *fp;


	if (check_options(argc, argv, user_opt) == -1) {
		usage();
		exit(-1);
	}

	if (!(fp = fopen(user_opt, "r"))) {
		fprintf(stderr, "Unable open %s for reading\n", user_opt);
		exit(-1);
	}

	while (fread(&tmp_lease, sizeof tmp_lease, 1, fp) == 1) {
		print_macaddr(tmp_lease.chaddr);
		print_ipaddr(tmp_lease.yiaddr);
		printf("Host name %s\n", tmp_lease.hostname);
	}
	printf("\nFinish reading %s\n", user_opt);
	fclose(fp);
	return 0;
}
