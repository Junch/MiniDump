#pragma once

#ifdef MINIDUMP_EXPORTS
#define MINIDUMP_API __declspec(dllexport)
#else
#define MINIDUMP_API __declspec(dllimport)
#endif

MINIDUMP_API void initMiniDump();