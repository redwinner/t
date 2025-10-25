/*
   LD_PRELOAD library to make getifaddrs to use a virtual
   IP address as localaddress. Specified via the enviroment
   variable BIND_IF.

   Compile on Linux with:
   gcc -nostartfiles -fpic -shared dump_hk.c -o getifaddrs.so -ldl -D_GNU_SOURCE


   Example in bash to make inetd only listen to the localhost
   lo interface, thus disabling remote connections and only
   enable to/from localhost:

   BIND_IF="xgbe0" LD_PRELOAD=./getifaddrs.so ./getifs

   Example in bash to use your virtual IP as your outgoing
   sourceaddress for ircII:

   BIND_IF="eth0" LD_PRELOAD=./getifaddrs.so ./getifs 

   Note that you have to set up your servers virtual IP first.

   This program was made by redwinner

   redwinner.cn

   TODO: I would like to extend it to the accept calls too, like a
   general tcp-wrapper. Also like an junkbuster for web-banners.
*/
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef AF_PACKET
#define AF_PACKET 17
#endif

#include <dlfcn.h>
#include <errno.h>

int (*real_getifaddrs)(struct ifaddrs **ifap);

char *bind_if_env;
static char * me = "redwinner.cn";

#define LIBC_NAME	"libc.so.6" 

void _init (void)
{
	const char *err;
	void	*libc;

	libc = dlopen(LIBC_NAME, RTLD_NOW);
	fprintf (stdout, "hook (_init):\n");
	real_getifaddrs = dlsym (libc, "getifaddrs");
	if ((err = dlerror ()) != NULL) {
		fprintf (stderr, "dlsym (getifaddrs): %s\n", err);
	}

	if (bind_if_env = getenv ("BIND_IF")) {

	}
}

int
getifaddrs(struct ifaddrs **ifap)
{
	/* fprintf (stdout, "hook (getifaddrs):\n");*/
	int res = real_getifaddrs(ifap);
	int family, s;
	char host[NI_MAXHOST];

	/* Walk through linked list, maintaining head pointer so we
		can free list later. */

	for (struct ifaddrs *ifa = *ifap; ifa != NULL;
			ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		/* Display interface name and family (including symbolic
			form of the latter for the common families). */

/*		printf("%-8s %s (%d)\n",
				ifa->ifa_name,
				(family == AF_PACKET) ? "AF_PACKET" :
				(family == AF_INET) ? "AF_INET" :
				(family == AF_INET6) ? "AF_INET6" : "???",
				family); */

		/* For an AF_INET* interface address, display the address. */

		if (family == AF_INET || family == AF_INET6) {
			if (!strcmp(bind_if_env, ifa->ifa_name)) {

/*				s = getnameinfo(ifa->ifa_addr,
						(family == AF_INET) ? sizeof(struct sockaddr_in) :
												sizeof(struct sockaddr_in6),
						host, NI_MAXHOST,
						NULL, 0, NI_NUMERICHOST);
				if (s != 0) {
					printf("getnameinfo() failed: %s\n", gai_strerror(s));
				}

				printf("\t\taddress: <%s>\n", host);*/
			} else {
				/*mark with 0*/
				ifa->ifa_addr->sa_family = 0;
			}
		}
	}

	return res;
}

int redwinner_hook_main(int argc, char *argv[])
{
	struct ifaddrs *ifaddr;
	int family, s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	/* Walk through linked list, maintaining head pointer so we
		can free list later. */

	for (struct ifaddrs *ifa = ifaddr; ifa != NULL;
			ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		/* Display interface name and family (including symbolic
			form of the latter for the common families). */

		printf("%-8s %s (%d)\n",
				ifa->ifa_name,
				(family == AF_PACKET) ? "AF_PACKET" :
				(family == AF_INET) ? "AF_INET" :
				(family == AF_INET6) ? "AF_INET6" : "???",
				family);

		/* For an AF_INET* interface address, display the address. */

		if (family == AF_INET || family == AF_INET6) {
			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ? sizeof(struct sockaddr_in) :
											sizeof(struct sockaddr_in6),
					host, NI_MAXHOST,
					NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				exit(EXIT_FAILURE);
			}

			printf("\t\taddress: <%s>\n", host);

		} else if (family == AF_PACKET && ifa->ifa_data != NULL) {
		}
	}

	freeifaddrs(ifaddr);
	exit(EXIT_SUCCESS);
}