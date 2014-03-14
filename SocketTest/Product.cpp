#include "Product.h"

int writen(SOCKET fd, const void *vptr, size_t n)
{
	size_t  nleft;
	int     nwritten;
	const char  *ptr = (const char *)vptr;
	nleft = n;
	while (nleft > 0) {
		if ((nwritten = send(fd, ptr, nleft, NULL)) == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}

		nleft -= nwritten;
		ptr += nwritten;
	}

	return n;
}
