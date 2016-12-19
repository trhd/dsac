/**
 * Copyright (C) 2016 Hemmo Nieminen
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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "cmocka-wrapper.h"

static int ref_idx = 0;
static const unsigned char refs[][9] =
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x15, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x15, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x15, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x15, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80},
	{0x15, 0x05, 0x80, 0x00, 0xff, 0x00, 0x00, 0x00, 0x80},
	{0x15, 0x05, 0x80, 0x00, 0xff, 0x00, 0x00, 0x00, 0x80},
	{0x15, 0x05, 0x80, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x80},
	{0x15, 0x05, 0x80, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x80},
};

static void
compare_against_refs(const unsigned char buf[9])
{
	int i, j;

	assert((size_t)ref_idx <= sizeof(refs)/(sizeof(char)*9));

	assert_memory_equal(buf, refs[ref_idx], sizeof(refs[ref_idx]));

	for (i = 0 ; i < 9 ; ++i)
		for (j = 0; j < 8; ++j)
			assert_false(bitmap_get(buf, i * 8 + j) != (refs[ref_idx][i] & (1 << j)) >> j);

	ref_idx++;
}

/***********************************************************************/

static void
FT_basic_test()
{
	unsigned char buf[9];

	ref_idx = 0;
	bitmap_initialize(buf, sizeof(buf));

	compare_against_refs(buf);
	assert_false(bitmap_set(buf, 0 * 8 + 0));

	compare_against_refs(buf);
	assert_false(bitmap_set(buf, 0 * 8 + 2));

	compare_against_refs(buf);
	assert_false(bitmap_set(buf, 0 * 8 + 4));

	compare_against_refs(buf);
	assert_false(bitmap_set(buf, 1 * 8 + 0));

	compare_against_refs(buf);
	assert_false(bitmap_set(buf, 1 * 8 + 2));

	compare_against_refs(buf);
	assert_false(bitmap_set(buf, 2 * 8 + 7));

	compare_against_refs(buf);
	assert_false(bitmap_set(buf, 8 * 8 + 7));

	compare_against_refs(buf);
	assert_false(bitmap_set(buf, 4 * 8 + 0));
	assert_false(bitmap_set(buf, 4 * 8 + 1));
	assert_false(bitmap_set(buf, 4 * 8 + 2));
	assert_false(bitmap_set(buf, 4 * 8 + 3));
	assert_false(bitmap_set(buf, 4 * 8 + 4));
	assert_false(bitmap_set(buf, 4 * 8 + 5));
	assert_false(bitmap_set(buf, 4 * 8 + 6));
	assert_false(bitmap_set(buf, 4 * 8 + 7));

	compare_against_refs(buf);
	assert_true(bitmap_set(buf, 4 * 8 + 0));
	assert_true(bitmap_set(buf, 4 * 8 + 1));
	assert_true(bitmap_set(buf, 4 * 8 + 2));
	assert_true(bitmap_set(buf, 4 * 8 + 3));
	assert_true(bitmap_set(buf, 4 * 8 + 4));
	assert_true(bitmap_set(buf, 4 * 8 + 5));
	assert_true(bitmap_set(buf, 4 * 8 + 6));
	assert_true(bitmap_set(buf, 4 * 8 + 7));

	compare_against_refs(buf);
	assert_true(bitmap_clear(buf, 4 * 8 + 1));

	compare_against_refs(buf);
	assert_false(bitmap_clear(buf, 4 * 8 + 1));

	compare_against_refs(buf);
}

/***********************************************************************/

static void
FT_guard_test()
{
	unsigned char buf[9];
	struct bitmap_guard grd;

	ref_idx = 0;
	bitmap_guard_initialize(&grd, buf, sizeof(buf));

	compare_against_refs(buf);
	assert_false(bitmap_guard_set(&grd, 0 * 8 + 0));

	compare_against_refs(buf);
	assert_false(bitmap_guard_set(&grd, 0 * 8 + 2));

	compare_against_refs(buf);
	assert_false(bitmap_guard_set(&grd, 0 * 8 + 4));

	compare_against_refs(buf);
	assert_false(bitmap_guard_set(&grd, 1 * 8 + 0));

	compare_against_refs(buf);
	assert_false(bitmap_guard_set(&grd, 1 * 8 + 2));

	compare_against_refs(buf);
	assert_false(bitmap_guard_set(&grd, 2 * 8 + 7));

	compare_against_refs(buf);
	assert_false(bitmap_guard_set(&grd, 8 * 8 + 7));

	compare_against_refs(buf);
	assert_false(bitmap_guard_set(&grd, 4 * 8 + 0));
	assert_false(bitmap_guard_set(&grd, 4 * 8 + 1));
	assert_false(bitmap_guard_set(&grd, 4 * 8 + 2));
	assert_false(bitmap_guard_set(&grd, 4 * 8 + 3));
	assert_false(bitmap_guard_set(&grd, 4 * 8 + 4));
	assert_false(bitmap_guard_set(&grd, 4 * 8 + 5));
	assert_false(bitmap_guard_set(&grd, 4 * 8 + 6));
	assert_false(bitmap_guard_set(&grd, 4 * 8 + 7));

	compare_against_refs(buf);
	assert_true(bitmap_guard_set(&grd, 4 * 8 + 0));
	assert_true(bitmap_guard_set(&grd, 4 * 8 + 1));
	assert_true(bitmap_guard_set(&grd, 4 * 8 + 2));
	assert_true(bitmap_guard_set(&grd, 4 * 8 + 3));
	assert_true(bitmap_guard_set(&grd, 4 * 8 + 4));
	assert_true(bitmap_guard_set(&grd, 4 * 8 + 5));
	assert_true(bitmap_guard_set(&grd, 4 * 8 + 6));
	assert_true(bitmap_guard_set(&grd, 4 * 8 + 7));

	compare_against_refs(buf);
	assert_false(bitmap_guard_clear(&grd, 4 * 8 + 1) <= 0);

	compare_against_refs(buf);
	assert_false(bitmap_guard_clear(&grd, 4 * 8 + 1));

	compare_against_refs(buf);

	assert_false(bitmap_guard_get(&grd, 9 * 8) >= 0);
	assert_false(bitmap_guard_set(&grd, 9 * 8) >= 0);
	assert_false(bitmap_guard_clear(&grd, 9 * 8) >= 0);
}

/***********************************************************************/

static void
FT_basic_loop_test()
{
	unsigned char buf[10];
	unsigned int i;

	bitmap_initialize(buf, sizeof(buf));

	for (i = 0; i < sizeof(buf) * 8; ++i)
	{
		assert_false(bitmap_get(buf, i));
		assert_false(bitmap_set(buf, i));
		assert_true(bitmap_get(buf, i));
		assert_true(bitmap_set(buf, i));
		assert_true(bitmap_get(buf, i));
		assert_true(bitmap_clear(buf, i));
		assert_false(bitmap_get(buf, i));
		assert_false(bitmap_clear(buf, i));
		assert_false(bitmap_get(buf, i));
	}
}

/***********************************************************************/

static void
FT_guard_loop_test()
{
	unsigned char buf[10];
	unsigned int i;
	struct bitmap_guard grd;

	bitmap_guard_initialize(&grd, buf, sizeof(buf));

	for (i = 0; i < sizeof(buf) * 8; ++i)
	{
		assert_false(bitmap_guard_get(&grd, i));
		assert_false(bitmap_guard_set(&grd, i));
		assert_true(bitmap_guard_get(&grd, i));
		assert_true(bitmap_guard_set(&grd, i));
		assert_true(bitmap_guard_get(&grd, i));
		assert_true(bitmap_guard_clear(&grd, i));
		assert_false(bitmap_guard_get(&grd, i));
		assert_false(bitmap_guard_clear(&grd, i));
		assert_false(bitmap_guard_get(&grd, i));
	}
}


/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(FT_basic_test),
		cmocka_unit_test(FT_basic_loop_test),
		cmocka_unit_test(FT_guard_test),
		cmocka_unit_test(FT_guard_loop_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
