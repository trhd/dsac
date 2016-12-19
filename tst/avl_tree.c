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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"
#include "cmocka-wrapper.h"

unsigned int ITERATION_COUNTER;

/***********************************************************************/

static void
UT_AVL_TREE_INITIALIZER()
{
	static struct avl_tree s = AVL_TREE_INITIALIZER((int(*)(const void *, const void *))strcmp);
	struct avl_tree * d = test_malloc(sizeof(struct avl_tree));

	memset(d, 0, sizeof(*d));
	avl_initialize(d, (int(*)(const void *, const void *))strcmp);

	assert_memory_equal(d, &s, sizeof(s));

	test_free(d);
}

/***********************************************************************/

static void
test_find_n_remove_minmax(struct avl_tree * m , struct avl_node * n, unsigned int s, int d)
{
	assert(m);
	assert(n);

	if (!s)
		assert_true(avl_empty(m));
	else
		assert_false(avl_empty(m));

	assert_int_equal(avl_size(m), s);

	assert_in_range(d, 1, -1);

	if (d == -1)
	{
		assert_ptr_equal(avl_find_min(m), n);
		assert_ptr_equal(avl_remove_min(m), n);
	}
	else if (d == 1)
	{
		assert_ptr_equal(avl_find_max(m), n);
		assert_ptr_equal(avl_remove_max(m), n);
	}

	if (s == 1)
		assert_true(avl_empty(m));

	assert_int_equal(avl_size(m), s - 1);
}

static void
test_find_n_remove(struct avl_tree * m, const void * p)
{
	const struct avl_node * n = avl_find(m, p);

	assert_non_null(n);
	assert_ptr_equal(avl_get(n), p);
	assert_ptr_equal(avl_remove(m , p), n);
}

static void
my_insert(struct avl_tree * m, struct avl_node * n, unsigned int s)
{
	if (s)
		assert_false(avl_empty(m));
	else
		assert_true(avl_empty(m));

	assert_int_equal(avl_size(m), s);

	assert_false(avl_insert(m, n));
	assert_true(avl_insert(m, n));

	assert_int_equal(avl_size(m), s + 1);

	assert_false(avl_empty(m));
}

static void
test_find(struct avl_tree const * m, int nc, struct avl_node ns[])
{
	while (--nc >= 0)
		assert_ptr_equal(avl_find(m, avl_get(&ns[nc])), &ns[nc]);

	assert_null(avl_find(m, "A non-existing string (in the tree)."));
}

static int
test_iteration_preorder(struct avl_node const * n, void * a)
{
	static const struct avl_node *prev;

	if (n)
	{
		ITERATION_COUNTER++;

		if (prev && strcmp((char*)avl_get(prev), (char*)avl_get(n)) >= 0)
		{
			print_error("AVL tree is not iterating in pre-order.\n");
			return -1;
		}

		if ((unsigned long)a != 0x715517)
		{
			print_error("Expected second argument to contain the magic number %x.\n",
					0x715517);
			return -1;
		}
	}


	prev = n;
	return 0;
}

static void
test_iteration(struct avl_tree *m)
{

	ITERATION_COUNTER = 0;
	test_iteration_preorder(NULL, NULL);

	assert_false(avl_iterate(m, test_iteration_preorder, (void*)0x715517));
	assert_int_equal(ITERATION_COUNTER, avl_size(m));
}

static void
FT_basic_usage()
{
	enum { node_count = 80 };
	struct avl_tree * m = test_malloc(sizeof(struct avl_tree));
	struct avl_node * ns = test_malloc(node_count * sizeof(struct avl_node));;

	for (int i = 0; i < node_count; ++i)
	{
		avl_set(&ns[i], test_malloc(2 * sizeof(char)));
		((char*)avl_get(&ns[i]))[0] = (char)((int)'0' + i);
		((char*)avl_get(&ns[i]))[1] = '\0';
	}

	avl_initialize(m, (int(*)(const void *, const void *))strcmp);

	for (int i = 0; i < node_count; ++i)
	{
		if (i)
			assert_false(avl_empty(m));
		else
			assert_true(avl_empty(m));

		my_insert(m, &ns[i], i);
		test_find(m, i, ns);
		test_iteration(m);

		assert_false(avl_empty(m));
	}

	for (int i = 0; i < node_count; ++i)
	{
		assert_false(avl_empty(m));

		test_find_n_remove_minmax(m, &ns[i], node_count - i, -1);
		test_iteration(m);

		if (i == node_count - 1)
			assert_true(avl_empty(m));
		else
			assert_false(avl_empty(m));
	}

	for (int i = node_count - 1; i >= 0; --i)
	{
		if (i == node_count - 1)
			assert_true(avl_empty(m));
		my_insert(m, &ns[i], node_count - i - 1);
		assert_false(avl_empty(m));
	}

	test_find(m, node_count, ns);

	for (int i = 0; i < node_count; ++i)
	{
		assert_false(avl_empty(m));
		test_find_n_remove_minmax(m, &ns[i], node_count - i, -1);
		if (i == node_count - 1)
			assert_true(avl_empty(m));
	}

	for (int i = 0; i < node_count; ++i)
	{
		if (!i)
			assert_true(avl_empty(m));
		my_insert(m, &ns[i], i);
		assert_false(avl_empty(m));
	}

	test_find(m, node_count, ns);

	for (int i = node_count - 1; i >= 0; --i)
	{
		assert_false(avl_empty(m));
		test_find_n_remove_minmax(m, &ns[i], i + 1, 1);
		if (!i)
			assert_true(avl_empty(m));
	}

	for (int i = node_count - 1; i >= 0; --i)
	{
		if (i == node_count - 1)
			assert_true(avl_empty(m));
		my_insert(m, &ns[i], node_count - i - 1);
		assert_false(avl_empty(m));
	}

	test_find(m, node_count, ns);

	for (int i = node_count - 1; i >= 0; --i)
	{
		assert_false(avl_empty(m));
		test_find_n_remove_minmax(m, &ns[i], i + 1, 1);
		if (!i)
			assert_true(avl_empty(m));
	}

	assert_int_equal(node_count % 2, 0);

	for (int i = 0; i < node_count / 2; ++i)
	{
		my_insert(m, &ns[i], i * 2);
		my_insert(m, &ns[node_count - i - 1], i * 2 + 1);
	}

	test_find(m, node_count, ns);

	assert_false(avl_empty(m));

	for (int i = 0; i < node_count / 2; ++i)
	{
		test_find_n_remove_minmax(m, &ns[i], node_count - i * 2, -1);
		test_find_n_remove_minmax(m, &ns[node_count - i - 1], node_count - i * 2 - 1, 1);
	}

	assert_true(avl_empty(m));

	my_insert(m, &ns[1], 0);
	my_insert(m, &ns[5], 1);
	my_insert(m, &ns[10], 2);
	my_insert(m, &ns[25], 3);
	my_insert(m, &ns[7], 4);
	my_insert(m, &ns[15], 5);
	my_insert(m, &ns[34], 6);
	my_insert(m, &ns[3], 7);
	my_insert(m, &ns[30], 8);
	my_insert(m, &ns[12], 9);

	/*
	 * avl_next() not implemented
	 * 
	 * assert_ptr_equal(avl_next(m, NULL), &ns[1]);
	 * assert_ptr_equal(avl_next(m, &ns[1]), &ns[3]);
	 * assert_ptr_equal(avl_next(m, &ns[3]), &ns[5]);
	 * assert_ptr_equal(avl_next(m, &ns[5]), &ns[7]);
	 * assert_ptr_equal(avl_next(m, &ns[7]), &ns[10]);
	 * assert_ptr_equal(avl_next(m, &ns[10]), &ns[12]);
	 * assert_ptr_equal(avl_next(m, &ns[12]), &ns[15]);
	 * assert_ptr_equal(avl_next(m, &ns[15]), &ns[25]);
	 * assert_ptr_equal(avl_next(m, &ns[25]), &ns[30]);
	 * assert_ptr_equal(avl_next(m, &ns[30]), &ns[34]);
	 * assert_ptr_equal(avl_next(m, &ns[34]), NULL);
	 */

	test_find_n_remove(m, avl_get(&ns[10]));
	test_find_n_remove(m, avl_get(&ns[7]));
	test_find_n_remove(m, avl_get(&ns[30]));
	test_find_n_remove(m, avl_get(&ns[3]));
	test_find_n_remove(m, avl_get(&ns[12]));
	test_find_n_remove(m, avl_get(&ns[34]));
	test_find_n_remove(m, avl_get(&ns[5]));
	test_find_n_remove(m, avl_get(&ns[25]));
	test_find_n_remove(m, avl_get(&ns[15]));
	test_find_n_remove(m, avl_get(&ns[1]));

	assert_true(avl_empty(m));

	for (int i = 0; i < node_count; ++i)
		test_free((void*)avl_get(&ns[i]));

	test_free(ns);
	test_free(m);
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_AVL_TREE_INITIALIZER),
		cmocka_unit_test(FT_basic_usage),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
