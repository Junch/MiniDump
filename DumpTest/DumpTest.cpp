// DumpTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "minidump.h"

int _tmain(int argc, _TCHAR* argv[])
{
	initMiniDump();
	_tsystem(_T("pause"));
	return 0;
}

