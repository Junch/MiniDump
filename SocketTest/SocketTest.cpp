#include <stdio.h>
#include <WinSock2.h>

#include <gmock\gmock.h>
#include "hippomocks.h"

#include "Product.h"

#pragma comment(lib, "Ws2_32.lib")

TEST(SOCKET, socket){
	MockRepository mocks;
	mocks.ExpectCallFunc(socket).Return(100);
	
	SOCKET sock_id;
	sock_id = socket(AF_INET, SOCK_STREAM, 0);

	ASSERT_EQ(100, sock_id);
}

TEST(SOCKET, writen_200characters_successfully){
	char buff[1024];
	SOCKET fd = 123;
	size_t len = 150;

	MockRepository mocks;
	mocks.ExpectCallFunc(send).With(fd, buff, len, _).Return(100);
	mocks.ExpectCallFunc(send).With(fd, buff + 100, len-100, _).Return(len-100);

	int ret = writen(fd, buff, len);
	ASSERT_THAT(ret, testing::Eq(len));
}

TEST(SOCKET, writen_200characters_failing){
	MockRepository mocks;
	mocks.ExpectCallFunc(send).Return(100);
	mocks.ExpectCallFunc(send).Return(SOCKET_ERROR);

	char buff[1024];
	int ret = writen(1, buff, 200);
	ASSERT_THAT(ret, testing::Eq(SOCKET_ERROR));
}
