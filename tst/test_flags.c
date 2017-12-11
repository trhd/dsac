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

#include <stdbool.h>
#include "cmocka-wrapper.h"
#include "flags.h"

enum constants
{
	FLAG_HOLDER_SIZE = 5000
};

struct flag_holder
{
	unsigned int header;
	FLAGS(FLAG_HOLDER_SIZE);
	unsigned int footer;
};

struct flag_holder_lite
{
	FLAGS(FLAG_HOLDER_SIZE);
};

static void
initialize_header_and_footer(struct flag_holder *f)
{
	assert_non_null(f);

	f->header = 0xdeadbeef;
	f->footer = 0xcafebabe;
}

static void
assert_intact_header_and_footer(const struct flag_holder *f)
{
	assert_non_null(f);

	assert_int_equal(f->header, 0xdeadbeef);
	assert_int_equal(f->footer, 0xcafebabe);
}

static void
try_setting_an_unset_bit(struct flag_holder *f, size_t b)
{
	assert_non_null(f);

	assert_false(flags_get(f, b));
	flags_set(f, b);
	assert_true(flags_get(f, b));
}

static void
try_unsetting_a_set_bit(struct flag_holder *f, size_t b)
{
	assert_non_null(f);

	assert_true(flags_get(f, b));
	flags_clear(f, b);
	assert_false(flags_get(f, b));
}

static void
try_setting_and_unsetting_a_bit(struct flag_holder *f, size_t b)
{
	assert_non_null(f);

	try_setting_an_unset_bit(f, b);
	try_unsetting_a_set_bit(f, b);
}

static bool
holder_is_all_clear(const struct flag_holder *f)
{
	assert_non_null(f);

	int i;

	for (i = 0; i < FLAG_HOLDER_SIZE; ++i)
		if (flags_get(f, i))
			return false;
	return true;
}

void * null_ptr = NULL;

/***********************************************************************/

static void
UT_flags_initialize()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));

	expect_assert_failure(flags_initialize((struct flag_holder_lite *)null_ptr));
	flags_initialize(h);

	test_free(h);
}

/***********************************************************************/

static void
UT_flags_set()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));

	flags_initialize(h);

	expect_assert_failure(flags_get((struct flag_holder_lite *)null_ptr, 1));
	assert_false(flags_get(h, 1));

	expect_assert_failure(flags_get(h, sizeof(h->_flags) * 8 + 1));

	test_free(h);
}

/***********************************************************************/

static void
UT_flags_get()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));
	int i;

	flags_initialize(h);

	expect_assert_failure(flags_set(h, sizeof(h->_flags) * 8 + 1));
	expect_assert_failure(flags_set((struct flag_holder_lite *)null_ptr, 1));

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		flags_set(h, i);

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		if (i % 2)
			assert_false(flags_get(h, i));
		else
			assert_true(flags_get(h, i));
	}

	test_free(h);
}

/***********************************************************************/

static void
UT_flags_clear()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));
	int i;

	flags_initialize(h);

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		flags_set(h, i);

	expect_assert_failure(flags_clear((struct flag_holder_lite *)null_ptr, 1));
	expect_assert_failure(flags_clear(h, sizeof(h->_flags) * 8 + 1));

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		if (i % 2)
			assert_false(flags_get(h, i));
		else
		{
			assert_true(flags_get(h, i));
			flags_clear(h, i);
		}
	}

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
		assert_false(flags_get(h, i));

	test_free(h);
}

/***********************************************************************/

static void
UT_flags_copy()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite)),
		*hh = test_malloc(sizeof(struct flag_holder_lite));
	int i;

	flags_initialize(h);
	flags_initialize(hh);

	expect_assert_failure(flags_copy(h, (struct flag_holder_lite *)null_ptr));
	expect_assert_failure(flags_copy((struct flag_holder_lite *)null_ptr, h));
	expect_assert_failure(flags_copy((struct flag_holder_lite *)null_ptr, (struct flag_holder_lite *)null_ptr));

	assert_false(flags_compare(h, hh));

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		flags_set(h, i);

	assert_true(flags_compare(h, hh));

	flags_copy(hh, h);
	assert_false(flags_compare(h, hh));

	test_free(h);
	test_free(hh);
}

/***********************************************************************/

static void
UT_flags_compare()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite)),
		*hh = test_malloc(sizeof(struct flag_holder_lite));
	int i;

	flags_initialize(h);
	flags_initialize(hh);

	expect_assert_failure(flags_compare(h, (struct flag_holder_lite *)null_ptr));
	expect_assert_failure(flags_compare((struct flag_holder_lite *)null_ptr, h));
	expect_assert_failure(flags_compare((struct flag_holder_lite *)null_ptr, (struct flag_holder_lite *)null_ptr));

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		assert_false(flags_compare(h, hh));
		flags_set(h, i);
		assert_true(flags_compare(h, hh));
		flags_set(hh, i);
		assert_false(flags_compare(h, hh));
	}

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
	{
		assert_false(flags_compare(h, hh));
		flags_clear(h, i);
		assert_true(flags_compare(h, hh));
		flags_clear(hh, i);
		assert_false(flags_compare(h, hh));
	}

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 8)
	{
		assert_false(flags_compare(h, hh));
		flags_set(h, i);
		assert_true(flags_compare(h, hh));
		flags_set(hh, i);
		assert_false(flags_compare(h, hh));
	}

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 32)
	{
		assert_false(flags_compare(h, hh));
		flags_clear(h, i);
		assert_true(flags_compare(h, hh));
		flags_clear(hh, i);
		assert_false(flags_compare(h, hh));
	}

	test_free(h);
	test_free(hh);
}

/***********************************************************************/

static void
UT_flags_assert()
{

	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));
	volatile int i;

	flags_initialize(h);

	expect_assert_failure(flags_assert(h, sizeof(h->_flags) * 8 + 1));
	expect_assert_failure(flags_assert((struct flag_holder_lite *)null_ptr, 1));

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		flags_set(h, i);

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		if (i % 2)
		{
			expect_assert_failure(flags_assert(h, i));
		}
		else
			flags_assert(h, i);
	}

	test_free(h);
}

/***********************************************************************/

static void
UT_flags_assert_not()
{
	struct flag_holder_lite *h = test_malloc(sizeof(struct flag_holder_lite));
	int i;

	flags_initialize(h);

	expect_assert_failure(flags_assert_not(h, sizeof(h->_flags) * 8 + 1));
	expect_assert_failure(flags_assert_not((struct flag_holder_lite *)null_ptr, 1));

	for (i = 0; i < FLAG_HOLDER_SIZE; i += 2)
		flags_set(h, i);

	for (i = 0; i < FLAG_HOLDER_SIZE; i++)
	{
		if (i % 2)
		{
			flags_assert_not(h, i);
		}
		else
		{
			expect_assert_failure(flags_assert_not(h, i));
		}
	}

	test_free(h);
}

/***********************************************************************/

static void
initialization_clears_memory(void __attribute__((unused)) **state)
{
	struct flag_holder f;
	int i;

	initialize_header_and_footer(&f);
	flags_initialize(&f);

	assert_true(holder_is_all_clear(&f));

	for (i = 0; i < FLAG_HOLDER_SIZE; ++i)
		if (i % 2)
			try_setting_an_unset_bit(&f, i);

	assert_false(holder_is_all_clear(&f));

	flags_initialize(&f);

	assert_true(holder_is_all_clear(&f));
	assert_intact_header_and_footer(&f);
}

/***********************************************************************/

static void
setting_and_clearing_bits(void __attribute__((unused)) **state)
{
	struct flag_holder f;
	int i;

	initialize_header_and_footer(&f);
	flags_initialize(&f);

	try_setting_and_unsetting_a_bit(&f, 1);
	try_setting_and_unsetting_a_bit(&f, 1);
	try_setting_and_unsetting_a_bit(&f, 1);
	try_setting_and_unsetting_a_bit(&f, 2);
	try_setting_and_unsetting_a_bit(&f, 2);
	try_setting_and_unsetting_a_bit(&f, 2);
	try_setting_and_unsetting_a_bit(&f, 3);
	try_setting_and_unsetting_a_bit(&f, 3);
	try_setting_and_unsetting_a_bit(&f, 3);

	for (i = 0; i < FLAG_HOLDER_SIZE; ++i)
	{
		try_setting_and_unsetting_a_bit(&f, i);
		try_setting_and_unsetting_a_bit(&f, i);

		try_setting_an_unset_bit(&f, i);
		flags_set(&f, i);
		assert_true(flags_get(&f, i));
		flags_set(&f, i);
		assert_true(flags_get(&f, i));
	}

	for (i = 0; i < FLAG_HOLDER_SIZE; ++i)
		assert_true(flags_get(&f, i));

	for (i = 0; i < FLAG_HOLDER_SIZE; ++i)
		try_unsetting_a_set_bit(&f, i);

	for (i = 0; i < FLAG_HOLDER_SIZE; ++i)
		assert_false(flags_get(&f, i));

	for (i = 0; i < FLAG_HOLDER_SIZE; ++i)
		try_setting_an_unset_bit(&f, i);

	flags_initialize(&f);

	assert_true(holder_is_all_clear(&f));
	assert_intact_header_and_footer(&f);
}

/***********************************************************************/

static void
copying_a_whole_set_of_flags()
{
	struct flag_holder f, g, h;
	int i;

	flags_initialize(&f);
	flags_initialize(&g);
	flags_initialize(&h);
	initialize_header_and_footer(&f);
	initialize_header_and_footer(&g);
	initialize_header_and_footer(&h);

	for (i = 0; i < 1024; ++i)
	{
		if (i % 2)
			try_setting_and_unsetting_a_bit(&f, i);
		else
			try_setting_an_unset_bit(&f, i);

		assert_false(flags_get(&g, i));
		assert_false(flags_get(&h, i));
	}

	flags_copy(&g, &f);

	for (i = 0; i < 1024; ++i)
	{
		if (i % 2)
		{
			assert_false(flags_get(&f, i));
			assert_false(flags_get(&g, i));
		}
		else
		{
			assert_true(flags_get(&f, i));
			assert_true(flags_get(&g, i));
		}
		assert_false(flags_get(&h, i));
	}

	flags_copy(&g, &h);

	for (i = 0; i < 1024; ++i)
	{
		if (i % 2)
		{
			assert_false(flags_get(&f, i));
		}
		else
		{
			assert_true(flags_get(&f, i));
		}

		assert_false(flags_get(&g, i));
		assert_false(flags_get(&h, i));
	}

	assert_intact_header_and_footer(&f);
	assert_intact_header_and_footer(&g);
	assert_intact_header_and_footer(&h);
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_flags_initialize),
		cmocka_unit_test(UT_flags_set),
		cmocka_unit_test(UT_flags_get),
		cmocka_unit_test(UT_flags_clear),
		cmocka_unit_test(UT_flags_copy),
		cmocka_unit_test(UT_flags_compare),
		cmocka_unit_test(UT_flags_assert),
		cmocka_unit_test(UT_flags_assert_not),
		cmocka_unit_test(initialization_clears_memory),
		cmocka_unit_test(setting_and_clearing_bits),
		cmocka_unit_test(copying_a_whole_set_of_flags),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
