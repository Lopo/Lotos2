#ifndef LOTOSPP_STRINGS_STRINGPRINTF_H
#define LOTOSPP_STRINGS_STRINGPRINTF_H

#include "System/compiler_specific.h"
#include <string>
#include <cstdarg>


namespace LotosPP::Strings {

// Return a C++ string given printf-like input.
std::string StringPrintf(const char* format, ...)
	PRINTF_FORMAT(1, 2) WARN_UNUSED_RESULT;
// Return a C++ string given vprintf-like input.
std::string StringPrintV(const char* format, va_list ap)
	PRINTF_FORMAT(1, 0) WARN_UNUSED_RESULT;

// Lower-level routine that takes a va_list and appends to a specified
// string.  All other routines are just convenience wrappers around it.
void StringAppendV(std::string* dst, const char* format, va_list ap)
	PRINTF_FORMAT(2, 0);

	}

#endif
