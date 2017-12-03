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

#include <stdio.h>
#include "cmocka-wrapper.h"
#include "fifo.h"

/***********************************************************************/

static void
FT_basic_usage__1()
{
	enum { val_count = 1000 };
	unsigned int vals[val_count];
	fifo_t valbufs[val_count];
	fifo_meta_t meta;

	for (int i = 0; i < val_count; ++i)
	{
		vals[i] = i;
		valbufs[i].data = vals + i;
	}

	fifo_initialize(&meta);

	assert_true(fifo_empty(&meta));
	assert_int_equal(fifo_size(&meta), 0);

	for (int i = 0; i < val_count; ++i)
	{
		assert_false(fifo_push(&meta, valbufs + i));
		assert_int_equal(fifo_size(&meta), i + 1);
	}

	assert_false(fifo_empty(&meta));

	for (int i = 0; i < val_count; ++i)
	{
		for (int j = 0; j < 3; ++j)
			assert_int_equal(fifo_peek(&meta), valbufs + i);
		assert_ptr_equal(fifo_pop(&meta), valbufs + i);
		assert_int_equal(fifo_size(&meta), val_count - i - 1);
	}

	assert_true(fifo_empty(&meta));
	assert_null(fifo_peek(&meta));
	assert_null(fifo_pop(&meta));
}

/***********************************************************************/

static void
FT_basic_usage__2()
{
	unsigned int vals[] =
	{
		1, 1324, 1988432, 11, 13, 15, 165, 184, 151, 154, 168, 186, 198, 1654, 11, 1658, 16844, 121, 165, 184,
		2, 2324, 2988432, 21, 23, 25, 265, 284, 251, 254, 268, 286, 298, 2654, 21, 2658, 26844, 221, 265, 284,
		3, 3324, 3988432, 31, 33, 35, 365, 384, 351, 354, 368, 386, 398, 3654, 31, 3658, 36844, 321, 365, 384,
		4, 4324, 4988432, 41, 43, 45, 465, 484, 451, 454, 468, 486, 498, 4654, 41, 4658, 46844, 421, 465, 484,
		5, 5324, 5988432, 51, 53, 55, 565, 584, 551, 554, 568, 586, 598, 5654, 51, 5658, 56844, 521, 565, 584
	};
	enum { val_count = sizeof(vals) / sizeof(unsigned int) };
	fifo_t valbufs[val_count];
	fifo_meta_t meta;

	for (int i = 0; i < val_count; ++i)
	{
		vals[i] = i;
		valbufs[i].data = vals + i;
	}

	fifo_initialize(&meta);

	assert_true(fifo_empty(&meta));
	assert_int_equal(fifo_size(&meta), 0);

	for (int i = 0; i < val_count; ++i)
		assert_false(fifo_push(&meta, valbufs + i));

	assert_false(fifo_empty(&meta));
	assert_int_equal(fifo_size(&meta), val_count);

	for (int i = 0; i < val_count; ++i)
	{
		for (int j = 0; j < 3; ++j)
			assert_ptr_equal(fifo_peek(&meta), valbufs + i);

		assert_ptr_equal(fifo_pop(&meta), valbufs + i);
	}

	assert_true(fifo_empty(&meta));
	assert_int_equal(fifo_size(&meta), 0);
	assert_null(fifo_peek(&meta));
	assert_null(fifo_pop(&meta));
}

/***********************************************************************/

static void
FT_basic_usage__3()
{
	enum { val_count = 1000 };
	unsigned int vals[val_count];
	fifo_t valbufs[val_count];
	fifo_meta_t meta;

	for (int i = 0; i < val_count; ++i)
	{
		vals[i] = i;
		valbufs[i].data = vals + i;
	}

	fifo_initialize(&meta);

	for (int i = 0; i < val_count; ++i)
	{
		assert_true(fifo_empty(&meta));
		assert_int_equal(fifo_size(&meta), 0);

		assert_false(fifo_push(&meta, valbufs + i));
		assert_false(fifo_empty(&meta));
		assert_int_equal(fifo_size(&meta), 1);

		for (int j = 0; j < 3; ++j)
			assert_ptr_equal(fifo_peek(&meta), valbufs + i);

		assert_ptr_equal(fifo_pop(&meta), valbufs + i);

		assert_true(fifo_empty(&meta));
		assert_int_equal(fifo_size(&meta), 0);

		assert_null(fifo_peek(&meta));
		assert_null(fifo_pop(&meta));
	}
}

/***********************************************************************/

static void
FT_basic_usage__4()
{
	enum { val_count = 1000 };
	unsigned int vals[val_count], i, j;
	fifo_t valbufs[val_count];
	fifo_meta_t meta;

	for (i = 0; i < val_count; ++i)
	{
		vals[i] = i;
		valbufs[i].data = vals + i;
	}

	fifo_initialize(&meta);
	i = 0, j = 0;

	assert_true(fifo_empty(&meta));
	assert_int_equal(fifo_size(&meta), i - j);

	assert_false(fifo_push(&meta, valbufs + i++));
	assert_false(fifo_empty(&meta));
	assert_int_equal(fifo_size(&meta), i - j);

	assert_false(fifo_push(&meta, valbufs + i++));
	assert_ptr_equal(fifo_peek(&meta), valbufs + j);

	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);
	assert_false(fifo_push(&meta, valbufs + i++));

	assert_false(fifo_push(&meta, valbufs + i++));
	assert_false(fifo_push(&meta, valbufs + i++));
	assert_false(fifo_push(&meta, valbufs + i++));

	assert_ptr_equal(fifo_peek(&meta), valbufs + j);
	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);

	assert_false(fifo_push(&meta, valbufs + i++));
	assert_false(fifo_push(&meta, valbufs + i++));

	assert_ptr_equal(fifo_peek(&meta), valbufs + j);
	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);

	assert_ptr_equal(fifo_peek(&meta), valbufs + j);
	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);

	assert_ptr_equal(fifo_peek(&meta), valbufs + j);
	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);

	assert_false(fifo_push(&meta, valbufs + i++));
	assert_ptr_equal(fifo_peek(&meta), valbufs + j);
	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);

	assert_false(fifo_push(&meta, valbufs + i++));
	assert_ptr_equal(fifo_peek(&meta), valbufs + j);
	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);

	assert_ptr_equal(fifo_peek(&meta), valbufs + j);
	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);

	assert_ptr_equal(fifo_peek(&meta), valbufs + j);
	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);

	assert_ptr_equal(fifo_peek(&meta), valbufs + j);
	assert_ptr_equal(fifo_pop(&meta), valbufs + j++);

	assert_true(fifo_empty(&meta));
	assert_int_equal(fifo_size(&meta), 0);

	assert_null(fifo_peek(&meta));
	assert_null(fifo_pop(&meta));
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(FT_basic_usage__1),
		cmocka_unit_test(FT_basic_usage__2),
		cmocka_unit_test(FT_basic_usage__3),
		cmocka_unit_test(FT_basic_usage__4),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
