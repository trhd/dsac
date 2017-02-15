/**
 * Copyright (C) 2016-2017 Hemmo Nieminen
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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"
#include "cmocka-wrapper.h"

unsigned int ITERATION_COUNTER;

static int
_iteration_dummy_fn(struct avl_node const __attribute__((unused)) * n,
		void __attribute__((unused)) * p)
{
	return 0;
}

/***********************************************************************/

static void
UT_avl_initialize()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_initialize__NULL()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	expect_assert_failure(avl_initialize(NULL, NULL));
	expect_assert_failure(avl_initialize(NULL, (int(*)(void const *, void const *))memcmp));
	expect_assert_failure(avl_initialize(t, NULL));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_insert()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * buf = test_malloc(2 * node_count);

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf[2 * i] = 'a' + i;
		buf[2 * i + 1] = '\0';
		avl_set(&n[i], &buf[2 * i]);

		assert_false(avl_insert(t, &n[i]));
		assert_int_equal(avl_insert(t, &n[i]), -ENOTUNIQ);
	}

	test_free(n);
	test_free(buf);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_insert__NULL()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(sizeof(struct avl_node));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);
	avl_set(n, "foobar");

	expect_assert_failure(avl_insert(NULL, NULL));
	expect_assert_failure(avl_insert(NULL, n));
	expect_assert_failure(avl_insert(t, NULL));

	test_free(n);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_insert__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(sizeof(struct avl_node));

	memset(t, 0, sizeof(*t));
	avl_set(n, "foobar");

	expect_assert_failure(avl_insert(t, n));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);
	assert_false(avl_insert(t, n));

	test_free(n);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_insert__uninitialized_node()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(sizeof(struct avl_node));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);
	memset(n, 0, sizeof(*n));

	expect_assert_failure(avl_insert(t, n));

	avl_set(n, "foobar");
	assert_false(avl_insert(t, n));

	test_free(n);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_remove()
{
	enum { node_count = 10 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * b = test_malloc(node_count * 2);

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		b[2 * i] = 'a' + i;
		b[2 * i + 1] = '\0';
		avl_set(&n[i], b + 2 * i);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_null(avl_remove(t, &n[i]));
		assert_int_equal(avl_size(t), 0);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_int_equal(avl_size(t), 0);
		assert_false(avl_insert(t, &n[i]));
		assert_int_equal(avl_size(t), 1);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
		assert_null(avl_remove(t, avl_get(&n[i])));
		assert_int_equal(avl_size(t), 0);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_null(avl_remove(t, avl_get(&n[i])));
		assert_int_equal(avl_size(t), 0);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_int_equal(avl_size(t), i);
		assert_false(avl_insert(t, &n[i]));
		assert_int_equal(avl_size(t), i + 1);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_int_equal(avl_size(t), node_count - i);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
		assert_null(avl_remove(t, avl_get(&n[i])));
		assert_int_equal(avl_size(t), node_count - i - 1);
	}

	test_free(n);
	test_free(t);
	test_free(b);
}

/***********************************************************************/
static void
UT_avl_remove__NULL()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(sizeof(struct avl_node));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);
	avl_set(n, "foobar");

	expect_assert_failure(avl_remove(NULL, NULL));
	expect_assert_failure(avl_remove(NULL, n));

	test_free(n);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_remove__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(sizeof(struct avl_node));

	memset(t, 0, sizeof(*t));
	avl_set(n, "foobar");

	expect_assert_failure(avl_remove(t, n));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);
	assert_null(avl_remove(t, n));

	test_free(n);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_set()
{
	struct avl_node * n = test_malloc(sizeof(struct avl_node));

	memset(n, 0, sizeof(*n));

	avl_set(n, "foobar");
	assert_string_equal((char*)avl_get(n), "foobar");

	avl_set(n, "FOOBAR");
	assert_string_equal((char*)avl_get(n), "FOOBAR");

	test_free(n);
}

/***********************************************************************/

static void
UT_avl_set__NULL()
{
	expect_assert_failure(avl_set(NULL, "foobar"));
}

/***********************************************************************/

static void
UT_avl_get()
{
	struct avl_node * n = test_malloc(sizeof(struct avl_node));

	memset(n, 0, sizeof(*n));

	avl_set(n, "barfoo");
	assert_string_equal((char*)avl_get(n), "barfoo");

	avl_set(n, "BARFOO");
	assert_string_equal((char*)avl_get(n), "BARFOO");

	test_free(n);
}

/***********************************************************************/

static void
UT_avl_get__NULL()
{
	expect_assert_failure(avl_get(NULL));
}

/***********************************************************************/

static void
UT_avl_size()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * buf = test_malloc(2 * node_count);

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf[2 * i] = 'a' + i;
		buf[2 * i + 1] = '\0';
		avl_set(&n[i], &buf[2 * i]);

		assert_int_equal(avl_size(t), i);
		assert_false(avl_insert(t, &n[i]));
		assert_int_equal(avl_size(t), i + 1);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_int_equal(avl_size(t), node_count - i);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
		assert_int_equal(avl_size(t), node_count - i - 1);
	}

	test_free(n);
	test_free(buf);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_size__NULL()
{
	expect_assert_failure(avl_size(NULL));
}

/***********************************************************************/

static void
UT_avl_size__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_size(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_empty()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(2 * sizeof(struct avl_node));


	avl_set(&n[0], "eka");
	avl_set(&n[1], "toka");

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	assert_true(avl_empty(t));
	assert_false(avl_insert(t, &n[0]));
	assert_false(avl_empty(t));
	assert_false(avl_insert(t, &n[1]));
	assert_false(avl_empty(t));
	assert_ptr_equal(avl_remove(t, avl_get(&n[0])), &n[0]);
	assert_false(avl_empty(t));
	assert_ptr_equal(avl_remove(t, avl_get(&n[1])), &n[1]);
	assert_true(avl_empty(t));

	test_free(n);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_empty__NULL()
{
	expect_assert_failure(avl_empty(NULL));
}

/***********************************************************************/

static void
UT_avl_empty__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_empty(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_find_min()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * buf = test_malloc(2 * node_count);

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf[2 * i] = 'a' + i;
		buf[2 * i + 1] = '\0';
		avl_set(&n[i], &buf[2 * i]);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_ptr_equal(avl_find_min(t), &n[0]);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_ptr_equal(avl_find_min(t), &n[i]);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_ptr_equal(avl_find_min(t), &n[i]);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		assert_ptr_equal(avl_find_min(t), &n[0]);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_ptr_equal(avl_find_min(t), &n[i]);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_ptr_equal(avl_find_min(t), &n[i]);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_ptr_equal(avl_find_min(t), &n[0]);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		assert_ptr_equal(avl_find_min(t), &n[0]);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
	}

	test_free(n);
	test_free(buf);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_find_min__empty()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	assert_null(avl_find_min(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_find_min__NULL()
{
	expect_assert_failure(avl_find_min(NULL));
}

/***********************************************************************/

static void
UT_avl_find_min__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_find_min(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_find_max()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * buf = test_malloc(2 * node_count);

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf[2 * i] = 'a' + i;
		buf[2 * i + 1] = '\0';
		avl_set(&n[i], &buf[2 * i]);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_ptr_equal(avl_find_max(t), &n[i]);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_ptr_equal(avl_find_max(t), &n[node_count - 1]);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_ptr_equal(avl_find_max(t), &n[node_count - 1]);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		assert_ptr_equal(avl_find_max(t), &n[i]);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_ptr_equal(avl_find_max(t), &n[node_count - 1]);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_ptr_equal(avl_find_max(t), &n[node_count - 1]);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_ptr_equal(avl_find_max(t), &n[i]);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		assert_ptr_equal(avl_find_max(t), &n[i]);
		assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
	}

	test_free(n);
	test_free(buf);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_find_max__empty()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	assert_null(avl_find_max(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_find_max__NULL()
{
	expect_assert_failure(avl_find_max(NULL));
}

/***********************************************************************/

static void
UT_avl_find_max__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_find_max(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_remove_min()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * buf = test_malloc(2 * node_count);

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf[2 * i] = 'a' + i;
		buf[2 * i + 1] = '\0';
		avl_set(&n[i], &buf[2 * i]);
	}

	for (int i = 0 ; i < node_count ; i++)
		assert_false(avl_insert(t, &n[i]));

	for (int i = 0 ; i < node_count ; i++)
		assert_ptr_equal(avl_remove_min(t), &n[i]);

	for (int i = node_count - 1 ; i >= 0 ; i--)
		assert_false(avl_insert(t, &n[i]));

	for (int i = 0 ; i < node_count ; i++)
		assert_ptr_equal(avl_remove_min(t), &n[i]);

	for (int i = 0 ; i < node_count / 2 ; i++)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_false(avl_insert(t, &n[node_count - 1 - i]));
	}

	for (int i = 0 ; i < node_count ; i++)
		assert_ptr_equal(avl_remove_min(t), &n[i]);

	test_free(n);
	test_free(buf);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_remove_min__empty()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	assert_null(avl_remove_min(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_remove_min__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_remove_min(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_remove_min__NULL()
{
	expect_assert_failure(avl_remove_min(NULL));
}

/***********************************************************************/

static void
UT_avl_remove_max()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * buf = test_malloc(2 * node_count);

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf[2 * i] = 'a' + i;
		buf[2 * i + 1] = '\0';
		avl_set(&n[i], &buf[2 * i]);
	}

	for (int i = 0 ; i < node_count ; i++)
		assert_false(avl_insert(t, &n[i]));

	for (int i = 0 ; i < node_count ; i++)
		assert_ptr_equal(avl_remove_max(t), &n[node_count - 1 - i]);

	for (int i = node_count - 1 ; i >= 0 ; i--)
		assert_false(avl_insert(t, &n[i]));

	for (int i = 0 ; i < node_count ; i++)
		assert_ptr_equal(avl_remove_max(t), &n[node_count - 1 - i]);

	for (int i = 0 ; i < node_count / 2 ; i++)
	{
		assert_false(avl_insert(t, &n[i]));
		assert_false(avl_insert(t, &n[node_count - 1 - i]));
	}

	for (int i = 0 ; i < node_count ; i++)
		assert_ptr_equal(avl_remove_max(t), &n[node_count - 1 - i]);

	test_free(n);
	test_free(buf);
	test_free(t);
}

/***********************************************************************/

static void
UT_avl_remove_max__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_remove_max(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_remove_max__empty()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	assert_null(avl_remove_max(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_remove_max__NULL()
{
	expect_assert_failure(avl_remove_max(NULL));
}

/***********************************************************************/

static void
UT_avl_find()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * buf = test_malloc(2 * node_count);

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf[2 * i] = 'a' + i;
		buf[2 * i + 1] = '\0';
		avl_set(&n[i], &buf[2 * i]);
	}

	for (int i = 0 ; i < node_count ; i += 2)
	{
		assert_null(avl_find(t, avl_get(&n[i])));
		assert_false(avl_insert(t, &n[i]));
		assert_ptr_equal(avl_find(t, avl_get(&n[i])), &n[i]);
	}

	for (int i = node_count - 1 ; i >= 0 ; i--)
	{
		if (!(i % 2))
			assert_ptr_equal(avl_find(t, avl_get(&n[i])), &n[i]);
		else
			assert_null(avl_find(t, avl_get(&n[i])));
	}

	for (int i = 0 ; i < node_count ; i++)
	{
		if (avl_find(t, avl_get(&n[i])))
		{
			assert_ptr_equal(avl_remove(t, avl_get(&n[i])), &n[i]);
			assert_null(avl_find(t, avl_get(&n[i])));
		}
	}

	test_free(n);
	test_free(t);
	test_free(buf);
}

/***********************************************************************/

static void
UT_avl_find__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_find(t, "foobar"));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_find__empty()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	assert_null(avl_find(t, "foobar"));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_find__NULL()
{
	expect_assert_failure(avl_find(NULL, NULL));
	expect_assert_failure(avl_find(NULL, (void*)0x1337));
}

/***********************************************************************/

static int
_assert_preorder(struct avl_node const * n, void * p)
{
	assert_non_null(n);
	assert_non_null(p);

	char const ** prev = (char const **)p,
	           * cur = (char const *)avl_get(n);

	if (*prev)
		assert_true(strcmp(*prev, cur) <= 0);

	*prev = cur;

	return 0;
}

static int
_assert_count(struct avl_node const * n, void * c)
{
	assert_non_null(n);
	assert_non_null(c);

	int * count = (int *)c;

	++*count;

	return 0;
}

static void
UT_avl_iterate()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * buf = test_malloc(2 * node_count),
	     * prev;
	int count;

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf[2 * i] = 'a' + i;
		buf[2 * i + 1] = '\0';
		avl_set(&n[i], &buf[2 * i]);

		prev = NULL;
		assert_false(avl_iterate(t, _assert_preorder, &prev));
		count = 0;
		assert_false(avl_iterate(t, _assert_count, &count));
		assert_int_equal(count, i);

		assert_false(avl_insert(t, &n[i]));

		prev = NULL;
		assert_false(avl_iterate(t, _assert_preorder, &prev));
		count = 0;
		assert_false(avl_iterate(t, _assert_count, &count));
		assert_int_equal(count, i + 1);
	}

	for (int i = 0 ; i < node_count ; i++)
	{

		prev = NULL;
		assert_false(avl_iterate(t, _assert_preorder, &prev));
		count = 0;
		assert_false(avl_iterate(t, _assert_count, &count));
		assert_int_equal(count, node_count - i);

		if (i % 2)
			assert_non_null(avl_remove_min(t));
		else
			assert_non_null(avl_remove_max(t));

		prev = NULL;
		assert_false(avl_iterate(t, _assert_preorder, &prev));
		count = 0;
		assert_false(avl_iterate(t, _assert_count, &count));
		assert_int_equal(count, node_count - i - 1);
	}


	test_free(n);
	test_free(t);
	test_free(buf);
}

/***********************************************************************/

static void
UT_avl_iterate__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_iterate(t, _iteration_dummy_fn, (void*)0x5315));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_iterate__empty()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	assert_false(avl_iterate(t, _iteration_dummy_fn, NULL));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_iterate__NULL()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(sizeof(struct avl_node));
	int count = 0;

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);
	avl_set(n, "foobar");

	assert_false(avl_insert(t, n));

	expect_assert_failure(avl_iterate(NULL, NULL, NULL));

	expect_assert_failure(avl_iterate(t, NULL, NULL));
	expect_assert_failure(avl_iterate(NULL, _assert_count, &count));

	test_free(t);
	test_free(n);
}

/***********************************************************************/

#if !defined(NDEBUG) || defined(UNIT_TESTING)

static void
UT_avl_verify()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	char * buf = test_malloc(2 * node_count);

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf[2 * i] = 'a' + i;
		buf[2 * i + 1] = '\0';
		avl_set(&n[i], &buf[2 * i]);

		assert_false(avl_insert(t, &n[i]));
	}

	assert_false(avl_verify(t));

	t->root->left = NULL;
	assert_true(avl_verify(t));

	test_free(n);
	test_free(t);
	test_free(buf);
}

/***********************************************************************/

static void
UT_avl_verify__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_verify(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_verify__empty()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	assert_false(avl_verify(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_verify__NULL()
{
	expect_assert_failure(avl_verify(NULL));
}

/***********************************************************************/

static void
UT_avl_print()
{
	enum { node_count = 26 };
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));
	struct avl_node * n = test_malloc(node_count * sizeof(struct avl_node));
	static const char res[] =
		"/==============================\\\n" \
		"                / a(0)\n" \
		"            / b(1)\n" \
		"        / c(2)\n" \
		"                / d(0)\n" \
		"            \\ e(1)\n" \
		"    / f(3)\n" \
		"                / g(0)\n" \
		"            / h(1)\n" \
		"        \\ i(2)\n" \
		"                / j(0)\n" \
		"            \\ k(1)\n" \
		"-l(5)\n" \
		"                / m(0)\n" \
		"            / n(1)\n" \
		"        / o(2)\n" \
		"                / p(0)\n" \
		"            \\ q(1)\n" \
		"    \\ r(4)\n" \
		"                / s(0)\n" \
		"            / t(1)\n" \
		"        \\ u(3)\n" \
		"                    / v(0)\n" \
		"                / w(1)\n" \
		"            \\ x(2)\n" \
		"                \\ y(1)\n" \
		"                    \\ z(0)\n" \
		"\\==============================/\n";
	char * buf1 = test_malloc(2 * node_count),
	     * buf2 = test_malloc(2 * strlen(res));
	FILE * out, *orig_out;

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	for (int i = 0 ; i < node_count ; i++)
	{
		buf1[2 * i] = 'a' + i;
		buf1[2 * i + 1] = '\0';
		avl_set(&n[i], &buf1[2 * i]);
		assert_false(avl_insert(t, &n[i]));
	}

	out = fmemopen(buf2, sizeof(res), "w");
	assert_non_null(out);
	orig_out = stdout;
	stdout = out;
	avl_print(t);
	stdout = orig_out;
	fclose(out);

	assert_string_equal(buf2, res);

	test_free(n);
	test_free(t);
	test_free(buf1);
	test_free(buf2);
}

/***********************************************************************/

static void
UT_avl_print__uninitialized_tree()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	memset(t, 0, sizeof(*t));

	expect_assert_failure(avl_print(t));

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_print__empty()
{
	struct avl_tree * t = test_malloc(sizeof(struct avl_tree));

	avl_initialize(t, (int(*)(void const *, void const *))strcmp);

	avl_print(t);

	test_free(t);
}

/***********************************************************************/

static void
UT_avl_print__NULL()
{
	expect_assert_failure(avl_print(NULL));
}

#endif

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

#if 0
	// avl_next() not implemented
	assert_ptr_equal(avl_next(m, NULL), &ns[1]);
	assert_ptr_equal(avl_next(m, &ns[1]), &ns[3]);
	assert_ptr_equal(avl_next(m, &ns[3]), &ns[5]);
	assert_ptr_equal(avl_next(m, &ns[5]), &ns[7]);
	assert_ptr_equal(avl_next(m, &ns[7]), &ns[10]);
	assert_ptr_equal(avl_next(m, &ns[10]), &ns[12]);
	assert_ptr_equal(avl_next(m, &ns[12]), &ns[15]);
	assert_ptr_equal(avl_next(m, &ns[15]), &ns[25]);
	assert_ptr_equal(avl_next(m, &ns[25]), &ns[30]);
	assert_ptr_equal(avl_next(m, &ns[30]), &ns[34]);
	assert_ptr_equal(avl_next(m, &ns[34]), NULL);
#endif

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
		cmocka_unit_test(UT_avl_initialize),
		cmocka_unit_test(UT_avl_initialize__NULL),

		cmocka_unit_test(UT_avl_insert),
		cmocka_unit_test(UT_avl_insert__NULL),
		cmocka_unit_test(UT_avl_insert__uninitialized_tree),
		cmocka_unit_test(UT_avl_insert__uninitialized_node),

		cmocka_unit_test(UT_avl_remove),
		cmocka_unit_test(UT_avl_remove__NULL),
		cmocka_unit_test(UT_avl_remove__uninitialized_tree),

		cmocka_unit_test(UT_avl_set),
		cmocka_unit_test(UT_avl_set__NULL),

		cmocka_unit_test(UT_avl_get),
		cmocka_unit_test(UT_avl_get__NULL),

		cmocka_unit_test(UT_avl_size),
		cmocka_unit_test(UT_avl_size__NULL),
		cmocka_unit_test(UT_avl_size__uninitialized_tree),

		cmocka_unit_test(UT_avl_empty),
		cmocka_unit_test(UT_avl_empty__NULL),
		cmocka_unit_test(UT_avl_empty__uninitialized_tree),

		cmocka_unit_test(UT_avl_find_min),
		cmocka_unit_test(UT_avl_find_min__empty),
		cmocka_unit_test(UT_avl_find_min__NULL),
		cmocka_unit_test(UT_avl_find_min__uninitialized_tree),

		cmocka_unit_test(UT_avl_find_max),
		cmocka_unit_test(UT_avl_find_max__empty),
		cmocka_unit_test(UT_avl_find_max__NULL),
		cmocka_unit_test(UT_avl_find_max__uninitialized_tree),

		cmocka_unit_test(UT_avl_remove_min),
		cmocka_unit_test(UT_avl_remove_min__uninitialized_tree),
		cmocka_unit_test(UT_avl_remove_min__empty),
		cmocka_unit_test(UT_avl_remove_min__NULL),

		cmocka_unit_test(UT_avl_remove_max),
		cmocka_unit_test(UT_avl_remove_max__uninitialized_tree),
		cmocka_unit_test(UT_avl_remove_max__empty),
		cmocka_unit_test(UT_avl_remove_max__NULL),

		cmocka_unit_test(UT_avl_find),
		cmocka_unit_test(UT_avl_find__uninitialized_tree),
		cmocka_unit_test(UT_avl_find__empty),
		cmocka_unit_test(UT_avl_find__NULL),

		cmocka_unit_test(UT_avl_iterate),
		cmocka_unit_test(UT_avl_iterate__uninitialized_tree),
		cmocka_unit_test(UT_avl_iterate__empty),
		cmocka_unit_test(UT_avl_iterate__NULL),

#if !defined(NDEBUG) || defined(UNIT_TESTING)

		cmocka_unit_test(UT_avl_verify),
		cmocka_unit_test(UT_avl_verify__uninitialized_tree),
		cmocka_unit_test(UT_avl_verify__empty),
		cmocka_unit_test(UT_avl_verify__NULL),

		cmocka_unit_test(UT_avl_print),
		cmocka_unit_test(UT_avl_print__uninitialized_tree),
		cmocka_unit_test(UT_avl_print__empty),
		cmocka_unit_test(UT_avl_print__NULL),

#endif

		cmocka_unit_test(UT_AVL_TREE_INITIALIZER),

		cmocka_unit_test(FT_basic_usage),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
