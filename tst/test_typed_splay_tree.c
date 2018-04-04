/**
 * dsac -- Data Structures and Alorithms for C
 * Copyright (C) 2018 Hemmo Nieminen
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

#include <string.h>
#include "cmocka-wrapper.h"

#define TYPED_SPLAY_TREE_CONFIG_TYPE unsigned long
#define TYPED_SPLAY_TREE_CONFIG_PREFIX ulongsplay
#include "typed_splay_tree.h"

#define TYPED_SPLAY_TREE_CONFIG_TYPE char
#define TYPED_SPLAY_TREE_CONFIG_PREFIX charsplay
#include "typed_splay_tree.h"

static int
ulongcmp(unsigned long const * a, unsigned long const * b)
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
charcmp(char const * a, char const * b)
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

/***********************************************************************/

struct _FT_basic_usage__helper_struct
{
	union
	{
		unsigned long ul;
		char ch;
	} prev;
	unsigned int count;
};

static int
_FT_basic_usage__helper(ulongsplay_t const * ulp, void * arg)
{
	assert(ulp);
	assert(arg);

	struct _FT_basic_usage__helper_struct * s = (struct _FT_basic_usage__helper_struct *)arg;

	if (s->prev.ul)
		assert_true(s->prev.ul <= (unsigned long)ulongsplay_get((ulongsplay_t *)ulp));

	s->count++;

	return 0;
}

static void
FT_basic_usage()
{
	unsigned long * ulongbuf = test_malloc(sizeof(unsigned long) * 20);
	char * charbuf = test_malloc(sizeof(char) * 20);
	ulongsplay_meta_t * uls = test_malloc(sizeof(ulongsplay_meta_t)),
	                  * uls2 = test_malloc(sizeof(ulongsplay_meta_t));
	charsplay_meta_t * chs = test_malloc(sizeof(charsplay_meta_t));
	ulongsplay_t * ulongn = test_malloc(sizeof(ulongsplay_t) * 20);
	charsplay_t * charn = test_malloc(sizeof(charsplay_t) * 20);
	struct _FT_basic_usage__helper_struct iterarg;

	ulongsplay_initialize(uls, ulongcmp);
	ulongsplay_initialize(uls2, ulongcmp);
	charsplay_initialize(chs, charcmp);

	assert_null(charsplay_remove_any(chs));
	assert_null(charsplay_find_any(chs));

	assert_true(charsplay_is_empty(chs));
	assert_true(ulongsplay_is_empty(uls));

	for (long i = 0 ; i < 20 ; i++)
	{
		charsplay_t const * chtmp ;

		ulongbuf[i] = i;
		charbuf[i] = (char)i;

		ulongsplay_set(&ulongn[i], &ulongbuf[i]);
		charsplay_set(&charn[i], &charbuf[i]);

		assert_ptr_equal(ulongsplay_get(&ulongn[i]), &ulongbuf[i]);
		assert_ptr_equal(charsplay_get(&charn[i]), &charbuf[i]);

		assert_int_equal(ulongsplay_size(uls), i);
		assert_int_equal(charsplay_size(chs), i);

		assert_false(ulongsplay_insert(uls, &ulongn[i]));
		assert_false(charsplay_insert(chs, &charn[i]));

		assert_int_equal(ulongsplay_size(uls), i + 1);
		assert_int_equal(charsplay_size(chs), i + 1);

		assert_true(ulongsplay_insert(uls, &ulongn[i]));
		assert_true(charsplay_insert(chs, &charn[i]));

		assert_int_equal(ulongsplay_size(uls), i + 1);
		assert_int_equal(charsplay_size(chs), i + 1);

		assert_ptr_equal(ulongsplay_find(uls, &ulongbuf[i]), &ulongn[i]);
		assert_ptr_equal(ulongsplay_remove(uls, &ulongbuf[i]), &ulongn[i]);
		assert_ptr_equal(charsplay_find(chs, &charbuf[i]), &charn[i]);
		assert_ptr_equal(charsplay_remove(chs, &charbuf[i]), &charn[i]);

		assert_int_equal(ulongsplay_size(uls), i);
		assert_int_equal(charsplay_size(chs), i);

		assert_null(ulongsplay_find(uls, &ulongbuf[i]));
		assert_null(ulongsplay_remove(uls, &ulongbuf[i]));
		assert_null(charsplay_find(chs, &charbuf[i]));
		assert_null(charsplay_remove(chs, &charbuf[i]));

		assert_int_equal(ulongsplay_size(uls), i);
		assert_int_equal(charsplay_size(chs), i);

		assert_false(ulongsplay_insert(uls, &ulongn[i]));
		assert_false(charsplay_insert(chs, &charn[i]));

		assert_int_equal(ulongsplay_size(uls), i + 1);
		assert_int_equal(charsplay_size(chs), i + 1);

		chtmp = charsplay_find_any(chs);
		assert_non_null(chtmp);
		chtmp = charsplay_remove_any(chs);
		assert_non_null(chtmp);
		assert_false(charsplay_insert(chs, (charsplay_t *)chtmp));

		chtmp = charsplay_find_min(chs);
		assert_ptr_equal(chtmp, &charn[0]);
		chtmp = charsplay_remove_min(chs);
		assert_ptr_equal(chtmp, &charn[0]);
		assert_false(charsplay_insert(chs, (charsplay_t *)chtmp));

		chtmp = charsplay_find_max(chs);
		assert_ptr_equal(chtmp, &charn[i]);
		chtmp = charsplay_remove_max(chs);
		assert_ptr_equal(chtmp, &charn[i]);
		assert_false(charsplay_insert(chs, (charsplay_t *)chtmp));

		assert_false(charsplay_is_empty(chs));
		assert_false(ulongsplay_is_empty(uls));

		memset(&iterarg, 0, sizeof(iterarg));
		assert_false(ulongsplay_iterate(uls, _FT_basic_usage__helper, &iterarg));
	}

	for (int i = 0 ; i < 10 ; i++)
	{
		ulongsplay_t const * t;

		t = ulongsplay_find_min(uls);
		assert_ptr_equal(t, &ulongn[i]);
		t = ulongsplay_remove_min(uls);
		assert_ptr_equal(t, &ulongn[i]);
		assert_false(ulongsplay_insert(uls2, (ulongsplay_t *)t));

		t = ulongsplay_find_max(uls);
		assert_ptr_equal(t, &ulongn[19 - i]);
		t = ulongsplay_remove_max(uls);
		assert_ptr_equal(t, &ulongn[19 - i]);
		assert_false(ulongsplay_insert(uls2, (ulongsplay_t *)t));

		memset(&iterarg, 0, sizeof(iterarg));
		assert_false(ulongsplay_iterate(uls, _FT_basic_usage__helper, &iterarg));
		memset(&iterarg, 0, sizeof(iterarg));
		assert_false(ulongsplay_iterate(uls2, _FT_basic_usage__helper, &iterarg));
	}


	for (int i = 1 ; i < 10 ; i++)
	{
		assert_false(ulongsplay_is_empty(uls2));

		assert_ptr_equal(ulongsplay_find_next(uls2, &ulongbuf[0]), &ulongn[i]);
		assert_ptr_equal(ulongsplay_remove_next(uls2, &ulongbuf[0]), &ulongn[i]);
		assert_ptr_equal(ulongsplay_find_prev(uls2, &ulongbuf[19]), &ulongn[19-i]);
		assert_ptr_equal(ulongsplay_remove_prev(uls2, &ulongbuf[19]), &ulongn[19-i]);
	}

	assert_false(ulongsplay_is_empty(uls2));

	assert_null(ulongsplay_find_next(uls2, &ulongbuf[19]));
	assert_null(ulongsplay_remove_next(uls2, &ulongbuf[19]));
	assert_false(ulongsplay_is_empty(uls2));

	assert_null(ulongsplay_find_prev(uls2, &ulongbuf[0]));
	assert_null(ulongsplay_remove_prev(uls2, &ulongbuf[0]));
	assert_false(ulongsplay_is_empty(uls2));

	assert_ptr_equal(ulongsplay_find_next(uls2, &ulongbuf[0]), &ulongn[19]);
	assert_ptr_equal(ulongsplay_remove_next(uls2, &ulongbuf[0]), &ulongn[19]);
	assert_false(ulongsplay_is_empty(uls2));

	assert_null(ulongsplay_find_next(uls2, &ulongbuf[0]));
	assert_null(ulongsplay_remove_next(uls2, &ulongbuf[0]));
	assert_false(ulongsplay_is_empty(uls2));

	assert_ptr_equal(ulongsplay_find(uls2, &ulongbuf[0]), &ulongn[0]);
	assert_ptr_equal(ulongsplay_remove(uls2, &ulongbuf[0]), &ulongn[0]);
	assert_true(ulongsplay_is_empty(uls2));

	memset(&iterarg, 0, sizeof(iterarg));
	assert_false(ulongsplay_iterate(uls2, _FT_basic_usage__helper, &iterarg));

	for (int i = 0 ; i < 20 ; i++)
		assert_false(ulongsplay_insert(uls, &ulongn[i]));

	memset(&iterarg, 0, sizeof(iterarg));
	assert_false(ulongsplay_iterate(uls, _FT_basic_usage__helper, &iterarg));

	for (int i = 0 ; i < 10 ; i++)
	{
		assert_ptr_equal(ulongsplay_find_max(uls), &ulongn[19 - i]);
		assert_ptr_equal(ulongsplay_remove_max(uls), &ulongn[19 - i]);
		memset(&iterarg, 0, sizeof(iterarg));
		assert_false(ulongsplay_iterate(uls, _FT_basic_usage__helper, &iterarg));

		assert_ptr_equal(ulongsplay_find_min(uls), &ulongn[0 + i]);
		assert_ptr_equal(ulongsplay_remove_min(uls), &ulongn[0 + i]);
		memset(&iterarg, 0, sizeof(iterarg));
		assert_false(ulongsplay_iterate(uls, _FT_basic_usage__helper, &iterarg));
	}

	test_free(uls);
	test_free(uls2);
	test_free(chs);
	test_free(ulongn);
	test_free(charn);
	test_free(ulongbuf);
	test_free(charbuf);
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
