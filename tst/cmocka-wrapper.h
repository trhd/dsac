#pragma once

#ifndef UNIT_TESTING
#error Probably not what you were trying to do.
#endif

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <stdbool.h>

#ifdef UNIT_TESTING

#ifndef NDEBUG
#undef assert
#define assert(e) mock_assert((long)(e), #e, __FILE__, __LINE__)
#endif

#define abort() do { mock_assert(false, "abort()", __FILE__, __LINE__) ; exit(1) ; } while (false)

#undef malloc
#define malloc(size) _mock_malloc(size)

#undef realloc
#define realloc(ptr, size) _mock_realloc(ptr, size)

extern bool do_mock_malloc;
void * _mock_malloc(size_t);

extern bool do_mock_realloc;
void * _mock_realloc(void *, size_t);

int mock_reset_all(void**);

#define MOCK(function_to_mock, mock_function_return_value, test)\
	do_mock_##function_to_mock = true;\
	will_return(_mock_##function_to_mock, mock_function_return_value);\
	test;\
	do_mock_##function_to_mock = false;

#endif
