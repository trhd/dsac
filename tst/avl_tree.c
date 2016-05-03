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
#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"

#define print_error(...) fprintf(stderr, "ERROR: " __VA_ARGS__)

#if CMAKE_BUILD_TYPE == Debug
#define VERIFY(a) \
do\
{\
	if (avl_verify(a))\
	{\
		print_error("AVL tree verification failed.\n");\
		avl_print(a);\
		return -1;\
	}\
	else\
		avl_print(a);\
} while (0)
#else
#define VERIFY(a)
#endif

#if 1
#define DECLARE() printf("Started %s.\n", __func__)
#define print_debug(...) printf(__VA_ARGS__)
#else
#define DECLARE()
#define print_debug(...)
#endif

unsigned int ITERATION_COUNTER;

static int
test_find_n_remove_minmax(struct avl_tree *m , struct avl_node *n, unsigned int s, int d)
{
	unsigned int z;
	const struct avl_node *t;

	DECLARE();

	if (!s && !avl_empty(m))
	{
		print_error("Non-empty AVL tree not reported as such.\n");
		return -1;
	} else if (s && avl_empty(m))
	{
		print_error("Empty AVL tree not reported as such.\n");
		return -1;
	}

	if ((z = avl_size(m)) != s)
	{
		print_error("AVL tree reported incorrect size (got: %u, expected: %u).\n",
				z, s);
		return -1;
	}

	if (d == -1)
		t = avl_find_min(m);
	else if (d == 1)
		t = avl_find_max(m);
	else
	{
		print_error("Invalid argument.\n");
		return -1;
	}

	if (!t || t != n)
	{
		print_error("Failed to find minimum/maximum node from AVL tree.\n");
		return -1;
	}

	if (d == -1)
		t = avl_remove_min(m);
	else if (d == 1)
		t = avl_remove_max(m);
	else
	{
		print_error("Invalid argument.\n");
		return -1;
	}

	if (!t || t != n)
	{
		print_error("Failed to remove minimum/maximum node from AVL tree.\n");
		return -1;
	}

	if (avl_size(m) != s - 1)
	{
		print_error("AVL tree reported incorrect size.\n");
		return -1;
	}

	VERIFY(m);

	return 0;
}

static int
test_find_n_remove(struct avl_tree *m, const void *p)
{
	const struct avl_node *n;

	DECLARE();

	n = avl_find(m, p);

	if (!n)
	{
		print_error("AVL tree failed to find the requested entry '%s'.\n", (char*)p);
		return -1;
	}

	if (avl_get(n) != p)
	{
		print_error("AVL tree found incorrect entry.\n");
		return -1;
	}

	n = avl_remove(m, p);

	if (!n)
	{
		print_error("AVL tree failed to remove the requested entry.\n");
		return -1;
	}

	if (avl_get(n) != p)
	{
		print_error("AVL tree removed incorrect entry.\n");
		return -1;
	}

	print_debug("Removed '%s'.\n", (char*)p);

	VERIFY(m);

	return 0;
}

static int
my_insert(struct avl_tree *m, struct avl_node *n, unsigned int s)
{
	DECLARE();

	struct avl_node tmp;

	avl_set(&tmp, avl_get(n));

	if (s && avl_empty(m))
	{
		print_error("Non-empty AVL tree not reported as such.\n");
		return -1;
	}
	else if (!s && !avl_empty(m))
	{
		print_error("Empty AVL tree not reported as such.\n");
		return -1;
	}

	if (avl_size(m) != s)
	{
		print_error("AVL tree reported incorrect size.\n");
		return -1;
	}

	if (avl_insert(m, n))
	{
		print_error("Failed to insert AVL node into AVL tree.\n");
		return -1;
	}

	if (!avl_insert(m, &tmp))
	{
		print_error("Somehow managed to insert AVL node into AVL tree twice.\n");
		return -1;
	}

	if (avl_size(m) != s + 1)
	{
		print_error("AVL tree reported incorrect size.\n");
		return -1;
	}

	if (avl_empty(m))
	{
		print_error("Non-empty AVL tree not reported as such.\n");
		return -1;
	}

	print_debug("Added '%s'.\n", (char*)avl_get(n));

	VERIFY(m);

	return 0;
}

static int
test_find(const struct avl_tree *m, const int nc, struct avl_node ns[])
{
	int i;

	DECLARE();

	for (i = 0; i < nc; ++i)
	{
		if (avl_find(m, avl_get(&ns[i])) != &ns[i])
		{
			print_error("Failed to find an existing node from the AVL tree.\n");
			return -1;
		}
	}

	if (avl_find(m, "This sure doesn't exist in the tree."))
	{
		print_error("AVL tree returned an entry for non-existing node.\n");
		return -1;
	}

	return 0;
}

static int
test_iteration_preorder(const struct avl_node *n, void *a)
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

int
test_iteration(struct avl_tree *m)
{

	ITERATION_COUNTER = 0;
	test_iteration_preorder(NULL, NULL);

	if (avl_iterate(m, test_iteration_preorder, (void*)0x715517))
		return -1;

	if (ITERATION_COUNTER != avl_size(m))
	{
		print_error("ITERATION_COUNTER doesn't match the AVL tree's size (%u vs. %u).\n",
				ITERATION_COUNTER, avl_size(m));
		return -1;
	}

	return 0;
}

int
main(int ac __attribute__((unused)),
		char **av __attribute__((unused)))
{
	static const int node_count = 80;
	struct avl_tree m;
	struct avl_node ns[node_count];
	int i;

	for (i = 0; i < node_count; ++i)
	{
		avl_set(&ns[i], malloc(2*sizeof(char)));
		((char*)avl_get(&ns[i]))[0] = (char)((int)'0' + i);
		((char*)avl_get(&ns[i]))[1] = '\0';
	}

	avl_initialize(&m, (int(*)(const void *, const void *))strcmp);

	if (!avl_empty(&m))
	{
		print_error("Empty AVL tree not reported as such.\n");
		goto err;
	}

	for (i = 0; i < node_count; ++i)
		if (my_insert(&m, &ns[i], i))
			goto err;

	if (test_find(&m, node_count, ns))
		goto err;

	if (test_iteration(&m))
		goto err;

	for (i = 0; i < node_count; ++i)
		if (test_find_n_remove_minmax(&m, &ns[i], node_count - i, -1))
			goto err;

	if (!avl_empty(&m))
	{
		print_error("Empty AVL tree not reported as such.\n");
		goto err;
	}

	if (test_iteration(&m))
		goto err;

	for (i = node_count - 1; i >= 0; --i)
		if (my_insert(&m, &ns[i], node_count - i - 1))
			goto err;

	if (test_find(&m, node_count, ns))
		goto err;

	for (i = 0; i < node_count; ++i)
		if (test_find_n_remove_minmax(&m, &ns[i], node_count - i, -1))
			goto err;

	if (!avl_empty(&m))
	{
		print_error("Empty AVL tree not reported as such.\n");
		goto err;
	}

	for (i = 0; i < node_count; ++i)
		if (my_insert(&m, &ns[i], i))
			goto err;

	if (test_find(&m, node_count, ns))
		goto err;

	for (i = node_count - 1; i >= 0; --i)
		if (test_find_n_remove_minmax(&m, &ns[i], i + 1, 1))
			goto err;

	if (!avl_empty(&m))
	{
		print_error("Empty AVL tree not reported as such.\n");
		goto err;
	}

	for (i = node_count - 1; i >= 0; --i)
		if (my_insert(&m, &ns[i], node_count - i - 1))
			goto err;

	if (test_find(&m, node_count, ns))
		goto err;

	for (i = node_count - 1; i >= 0; --i)
		if (test_find_n_remove_minmax(&m, &ns[i], i + 1, 1))
			goto err;

	if (!avl_empty(&m))
	{
		print_error("Empty AVL tree not reported as such.\n");
		goto err;
	}

	assert(node_count % 2 == 0);
	for (i = 0; i < node_count / 2; ++i)
	{
		if (my_insert(&m, &ns[i], i * 2))
			goto err;

		if (my_insert(&m, &ns[node_count - i - 1], i * 2 + 1))
			goto err;
	}

	if (test_find(&m, node_count, ns))
		goto err;

	for (i = 0; i < node_count / 2; ++i)
	{
		if (test_find_n_remove_minmax(&m, &ns[i], node_count - i * 2, -1))
			goto err;

		if (test_find_n_remove_minmax(&m, &ns[node_count - i - 1], node_count - i * 2 - 1, 1))
			goto err;
	}

	if (!avl_empty(&m))
	{
		print_error("Empty AVL tree not reported as such.\n");
		goto err;
	}

	if (my_insert(&m, &ns[1], 0)
			|| my_insert(&m, &ns[5], 1)
			|| my_insert(&m, &ns[10], 2)
			|| my_insert(&m, &ns[25], 3)
			|| my_insert(&m, &ns[7], 4)
			|| my_insert(&m, &ns[15], 5)
			|| my_insert(&m, &ns[34], 6)
			|| my_insert(&m, &ns[3], 7)
			|| my_insert(&m, &ns[30], 8)
			|| my_insert(&m, &ns[12], 9))
	{
		print_error("Failed to insert all the requested entries.\n");
		goto err;
	}

	/*
	 *run = NULL;
	 *if ((run = (struct avl_node *)avl_next(&m, run)) != &ns[1]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != &ns[3]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != &ns[5]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != &ns[7]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != &ns[10]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != &ns[12]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != &ns[15]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != &ns[25]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != &ns[30]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != &ns[34]
	 *               || (run = (struct avl_node *)avl_next(&m, run)) != NULL)
	 *{
	 *        print_error("AVL tree failed to go through the tree in pre-order.\n");
	 *        print_in_preorcer(&m);
	 *        goto err;
	 *}
	 */

	if (test_find_n_remove(&m, avl_get(&ns[10]))
			|| test_find_n_remove(&m, avl_get(&ns[7]))
			|| test_find_n_remove(&m, avl_get(&ns[30]))
			|| test_find_n_remove(&m, avl_get(&ns[3]))
			|| test_find_n_remove(&m, avl_get(&ns[12]))
			|| test_find_n_remove(&m, avl_get(&ns[34]))
			|| test_find_n_remove(&m, avl_get(&ns[5]))
			|| test_find_n_remove(&m, avl_get(&ns[25]))
			|| test_find_n_remove(&m, avl_get(&ns[15]))
			|| test_find_n_remove(&m, avl_get(&ns[1])))
	{
		print_error("Failed to find and remove all the requested entries.\n");
		goto err;
	}

	if (!avl_empty(&m))
	{
		print_error("Empty AVL tree not reported as such (reports size %u).\n",
				avl_size(&m));
		goto err;
	}

	for (i = 0; i < node_count; ++i)
		free((void*)avl_get(&ns[i]));

	printf("AVL test ended without errors.\n");
	return EXIT_SUCCESS;
err:
	for (i = 0; i < node_count; ++i)
		free((void*)avl_get(&ns[i]));

	fprintf(stderr, "AVL test ended with an error.\n");
	return EXIT_FAILURE;
}
