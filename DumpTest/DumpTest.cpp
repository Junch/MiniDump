// DumpTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "minidump.h"
#include <gmock\gmock.h>

TEST(minidump, hello){
	initMiniDump();
}
