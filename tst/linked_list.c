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
#include <string.h>
#include <sys/types.h>
#include "cmocka-wrapper.h"
#include "linked_list.h"

static const char * data_bufs[] =
{
	"aa",
	"bb",
	"cc",
	"dd",
	"ee",
	"ff",
	"gg",
	"hh",
	"ii",
	"jj"
};

enum { bufs_len = sizeof(data_bufs) / sizeof(char*) };

/***********************************************************************/

static int
_assert_iteration_functionality(const struct linked_list *n, int *c)
{
	static const struct linked_list *p = NULL;

	if (*c && n == p)
	{
		printf("Iterating same node more than once?\n");
		return -1;
	}

	p = n;
	++*c;

	return 0;
}

static void
assert_iteration_functionality(const struct linked_list_meta *m)
{
	int s = linked_list_size(m),
	    c = 0,
	    t = linked_list_iterate(m, (int(*)(const struct linked_list *, void *))_assert_iteration_functionality, &c);

	assert_int_equal(s, c);
	assert_int_equal(t, 0);
}


static void __attribute__((unused))
print_list(struct linked_list_meta *m)
{
	struct linked_list **l = &m->head;

	printf("\n  Printing liked list:");

	while (*l)
	{
		printf(" <%s>", (char*)linked_list_get(*l));
		l = linked_list_next(*l);
	}

	printf("\n");
}

/***********************************************************************/

static void
FT_basic_usage__1()
{
	struct linked_list ll_bufs[bufs_len], **tt, *t;
	struct linked_list_meta llmeta;

	for (int i = bufs_len - 1; i >= 0; --i)
		linked_list_set(&ll_bufs[i], data_bufs[i]);

	linked_list_initialize(&llmeta);

	assert_true(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 0);
	assert_null(*linked_list_find_head(&llmeta));

	for (int i = 0; i < bufs_len; ++i)
	{
		assert_true(linked_list_empty(&llmeta));
		assert_int_equal(linked_list_size(&llmeta), 0);
		assert_null(*linked_list_find_head(&llmeta));

		assert_null(linked_list_find(&llmeta, data_bufs[i]));
		assert_null(linked_list_find_match(&llmeta, data_bufs[i], (int(*)(const void *, const void *))strcmp));

		if (i % 2)
			assert_false(linked_list_insert_front(&llmeta, &ll_bufs[i]));
		else
			assert_false(linked_list_insert_back(&llmeta, &ll_bufs[i]));
		assert_true(linked_list_insert_front(&llmeta, &ll_bufs[i]));
		assert_true(linked_list_insert_back(&llmeta, &ll_bufs[i]));

		assert_false(linked_list_empty(&llmeta));
		assert_false(linked_list_size(&llmeta) <= 0);
		assert_non_null(*linked_list_find_head(&llmeta));

		assert_int_equal(linked_list_size(&llmeta), 1);

		assert_iteration_functionality(&llmeta);

		tt = linked_list_find(&llmeta, data_bufs[i]);
		assert_non_null(tt);
		assert_string_equal((char*)linked_list_get(*tt), data_bufs[i]);
		assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[i], (int(*)(const void *, const void *))strcmp));

		t = linked_list_remove(&llmeta, data_bufs[i]);
		assert_null(linked_list_remove_match(&llmeta, data_bufs[i], (int(*)(const void *, const void *))strcmp));
		assert_non_null(t);
		assert_string_equal((char*)linked_list_get(t), data_bufs[i]);

		assert_null(linked_list_remove(&llmeta, data_bufs[i]));
	}
}

/***********************************************************************/

static void
FT_basic_usage__2()
{
	struct linked_list ll_bufs[bufs_len], **tt, *t;
	struct linked_list_meta llmeta;

	for (int i = bufs_len - 1; i >= 0; --i)
		linked_list_set(&ll_bufs[i], data_bufs[i]);

	linked_list_initialize(&llmeta);

	assert_true(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 0);
	assert_null(*linked_list_find_head(&llmeta));

	for (int i = 0; i < bufs_len; ++i)
	{
		if (i % 2)
			assert_false(linked_list_insert_front(&llmeta, &ll_bufs[i]));
		else
			assert_false(linked_list_insert_back(&llmeta, &ll_bufs[i]));
	}

	assert_iteration_functionality(&llmeta);

	assert_false(linked_list_empty(&llmeta));
	assert_false(linked_list_size(&llmeta) <= 0);
	assert_non_null(*linked_list_find_head(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), bufs_len);

	for (int i = 0; i < bufs_len; ++i)
	{
		tt = linked_list_find(&llmeta, data_bufs[i]);
		assert_non_null(tt);
		assert_string_equal((char*)linked_list_get(*tt), data_bufs[i]);
		assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[i],
					(int(*)(const void *, const void *))strcmp));
	}

	for (int i = 0; i < bufs_len; ++i)
	{
		t = linked_list_remove(&llmeta, data_bufs[i]);
		assert_null(linked_list_remove_match(&llmeta, data_bufs[i],
					(int(*)(const void *, const void *))strcmp));
		assert_non_null(t);
		assert_string_equal((char*)linked_list_get(t), data_bufs[i]);
	}

	assert_true(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 0);
	assert_null(*linked_list_find_head(&llmeta));

	for (int i = 0; i < bufs_len; ++i)
	{
		assert_null(linked_list_find(&llmeta, data_bufs[i]));
		assert_null(linked_list_find_match(&llmeta, data_bufs[i], (int(*)(const void *, const void *))strcmp));
	}
}

/***********************************************************************/

static void
FT_basic_usage__3()
{
	struct linked_list ll_bufs[bufs_len], **tt, *t;
	struct linked_list_meta llmeta;

	for (int i = bufs_len - 1; i >= 0; --i)
		linked_list_set(&ll_bufs[i], data_bufs[i]);

	linked_list_initialize(&llmeta);

	assert_true(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 0);
	assert_null(*linked_list_find_head(&llmeta));

	for (int i = 0; i < bufs_len; ++i)
	{
		if (i % 2)
			assert_false(linked_list_insert_back(&llmeta, &ll_bufs[i]));
		else
			assert_false(linked_list_insert_front(&llmeta, &ll_bufs[i]));
	}

	assert_int_equal(linked_list_size(&llmeta), bufs_len);
	assert_false(linked_list_empty(&llmeta));
	assert_non_null(*linked_list_find_head(&llmeta));

	for (int i = 0; i < bufs_len; ++i)
	{
		tt = linked_list_find(&llmeta, data_bufs[i]);
		assert_non_null(tt);
		assert_string_equal((char*)linked_list_get(*tt), data_bufs[i]);
		assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[i],
				(int(*)(const void *, const void *))strcmp));
	}

	t = linked_list_remove(&llmeta, data_bufs[2]);
	assert_null(linked_list_remove_match(&llmeta, data_bufs[2],
				(int(*)(const void *, const void *))strcmp));
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[2]);

	assert_int_equal(linked_list_size(&llmeta), bufs_len - 1);

	assert_false(linked_list_empty(&llmeta));
	assert_false(linked_list_size(&llmeta) <= 0);
	assert_non_null(*linked_list_find_head(&llmeta));

	tt = linked_list_find(&llmeta, data_bufs[0]);
	assert_non_null(tt);
	assert_string_equal((char*)linked_list_get(*tt), data_bufs[0]);
	assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[0],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find(&llmeta, data_bufs[1]);
	assert_non_null(tt);
	assert_string_equal((char*)linked_list_get(*tt), data_bufs[1]);
	assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[1],
				(int(*)(const void *, const void *))strcmp));

	assert_null(linked_list_find(&llmeta, data_bufs[2]));
	assert_null(linked_list_find_match(&llmeta, data_bufs[2],
				(int(*)(const void *, const void *))strcmp));

	for (int i = 3; i < bufs_len; ++i)
	{
		tt = linked_list_find(&llmeta, data_bufs[i]);
		assert_non_null(tt);
		assert_string_equal((char*)linked_list_get(*tt), data_bufs[i]);
		assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[i],
					(int(*)(const void *, const void *))strcmp));
	}

	assert_false(linked_list_empty(&llmeta));
	assert_false(linked_list_size(&llmeta) <= 0);
	assert_non_null(*linked_list_find_head(&llmeta));

	t = linked_list_remove(&llmeta, data_bufs[3]);
	assert_null(linked_list_remove_match(&llmeta, data_bufs[3],
				(int(*)(const void *, const void *))strcmp));
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[3]);

	t = linked_list_remove_match(&llmeta, data_bufs[5],
			(int(*)(const void *, const void *))strcmp);
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[5]);

	t = linked_list_remove(&llmeta, data_bufs[7]);
	assert_null(linked_list_remove_match(&llmeta, data_bufs[7],
				(int(*)(const void *, const void *))strcmp));
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[7]);

	assert_int_equal(linked_list_size(&llmeta), bufs_len - 4);

	assert_false(linked_list_empty(&llmeta));
	assert_false(linked_list_size(&llmeta) <= 0);
	assert_non_null(*linked_list_find_head(&llmeta));

	tt = linked_list_find(&llmeta, data_bufs[0]);
	assert_non_null(tt);
	assert_string_equal((char*)linked_list_get(*tt), data_bufs[0]);
	assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[0],
			(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find(&llmeta, data_bufs[1]);
	assert_non_null(tt);
	assert_string_equal((char*)linked_list_get(*tt), data_bufs[1]);
	assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[1],
				(int(*)(const void *, const void *))strcmp));

	assert_null(linked_list_find(&llmeta, data_bufs[2]));
	assert_null(linked_list_find_match(&llmeta, data_bufs[2],
				(int(*)(const void *, const void *))strcmp));

	assert_null(linked_list_find(&llmeta, data_bufs[3]));
	assert_null(linked_list_find_match(&llmeta, data_bufs[3],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find(&llmeta, data_bufs[4]);
	assert_non_null(tt);
	assert_string_equal((char*)linked_list_get(*tt), data_bufs[4]);
	assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[4],
				(int(*)(const void *, const void *))strcmp));

	assert_null(linked_list_find(&llmeta, data_bufs[5]));
	assert_null(linked_list_find_match(&llmeta, data_bufs[5],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find(&llmeta, data_bufs[6]);
	assert_non_null(tt);
	assert_string_equal((char*)linked_list_get(*tt), data_bufs[6]);
	assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[6],
				(int(*)(const void *, const void *))strcmp));

	assert_null(linked_list_find(&llmeta, data_bufs[7]));
	assert_null(linked_list_find_match(&llmeta, data_bufs[7],
			(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find(&llmeta, data_bufs[8]);
	assert_non_null(tt);
	assert_string_equal((char*)linked_list_get(*tt), data_bufs[8]);
	assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[8],
			(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find(&llmeta, data_bufs[9]);
	assert_non_null(tt);
	assert_string_equal((char*)linked_list_get(*tt), data_bufs[9]);
	assert_ptr_equal(tt, linked_list_find_match(&llmeta, data_bufs[9],
			(int(*)(const void *, const void *))strcmp));

	assert_iteration_functionality(&llmeta);

	t = linked_list_remove(&llmeta, data_bufs[0]);
	assert_null(linked_list_remove_match(&llmeta, data_bufs[0],
				(int(*)(const void *, const void *))strcmp));
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[0]);

	t = linked_list_remove_match(&llmeta, data_bufs[1],
		(int(*)(const void *, const void *))strcmp);
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[1]);

	t = linked_list_remove(&llmeta, data_bufs[4]);
	assert_null(linked_list_remove_match(&llmeta, data_bufs[4],
			(int(*)(const void *, const void *))strcmp));
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[4]);

	assert_iteration_functionality(&llmeta);

	t = linked_list_remove_match(&llmeta, data_bufs[6],
		(int(*)(const void *, const void *))strcmp);
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[6]);

	t = linked_list_remove(&llmeta, data_bufs[8]);
	assert_null(linked_list_remove_match(&llmeta, data_bufs[8],
			(int(*)(const void *, const void *))strcmp));
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[8]);

	t = linked_list_remove_match(&llmeta, data_bufs[9],
		(int(*)(const void *, const void *))strcmp);
	assert_non_null(t);
	assert_string_equal((char*)linked_list_get(t), data_bufs[9]);

	assert_true(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 0);
	assert_null(*linked_list_find_head(&llmeta));

	for (int i = 0; i < bufs_len; ++i)
	{
		assert_null(linked_list_find(&llmeta, data_bufs[i]));
		assert_null(linked_list_find_match(&llmeta, data_bufs[i],
					(int(*)(const void *, const void *))strcmp));
	}
}

/***********************************************************************/

static void
FT_basic_usage__4()
{
	struct linked_list ll_bufs[bufs_len], **tt, *t;
	struct linked_list_meta llmeta;

	for (int i = bufs_len - 1; i >= 0; --i)
		linked_list_set(&ll_bufs[i], data_bufs[i]);

	linked_list_initialize(&llmeta);

	assert_true(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 0);
	assert_null(*linked_list_find_head(&llmeta));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[5]));

	assert_iteration_functionality(&llmeta);

	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[5]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[5]));

	assert_false(linked_list_insert_front(&llmeta, &ll_bufs[8]));

	assert_iteration_functionality(&llmeta);

	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[8]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[8]));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[2]));

	assert_iteration_functionality(&llmeta);

	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[2]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[2]));

	assert_false(linked_list_insert_front(&llmeta, &ll_bufs[6]));

	assert_iteration_functionality(&llmeta);

	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[6]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[6]));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[7]));

	assert_iteration_functionality(&llmeta);

	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[7]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[7]));

	assert_false(linked_list_insert_front(&llmeta, &ll_bufs[3]));

	assert_iteration_functionality(&llmeta);

	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[3]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[3]));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[1]));

	assert_iteration_functionality(&llmeta);

	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[1]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[1]));

	assert_false(linked_list_insert_front(&llmeta, &ll_bufs[4]));

	assert_iteration_functionality(&llmeta);

	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[4]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[4]));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[9]));

	assert_iteration_functionality(&llmeta);

	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[9]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[9]));

	assert_false(linked_list_empty(&llmeta));
	assert_non_null(*linked_list_find_head(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 9);

	assert_non_null(linked_list_next(&ll_bufs[4]));
	assert_ptr_equal(&ll_bufs[3], *linked_list_next(&ll_bufs[4]));

	assert_non_null(linked_list_next(&ll_bufs[3]));
	assert_ptr_equal(&ll_bufs[6], *linked_list_next(&ll_bufs[3]));

	assert_non_null(linked_list_next(&ll_bufs[6]));
	assert_ptr_equal(&ll_bufs[8], *linked_list_next(&ll_bufs[6]));

	assert_non_null(linked_list_next(&ll_bufs[8]));
	assert_ptr_equal(&ll_bufs[5], *linked_list_next(&ll_bufs[8]));

	assert_non_null(linked_list_next(&ll_bufs[5]));
	assert_ptr_equal(&ll_bufs[2], *linked_list_next(&ll_bufs[5]));

	assert_non_null(linked_list_next(&ll_bufs[2]));
	assert_ptr_equal(&ll_bufs[7], *linked_list_next(&ll_bufs[2]));

	assert_non_null(linked_list_next(&ll_bufs[7]));
	assert_ptr_equal(&ll_bufs[1], *linked_list_next(&ll_bufs[7]));

	assert_non_null(linked_list_next(&ll_bufs[1]));
	assert_ptr_equal(&ll_bufs[9], *linked_list_next(&ll_bufs[1]));

	assert_int_equal(linked_list_size(&llmeta), 9);

	tt = linked_list_find_head(&llmeta);
	assert_non_null(tt);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_head(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[4]);
	assert_ptr_not_equal(t, linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove(&llmeta, data_bufs[4]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[4],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_head(&llmeta);
	assert_non_null(tt);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_head(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[3]);
	assert_ptr_not_equal(t, linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove(&llmeta, data_bufs[3]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[3],
			(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_head(&llmeta);
	assert_non_null(tt);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_head(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[6]);
	assert_ptr_not_equal(t, linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove(&llmeta, data_bufs[6]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[6],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_head(&llmeta);
	assert_non_null(tt);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_head(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[8]);
	assert_ptr_not_equal(t, linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove(&llmeta, data_bufs[8]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[8],
			(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_head(&llmeta);
	assert_non_null(tt);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_head(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[5]);
	assert_ptr_not_equal(t, linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove(&llmeta, data_bufs[5]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[5],
			(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_head(&llmeta);
	assert_non_null(tt);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_head(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[2]);
	assert_ptr_not_equal(t, linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove(&llmeta, data_bufs[2]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[2],
			(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_head(&llmeta);
	assert_non_null(tt);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_head(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[7]);
	assert_ptr_not_equal(t, linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove(&llmeta, data_bufs[7]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[7],
			(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_head(&llmeta);
	assert_non_null(tt);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_head(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[1]);
	assert_ptr_not_equal(t, linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove(&llmeta, data_bufs[1]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[1],
			(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_head(&llmeta);
	assert_non_null(tt);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_head(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[9]);
	assert_ptr_not_equal(t, linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove(&llmeta, data_bufs[9]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[9],
			(int(*)(const void *, const void *))strcmp));

	assert_true(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 0);
	assert_null(*linked_list_find_head(&llmeta));

	assert_iteration_functionality(&llmeta);

	assert_null(linked_list_remove_head(&llmeta));

	for (int i = 0; i < bufs_len; ++i)
	{
		assert_null(linked_list_find(&llmeta, data_bufs[i]));
		assert_null(linked_list_find_match(&llmeta, data_bufs[i],
					(int(*)(const void *, const void *))strcmp));
	}
}

/***********************************************************************/

static void
FT_basic_usage__5()
{
	struct linked_list ll_bufs[bufs_len], **tt, *t;
	struct linked_list_meta llmeta;

	for (int i = bufs_len - 1; i >= 0; --i)
		linked_list_set(&ll_bufs[i], data_bufs[i]);

	linked_list_initialize(&llmeta);

	assert_true(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 0);
	assert_null(*linked_list_find_head(&llmeta));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[5]));
	assert_iteration_functionality(&llmeta);
	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[5]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[5]));

	assert_false(linked_list_insert_front(&llmeta, &ll_bufs[8]));
	assert_iteration_functionality(&llmeta);
	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[8]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[8]));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[2]));
	assert_iteration_functionality(&llmeta);
	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[2]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[2]));

	assert_false(linked_list_insert_front(&llmeta, &ll_bufs[6]));
	assert_iteration_functionality(&llmeta);
	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[6]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[6]));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[7]));
	assert_iteration_functionality(&llmeta);
	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[7]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[7]));

	assert_false(linked_list_insert_front(&llmeta, &ll_bufs[3]));
	assert_iteration_functionality(&llmeta);
	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[3]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[3]));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[1]));
	assert_iteration_functionality(&llmeta);
	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[1]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[1]));

	assert_false(linked_list_insert_front(&llmeta, &ll_bufs[4]));
	assert_iteration_functionality(&llmeta);
	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[4]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[4]));

	assert_false(linked_list_insert_back(&llmeta, &ll_bufs[9]));
	assert_iteration_functionality(&llmeta);
	assert_true(linked_list_insert_front(&llmeta, &ll_bufs[9]));
	assert_true(linked_list_insert_back(&llmeta, &ll_bufs[9]));

	assert_false(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 9);
	assert_non_null(*linked_list_find_head(&llmeta));

	tt = linked_list_find_tail(&llmeta);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_tail(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[9]);
	assert_ptr_not_equal(t, linked_list_find_tail(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove(&llmeta, data_bufs[9]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[9],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_tail(&llmeta);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_tail(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[1]);
	assert_ptr_not_equal(t, linked_list_find_tail(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove(&llmeta, data_bufs[1]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[1],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_tail(&llmeta);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_tail(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[7]);
	assert_ptr_not_equal(t, linked_list_find_tail(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove(&llmeta, data_bufs[7]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[7],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_tail(&llmeta);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_tail(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[2]);
	assert_ptr_not_equal(t, linked_list_find_tail(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove(&llmeta, data_bufs[2]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[2],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_tail(&llmeta);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_tail(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[5]);
	assert_ptr_not_equal(t, linked_list_find_tail(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove(&llmeta, data_bufs[5]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[5],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_tail(&llmeta);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_tail(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[8]);
	assert_ptr_not_equal(t, linked_list_find_tail(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove(&llmeta, data_bufs[8]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[8],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_tail(&llmeta);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_tail(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[6]);
	assert_ptr_not_equal(t, linked_list_find_tail(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove(&llmeta, data_bufs[6]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[6],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_tail(&llmeta);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_tail(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[3]);
	assert_ptr_not_equal(t, linked_list_find_tail(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove(&llmeta, data_bufs[3]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[3],
				(int(*)(const void *, const void *))strcmp));

	tt = linked_list_find_tail(&llmeta);
	t = *tt;
	assert_ptr_equal(t, linked_list_remove_tail(&llmeta));
	assert_string_equal((char*)linked_list_get(t), data_bufs[4]);
	assert_ptr_not_equal(t, linked_list_find_tail(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove(&llmeta, data_bufs[4]));
	assert_null(linked_list_remove_match(&llmeta, data_bufs[4],
				(int(*)(const void *, const void *))strcmp));

	assert_true(linked_list_empty(&llmeta));
	assert_int_equal(linked_list_size(&llmeta), 0);
	assert_null(*linked_list_find_head(&llmeta));
	assert_iteration_functionality(&llmeta);
	assert_null(linked_list_remove_head(&llmeta));

	for (int i = 0; i < bufs_len; ++i)
	{
		assert_null(linked_list_find(&llmeta, data_bufs[i]));
		assert_null(linked_list_find_match(&llmeta, data_bufs[i],
					(int(*)(const void *, const void *))strcmp));
	}
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(FT_basic_usage__1),
		cmocka_unit_test(FT_basic_usage__2),
		cmocka_unit_test(FT_basic_usage__3),
		cmocka_unit_test(FT_basic_usage__4),
		cmocka_unit_test(FT_basic_usage__5)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
