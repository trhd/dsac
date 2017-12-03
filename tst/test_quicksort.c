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
#include <string.h>
#include "cmocka-wrapper.h"
#include "quicksort.h"

static long
cmp_long(long a, long b)
{
	if (a < b)
		return -1;
	else if (a > b)
		return 1;
	else
		return 0;
}

/***********************************************************************/

static void
FT_sort_10()
{
	static const long test_data_numbers_sorted[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	};

	long test_data_numbers_mixed[] =
	{
		3, 2, 8, 5, 1, 6, 7, 0, 4, 9
	};

	assert_int_equal(sizeof(long), sizeof(void *));
	assert_int_equal(sizeof(test_data_numbers_mixed), sizeof(test_data_numbers_sorted));

	quicksort((const void **)test_data_numbers_mixed,
			sizeof(test_data_numbers_mixed) / sizeof(long),
			(int(*)(const void *, const void *))cmp_long);

	assert_memory_equal(test_data_numbers_mixed,
			test_data_numbers_sorted,
			sizeof(test_data_numbers_mixed));
}

/***********************************************************************/

static void
FT_sort_20()
{
	static const long test_data_numbers_sorted[] =
	{
		0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9
	};

	long test_data_numbers_mixed[] =
	{
		3, 2, 8, 0, 4, 5, 1, 6, 7, 9,
		3, 1, 6, 2, 8, 7, 0, 4, 9, 5
	};

	assert_int_equal(sizeof(long), sizeof(void *));
	assert_int_equal(sizeof(test_data_numbers_mixed), sizeof(test_data_numbers_sorted));

	quicksort((const void **)test_data_numbers_mixed,
			sizeof(test_data_numbers_mixed) / sizeof(long),
			(int(*)(const void *, const void *))cmp_long);

	assert_memory_equal(test_data_numbers_mixed,
			test_data_numbers_sorted,
			sizeof(test_data_numbers_mixed));
}

/***********************************************************************/

static void
FT_sort_50()
{
	static const long test_data_numbers_sorted[] =
	{
		0,   1,  2,  3,  3,  3,  4,  5,  6,  7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
		40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	};

	long test_data_numbers_mixed[] =
	{
		30, 33, 17, 11, 14, 18, 19,  2,  8,  9,
		0,  3, 43, 45, 41, 36, 37, 31, 48, 49,
		15, 16, 23, 24, 25,  1,  5,  6, 28, 29,
		40, 42, 34, 32, 38, 12, 13, 10, 35, 39, 3,
		20, 21, 22, 26, 27, 46, 47, 44,  4,  7,
		3,
	};

	assert_int_equal(sizeof(long), sizeof(void *));
	assert_int_equal(sizeof(test_data_numbers_mixed), sizeof(test_data_numbers_sorted));

	quicksort((const void **)test_data_numbers_mixed,
			sizeof(test_data_numbers_mixed) / sizeof(long),
			(int(*)(const void *, const void *))cmp_long);

	assert_memory_equal(test_data_numbers_mixed,
			test_data_numbers_sorted,
			sizeof(test_data_numbers_mixed));
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(FT_sort_10),
		cmocka_unit_test(FT_sort_20),
		cmocka_unit_test(FT_sort_50)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
