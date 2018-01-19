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
#include <sys/types.h>
#include "doubly_linked_list.h"
#include "cmocka-wrapper.h"

#ifndef NULL
# define NULL ((void*)0)
#endif

static const char * TEST_DATA[] =
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

enum { TEST_DATA_LEN = sizeof(TEST_DATA) / sizeof(char*) };

/***********************************************************************/

static void
FT_basic_usage__1()
{
	struct doubly_linked_list_meta dlmeta;
	struct doubly_linked_list ll_bufs[TEST_DATA_LEN], *t;

	for (int i = TEST_DATA_LEN - 1; i >= 0; --i)
		doubly_linked_list_set(&ll_bufs[i], TEST_DATA[i]);

	doubly_linked_list_initialize(&dlmeta);

	for (int i = 0; i < TEST_DATA_LEN; ++i)
	{
		assert_true(doubly_linked_list_empty(&dlmeta));
		assert_null(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[i]));
		assert_null(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[i]));
		assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), NULL);
		assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), NULL);;
		assert_int_equal(doubly_linked_list_size(&dlmeta), 0);

		if (i % 2)
			doubly_linked_list_insert_front(&dlmeta, &ll_bufs[i]);
		else
			doubly_linked_list_insert_back(&dlmeta, &ll_bufs[i]);

		assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), &ll_bufs[i]);
		assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), &ll_bufs[i]);

		assert_false(doubly_linked_list_empty(&dlmeta));
		assert_int_equal(doubly_linked_list_size(&dlmeta), 1);

		t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[i]);
		assert_non_null(t);
		assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[i]);
		assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[i]), t);

		if (i % 3 % 2)
			t = doubly_linked_list_remove_front(&dlmeta, TEST_DATA[i]);
		else
			t = doubly_linked_list_remove_back(&dlmeta, TEST_DATA[i]);
		assert_non_null(t);
		assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[i]);

		assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[i]));
		assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[i]));

		assert_null(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[i]));
		assert_null(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[i]));
	}
}

/***********************************************************************/

static void
FT_basic_usage__2()
{
	struct doubly_linked_list_meta dlmeta;
	struct doubly_linked_list ll_bufs[TEST_DATA_LEN], *t;

	for (int i = TEST_DATA_LEN - 1; i >= 0; --i)
		doubly_linked_list_set(&ll_bufs[i], TEST_DATA[i]);

	doubly_linked_list_initialize(&dlmeta);

	assert_true(doubly_linked_list_empty(&dlmeta));

	for (int i = 0; i < TEST_DATA_LEN; ++i)
	{
		assert_int_equal(doubly_linked_list_size(&dlmeta), i);

		if (i % 2)
			doubly_linked_list_insert_front(&dlmeta, &ll_bufs[i]);
		else
			doubly_linked_list_insert_back(&dlmeta, &ll_bufs[i]);

		assert_false(doubly_linked_list_empty(&dlmeta));
		assert_int_equal(doubly_linked_list_size(&dlmeta), i + 1);
	}

	for (int i = 0; i < TEST_DATA_LEN; ++i)
	{
		t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[i]);
		assert_non_null(t);
		assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[i]);
		assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[i]), t);

		assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
		assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);
	}

	for (int i = 0; i < TEST_DATA_LEN; ++i)
	{
		if (i % 2)
			t = doubly_linked_list_remove_front(&dlmeta, TEST_DATA[i]);
		else
			t = doubly_linked_list_remove_back(&dlmeta, TEST_DATA[i]);
		assert_non_null(t);
		assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[i]);

		assert_int_equal(doubly_linked_list_size(&dlmeta), TEST_DATA_LEN - i - 1);
		assert_true(doubly_linked_list_empty(&dlmeta) || i < TEST_DATA_LEN);
	}

	assert_true(doubly_linked_list_empty(&dlmeta));

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), NULL);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), NULL);

	for (int i = 0; i < TEST_DATA_LEN; ++i)
	{
		assert_false(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[i]));
		assert_false(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[i]));
	}
}

/***********************************************************************/

static void
FT_basic_usage__3()
{
	struct doubly_linked_list_meta dlmeta;
	struct doubly_linked_list ll_bufs[TEST_DATA_LEN], *t;

	for (int i = TEST_DATA_LEN - 1; i >= 0; --i)
		doubly_linked_list_set(&ll_bufs[i], TEST_DATA[i]);

	doubly_linked_list_initialize(&dlmeta);

	assert_true(doubly_linked_list_empty(&dlmeta));
	assert_null(doubly_linked_list_remove_head(&dlmeta));
	assert_null(doubly_linked_list_remove_tail(&dlmeta));

	for (int i = 0; i < TEST_DATA_LEN; ++i)
	{
		assert_int_equal(doubly_linked_list_size(&dlmeta), i);

		if (i % 2)
			doubly_linked_list_insert_back(&dlmeta, &ll_bufs[i]);
		else
			doubly_linked_list_insert_front(&dlmeta, &ll_bufs[i]);

		assert_false(doubly_linked_list_empty(&dlmeta));
		assert_int_equal(doubly_linked_list_size(&dlmeta), i + 1);
	}

	for (int i = 0; i < TEST_DATA_LEN; ++i)
	{
		t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[i]);
		assert_non_null(t);
		assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[i]);
		assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[i]), t);
	}

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	t = doubly_linked_list_remove_back(&dlmeta, TEST_DATA[2]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[2]);

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	assert_int_equal(doubly_linked_list_size(&dlmeta), TEST_DATA_LEN - 1);
	assert_false(doubly_linked_list_empty(&dlmeta));

	t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[0]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[0]);
	assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[0]), t);

	t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[1]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[1]);
	assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[1]), t);

	assert_null(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[2]));
	assert_null(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[2]));

	for (int i = 3; i < TEST_DATA_LEN; ++i)
	{
		t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[i]);
		assert_non_null(t);
		assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[i]);
		assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[i]), t);
	}

	assert_false(doubly_linked_list_empty(&dlmeta));

	t = doubly_linked_list_remove_front(&dlmeta, TEST_DATA[3]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[3]);

	t = doubly_linked_list_remove_front(&dlmeta, TEST_DATA[5]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[5]);

	t = doubly_linked_list_remove_front(&dlmeta, TEST_DATA[7]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[7]);

	assert_int_equal(doubly_linked_list_size(&dlmeta), TEST_DATA_LEN - 4);

	assert_false(doubly_linked_list_empty(&dlmeta));

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[0]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[0]);
	assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[0]), t);

	t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[1]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[1]);
	assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[1]), t);

	assert_null(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[2]));
	assert_null(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[2]));

	assert_null(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[3]));
	assert_null(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[3]));

	t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[4]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[4]);
	assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[4]), t);

	assert_null(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[5]));
	assert_null(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[5]));

	t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[6]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[6]);
	assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[6]), t);

	assert_false(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[7]));
	assert_false(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[7]));

	t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[8]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[8]);
	assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[8]), t);

	t = doubly_linked_list_find_front(dlmeta.head, TEST_DATA[9]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[9]);
	assert_ptr_equal(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[9]), t);

	t = doubly_linked_list_remove_back(&dlmeta, TEST_DATA[0]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[0]);

	t = doubly_linked_list_remove_front(&dlmeta, TEST_DATA[1]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[1]);

	t = doubly_linked_list_remove_back(&dlmeta, TEST_DATA[4]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[4]);

	t = doubly_linked_list_remove_back(&dlmeta, TEST_DATA[6]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[6]);

	t = doubly_linked_list_remove_back(&dlmeta, TEST_DATA[8]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[8]);

	t = doubly_linked_list_remove_front(&dlmeta, TEST_DATA[9]);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[9]);

	assert_true(doubly_linked_list_empty(&dlmeta));
	assert_int_equal(doubly_linked_list_size(&dlmeta), 0);
	assert_null(doubly_linked_list_remove_head(&dlmeta));
	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), NULL);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), NULL);

	for (int i = 0; i < TEST_DATA_LEN; ++i)
	{
		assert_false(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[i]));
		assert_false(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[i]));
	}
}

/***********************************************************************/

static void
FT_basic_usage__4()
{
	struct doubly_linked_list_meta dlmeta;
	struct doubly_linked_list ll_bufs[TEST_DATA_LEN], *t;

	for (int i = TEST_DATA_LEN - 1; i >= 0; --i)
		doubly_linked_list_set(&ll_bufs[i], TEST_DATA[i]);

	doubly_linked_list_initialize(&dlmeta);

	assert_true(doubly_linked_list_empty(&dlmeta));

	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[5]);
	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), &ll_bufs[5]);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), &ll_bufs[5]);

	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[8]);

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[2]);

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[6]);

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[7]);

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[3]);

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[1]);

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[4]);

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[9]);

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), dlmeta.head);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), dlmeta.tail);

	assert_false(doubly_linked_list_empty(&dlmeta));

	assert_ptr_equal(doubly_linked_list_next(&ll_bufs[4]), &ll_bufs[3]);
	assert_ptr_equal(doubly_linked_list_next(&ll_bufs[3]), &ll_bufs[6]);
	assert_ptr_equal(doubly_linked_list_next(&ll_bufs[6]), &ll_bufs[8]);
	assert_ptr_equal(doubly_linked_list_next(&ll_bufs[8]), &ll_bufs[5]);
	assert_ptr_equal(doubly_linked_list_next(&ll_bufs[5]), &ll_bufs[2]);
	assert_ptr_equal(doubly_linked_list_next(&ll_bufs[2]), &ll_bufs[7]);
	assert_ptr_equal(doubly_linked_list_next(&ll_bufs[7]), &ll_bufs[1]);
	assert_ptr_equal(doubly_linked_list_next(&ll_bufs[1]), &ll_bufs[9]);
	assert_ptr_equal(doubly_linked_list_next(&ll_bufs[9]), NULL);

	assert_ptr_equal(doubly_linked_list_previous(&ll_bufs[3]), &ll_bufs[4]);
	assert_ptr_equal(doubly_linked_list_previous(&ll_bufs[6]), &ll_bufs[3]);
	assert_ptr_equal(doubly_linked_list_previous(&ll_bufs[8]), &ll_bufs[6]);
	assert_ptr_equal(doubly_linked_list_previous(&ll_bufs[5]), &ll_bufs[8]);
	assert_ptr_equal(doubly_linked_list_previous(&ll_bufs[2]), &ll_bufs[5]);
	assert_ptr_equal(doubly_linked_list_previous(&ll_bufs[7]), &ll_bufs[2]);
	assert_ptr_equal(doubly_linked_list_previous(&ll_bufs[1]), &ll_bufs[7]);
	assert_ptr_equal(doubly_linked_list_previous(&ll_bufs[9]), &ll_bufs[1]);
	assert_ptr_equal(doubly_linked_list_previous(&ll_bufs[4]), NULL);

	assert_int_equal(doubly_linked_list_size(&dlmeta), 9);

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[4]);

	assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[4]));
	assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[4]));

	t = doubly_linked_list_remove_tail(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[9]);

	assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[9]));
	assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[9]));

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[3]);

	assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[3]));
	assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[3]));

	t = doubly_linked_list_remove_tail(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[1]);

	assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[1]));
	assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[1]));

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[6]);

	assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[6]));
	assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[6]));

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[8]);

	assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[8]));
	assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[8]));

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[5]);

	assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[5]));
	assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[5]));

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[2]);

	assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[2]));
	assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[2]));

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), TEST_DATA[7]);

	assert_null(doubly_linked_list_remove_front(&dlmeta, TEST_DATA[7]));
	assert_null(doubly_linked_list_remove_back(&dlmeta, TEST_DATA[7]));

	assert_true(doubly_linked_list_empty(&dlmeta));
	assert_false(doubly_linked_list_remove_head(&dlmeta));

	assert_ptr_equal(doubly_linked_list_find_head(&dlmeta), NULL);
	assert_ptr_equal(doubly_linked_list_find_tail(&dlmeta), NULL);

	for (int i = 0; i < TEST_DATA_LEN; ++i)
	{
		assert_false(doubly_linked_list_find_front(dlmeta.head, TEST_DATA[i]));
		assert_false(doubly_linked_list_find_back(dlmeta.tail, TEST_DATA[i]));
	}
}

/***********************************************************************/

static void
FT_insert_duplicates()
{
	struct doubly_linked_list_meta dlmeta;
	struct doubly_linked_list ll_bufs[18], *t;

	doubly_linked_list_set(&ll_bufs[0], "a");
	doubly_linked_list_set(&ll_bufs[1], "a");
	doubly_linked_list_set(&ll_bufs[2], "a");
	doubly_linked_list_set(&ll_bufs[3], "a");
	doubly_linked_list_set(&ll_bufs[4], "a");
	doubly_linked_list_set(&ll_bufs[5], "a");

	doubly_linked_list_set(&ll_bufs[6], "b");
	doubly_linked_list_set(&ll_bufs[7], "b");
	doubly_linked_list_set(&ll_bufs[8], "b");
	doubly_linked_list_set(&ll_bufs[9], "b");
	doubly_linked_list_set(&ll_bufs[10], "b");
	doubly_linked_list_set(&ll_bufs[11], "b");

	doubly_linked_list_set(&ll_bufs[12], "c");
	doubly_linked_list_set(&ll_bufs[13], "c");
	doubly_linked_list_set(&ll_bufs[14], "c");
	doubly_linked_list_set(&ll_bufs[15], "c");
	doubly_linked_list_set(&ll_bufs[16], "c");
	doubly_linked_list_set(&ll_bufs[17], "c");

	doubly_linked_list_initialize(&dlmeta);

	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[0]);
	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[1]);
	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[2]);
	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[3]);
	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[4]);
	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[5]);

	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[6]);
	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[7]);
	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[8]);
	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[9]);
	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[10]);
	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[11]);

	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[12]);
	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[13]);
	doubly_linked_list_insert_front(&dlmeta, &ll_bufs[14]);
	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[15]);
	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[16]);
	doubly_linked_list_insert_back(&dlmeta, &ll_bufs[17]);

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "c");
	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "c");
	t = doubly_linked_list_remove_tail(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "c");

	t = doubly_linked_list_remove_front(&dlmeta, &"c");
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "c");
	t = doubly_linked_list_remove_back(&dlmeta, &"c");
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "c");
	t = doubly_linked_list_remove_front(&dlmeta, &"c");
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "c");

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "b");
	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "b");
	t = doubly_linked_list_remove_tail(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "b");

	t = doubly_linked_list_remove_front(&dlmeta, &"b");
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "b");
	t = doubly_linked_list_remove_back(&dlmeta, &"b");
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "b");
	t = doubly_linked_list_remove_front(&dlmeta, &"b");
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "b");

	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "a");
	t = doubly_linked_list_remove_head(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "a");
	t = doubly_linked_list_remove_tail(&dlmeta);
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "a");

	t = doubly_linked_list_remove_front(&dlmeta, &"a");
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "a");
	t = doubly_linked_list_remove_back(&dlmeta, &"a");
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "a");
	t = doubly_linked_list_remove_front(&dlmeta, &"a");
	assert_non_null(t);
	assert_string_equal((char*)doubly_linked_list_get(t), "a");
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
		cmocka_unit_test(FT_insert_duplicates)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
