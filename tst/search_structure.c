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

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "bitmap.h"
#include "cmocka-wrapper.h"
#include "search_structure.h"

/***********************************************************************/

static int
sscmp(void const * a, void const * b)
{
	unsigned long av = (unsigned long)a,
	              bv = (unsigned long)b;

	if (av > bv)
		return 1;
	else if (av < bv)
		return -1;
	else
		return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static bool
find_next_existing(struct bitmap_guard const * g, unsigned long * k)
{
	unsigned long l = *k;

	while (!bitmap_guard_get(g, *k))
	{
		*k = (*k + 1 ) % TEST_ELEMENT_COUNT;

		if (*k == l)
			return true;
	}

	return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static bool
find_next_non_existing(struct bitmap_guard const * g, unsigned long * k)
{
	unsigned long l = *k;

	while (bitmap_guard_get(g, *k))
	{
		*k = (*k + 1 ) % TEST_ELEMENT_COUNT;

		if (*k == l)
			return true;
	}

	return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static unsigned long
count_entries(struct bitmap_guard const * g)
{
	assert(g);

	unsigned long rv = 0;

	for (unsigned long i = 0 ; i < TEST_ELEMENT_COUNT ; i++)
	{
		if (bitmap_guard_get(g, i))
			rv++;
	}

	return rv;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct iteratiron_assertion_arguments
{
	struct bitmap_guard copy;
	unsigned int count;
};

static int
_iteration_assertion(SEARCH_STRUCTURE_ELEMENT const * e, void * p)
{
	assert(e);
	assert(p);

	struct iteratiron_assertion_arguments * args = (struct iteratiron_assertion_arguments *)p;
	unsigned long i = (unsigned long)SEARCH_STRUCTURE_ELEMENT_VALUE(e);

	assert_true(bitmap_guard_clear(&args->copy, i));
	args->count++;

	return 0;
}

static void
assert_iteration_correctness(SEARCH_STRUCTURE const * ss, unsigned char * flags, size_t flags_length)
{
	assert(ss);
	assert(flags);
	assert(flags_length);

	unsigned char buf[flags_length];
	struct iteratiron_assertion_arguments args;

	args.count = 0;
	bitmap_guard_initialize(&args.copy, buf, sizeof(buf));
	memcpy(buf, flags, flags_length);

	assert_false(SEARCH_STRUCTURE_ITERATE(ss, _iteration_assertion, &args));
	assert_int_equal(args.count, SEARCH_STRUCTURE_GET_ENTRY_COUNT(ss));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static void
FT_random_operations()
{
	enum test_op
	{
		TEST_OP_INSERT,
		TEST_OP_INSERT_EXISTING,
		TEST_OP_REMOVE,
		TEST_OP_REMOVE_NONEXISTING,
		TEST_OP_REMOVE_ANY,
		TEST_OP_REMOVE_ANY_NONEXISTING,
		TEST_OP_FIND,
		TEST_OP_FIND_NONEXISTING,
		TEST_OP_FIND_ANY,
		TEST_OP_FIND_ANY_NONEXISTING,
		TEST_OP_ITERATE,
		TEST_OP_GET_ENTRY_COUNT,
		TEST_OP_IS_EMPTY,
		_TEST_OP_COUNT
	};
	SEARCH_STRUCTURE ss;
	SEARCH_STRUCTURE_ELEMENT sse[TEST_ELEMENT_COUNT], dummy;
	SEARCH_STRUCTURE_ELEMENT const * tmp;
	unsigned char flags[TEST_ELEMENT_COUNT/8 + 1], tm;
	struct bitmap_guard ssf;

	tm = clock();
	printf("Seeding with value %u.\n", tm);
	srand(tm);

	SEARCH_STRUCTURE_ELEMENT_INITIALIZE(&dummy);
	SEARCH_STRUCTURE_INITIALIZE(&ss, sscmp);
	bitmap_guard_initialize(&ssf, flags, sizeof(flags));

	for (unsigned long i = 0; i < TEST_ELEMENT_COUNT; ++i)
	{
		SEARCH_STRUCTURE_ELEMENT_INITIALIZE(&sse[i]);
		SEARCH_STRUCTURE_ELEMENT_DEFINE(&sse[i], (void*)i);
	}

	for (unsigned long i = 0, j = rand() % TEST_ELEMENT_COUNT;
			i < TEST_ITERATION_COUNT ;
			++i, j = rand() % TEST_ELEMENT_COUNT)
	{
		switch (rand() % _TEST_OP_COUNT)
		{
			case TEST_OP_INSERT:
				if (find_next_non_existing(&ssf, &j))
					break;
				assert_false(SEARCH_STRUCTURE_INSERT(&ss, &sse[j]));
				assert_false(bitmap_guard_set(&ssf, j));
				break;
			case TEST_OP_INSERT_EXISTING:
				if (find_next_existing(&ssf, &j))
					break;
				SEARCH_STRUCTURE_ELEMENT_DEFINE(&dummy, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[j]));
				assert_true(SEARCH_STRUCTURE_INSERT(&ss, &dummy));
				break;
			case TEST_OP_REMOVE:
				if (find_next_existing(&ssf, &j))
					break;
				tmp = SEARCH_STRUCTURE_REMOVE(&ss, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[j]));
				assert_non_null(tmp);
				assert_false(sscmp(SEARCH_STRUCTURE_ELEMENT_VALUE(tmp), SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[j])));
				assert_true(bitmap_guard_clear(&ssf, j));
				break;
			case TEST_OP_REMOVE_NONEXISTING:
				if (find_next_non_existing(&ssf, &j))
					break;
				assert_null(SEARCH_STRUCTURE_REMOVE(&ss, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[j])));
				break;
			case TEST_OP_REMOVE_ANY:
				if (find_next_existing(&ssf, &j))
					break;
				tmp = SEARCH_STRUCTURE_REMOVE_ANY(&ss);
				assert_non_null(tmp);
				for (j = 0 ; j < TEST_ELEMENT_COUNT ; j++)
				{
					if (!sscmp(SEARCH_STRUCTURE_ELEMENT_VALUE(tmp), SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[j])))
						break;
				}
				assert_true(j < TEST_ELEMENT_COUNT);
				assert_true(bitmap_guard_clear(&ssf, j));
				break;
			case TEST_OP_REMOVE_ANY_NONEXISTING:
				if (!find_next_existing(&ssf, &j))
					break;
				tmp = SEARCH_STRUCTURE_REMOVE_ANY(&ss);
				assert_null(tmp);
				break;
			case TEST_OP_FIND_ANY:
				if (find_next_existing(&ssf, &j))
					break;
				tmp = SEARCH_STRUCTURE_FIND_ANY(&ss);
				assert_non_null(tmp);
				for (j = 0 ; j < TEST_ELEMENT_COUNT ; j++)
				{
					if (!sscmp(SEARCH_STRUCTURE_ELEMENT_VALUE(tmp), SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[j])))
						break;
				}
				assert_true(j < TEST_ELEMENT_COUNT);
				break;
			case TEST_OP_FIND_ANY_NONEXISTING:
				if (!find_next_existing(&ssf, &j))
					break;
				tmp = SEARCH_STRUCTURE_FIND_ANY(&ss);
				assert_null(tmp);
				break;
			case TEST_OP_FIND:
				if (find_next_existing(&ssf, &j))
					break;
				tmp = SEARCH_STRUCTURE_FIND(&ss, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[j]));
				assert_false(sscmp(SEARCH_STRUCTURE_ELEMENT_VALUE(tmp), SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[j])));
				break;
			case TEST_OP_FIND_NONEXISTING:
				if (find_next_non_existing(&ssf, &j))
					break;
				assert_null(SEARCH_STRUCTURE_FIND(&ss, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[j])));
				break;
			case TEST_OP_ITERATE:
				assert_iteration_correctness(&ss, flags, sizeof(flags));
				break;
			case TEST_OP_GET_ENTRY_COUNT:
				assert_int_equal(count_entries(&ssf), SEARCH_STRUCTURE_GET_ENTRY_COUNT(&ss));
				break;
			case TEST_OP_IS_EMPTY:
				j = 0;
				if (find_next_existing(&ssf, &j))
					assert_true(SEARCH_STRUCTURE_IS_EMPTY(&ss));
				else
					assert_false(SEARCH_STRUCTURE_IS_EMPTY(&ss));
				break;
		}
	}
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(FT_random_operations)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
