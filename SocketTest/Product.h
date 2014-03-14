#pragma once
#ifndef PRODUCT_H_
#define PRODUCT_H_
#include <WinSock2.h>

int writen(SOCKET fd, const void *vptr, size_t n);

#endif