/**
 * dsac -- Data Structures and Alorithms for C
 * Copyright (C) 2016-2017 Hemmo Nieminen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include "cmocka-wrapper.h"
#include "debug_flags.h"

enum constants
{
	FLAG_HOLDER_SIZE = 5000
};

struct flag_holder_lite
{
	DEBUG_FLAGS(FLAG_HOLDER_SIZE)
};

/***********************************************************************/

static void
UT_debug_flags_initialize()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));

	expect_assert_failure(debug_flags_initialize((struct flag_holder_lite *)NULL));
	debug_flags_initialize(h);

	test_free(h);
}

/***********************************************************************/

static void
UT_debug_flags_set()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));

	debug_flags_initialize(h);

	expect_assert_failure(debug_flags_get((struct flag_holder_lite *)NULL, 1));
	assert_false(debug_flags_get(h, 1));

	expect_assert_failure(debug_flags_get(h, sizeof(h->_debug_flags) * 8 + 1));

	test_free(h);
}

/***********************************************************************/

static void
UT_debug_flags_get()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));
	volatile int i;

	debug_flags_initialize(h);

	expect_assert_failure(debug_flags_set(h, sizeof(h->_debug_flags) * 8 + 1));
	expect_assert_failure(debug_flags_set((struct flag_holder_lite *)NULL, 1));

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		debug_flags_set(h, i);

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		if (i % 2)
			assert_false(debug_flags_get(h, i));
		else
			assert_true(debug_flags_get(h, i));
	}

	test_free(h);
}

/***********************************************************************/

static void
UT_debug_flags_clear()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));
	int i;

	debug_flags_initialize(h);

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		debug_flags_set(h, i);

	expect_assert_failure(debug_flags_clear((struct flag_holder_lite *)NULL, 1));
	expect_assert_failure(debug_flags_clear(h, sizeof(h->_debug_flags) * 8 + 1));

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		if (i % 2)
			assert_false(debug_flags_get(h, i));
		else
		{
			assert_true(debug_flags_get(h, i));
			debug_flags_clear(h, i);
		}
	}

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
		assert_false(debug_flags_get(h, i));

	test_free(h);
}

/***********************************************************************/

static void
UT_debug_flags_copy()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite)),
		*hh = test_malloc(sizeof(struct flag_holder_lite));
	int i;

	debug_flags_initialize(h);
	debug_flags_initialize(hh);

	expect_assert_failure(debug_flags_copy(h, (struct flag_holder_lite *)NULL));
	expect_assert_failure(debug_flags_copy((struct flag_holder_lite *)NULL, h));
	expect_assert_failure(debug_flags_copy((struct flag_holder_lite *)NULL, (struct flag_holder_lite *)NULL));

	assert_false(debug_flags_compare(h, hh));

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		debug_flags_set(h, i);

	assert_true(debug_flags_compare(h, hh));

	debug_flags_copy(hh, h);
	assert_false(debug_flags_compare(h, hh));

	test_free(h);
	test_free(hh);
}

/***********************************************************************/

static void
UT_debug_flags_compare()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite)),
		*hh = test_malloc(sizeof(struct flag_holder_lite));
	int i;

	debug_flags_initialize(h);
	debug_flags_initialize(hh);

	expect_assert_failure(debug_flags_compare(h, (struct flag_holder_lite *)NULL));
	expect_assert_failure(debug_flags_compare((struct flag_holder_lite *)NULL, h));
	expect_assert_failure(debug_flags_compare((struct flag_holder_lite *)NULL, (struct flag_holder_lite *)NULL));

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		assert_false(debug_flags_compare(h, hh));
		debug_flags_set(h, i);
		assert_true(debug_flags_compare(h, hh));
		debug_flags_set(hh, i);
		assert_false(debug_flags_compare(h, hh));
	}

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
	{
		assert_false(debug_flags_compare(h, hh));
		debug_flags_clear(h, i);
		assert_true(debug_flags_compare(h, hh));
		debug_flags_clear(hh, i);
		assert_false(debug_flags_compare(h, hh));
	}

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 8)
	{
		assert_false(debug_flags_compare(h, hh));
		debug_flags_set(h, i);
		assert_true(debug_flags_compare(h, hh));
		debug_flags_set(hh, i);
		assert_false(debug_flags_compare(h, hh));
	}

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 32)
	{
		assert_false(debug_flags_compare(h, hh));
		debug_flags_clear(h, i);
		assert_true(debug_flags_compare(h, hh));
		debug_flags_clear(hh, i);
		assert_false(debug_flags_compare(h, hh));
	}

	test_free(h);
	test_free(hh);
}

/***********************************************************************/

static void
UT_debug_flags_assert()
{

	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));
	volatile int i;

	debug_flags_initialize(h);

	expect_assert_failure(debug_flags_assert(h, sizeof(h->_debug_flags) * 8 + 1));
	expect_assert_failure(debug_flags_assert((struct flag_holder_lite *)NULL, 1));

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		debug_flags_set(h, i);

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		if (i % 2)
		{
			expect_assert_failure(debug_flags_assert(h, i));
		}
		else
			debug_flags_assert(h, i);
	}

	test_free(h);
}

/***********************************************************************/

static void
UT_debug_flags_assert_not()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));
	int i;

	debug_flags_initialize(h);

	expect_assert_failure(debug_flags_assert_not(h, sizeof(h->_debug_flags) * 8 + 1));
	expect_assert_failure(debug_flags_assert_not((struct flag_holder_lite *)NULL, 1));

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		debug_flags_set(h, i);

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		if (i % 2)
		{
			debug_flags_assert_not(h, i);
		}
		else
		{
			expect_assert_failure(debug_flags_assert_not(h, i));
		}
	}

	test_free(h);
}

/***********************************************************************/

static void
ensure_initialization_clears_bits()
{

	struct
	{
		DEBUG_FLAGS(1234)
	} f;
	uint8_t buf[sizeof(f)];

	memset(&f, (unsigned int)-1, sizeof(f));
	memset(&buf, 0, sizeof(buf));

	debug_flags_initialize(&f);

	assert_int_equal(memcmp(&f, buf, sizeof(buf)), 0);
}

/***********************************************************************/

#if !defined(NDEBUG) || defined(UNIT_TESTING)

#define CHECK_CLEAR(a, b)\
	assert_false(debug_flags_get((a), (b)))

#define CHECK_SET(a, b)\
	assert_true(debug_flags_get((a), (b)))

#else

#define CHECK_CLEAR(...)
#define CHECK_SET(...)

#endif

#define CSC(a,b)\
{\
	CHECK_CLEAR(a, b);\
	debug_flags_set((a), (b));\
	CHECK_SET(a, b);\
}

#define CSCLC(a, b)\
{\
	CSC(a, b);\
	debug_flags_clear((a), (b));\
	CHECK_CLEAR(a, b);\
}

static void
set_n_clear()
{
	struct foobar
	{
		DEBUG_FLAGS(1000)
	} f __attribute__((unused));
	int i;

	debug_flags_initialize(&f);

	CSCLC(&f, 1);
	CSCLC(&f, 1);
	CSCLC(&f, 1);
	CSCLC(&f, 2);
	CSCLC(&f, 2);
	CSCLC(&f, 2);
	CSCLC(&f, 3);
	CSCLC(&f, 3);
	CSCLC(&f, 3);

	for (i = 0; i < 1000; ++i)
		CSCLC(&f, i);

	for (i = 0; i < 1000; ++i)
	{
		CSCLC(&f, i);
		CSCLC(&f, i);

		CSC(&f, i);
		debug_flags_set(&f, i);
		CHECK_SET(&f, i);
		debug_flags_set(&f, i);
		CHECK_SET(&f, i);
	}

	for (i = 0; i < 1000; ++i)
		CHECK_SET(&f, i);

	for (i = 0; i < 1000; ++i)
	{
		debug_flags_clear(&f, i);
		CHECK_CLEAR(&f, i);
	}

	for (i = 0; i < 1000; ++i)
		CHECK_CLEAR(&f, i);

	for (i = 0; i < 1000; ++i)
		CSC(&f, i);

	debug_flags_initialize(&f);

	for (i = 0; i < 1000; ++i)
		CHECK_CLEAR(&f, i);
}

/***********************************************************************/

static void
size_check()
{
	struct foobar
	{
		uint32_t padding;
		DEBUG_FLAGS(32)
	} foobar;

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	assert_int_equal(sizeof(foobar), 8);
#else
	assert_int_equal(sizeof(foobar), 4);
#endif
}

/***********************************************************************/

DEBUG_FLAGS_ENUM(test_flag_enum,
	INIT = -1,
	RELEASED = 0,
	FOOBAR = 1,
	KEKE = 2
)

static void
enum_test()
{
#ifndef DEBUG_FLAGS_ENUM
#error DEBUG_FLAGS_ENUM should be defined.
#endif

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	assert_int_equal(INIT, -1);
	assert_int_equal(RELEASED, 0);
	assert_int_equal(FOOBAR, 1);
	assert_int_equal(KEKE, 2);
#else
	/** DEBUG_FLAGS_ENUM should expand to nothing. Compile error otherwise. */
	DEBUG_FLAGS_ENUM(kekek, INIT, FOOBAR)
	DEBUG_FLAGS_ENUM(kekek, INIT, FOOBAR)
	DEBUG_FLAGS_ENUM(kekek, INIT, FOOBAR)
	DEBUG_FLAGS_ENUM(kekek, INIT, FOOBAR)
#endif
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_debug_flags_initialize),
		cmocka_unit_test(UT_debug_flags_set),
		cmocka_unit_test(UT_debug_flags_get),
		cmocka_unit_test(UT_debug_flags_clear),
		cmocka_unit_test(UT_debug_flags_copy),
		cmocka_unit_test(UT_debug_flags_compare),
		cmocka_unit_test(UT_debug_flags_assert),
		cmocka_unit_test(UT_debug_flags_assert_not),
		cmocka_unit_test(ensure_initialization_clears_bits),
		cmocka_unit_test(set_n_clear),
		cmocka_unit_test(size_check),
		cmocka_unit_test(enum_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
