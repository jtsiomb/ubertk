#include "utk_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "utk_common.h"

void utk_error(const char *fmt, ...)
{
	va_list arg_list;

	va_start(arg_list, fmt);
	vfprintf(stderr, fmt, arg_list);
	va_end(arg_list);

#ifdef NDEBUG
	exit(EXIT_FAILURE);
#endif
}
