/**
 * dsac -- Data Structures and Alorithms for C
 * Copyright (C) 2016-2018 Hemmo Nieminen
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
#include "radix_tree.h"

/**
 * Some test data that can be used in tests...
 */
char * FIXED_STRINGS[][2] =
{
	{"foo", "bar1"},
	{"foob", "bar2"},
	{"fooc", "bar3"},
	{"food", "bar4"},
	{"foocc", "bar5"},

	/** insert to the end */
	{"1", "b11r"},
	{"111", "b1r2"},
	{"11111111", "b1r3"},

	/** insert into middle of an existing string */
	{"2222", "2ar1"},
	{"22", "2ar2"},

	/** Split/fork existing string. */
	{"3333", "bar10"},
	{"3344", "bar11"},

	/** new "root level sibling" */
	{"x", "x"}
};

static void
FIXED_STRINGS_assert_inserts(struct radix_tree * t)
{
	assert(t);

	size_t z = radix_tree_get_key_count(t);

	for (unsigned long i = 0 ; i < sizeof(FIXED_STRINGS) / sizeof(char *) / 2 ; i++)
	{
		assert_false(radix_tree_insert(t, FIXED_STRINGS[i][0], FIXED_STRINGS[i][1]));
		assert_int_equal(radix_tree_get_key_count(t), ++z);
	}
}

static void
FIXED_STRINGS_assert_lookups(struct radix_tree * t)
{
	assert(t);

	size_t const z = radix_tree_get_key_count(t);

	for (unsigned long i = 0 ; i < sizeof(FIXED_STRINGS) / sizeof(char *) / 2 ; i++)
	{
		assert_string_equal(radix_tree_find(t, FIXED_STRINGS[i][0]), FIXED_STRINGS[i][1]);
		assert_int_equal(radix_tree_get_key_count(t), z);
	}
}

static void
FIXED_STRINGS_assert_removals(struct radix_tree * t)
{
	assert(t);

	size_t z = radix_tree_get_key_count(t);

	for (unsigned long i = 0 ; i < sizeof(FIXED_STRINGS) / sizeof(char *) / 2 ; i++)
	{
		assert_string_equal(radix_tree_remove(t, FIXED_STRINGS[i][0]), FIXED_STRINGS[i][1]);
		assert_int_equal(radix_tree_get_key_count(t), --z);
	}
}

static bool do_mock_malloc_wrapper = false;

static inline void *
malloc_wrapper(size_t z)
{
	assert(z);

	if (do_mock_malloc_wrapper)
		return (void*)mock();

	void * rv = malloc(z);

#ifdef DEBUG_MEMORY_ALLOCATIONS
	printf("malloc: %p\n", rv);
#endif

	return rv;
}

static inline void
free_wrapper(void * p)
{
	assert(p);

#ifdef DEBUG_MEMORY_ALLOCATIONS
	printf("free: %p\n", p);
#endif

	free(p);
}

static bool do_mock_realloc_wrapper = false;

static inline void *
realloc_wrapper(void * p, size_t z)
{
	if (do_mock_realloc_wrapper)
		return (void*)mock();

	void * rv = realloc(p, z);

#ifdef DEBUG_MEMORY_ALLOCATIONS
	printf("realloc: %p - %p\n", p, rv);
#endif

	return rv;
}

/***********************************************************************/

static void
UT_radix_tree_initialize()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);
	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_initialize__NULL()
{
	expect_assert_failure(radix_tree_initialize(NULL, NULL, NULL));

	expect_assert_failure(radix_tree_initialize(malloc_wrapper, NULL, NULL));
	expect_assert_failure(radix_tree_initialize(NULL, free_wrapper, NULL));
	expect_assert_failure(radix_tree_initialize(NULL, NULL, realloc_wrapper));

	expect_assert_failure(radix_tree_initialize(malloc_wrapper, free_wrapper, NULL));
	expect_assert_failure(radix_tree_initialize(malloc_wrapper, NULL, realloc_wrapper));
	expect_assert_failure(radix_tree_initialize(NULL, free_wrapper, realloc_wrapper));
}

/***********************************************************************/

static void
UT_radix_tree_uninitialize()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);
	radix_tree_uninitialize(&t);
	assert_null(t);
}

/***********************************************************************/

static void
UT_radix_tree_uninitialize__NULL()
{
	expect_assert_failure(radix_tree_uninitialize(NULL));
}

/***********************************************************************/

static void
UT_radix_tree_insert()
{
	enum { node_count = 20, char_count = 20 };
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);
	char keys[node_count][char_count][2*char_count + 1] = {{{0}}},
	     vals[node_count][char_count][2*char_count + 1] = {{{0}}};

	for (int i = 0 ; i < node_count ; i++)
	{
		for (int j = 0 ; j < char_count ; j++)
		{
			for (int k = 0 ; k <= j ; k++)
			{
				strcat(keys[i][j], (char []){ 'a' + i, '\0' });
				strcat(vals[i][j], (char []){ 'A' + i, '\0' });

				if (k % 2)
				{
					strcat(keys[i][j], (char []){ 'a' + i, 'a' + i, '\0' });
					strcat(vals[i][j], (char []){ 'A' + i, 'A' + i, '\0' });
				}
			}
		}
	}

	int z = 0;
	for (int i = 0 ; i < node_count / 2 ; i++)
	{
		for (int j = 0 ; j < char_count / 2 ; j++)
		{
			/**
			 * First insert strings from first and last nodes and begin
			 * with shortest and longest strings...
			 */
			int tbi[4][2] =
			{
				{i,                     j},
				{i,                     char_count - 1 - j},
				{node_count - 1 - i,    j},
				{node_count - 1 - i,    char_count - 1 -j}
			};

			for (int k = 0 ; k < 4 ; k++)
			{
				assert_false(radix_tree_insert(t,
							keys[tbi[k][0]][tbi[k][1]], vals[tbi[k][0]][tbi[k][1]]));
				assert_int_equal(radix_tree_get_key_count(t), ++z);
				assert_string_equal(radix_tree_find(t,
							keys[tbi[k][0]][tbi[k][1]]), vals[tbi[k][0]][tbi[k][1]]);
			}
		}
	}

	/** Why not... */
	FIXED_STRINGS_assert_inserts(t);

	for (int i = 0 ; i < node_count ; i++)
	{
		for (int j = 0 ; j < char_count ; j++)
		{
			char const * v = radix_tree_find(t, keys[j][i]);
			assert_non_null(v);
			assert_string_equal(vals[j][i], v);
		}
	}
	FIXED_STRINGS_assert_lookups(t);

	radix_tree_uninitialize(&t);
	assert_null(t);
}

/***********************************************************************/
static void
UT_radix_tree_insert__ENOMEM()
{

	enum { node_count = 10 };
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	MOCK(malloc, NULL,
			assert_true(radix_tree_insert(t, "foozar", "bar"))
	);

	MOCK(malloc, test_malloc(100),
		MOCK(malloc, NULL,
			assert_true(radix_tree_insert(t, "foozar", "bar"))
		)
	);

	assert_false(radix_tree_insert(t, "foobar", "bar"));

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_insert__FIXED_STRINGS()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	FIXED_STRINGS_assert_inserts(t);
	FIXED_STRINGS_assert_lookups(t);

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_insert__double()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	assert_false(radix_tree_insert(t, "foo", "bar"));
	assert_true(radix_tree_insert(t, "foo", "bar"));
	assert_true(radix_tree_insert(t, "foo", "xxx"));

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_insert__NULL()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	expect_assert_failure(radix_tree_insert(NULL, NULL, NULL));

	expect_assert_failure(radix_tree_insert(t, "foo", NULL));
	expect_assert_failure(radix_tree_insert(NULL, "foo", "bar"));
	expect_assert_failure(radix_tree_insert(t, NULL, "bar"));

	expect_assert_failure(radix_tree_insert(t, NULL, NULL));
	expect_assert_failure(radix_tree_insert(NULL, "foo", NULL));
	expect_assert_failure(radix_tree_insert(NULL, NULL, "bar"));

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_find()
{
	enum { node_count = 30 };
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);
	char keys[node_count][2], vals[node_count][2];

	for (int i = 0 ; i < node_count; i++)
	{
		sprintf(keys[i], "%c", 'a' + i);
		sprintf(vals[i], "%c", 'A' + i);

		radix_tree_insert(t, keys[i], vals[i]);

		char const * v = radix_tree_find(t, keys[i]);
		assert_non_null(v);
		assert_string_equal(vals[i], v);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		char const * v = radix_tree_find(t, keys[i]);
		assert_non_null(v);
		assert_string_equal(vals[i], v);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		char const * v = radix_tree_find(t, keys[i]);
		assert_non_null(v);
		assert_string_equal(vals[i], v);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		if (i % 2)
			continue;

		char const * v = radix_tree_find(t, keys[i]);
		assert_non_null(v);
		assert_string_equal(vals[i], v);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		if (!(i % 2))
			continue;

		char const * v = radix_tree_find(t, keys[i]);
		assert_non_null(v);
		assert_string_equal(vals[i], v);
	}

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_find__find_any()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	assert_null(radix_tree_find(t, NULL));
	assert_false(radix_tree_insert(t, "foo", "barbaba"));
	assert_string_equal(radix_tree_find(t, NULL), "barbaba");
	assert_string_equal(radix_tree_find(t, NULL), "barbaba");
	assert_true(radix_tree_insert(t, "foo", "bar"));
	assert_true(radix_tree_insert(t, "foo", "xxx"));
	assert_string_equal(radix_tree_find(t, NULL), "barbaba");
	assert_string_equal(radix_tree_find(t, NULL), "barbaba");
	assert_string_equal(radix_tree_remove(t, NULL), "barbaba");
	assert_null(radix_tree_find(t, NULL));
	assert_false(radix_tree_insert(t, "foo1", "bar"));
	assert_false(radix_tree_insert(t, "foo2", "xxx"));
	assert_true(!strcmp(radix_tree_find(t, NULL), "bar") || !strcmp(radix_tree_find(t, NULL), "xxx"));
	assert_false(radix_tree_insert(t, "fooo", "bar"));
	assert_false(radix_tree_insert(t, "f", "bar"));
	assert_non_null(radix_tree_find(t, NULL));
	assert_non_null(radix_tree_find(t, NULL));
	assert_non_null(radix_tree_find(t, NULL));
	assert_non_null(radix_tree_find(t, NULL));
	assert_non_null(radix_tree_find(t, NULL));
	assert_non_null(radix_tree_remove(t, NULL));
	assert_non_null(radix_tree_find(t, NULL));
	assert_non_null(radix_tree_remove(t, NULL));
	assert_non_null(radix_tree_find(t, NULL));
	assert_non_null(radix_tree_remove(t, NULL));
	assert_non_null(radix_tree_find(t, NULL));
	assert_non_null(radix_tree_remove(t, NULL));
	assert_null(radix_tree_find(t, NULL));
	assert_null(radix_tree_remove(t, NULL));

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_find__NULL()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	expect_assert_failure(radix_tree_find(NULL, NULL));
	expect_assert_failure(radix_tree_find(NULL, "foo"));

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_remove__FIXED_STRINGS()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	FIXED_STRINGS_assert_inserts(t);
	FIXED_STRINGS_assert_removals(t);

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_remove__remove_any()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	FIXED_STRINGS_assert_inserts(t);

	for (unsigned long i = 0 ; i < sizeof(FIXED_STRINGS) / sizeof(char *) / 2 ; i++)
	{
		assert_int_equal(radix_tree_get_key_count(t), sizeof(FIXED_STRINGS) / sizeof(char *) / 2 - i);
		char * v = (char*)radix_tree_remove(t, NULL);
		assert_non_null(v);
		assert_int_equal(radix_tree_get_key_count(t), sizeof(FIXED_STRINGS) / sizeof(char *) / 2 - i - 1);
	}

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_remove__non_value_node()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	assert_false(radix_tree_insert(t, "3333", "foo"));
	assert_false(radix_tree_insert(t, "3344", "bar"));
	assert_null(radix_tree_remove(t, "33"));

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_remove__NULL()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	expect_assert_failure(radix_tree_remove(NULL, NULL));
	expect_assert_failure(radix_tree_remove(NULL, "jou"));

	radix_tree_uninitialize(&t);
}
/***********************************************************************/

static void
UT_radix_tree_get_key_count()
{

	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	assert_int_equal(radix_tree_get_key_count(t), 0);
	assert_false(radix_tree_insert(t, "1", "yksi"));
	assert_int_equal(radix_tree_get_key_count(t), 1);
	assert_false(radix_tree_insert(t, "2", "kaksi"));
	assert_int_equal(radix_tree_get_key_count(t), 2);
	assert_false(radix_tree_insert(t, "3", "kolme"));
	assert_int_equal(radix_tree_get_key_count(t), 3);
	assert_false(radix_tree_insert(t, "4", "neljä"));
	assert_int_equal(radix_tree_get_key_count(t), 4);
	assert_false(radix_tree_insert(t, "5", "viisi"));
	assert_false(radix_tree_insert(t, "6", "kuusi"));
	assert_int_equal(radix_tree_get_key_count(t), 6);
	assert_false(radix_tree_insert(t, "7", "seitsemän"));
	assert_int_equal(radix_tree_get_key_count(t), 7);
	assert_false(radix_tree_insert(t, "8", "kahdeksan"));
	assert_int_equal(radix_tree_get_key_count(t), 8);
	assert_false(radix_tree_insert(t, "9", "yhdeksän"));
	assert_int_equal(radix_tree_get_key_count(t), 9);
	assert_false(radix_tree_insert(t, "10", "kymmenen"));

	assert_int_equal(radix_tree_get_key_count(t), 10);

	assert_string_equal(radix_tree_remove(t, "1"), "yksi");
	assert_int_equal(radix_tree_get_key_count(t), 9);
	assert_string_equal(radix_tree_remove(t, "2"), "kaksi");
	assert_int_equal(radix_tree_get_key_count(t), 8);
	assert_string_equal(radix_tree_remove(t, "3"), "kolme");
	assert_int_equal(radix_tree_get_key_count(t), 7);
	assert_string_equal(radix_tree_remove(t, "4"), "neljä");
	assert_int_equal(radix_tree_get_key_count(t), 6);
	assert_string_equal(radix_tree_remove(t, "5"), "viisi");
	assert_string_equal(radix_tree_remove(t, "6"), "kuusi");
	assert_int_equal(radix_tree_get_key_count(t), 4);
	assert_string_equal(radix_tree_remove(t, "8"), "kahdeksan");
	assert_int_equal(radix_tree_get_key_count(t), 3);
	assert_string_equal(radix_tree_remove(t, "9"), "yhdeksän");
	assert_int_equal(radix_tree_get_key_count(t), 2);
	assert_string_equal(radix_tree_remove(t, "10"), "kymmenen");
	assert_int_equal(radix_tree_get_key_count(t), 1);
	assert_string_equal(radix_tree_remove(t, "7"), "seitsemän");
	assert_int_equal(radix_tree_get_key_count(t), 0);

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_get_key_count__NULL()
{
	expect_assert_failure(radix_tree_get_key_count(NULL));
}

/***********************************************************************/

static int
_UT_radix_tree_iterate__helper(char const * k, void * v, void * a)
{
	assert(k);
	assert(v);

	struct radix_tree * t = (struct radix_tree *)a;

	return radix_tree_insert(t, k, v);
}

static void
UT_radix_tree_iterate()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper),
	                  * u = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	assert_non_null(t);
	assert_non_null(u);

	FIXED_STRINGS_assert_inserts(t);
	assert_false(radix_tree_iterate(t, _UT_radix_tree_iterate__helper, u));
	assert_int_equal(radix_tree_get_key_count(t), radix_tree_get_key_count(u));
	FIXED_STRINGS_assert_lookups(u);

	radix_tree_uninitialize(&t);
	radix_tree_uninitialize(&u);
}

/***********************************************************************/

static int
_UT_radix_tree_iterate__error__helper(char const * k, void * v, void * a)
{
	assert(k);
	assert(v);

	int * i = (int *)a;

	if (++*i == 3)
		return 0xdecafe;
	else
		return 0;
}

static void
UT_radix_tree_iterate__error()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);
	int i = 0;

	assert_non_null(t);

	FIXED_STRINGS_assert_inserts(t);
	assert_int_equal(radix_tree_iterate(t, _UT_radix_tree_iterate__error__helper, &i), 0xdecafe);

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static int
_UT_radix_tree_iterate__NULL__helper(char const * k, void * v __attribute__((unused)), void * a __attribute__((unused)))
{
	assert(k);
	return 0;
}

static void
UT_radix_tree_iterate__NULL()
{
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	assert_false(radix_tree_insert(t, "jou", "maan"));

	expect_assert_failure(radix_tree_iterate(NULL, NULL, NULL));
	expect_assert_failure(radix_tree_iterate(t, NULL, NULL));
	expect_assert_failure(radix_tree_iterate(NULL, _UT_radix_tree_iterate__NULL__helper, NULL));

	expect_assert_failure(radix_tree_iterate(NULL, NULL, "foo"));
	expect_assert_failure(radix_tree_iterate(t, NULL, "foo"));
	expect_assert_failure(radix_tree_iterate(NULL, _UT_radix_tree_iterate__NULL__helper, "foo"));

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
FT_basic_usage()
{
	enum { node_count = 10 };
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	assert_false(radix_tree_insert(t, "f", "bar"));
	assert_false(radix_tree_insert(t, "foo", "bar2"));
	assert_false(radix_tree_insert(t, "foobar", "bar3"));
	assert_false(radix_tree_insert(t, "foozar", "bar4"));
	assert_false(radix_tree_insert(t, "fo", "bar5"));

	assert_int_equal(radix_tree_get_key_count(t), 5);

	assert_string_equal(radix_tree_find(t, "f"), "bar");
	assert_string_equal(radix_tree_find(t, "fo"), "bar5");
	assert_string_equal(radix_tree_find(t, "foobar"), "bar3");
	assert_string_equal(radix_tree_find(t, "foozar"), "bar4");
	assert_string_equal(radix_tree_find(t, "foo"), "bar2");

	assert_string_equal(radix_tree_remove(t, "f"), "bar");
	assert_string_equal(radix_tree_remove(t, "fo"), "bar5");
	assert_string_equal(radix_tree_remove(t, "foobar"), "bar3");
	assert_string_equal(radix_tree_remove(t, "foozar"), "bar4");
	assert_string_equal(radix_tree_remove(t, "foo"), "bar2");

	assert_int_equal(radix_tree_get_key_count(t), 0);

	assert_null(radix_tree_find(t, "f"));
	assert_null(radix_tree_find(t, "fo"));
	assert_null(radix_tree_find(t, "foobar"));
	assert_null(radix_tree_find(t, "foozar"));
	assert_null(radix_tree_find(t, "foo"));

	assert_false(radix_tree_insert(t, "foobar", "bar3"));
	assert_string_equal(radix_tree_find(t, "foobar"), "bar3");
	assert_false(radix_tree_insert(t, "foozar", "bar4"));
	assert_string_equal(radix_tree_find(t, "foozar"), "bar4");

	assert_int_equal(radix_tree_get_key_count(t), 2);

	assert_null(radix_tree_find(t, "f"));
	assert_null(radix_tree_find(t, "fo"));
	assert_string_equal(radix_tree_remove(t, "foobar"), "bar3");
	assert_null(radix_tree_find(t, "foo"));

	assert_int_equal(radix_tree_get_key_count(t), 1);

	assert_string_equal(radix_tree_remove(t, NULL), "bar4");
	assert_int_equal(radix_tree_get_key_count(t), 0);

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_dump()
{
	/**
	 * FIXME: make a better test
	 */
	struct radix_tree * t = radix_tree_initialize(malloc_wrapper, free_wrapper, realloc_wrapper);

	FIXED_STRINGS_assert_inserts(t);

	radix_tree_dump(t);

	radix_tree_uninitialize(&t);
}

/***********************************************************************/

static void
UT_radix_tree_dump__NULL()
{
	expect_assert_failure(radix_tree_dump(NULL));
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_radix_tree_initialize),
		cmocka_unit_test(UT_radix_tree_initialize__NULL),

		cmocka_unit_test(UT_radix_tree_uninitialize),
		cmocka_unit_test(UT_radix_tree_uninitialize__NULL),

		cmocka_unit_test(UT_radix_tree_insert),
		cmocka_unit_test(UT_radix_tree_insert__ENOMEM),
		cmocka_unit_test(UT_radix_tree_insert__FIXED_STRINGS),
		cmocka_unit_test(UT_radix_tree_insert__NULL),
		cmocka_unit_test(UT_radix_tree_insert__double),

		cmocka_unit_test(UT_radix_tree_find),
		cmocka_unit_test(UT_radix_tree_find__find_any),
		cmocka_unit_test(UT_radix_tree_find__NULL),

		cmocka_unit_test(UT_radix_tree_remove__FIXED_STRINGS),
		cmocka_unit_test(UT_radix_tree_remove__remove_any),
		cmocka_unit_test(UT_radix_tree_remove__non_value_node),
		cmocka_unit_test(UT_radix_tree_remove__NULL),

		cmocka_unit_test(UT_radix_tree_get_key_count),
		cmocka_unit_test(UT_radix_tree_get_key_count__NULL),

		cmocka_unit_test(UT_radix_tree_iterate),
		cmocka_unit_test(UT_radix_tree_iterate__error),
		cmocka_unit_test(UT_radix_tree_iterate__NULL),

		cmocka_unit_test(UT_radix_tree_dump),
		cmocka_unit_test(UT_radix_tree_dump__NULL),


		cmocka_unit_test(FT_basic_usage)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
