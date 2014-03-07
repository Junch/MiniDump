// DumpTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "minidump.h"
#include <stdexcept>
#include "guard.h"

void createCrash(){
	throw std::runtime_error("Create a crash");
}

int _tmain(int argc, _TCHAR* argv[])
{
	initMiniDump();
    RAII::make_ScopeGuard([](){
        printf("hello world\n");
    });

    createCrash();

	return 0;
}
