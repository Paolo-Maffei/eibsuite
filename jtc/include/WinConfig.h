// **********************************************************************
//
// Copyright (c) 2002
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#if defined(_MSC_VER) || defined(__BORLANDC__)
//
// This is for Visual C++ 6 and Borland C++ Builder
//
#   ifndef STRICT
#       define STRICT 1
#   endif

//
// Define _WINSOCKAPI_ to avoid including winsock.h.
//
#   define _WINSOCKAPI_
#   include <windows.h>
#   include <winsock2.h>

#   define SIZEOF_CHAR 1
#   define SIZEOF_SHORT 2
#   define SIZEOF_INT 4
#   define SIZEOF_LONG 4
#   define SIZEOF_LONG_LONG 8
#   define SIZEOF_FLOAT 4
#   define SIZEOF_DOUBLE 8
#   define SIZEOF_LONG_DOUBLE 0

#   define HAVE_IO_H
#   define HAVE_PROCESS_H
#   define HAVE_WINSOCK_H

//
// Disable IPv6 support on Windows by default.
// 
#   define HAVE_DISABLE_IPV6

#   ifdef HAVE_IOSTREAM
// If <iostream> is used, then <strstream> and <fstream> must also be
// used
#       define HAVE_SSTREAM
#       define HAVE_FSTREAM
#	define HAVE_IOMANIP
#       define HAVE_NEW
#   else
// If <sstream> is not used, then <strstrea.h> must be used, not
// <strstream.h>
#       define HAVE_STRSTREA_H
#   endif

#   define HAVE_STDDEF_H
#   define HAVE_STDLIB_H
#   define HAVE_STDIO_H
//[YGYG] aviod macro redifintion for other projects
#ifndef HAVE_STRING_H
#   define HAVE_STRING_H
#endif

#   define HAVE_SYS_STAT_H
#   define HAVE_SYS_TYPES_H
#   define HAVE_SYS_TIMEB_H

#   define HAVE__UNLINK
#   define HAVE__GETPID
#   define HAVE_WIN32_THREADS
#   define HAVE_EXCEPTION
#   define HAVE_WIN32_WCHAR_TYPE

#   if (defined(_MSC_VER) && _MSC_VER <= 1200) || defined(__BORLANDC__)
//     VC++ 6.0 or earlier does not support getaddrinfo(), getnameinfo()
//     or the sockaddr_storage structure.
#      define HAVE_NO_GETADDRINFO
#      define HAVE_NO_GETNAMEINFO
#      define HAVE_NO_SOCKADDR_STORAGE
#   endif

#   if !defined(_MSC_VER) || (_MSC_VER < 1310)
#       define LONGLONG_BUGS
#   endif

// TRANSPARENT macro conflicts with Messaging IDL
#   undef TRANSPARENT

#if defined(_MSC_VER)
//  "template-class specialization '...' is already instantiated"
#   pragma warning( disable : 4660 )
//  "C++ Exception Specificaton ignored"
#   pragma warning( disable : 4290 )
//  "'...' : inherits '...' via dominance"
#   pragma warning( disable : 4250 )
//  "'this' : used in base member initializer list"
#   pragma warning( disable : 4355 )
//  "'...' : identifier was truncated to '255' characters in the debug
//  information"
#   pragma warning( disable : 4786 )
//  warning C4996: 'strcat' was declared deprecated...since cccp will be removed shortly,
//  we'll diable this warning for now.
#   pragma warning( disable : 4996 )
//  Windows X64 warnings due to changing type sizes.  These need to be cleaned up.
#   pragma warning( disable : 4244 )
#   pragma warning( disable : 4267 )
#   define HAVE_VCPLUSPLUS_BUGS

#elif defined(__BORLANDC__)
#   pragma warn -8057
#   pragma warn -8004 // assigned value that is never used
#   pragma warn -8001 // Superfluous & with function
#   pragma warn -8065 // No prototype
#   pragma warn -8064 // No prototype
#   pragma warn -8071 // Loss of significant digits
#   pragma warn -8060 // Possible incorrect assignment
#   pragma warn -8012 // signed/unsigned comparisons
#   pragma warn -8066 // Unreachable code
#   pragma warn -8008 // Condition is always true
#   pragma warn -8072 // Suspicious pointer arithmetic
#   pragma warn -8084 // Suggest paranthenses to clarify precedence
#   pragma warn -8080 // '...' is declared but never used 
#   pragma warn -8027 // Functions containing (for|while) are not
                      // expanded inline
#   pragma warn -8026 // Bogus exception specification violation warnings.

//  Required by the Borland compiler to resolve ambiguity
//  problems.
#   define OB_ONLY_IN_TYPE_CONVERSION 

#endif

#   define SIZEOF_BOOL 1
#   define SIZEOF_WCHAR_T 2

//  Visual C++ 6 has typename, but we don't want it to be used.
//  typename is needed for Visual C++ 7.1 however.
#   if !defined(_MSC_VER) || (_MSC_VER < 1310)
#       define HAVE_NO_TYPENAME
#   endif
#   ifdef HAVE_IOSTREAM
#       define HAVE_STD_IOSTREAM
#   endif

#else

#   error Unknown compiler!

#endif

// Other configuration settings added by Makefile.mak files:
