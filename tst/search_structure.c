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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"
#include "search_structure.h"

enum test_op
{
	TEST_OP_INSERT,
	TEST_OP_INSERT_EXISTING,
	TEST_OP_REMOVE,
	TEST_OP_REMOVE_NONEXISTING,
	TEST_OP_FIND,
	TEST_OP_FIND_NONEXISTING,
	_TEST_OP_COUNT
};

static int
sscmp(const void *a, const void *b)
{
	unsigned long av = (unsigned long)a,
				  bv = (unsigned long)b;

	if (av > bv)
		return 1;
	if (av < bv)
		return -1;
	return 0;
}

static bool
find_next_existing(const struct bitmap_guard *g, unsigned long *k)
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

static bool
find_next_non_existing(const struct bitmap_guard *g, unsigned long *k)
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

int
main()
{
	SEARCH_STRUCTURE ss;
	SEARCH_STRUCTURE_ELEMENT sse[TEST_ELEMENT_COUNT], dummy;
	const SEARCH_STRUCTURE_ELEMENT *tmp;
	unsigned char flags[TEST_ELEMENT_COUNT/8 + 1];
	unsigned long i, j, k;
	struct bitmap_guard ssf;

	printf("Starting search structure test.\n");

	SEARCH_STRUCTURE_ELEMENT_INITIALIZE(&dummy);
	SEARCH_STRUCTURE_INITIALIZE(&ss, sscmp);
	bitmap_guard_initialize(&ssf, flags, sizeof(flags));

	for (i = 0; i < TEST_ELEMENT_COUNT; ++i)
	{
		SEARCH_STRUCTURE_ELEMENT_INITIALIZE(&sse[i]);
		SEARCH_STRUCTURE_ELEMENT_DEFINE(&sse[i], (void*)i);
	}

	for (i = 0; i < TEST_ITERATION_COUNT; ++i)
	{
		j = rand() % _TEST_OP_COUNT;
		k = rand() % TEST_ELEMENT_COUNT;

		switch (j)
		{
			case TEST_OP_INSERT:
				if (find_next_non_existing(&ssf, &k))
					break;

				if (SEARCH_STRUCTURE_INSERT(&ss, &sse[k]))
				{
					fprintf(stderr, "Failed to insert element #%lu into the search structure.\n", k);
					return EXIT_FAILURE;
				}

				printf("insert #%lu.\n", k);

				if (bitmap_guard_set(&ssf, k))
					abort();
				break;
			case TEST_OP_INSERT_EXISTING:
				if (find_next_existing(&ssf, &k))
					break;

				SEARCH_STRUCTURE_ELEMENT_DEFINE(&dummy, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[k]));

				if (!SEARCH_STRUCTURE_INSERT(&ss, &dummy))
				{
					fprintf(stderr, "Didn't expect to be able to re-insert an exising element #%lu.\n", k);
					return EXIT_FAILURE;
				}

				printf("insert-existing #%lu.\n", k);
				break;
			case TEST_OP_REMOVE:
				if (find_next_existing(&ssf, &k))
					break;

				if (!(tmp = SEARCH_STRUCTURE_REMOVE(&ss, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[k]))))
				{
					fprintf(stderr, "Failed to remove element #%lu from the search structure.\n", k);
					return EXIT_FAILURE;
				}

				if (sscmp(SEARCH_STRUCTURE_ELEMENT_VALUE(tmp), SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[k])))
				{
					fprintf(stderr, "Removed element doensn't match the reference element #%lu.\n", k);
					return EXIT_FAILURE;
				}

				printf("remove #%lu.\n", k);

				if (!bitmap_guard_clear(&ssf, k))
					abort();
				break;
			case TEST_OP_REMOVE_NONEXISTING:
				if (find_next_non_existing(&ssf, &k))
					break;

				if (SEARCH_STRUCTURE_REMOVE(&ss, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[k])))
				{
					fprintf(stderr, "Didn't expect remove to return anything for non-existing element #%lu.\n", k);
					return EXIT_FAILURE;
				}

				printf("remove-nonexisting #%lu.\n", k);
				break;
			case TEST_OP_FIND:
				if (find_next_existing(&ssf, &k))
					break;

				if (!(tmp = SEARCH_STRUCTURE_FIND(&ss, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[k]))))
				{
					fprintf(stderr, "Failed to find element #%lu from the search structure.\n", k);
					return EXIT_FAILURE;
				}

				if (sscmp(SEARCH_STRUCTURE_ELEMENT_VALUE(tmp), SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[k])))
				{
					fprintf(stderr, "Found element doensn't match the reference element #%lu.\n", k);
					return EXIT_FAILURE;
				}

				printf("find #%lu.\n", k);
				break;
			case TEST_OP_FIND_NONEXISTING:
				if (find_next_non_existing(&ssf, &k))
					break;

				if (SEARCH_STRUCTURE_FIND(&ss, SEARCH_STRUCTURE_ELEMENT_VALUE(&sse[k])))
				{
					fprintf(stderr, "Didn't expect to find element #%lu from the search structure.\n", k);
					return EXIT_FAILURE;
				}

				printf("find-nonexisting #%lu.\n", k);
				break;
		}
	}

	printf("Search structure test ended OK.\n");
	return EXIT_SUCCESS;
}
