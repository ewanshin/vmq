#ifndef __lplog_import__
#define __lplog_import__

#if defined(_WIN32) || defined(_WIN64)
#ifdef NDEBUG
#pragma comment(lib, "logger.lib")
#else
#pragma comment(lib, "logger.lib")
#endif
#endif

#endif
