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

#include "cmocka-wrapper.h"

#define TYPED_LINKED_LIST_CONFIG_TYPE unsigned int
#define TYPED_LINKED_LIST_CONFIG_PREFIX uint_list
#include "typed_linked_list.h"

#define TYPED_LINKED_LIST_CONFIG_TYPE char
#define TYPED_LINKED_LIST_CONFIG_PREFIX char_list
#include "typed_linked_list.h"


/***********************************************************************/

static int
uint_iterator(uint_list_t const * n, void * p)
{
	assert(!p);

	static unsigned int prev = -1;
	unsigned int cur = *uint_list_get(n);

	assert_int_equal(prev, cur - 1);
	prev = cur;

	return 0;
}

static int
char_iterator(char_list_t const * n, void * p)
{
	assert(!p);

	static char prev = 'a' + 20;
	char cur = *uint_list_get(n);

	assert_int_equal(prev, cur + 1);
	prev = cur;

	return 0;
}

static int
char_cmp(char const * a, char const * b)
{
	assert(a);
	assert(b);

	if (*a < *b)
		return -1;
	else if (*a > *b)
		return 1;
	else
		return 0;
}

static int
uint_cmp(unsigned int const * a, unsigned int const * b)
{
	assert(a);
	assert(b);

	if (*a < *b)
		return -1;
	else if (*a > *b)
		return 1;
	else
		return 0;
}

static void
FT_basic_usage()
{
	enum { char_count = 20, uint_count = 20 };
	uint_list_meta_t * uint_list = test_malloc(sizeof(uint_list_meta_t));
	char_list_meta_t * char_list = test_malloc(sizeof(char_list_meta_t));
	char_list_t * charnodebuf = test_malloc(sizeof(char_list_t) * char_count);
	char * charbuf = test_malloc(sizeof(char) * char_count);
	uint_list_t * uintnodebuf = test_malloc(sizeof(uint_list_t) * uint_count);
	unsigned int * uintbuf = test_malloc(sizeof(unsigned int) * uint_count);

	assert_non_null(uint_list);
	assert_non_null(char_list);
	assert_non_null(charbuf);

	uint_list_initialize(uint_list);
	char_list_initialize(char_list);

	assert_true(uint_list_empty(uint_list));

	for (int i = 0 ; i < char_count ; i++)
	{
		charbuf[i] = 'a' + i;
		char_list_set(&charnodebuf[i], &charbuf[i]);
		uintbuf[i] = i;
		uint_list_set(&uintnodebuf[i], &uintbuf[i]);
	}

	for (int i = 0 ; i < char_count ; i++)
	{
		assert_int_equal(char_list_size(char_list), i);
		char_list_insert_front(char_list, &charnodebuf[i]);
		uint_list_insert_back(uint_list, &uintnodebuf[i]);
		assert_int_equal(uint_list_size(uint_list), i + 1);
	}

	assert_false(char_list_empty(char_list));

	assert_int_equal(*uint_list_get(&uintnodebuf[3]), 3);
	assert_int_equal(*uint_list_get(*uint_list_find_head(uint_list)), 0);
	assert_int_equal(*uint_list_get(*uint_list_find_tail(uint_list)), 19);

	assert_int_equal(*char_list_get(&charnodebuf[3]), 'd');
	assert_int_equal(*char_list_get(*char_list_find_head(char_list)), 'a' + 19);
	assert_int_equal(*char_list_get(*char_list_find_tail(char_list)), 'a');

	assert_false(uint_list_iterate(uint_list, uint_iterator, NULL));
	assert_false(char_list_iterate(char_list, char_iterator, NULL));

	uint_list_t ** u = uint_list_find(uint_list, &uintbuf[10]);
	assert_int_equal(*uint_list_get(*u), 10);
	u = uint_list_next(*u);
	u = uint_list_next(*u);
	assert_int_equal(*uint_list_get(*u), 12);
	assert_ptr_equal(uint_list_detach(uint_list, u), *u);

	u = uint_list_find_match(uint_list, &uintbuf[7], uint_cmp);
	assert_int_equal(*uint_list_get(*u), 7);

	uint_list_t * U = uint_list_remove(uint_list, &uintbuf[11]);
	assert_int_equal(*uint_list_get(U), 11);

	char_list_t ** c = char_list_find(char_list, &charbuf[10]);
	assert_int_equal(*char_list_get(*c), 'a' + 10);
	c = char_list_next(*c);
	c = char_list_next(*c);
	assert_int_equal(*char_list_get(*c), 'a' + 8);
	assert_ptr_equal(char_list_detach(char_list, c), *c);

	c = char_list_find_match(char_list, "d", char_cmp);
	assert_int_equal(*char_list_get(*c), 'd');

	char_list_t * C = char_list_remove(char_list, &charbuf[11]);
	assert_int_equal(*char_list_get(C), 'a' + 11);

	assert_int_equal(*char_list_get(char_list_remove_match(char_list, "d", char_cmp)), 'd');
	assert_int_equal(*char_list_get(char_list_remove_head(char_list)), 'a' + 19);
	assert_int_equal(*char_list_get(char_list_remove_tail(char_list)), 'a');

	assert_int_equal(*uint_list_get(uint_list_remove_match(uint_list, &uintbuf[7], uint_cmp)), 7);
	assert_int_equal(*uint_list_get(uint_list_remove_head(uint_list)), 0);
	assert_int_equal(*uint_list_get(uint_list_remove_tail(uint_list)), 19);

	test_free(uint_list);
	test_free(char_list);
	test_free(charbuf);
	test_free(charnodebuf);
	test_free(uintbuf);
	test_free(uintnodebuf);
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(FT_basic_usage),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
