/**
 * Copyright (C) 2016-2017 Hemmo Nieminen
 *
 * This file is part of dsac (Data Structures and Alorithms for C).
 *
 * dsac is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * dsac is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dsac. If not, see <http://www.gnu.org/licenses/>.
 */

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

#undef assert
static inline void silence_scan_build(long p) { if (!p) exit(1) ; }
#define assert(e) (mock_assert((long)(e), #e, __FILE__, __LINE__), silence_scan_build((long)(e)))

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
