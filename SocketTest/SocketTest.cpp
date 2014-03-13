#include <stdio.h>
#include "hippomocks.h"
#include <WinSock2.h>
#include <gtest\gtest.h>

#pragma comment(lib, "Ws2_32.lib")

TEST(SOCKET, socket){
	MockRepository mocks;
	mocks.ExpectCallFunc(socket).Return(100);
	
	SOCKET sock_id;
	sock_id = socket(AF_INET, SOCK_STREAM, 0);

	ASSERT_EQ(100, sock_id);
}
