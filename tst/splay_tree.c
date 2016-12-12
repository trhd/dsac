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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmocka-wrapper.h"
#include "splay_tree.h"

#ifndef NDEBUG
#define SPLAY_PRINT(...) splay_print(__VA_ARGS__)
#else
#define SPLAY_PRINT
#endif

static struct splay_node alphabet_nodes[] =
{
	{ .data = "a" }, { .data = "b" }, { .data = "c" }, { .data = "d" },
	{ .data = "e" }, { .data = "f" }, { .data = "g" }, { .data = "h" },
	{ .data = "i" }, { .data = "j" }, { .data = "k" }, { .data = "l" },
	{ .data = "m" }, { .data = "n" }, { .data = "o" }, { .data = "p" },
	{ .data = "q" }, { .data = "r" }, { .data = "s" }, { .data = "t" },
	{ .data = "u" }, { .data = "v" }, { .data = "w" }, { .data = "x" },
	{ .data = "y" }, { .data = "z" }
};

static size_t ITERATION_COUNTER;

static void
assert_verify(struct splay_tree const * t)
{
#if !defined(NDEBUG) || defined(UNIT_TESTING)
	assert_int_equal(splay_verify(t), 0);
#endif
}

static int
assert_iteration_preorder(const struct splay_node * n, void * a)
{
	static struct splay_node const * prev;

	if (n)
	{
		ITERATION_COUNTER++;

		printf(" %s", (char*)splay_get(n));

		if (prev)
			assert_true(strcmp((char*)splay_get(prev), (char*)splay_get(n)) < 0);
		assert_ptr_equal(a, (void*)0x715517);
	}

	prev = n;
	return 0;
}

static void
assert_iteration(const struct splay_tree * t)
{
	assert(t);

	ITERATION_COUNTER = 0;

	printf("Iterating:");

	assert_iteration_preorder(NULL, NULL);

	assert_int_equal(splay_iterate(t, assert_iteration_preorder, (void*)0x715517), 0);
	assert_int_equal(ITERATION_COUNTER, splay_size(t));

	printf("\n");
}

static void
test_find(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);
	assert(splay_get(n));

	struct splay_node const * h = splay_find(t, splay_get(n));
	assert_non_null(h);
	assert_string_equal(splay_get(h), splay_get(n));
}

static void
test_nofind(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);
	assert(splay_get(n));

	assert_null(splay_find(t, splay_get(n)));
}

static void
test_insert(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);

	size_t z = splay_size(t);

	assert_false(splay_insert(t, n));

	assert_int_equal(splay_size(t), z + 1);
	assert_false(splay_is_empty(t));
	assert_verify(t);
}

static void
test_noremove(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);

	size_t z = splay_size(t);

	assert_null(splay_remove(t, splay_get(n)));
	assert_int_equal(splay_size(t), z);
}

static void
test_remove(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);

	size_t z = splay_size(t);

	test_find(t, n);
	assert_ptr_equal(splay_remove(t, splay_get(n)), n);
	test_nofind(t, n);
	assert_int_equal(splay_size(t), z - 1);
	test_noremove(t, n);
}

static void
do_insertion(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);

	printf("\n\n");
	SPLAY_PRINT(t);
	test_insert(t, n);
	assert_string_equal(splay_get(t->root), splay_get(n));
	printf("Inserted %s.\n", (char*)splay_get(n));
	SPLAY_PRINT(t);
	test_find(t, n);
	assert_iteration(t);
}

static void
do_removal(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);

	printf("\n\n");
	SPLAY_PRINT(t);
	test_remove(t, n);
	printf("Removed %s.\n", (char*)splay_get(n));
	SPLAY_PRINT(t);
}

static void
do_find_min(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);

	SPLAY_PRINT(t);

	struct splay_node const * m = splay_find_min(t);

	assert_non_null(m);
	assert_string_equal(splay_get(m), splay_get(n));

	printf("Removed minimum node '%s'.\n", (char*)splay_get(m));

	SPLAY_PRINT(t);
}

static void
do_find_max(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);

	SPLAY_PRINT(t);

	struct splay_node const * m = splay_find_max(t);

	assert_non_null(m);
	assert_string_equal(splay_get(m), splay_get(n));

	printf("Removed maximum node '%s'.\n", (char*)splay_get(m));

	SPLAY_PRINT(t);
}

static void
do_remove_min(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);

	SPLAY_PRINT(t);

	struct splay_node const * m = splay_remove_min(t);

	assert_non_null(m);
	assert_string_equal(splay_get(m), splay_get(n));

	printf("Removed minimum node '%s'.\n", (char*)splay_get(m));

	SPLAY_PRINT(t);

	test_nofind(t, n);
}

static void
do_remove_max(struct splay_tree * t, struct splay_node * n)
{
	assert(t);
	assert(n);

	SPLAY_PRINT(t);

	struct splay_node const * m = splay_remove_max(t);

	assert_non_null(m);
	assert_string_equal(splay_get(m), splay_get(n));

	printf("Removed maximum node '%s'.\n", (char*)splay_get(m));

	SPLAY_PRINT(t);

	test_nofind(t, n);
}

/***********************************************************************/

static void
FT_basic_test__1()
{
	struct splay_tree * t = test_malloc(sizeof(struct splay_tree));

	splay_initialize(t, (int(*)(const void *, const void *))strcmp);

	do_insertion(t, &alphabet_nodes[3]);

	do_insertion(t, &alphabet_nodes[6]);
	test_find(t, &alphabet_nodes[3]);

	do_insertion(t, &alphabet_nodes[9]);
	test_find(t, &alphabet_nodes[6]);
	test_find(t, &alphabet_nodes[3]);

	do_insertion(t, &alphabet_nodes[1]);
	test_find(t, &alphabet_nodes[6]);
	test_find(t, &alphabet_nodes[9]);
	test_find(t, &alphabet_nodes[3]);

	do_insertion(t, &alphabet_nodes[10]);
	test_find(t, &alphabet_nodes[3]);
	SPLAY_PRINT(t);
	test_find(t, &alphabet_nodes[9]);
	SPLAY_PRINT(t);
	test_find(t, &alphabet_nodes[1]);
	SPLAY_PRINT(t);
	test_find(t, &alphabet_nodes[6]);
	SPLAY_PRINT(t);

	do_removal(t, &alphabet_nodes[3]);
	do_removal(t, &alphabet_nodes[1]);
	do_removal(t, &alphabet_nodes[10]);
	do_removal(t, &alphabet_nodes[9]);
	do_removal(t, &alphabet_nodes[6]);

	do_insertion(t, &alphabet_nodes[4]);
	do_insertion(t, &alphabet_nodes[3]);
	do_insertion(t, &alphabet_nodes[6]);
	do_insertion(t, &alphabet_nodes[9]);
	do_insertion(t, &alphabet_nodes[8]);
	do_insertion(t, &alphabet_nodes[7]);
	do_insertion(t, &alphabet_nodes[2]);
	do_insertion(t, &alphabet_nodes[1]);
	do_insertion(t, &alphabet_nodes[0]);
	do_insertion(t, &alphabet_nodes[5]);
	do_insertion(t, &alphabet_nodes[14]);
	do_insertion(t, &alphabet_nodes[13]);
	do_insertion(t, &alphabet_nodes[17]);
	do_insertion(t, &alphabet_nodes[15]);
	do_insertion(t, &alphabet_nodes[16]);
	do_insertion(t, &alphabet_nodes[19]);
	do_insertion(t, &alphabet_nodes[12]);
	do_insertion(t, &alphabet_nodes[11]);
	do_insertion(t, &alphabet_nodes[10]);
	do_insertion(t, &alphabet_nodes[18]);
	do_insertion(t, &alphabet_nodes[25]);
	do_insertion(t, &alphabet_nodes[20]);
	do_insertion(t, &alphabet_nodes[23]);
	do_insertion(t, &alphabet_nodes[21]);
	do_insertion(t, &alphabet_nodes[24]);
	do_insertion(t, &alphabet_nodes[22]);

	for (int i = 0; i < 26 / 2; ++i)
	{
		do_find_min(t, &alphabet_nodes[i]);
		do_find_max(t, &alphabet_nodes[25 - i]);

		do_remove_min(t, &alphabet_nodes[i]);
		do_remove_max(t, &alphabet_nodes[25 - i]);
	}

	test_free(t);
}

/***********************************************************************/

static void
FT_basic_test__2()
{
	struct splay_tree * t = test_malloc(sizeof(struct splay_tree));

	splay_initialize(t, (int(*)(const void *, const void *))strcmp);

	do_insertion(t, &alphabet_nodes[10]);
	do_insertion(t, &alphabet_nodes[11]);
	do_insertion(t, &alphabet_nodes[12]);
	do_insertion(t, &alphabet_nodes[13]);
	do_insertion(t, &alphabet_nodes[14]);
	test_nofind(t, &alphabet_nodes[9]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[10]));
	test_nofind(t, &alphabet_nodes[20]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[14]));
	do_insertion(t, &alphabet_nodes[15]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[15]));
	test_find(t, &alphabet_nodes[13]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[13]));
	do_insertion(t, &alphabet_nodes[16]);
	do_insertion(t, &alphabet_nodes[17]);
	do_insertion(t, &alphabet_nodes[18]);
	do_insertion(t, &alphabet_nodes[19]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[19]));
	test_find(t, &alphabet_nodes[13]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[13]));

	test_noremove(t, &alphabet_nodes[25]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[19]));
	test_noremove(t, &alphabet_nodes[5]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[10]));

	do_removal(t, &alphabet_nodes[10]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[11]));
	do_removal(t, &alphabet_nodes[19]);
	assert_string_equal(splay_get(t->root), splay_get(&alphabet_nodes[18]));
	do_removal(t, &alphabet_nodes[15]);
	assert_true(strcmp(splay_get(t->root), splay_get(&alphabet_nodes[14])) == 0 ||
			strcmp(splay_get(t->root), splay_get(&alphabet_nodes[16])) == 0);
	do_removal(t, &alphabet_nodes[11]);
	do_removal(t, &alphabet_nodes[12]);
	do_removal(t, &alphabet_nodes[13]);
	do_removal(t, &alphabet_nodes[14]);
	do_removal(t, &alphabet_nodes[16]);
	do_removal(t, &alphabet_nodes[17]);
	do_removal(t, &alphabet_nodes[18]);

	test_free(t);
}

/***********************************************************************/

static void
FT_test_remove_any()
{
	struct splay_tree * t = test_malloc(sizeof(struct splay_tree));;

	splay_initialize(t, (int(*)(const void *, const void *))strcmp);

	for (size_t i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
		do_insertion(t, &alphabet_nodes[i]);

	for (size_t i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
		assert_non_null(splay_remove_any(t));

	assert_null(splay_remove_any(t));

	test_free(t);
}

/***********************************************************************/

static void
FT_find_any()
{
	struct splay_tree * t = test_malloc(sizeof(struct splay_tree));

	splay_initialize(t, (int(*)(const void *, const void *))strcmp);

	for (size_t i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
		do_insertion(t, &alphabet_nodes[i]);

	for (size_t i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
	{
		struct splay_node const * n = splay_find_any(t);

		assert_non_null(n);
		assert_ptr_equal(splay_remove(t, splay_get(n)), n);
	}

	assert_true(splay_is_empty(t));

	test_free(t);
}

/***********************************************************************/

static void
FT_next_prev_test()
{
	struct splay_tree * t = test_malloc(sizeof(struct splay_tree));

	splay_initialize(t, (int(*)(const void *, const void *))strcmp);

	assert_null(splay_find_next(t, "X"));
	assert_null(splay_find_prev(t, "X"));

	for (size_t i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
	{
		do_insertion(t, &alphabet_nodes[i]);
		assert_null(splay_find_next(t, &alphabet_nodes[i]));
	}

	for (size_t i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node) - 1; ++i)
	{
		assert_ptr_equal(splay_find_next(t, splay_get(&alphabet_nodes[i])), &alphabet_nodes[i + 1]);
		assert_null(splay_find_next(t, "0"));
		assert_null(splay_find_next(t, "X"));
	}

	for (size_t i = 1; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
	{
		assert_ptr_equal(splay_find_prev(t, splay_get(&alphabet_nodes[i])), &alphabet_nodes[i - 1]);
		assert_null(splay_find_prev(t, "0"));
		assert_null(splay_find_prev(t, "X"));
	}

	for (size_t i = 1; i < sizeof(alphabet_nodes) / sizeof(struct splay_node) / 2; ++i)
		assert_ptr_equal(splay_remove_next(t, splay_get(&alphabet_nodes[0])), &alphabet_nodes[i]);

	for (size_t i = sizeof(alphabet_nodes) / sizeof(struct splay_node) - 2;
			i >= sizeof(alphabet_nodes) / sizeof(struct splay_node) / 2;
			--i)
		assert_ptr_equal(splay_remove_prev(t, splay_get(&alphabet_nodes[sizeof(alphabet_nodes) / sizeof(struct splay_node) - 1])), &alphabet_nodes[i]);

	assert_ptr_equal(splay_remove_next(t, splay_get(&alphabet_nodes[0])),
			&alphabet_nodes[sizeof(alphabet_nodes) / sizeof(struct splay_node) - 1]);
	assert_ptr_equal(splay_remove_any(t), &alphabet_nodes[0]);
	assert_true(splay_is_empty(t));

	test_free(t);
}

/***********************************************************************/

static void
FT_next_prev_null()
{
	struct splay_tree * t = test_malloc(sizeof(struct splay_tree));

	splay_initialize(t, (int(*)(void const *, void const *))strcmp);

	do_insertion(t, &alphabet_nodes[0]);
	do_insertion(t, &alphabet_nodes[1]);
	do_insertion(t, &alphabet_nodes[2]);

	assert_null(splay_find_next(t, "c"));
	assert_null(splay_find_prev(t, "a"));

	assert_null(splay_remove_next(t, "c"));
	assert_null(splay_remove_prev(t, "a"));

	assert_ptr_equal(splay_find_next(t, "a"), &alphabet_nodes[1]);
	assert_ptr_equal(splay_find_next(t, "b"), &alphabet_nodes[2]);
	assert_ptr_equal(splay_find_prev(t, "b"), &alphabet_nodes[0]);
	assert_ptr_equal(splay_find_prev(t, "c"), &alphabet_nodes[1]);

	assert_ptr_equal(splay_remove_next(t, "a"), &alphabet_nodes[1]);
	assert_null(splay_remove_prev(t, "b"));
	assert_ptr_equal(splay_remove_prev(t, "c"), &alphabet_nodes[0]);
	assert_ptr_equal(splay_remove(t, "c"), &alphabet_nodes[2]);

	test_free(t);
}

/***********************************************************************/

int
fail_iteration(struct splay_node const * n __attribute__((unused)),
		void *a __attribute__((unused)))
{
	abort();
}

static void
FT_iterate_empty_tree()
{
	struct splay_tree * t = test_malloc(sizeof(struct splay_tree));

	splay_initialize(t, (int(*)(void const *, void const *))strcmp);

	assert_int_equal(splay_iterate(t, assert_iteration_preorder, NULL), 0);

	test_free(t);
}

/***********************************************************************/


int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(FT_test_remove_any),
		cmocka_unit_test(FT_find_any),
		cmocka_unit_test(FT_next_prev_null),
		cmocka_unit_test(FT_next_prev_test),
		cmocka_unit_test(FT_iterate_empty_tree),
		cmocka_unit_test(FT_basic_test__1),
		cmocka_unit_test(FT_basic_test__2)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
