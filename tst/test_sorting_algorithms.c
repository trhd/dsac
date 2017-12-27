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

#include "cmocka-wrapper.h"
#include "mergesort.h"
#include "quicksort.h"

static bool valgrind = false;

/***********************************************************************/

static unsigned long _FT_sequential_random_data_sort__iterations = 100;
static unsigned long _FT_sequential_random_data_sort__entries = 100000;

static inline int
_FT_sequential_random_data_sort__compare(void const * a, void const * b)
{
	unsigned long aa = (unsigned long)a,
	              bb = (unsigned long)b;

	if (aa < bb)
		return -1;
	else if (aa > bb)
		return 1;
	else
		return 0;
}

static void
FT_sequential_random_data_sort()
{
	unsigned long data_mergesort[_FT_sequential_random_data_sort__entries],
	              data_quicksort[_FT_sequential_random_data_sort__entries];

	for (unsigned long iteration = 0 ; iteration < _FT_sequential_random_data_sort__iterations ; iteration += valgrind ? 10 : 1)
	{
		for (unsigned long i = 0 ; i < _FT_sequential_random_data_sort__entries ; i++)
			data_mergesort[i] = data_quicksort[i] = rand()%_FT_sequential_random_data_sort__entries;

		quicksort((void const **)data_quicksort, _FT_sequential_random_data_sort__entries, _FT_sequential_random_data_sort__compare);
		mergesort((void const **)data_mergesort, _FT_sequential_random_data_sort__entries, _FT_sequential_random_data_sort__compare);

		assert_int_equal(sizeof(data_mergesort), sizeof(data_quicksort));
		assert_memory_equal(data_mergesort, data_quicksort, sizeof(data_mergesort));
	}
}

/***********************************************************************/

int
main()
{
	char const * e = getenv("VALGRIND");
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(FT_sequential_random_data_sort),
	};

	valgrind = e && atoi(e) > 0;

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
