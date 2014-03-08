// DumpTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "minidump.h"
#include <stdexcept>
#include <string>
#include <fstream>

void createCrash(){
	throw std::runtime_error("Create a crash");
}

int _tmain(int argc, _TCHAR* argv[])
{
	initMiniDump();

    createCrash();

	return 0;
}
