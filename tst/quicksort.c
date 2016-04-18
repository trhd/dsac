/**
 * dsac -- Data Structures and Algorithms for C.
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
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

int
main()
{
	assert(sizeof(long) == sizeof(void *));

#if 1
	static const long test_data_numbers_sorted[] =
	{
		0,   1,  2,  3,  3,  3,  4,  5,  6,  7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
		40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	};

	static const long test_data_numbers_mixed[] =
	{
		30, 33, 17, 11, 14, 18, 19,  2,  8,  9,
		0,  3, 43, 45, 41, 36, 37, 31, 48, 49,
		15, 16, 23, 24, 25,  1,  5,  6, 28, 29,
		40, 42, 34, 32, 38, 12, 13, 10, 35, 39, 3,
		20, 21, 22, 26, 27, 46, 47, 44,  4,  7,
		3,
	};
#else
	static const long test_data_numbers_sorted[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	};

	static const long test_data_numbers_mixed[] =
	{
		3, 2, 8, 5, 1, 6, 7, 0, 4, 9
	};
#endif
	size_t i;
	long tmp[sizeof(test_data_numbers_mixed) / sizeof(long)];


	memcpy(tmp, test_data_numbers_mixed, sizeof(tmp));

	quicksort((const void **)tmp, sizeof(tmp) / sizeof(long), (int(*)(const void *, const void *))cmp_long);

	if (memcmp(tmp, test_data_numbers_sorted, sizeof(tmp)))
	{
		printf("%s FAILED.\n", __func__);

		printf("mixt: ");
		for (i = 0; i < sizeof(test_data_numbers_mixed) / sizeof(long); ++i)
			printf("%ld ", test_data_numbers_mixed[i]);
		printf("\n");

		printf("sort: ");
		for (i = 0; i < sizeof(test_data_numbers_sorted) / sizeof(long); ++i)
			printf("%ld ", test_data_numbers_sorted[i]);
		printf("\n");

		printf("mine: ");
		for (i = 0; i < sizeof(tmp) / sizeof(long); ++i)
			printf("%ld ", tmp[i]);
		printf("\n");

		return 1;
	}

	printf("%s SUCCEEDED.\n", __func__);

	return 0;
}
