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
#include "splay_tree.h"

#define TEST(a, b, ...)\
{\
	if ((a) != (b))\
	{\
		fprintf(stderr, __VA_ARGS__);\
		abort();\
	}\
}\

#define TEST_NOT(a, b, ...)\
{\
	if ((a) == (b))\
	{\
		fprintf(stderr, __VA_ARGS__);\
		abort();\
	}\
}

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
assert_size(struct splay_tree *t, size_t z)
{
	assert(t);
	TEST(splay_size(t), z, "Splay tree's reported size not what was expected.\n");
}

static void
assert_nonempty(const struct splay_tree *t)
{
	assert(t);
	TEST(splay_is_empty(t), false, "Expected splay tree to report itself as empty.\n");
}

static int
assert_iteration_preorder(const struct splay_node *n, void *a)
{
	static const struct splay_node *prev;

	if (n)
	{
		ITERATION_COUNTER++;

		printf(" %s", (char*)splay_get(n));

		if (prev)
			TEST(strcmp((char*)splay_get(prev), (char*)splay_get(n)) < 0, true,
					"Splay tree is not iterating in pre-order.\n");

		TEST(a, (void*)0x715517, "Expected second argument to contain the magic number %x.\n",
				0x715517);
	}


	prev = n;
	return 0;
}

static void
assert_iteration(const struct splay_tree *t)
{
	ITERATION_COUNTER = 0;

	printf("Iterating:");

	assert_iteration_preorder(NULL, NULL);

	TEST(splay_iterate(t, assert_iteration_preorder, (void*)0x715517), 0,
			"An error occurred while iterating through the splay tree.\n");
	TEST(ITERATION_COUNTER, splay_size(t),
			"ITERATION_COUNTER doesn't match the splay tree's size (%zd vs. %zd).\n",
			ITERATION_COUNTER, splay_size(t));

	printf("\n");
}

static void
assert_verify(const struct splay_tree *t)
{
#ifndef NDEBUG
	TEST(splay_verify(t), 0, "Splay tree verification failed.\n");
#endif
}

static void
test_find(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);
	assert(splay_get(n));

	const struct splay_node *h = splay_find(t, splay_get(n));
	TEST_NOT(h, NULL, "Failed to locate the requested node from the tree.\n");
	TEST(strcmp(splay_get(h), splay_get(n)), 0,
			"Found splay tree node doesn't match what was expected.\n");
}

static void
test_nofind(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);
	assert(splay_get(n));

	TEST(splay_find(t, splay_get(n)), NULL, "Didn't expect splay_find to return anything.\n");
}

static void
test_insert(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);

	size_t z = splay_size(t);

	TEST(splay_insert(t, n), 0, "Failed to insert a node into the tree.\n");

	assert_size(t, z + 1);
	assert_nonempty(t);
	assert_verify(t);
}

static void
test_noremove(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);

	size_t z = splay_size(t);

	TEST(splay_remove(t, splay_get(n)), NULL,
			"splay_remove() something for non-existing entry.\n");

	assert_size(t, z);
}

static void
test_remove(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);

	size_t z = splay_size(t);

	test_find(t, n);
	TEST(splay_remove(t, splay_get(n)), n,
			"splay_remove returned incorrect entry.\n");
	test_nofind(t, n);
	assert_size(t, z - 1);
	test_noremove(t, n);
}

static void
do_insertion(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);

	printf("\n\n");
	SPLAY_PRINT(t);
	test_insert(t, n);
	printf("Inserted %s.\n", (char*)splay_get(n));
	SPLAY_PRINT(t);
	test_find(t, n);
	assert_iteration(t);
}

static void
do_removal(struct splay_tree *t, struct splay_node *n)
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
do_find_min(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);

	SPLAY_PRINT(t);

	const struct splay_node *m = splay_find_min(t);

	TEST_NOT(m, NULL, "Didn't get minimum entry.");
	TEST(strcmp(splay_get(m), splay_get(n)), 0,
			"Got wrong entry from find_min() (got %s, expected %s).\n",
			(char*)splay_get(m), (char*)splay_get(n));

	printf("Removed minimum node '%s'.\n", (char*)splay_get(m));

	SPLAY_PRINT(t);
}

static void
do_find_max(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);

	SPLAY_PRINT(t);

	const struct splay_node *m = splay_find_max(t);

	TEST_NOT(m, NULL, "Didn't get maximum entry.");
	TEST(strcmp(splay_get(m), splay_get(n)), 0,
			"Got wrong entry from find_max() (got %s, expected %s).\n",
			(char*)splay_get(m), (char*)splay_get(n));

	printf("Removed maximum node '%s'.\n", (char*)splay_get(m));

	SPLAY_PRINT(t);
}

static void
do_remove_min(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);

	SPLAY_PRINT(t);

	const struct splay_node *m = splay_remove_min(t);

	TEST_NOT(m, NULL, "Didn't get minimum entry.");
	TEST(strcmp(splay_get(m), splay_get(n)), 0,
			"Got wrong entry from remove_min() (got %s, expected %s).\n",
			(char*)splay_get(m), (char*)splay_get(n));

	printf("Removed minimum node '%s'.\n", (char*)splay_get(m));

	SPLAY_PRINT(t);

	test_nofind(t, n);
}

static void
do_remove_max(struct splay_tree *t, struct splay_node *n)
{
	assert(t);
	assert(n);

	SPLAY_PRINT(t);

	const struct splay_node *m = splay_remove_max(t);

	TEST_NOT(m, NULL, "Didn't get maximum entry.");
	TEST(strcmp(splay_get(m), splay_get(n)), 0,
			"Got wrong entry from remove_max() (got %s, expected %s).\n",
			(char*)splay_get(m), (char*)splay_get(n));

	printf("Removed maximum node '%s'.\n", (char*)splay_get(m));

	SPLAY_PRINT(t);

	test_nofind(t, n);
}


static int
first()
{
	struct splay_tree t;
	int i;

	splay_initialize(&t, (int(*)(const void *, const void *))strcmp);

	do_insertion(&t, &alphabet_nodes[3]);

	do_insertion(&t, &alphabet_nodes[6]);
	test_find(&t, &alphabet_nodes[3]);

	do_insertion(&t, &alphabet_nodes[9]);
	test_find(&t, &alphabet_nodes[6]);
	test_find(&t, &alphabet_nodes[3]);

	do_insertion(&t, &alphabet_nodes[1]);
	test_find(&t, &alphabet_nodes[6]);
	test_find(&t, &alphabet_nodes[9]);
	test_find(&t, &alphabet_nodes[3]);

	do_insertion(&t, &alphabet_nodes[10]);
	test_find(&t, &alphabet_nodes[3]);
	SPLAY_PRINT(&t);
	test_find(&t, &alphabet_nodes[9]);
	SPLAY_PRINT(&t);
	test_find(&t, &alphabet_nodes[1]);
	SPLAY_PRINT(&t);
	test_find(&t, &alphabet_nodes[6]);
	SPLAY_PRINT(&t);

	do_removal(&t, &alphabet_nodes[3]);
	do_removal(&t, &alphabet_nodes[1]);
	do_removal(&t, &alphabet_nodes[10]);
	do_removal(&t, &alphabet_nodes[9]);
	do_removal(&t, &alphabet_nodes[6]);

	do_insertion(&t, &alphabet_nodes[4]);
	do_insertion(&t, &alphabet_nodes[3]);
	do_insertion(&t, &alphabet_nodes[6]);
	do_insertion(&t, &alphabet_nodes[9]);
	do_insertion(&t, &alphabet_nodes[8]);
	do_insertion(&t, &alphabet_nodes[7]);
	do_insertion(&t, &alphabet_nodes[2]);
	do_insertion(&t, &alphabet_nodes[1]);
	do_insertion(&t, &alphabet_nodes[0]);
	do_insertion(&t, &alphabet_nodes[5]);
	do_insertion(&t, &alphabet_nodes[14]);
	do_insertion(&t, &alphabet_nodes[13]);
	do_insertion(&t, &alphabet_nodes[17]);
	do_insertion(&t, &alphabet_nodes[15]);
	do_insertion(&t, &alphabet_nodes[16]);
	do_insertion(&t, &alphabet_nodes[19]);
	do_insertion(&t, &alphabet_nodes[12]);
	do_insertion(&t, &alphabet_nodes[11]);
	do_insertion(&t, &alphabet_nodes[10]);
	do_insertion(&t, &alphabet_nodes[18]);
	do_insertion(&t, &alphabet_nodes[25]);
	do_insertion(&t, &alphabet_nodes[20]);
	do_insertion(&t, &alphabet_nodes[23]);
	do_insertion(&t, &alphabet_nodes[21]);
	do_insertion(&t, &alphabet_nodes[24]);
	do_insertion(&t, &alphabet_nodes[22]);

	for (i = 0; i < 26 / 2; ++i)
	{
		do_find_min(&t, &alphabet_nodes[i]);
		do_find_max(&t, &alphabet_nodes[25 - i]);

		do_remove_min(&t, &alphabet_nodes[i]);
		do_remove_max(&t, &alphabet_nodes[25 - i]);
	}

	return 0;
}

static int
second()
{
	struct splay_tree t;

	splay_initialize(&t, (int(*)(const void *, const void *))strcmp);

	do_insertion(&t, &alphabet_nodes[10]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[10])), 0,
			"Expected a different node to be located at the root.\n");
	do_insertion(&t, &alphabet_nodes[11]);
	do_insertion(&t, &alphabet_nodes[12]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[12])), 0,
			"Expected a different node to be located at the root.\n");
	do_insertion(&t, &alphabet_nodes[13]);
	do_insertion(&t, &alphabet_nodes[14]);
	test_nofind(&t, &alphabet_nodes[9]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[10])), 0,
			"Expected a different node to be located at the root.\n");
	test_nofind(&t, &alphabet_nodes[20]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[14])), 0,
			"Expected a different node to be located at the root.\n");
	do_insertion(&t, &alphabet_nodes[15]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[15])), 0,
			"Expected a different node to be located at the root.\n");
	test_find(&t, &alphabet_nodes[13]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[13])), 0,
			"Expected a different node to be located at the root.\n");
	do_insertion(&t, &alphabet_nodes[16]);
	do_insertion(&t, &alphabet_nodes[17]);
	do_insertion(&t, &alphabet_nodes[18]);
	do_insertion(&t, &alphabet_nodes[19]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[19])), 0,
			"Expected a different node to be located at the root.\n");
	test_find(&t, &alphabet_nodes[13]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[13])), 0,
			"Expected a different node to be located at the root.\n");

	test_noremove(&t, &alphabet_nodes[25]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[19])), 0,
			"Expected a different node to be located at the root.\n");
	test_noremove(&t, &alphabet_nodes[5]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[10])), 0,
			"Expected a different node to be located at the root.\n");

	do_removal(&t, &alphabet_nodes[10]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[11])), 0,
			"Expected a different node to be located at the root.\n");
	do_removal(&t, &alphabet_nodes[19]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[18])), 0,
			"Expected a different node to be located at the root.\n");
	do_removal(&t, &alphabet_nodes[15]);
	TEST(strcmp(splay_get(t.root), splay_get(&alphabet_nodes[14])) == 0 ||
			strcmp(splay_get(t.root), splay_get(&alphabet_nodes[16])) == 0, true,
			"Expected a different node to be located at the root.\n");
	do_removal(&t, &alphabet_nodes[11]);
	do_removal(&t, &alphabet_nodes[12]);
	do_removal(&t, &alphabet_nodes[13]);
	do_removal(&t, &alphabet_nodes[14]);
	do_removal(&t, &alphabet_nodes[16]);
	do_removal(&t, &alphabet_nodes[17]);
	do_removal(&t, &alphabet_nodes[18]);

	return 0;
}

static void
test_remove_any()
{
	size_t i;
	struct splay_tree t;

	splay_initialize(&t, (int(*)(const void *, const void *))strcmp);

	for (i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
		do_insertion(&t, &alphabet_nodes[i]);

	for (i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
		TEST_NOT(splay_remove_any(&t), NULL, "splay_remove_any() failed.");

	TEST(splay_remove_any(&t), NULL, "splay_remove_any() should have failed with empty tree.");
}

static void
next_prev_test()
{
	size_t i;
	struct splay_tree t;

	splay_initialize(&t, (int(*)(const void *, const void *))strcmp);

	for (i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
		do_insertion(&t, &alphabet_nodes[i]);

	for (i = 0; i < sizeof(alphabet_nodes) / sizeof(struct splay_node) - 1; ++i)
	{
		TEST(splay_find_next(&t, splay_get(&alphabet_nodes[i])),
				&alphabet_nodes[i + 1],
				"splay_find_next() returned incorrrect entry.");

		TEST(splay_find_next(&t, "0"), NULL,
				"splay_find_next() returned something for non-existing entry (\"0\").");

		TEST(splay_find_next(&t, "X"), NULL,
				"splay_find_next() returned something for non-existing entry (\"X\").");
	}

	TEST(splay_find_next(&t, "X"), NULL, "splay_find_next() returned non-null for non-existing entry.");


	for (i = 1; i < sizeof(alphabet_nodes) / sizeof(struct splay_node); ++i)
	{
		TEST(splay_find_prev(&t, splay_get(&alphabet_nodes[i])),
				&alphabet_nodes[i - 1],
				"splay_find_prev() returned incorrrect entry.");

		TEST(splay_find_prev(&t, "0"), NULL,
				"splay_find_prev() returned something for non-existing entry (\"0\").");

		TEST(splay_find_prev(&t, "X"), NULL,
				"splay_find_prev() returned something for non-existing entry (\"X\").");
	}

	TEST(splay_find_prev(&t, "X"), NULL, "splay_find_prev() returned non-null for non-existing entry.");

	for (i = 1; i < sizeof(alphabet_nodes) / sizeof(struct splay_node) / 2; ++i)
		TEST(splay_remove_next(&t, splay_get(&alphabet_nodes[0])),
				&alphabet_nodes[i],
				"splay_remove_next() returned incorrect entry.");

	for (i = sizeof(alphabet_nodes) / sizeof(struct splay_node) - 2;
			i >= sizeof(alphabet_nodes) / sizeof(struct splay_node) / 2;
			--i)
		TEST(splay_remove_prev(&t, splay_get(&alphabet_nodes[sizeof(alphabet_nodes) / sizeof(struct splay_node) - 1])),
				&alphabet_nodes[i],
				"splay_remove_prev() returned incorrect entry.");

	TEST(splay_remove_next(&t, splay_get(&alphabet_nodes[0])),
			&alphabet_nodes[sizeof(alphabet_nodes) / sizeof(struct splay_node) - 1],
			"Expected for the \"edge\" nodes to remain.");
	TEST(splay_remove_any(&t), &alphabet_nodes[0], "Expected the frist node to remain.");
	TEST(splay_is_empty(&t), true, "Expected splay tree to be empty now.");
}

static void
next_prev_null()
{
	struct splay_tree t;

	splay_initialize(&t, (int(*)(const void *, const void *))strcmp);

	do_insertion(&t, &alphabet_nodes[0]);
	do_insertion(&t, &alphabet_nodes[1]);
	do_insertion(&t, &alphabet_nodes[2]);

	TEST(splay_find_next(&t, "c"), NULL, "There should be no next from \"c\".");
	TEST(splay_find_prev(&t, "a"), NULL, "There should be no prev from \"a\".");

	TEST(splay_remove_next(&t, "c"), NULL, "There should be no next from \"c\".");
	TEST(splay_remove_prev(&t, "a"), NULL, "There should be no prev from \"a\".");

	TEST(splay_find_next(&t, "a"), &alphabet_nodes[1], "Failed to find \"b\".");
	TEST(splay_find_next(&t, "b"), &alphabet_nodes[2], "Failed to find \"c\".");
	TEST(splay_find_prev(&t, "b"), &alphabet_nodes[0], "Failed to find (backwards) \"a\".");
	TEST(splay_find_prev(&t, "c"), &alphabet_nodes[1], "Failed to find (backwards) \"b\".");

	TEST(splay_remove_next(&t, "a"), &alphabet_nodes[1], "Failed to remove \"b\".");
	TEST(splay_remove_prev(&t, "b"), NULL, "Key was already removed.");
	TEST(splay_remove_prev(&t, "c"), &alphabet_nodes[0], "Failed to remove (backwards) \"a\".");
	TEST(splay_remove(&t, "c"), &alphabet_nodes[2], "Failed to just remove \"c\".");
}

int main()
{
	test_remove_any();
	next_prev_null();
	next_prev_test();

	if (first() || second())
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
