#ifdef WIN32 // WINDOWS

#ifdef JTC_EXPORTS
#define JTC_IMPORT_EXPORT __declspec(dllexport)
#else
#define JTC_IMPORT_EXPORT __declspec(dllimport)
#endif

#undef HAVE_JTC_NO_IOSTREAM
#define JTC_DLL 1
#define HAVE_IOSTREAM 1
#include "WinConfig.h"

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#else // linux
#define JTC_IMPORT_EXPORT
#include "LinuxConfig.h"

#endif
