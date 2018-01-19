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

#include <stdio.h>
#include "cmocka-wrapper.h"

#define TYPED_FIFO_CONFIG_TYPE char
#define TYPED_FIFO_CONFIG_PREFIX chfifo
#include "typed_fifo.h"

struct f_yeah
{
	int a, b, c;
};

#define TYPED_FIFO_CONFIG_PREFIX yeahfifo
#define TYPED_FIFO_CONFIG_TYPE struct f_yeah
#include "typed_fifo.h"

/***********************************************************************/

static int
yeahcmp(struct f_yeah const * a, struct f_yeah const * b)
{
	assert(a);
	assert(b);

	if (a->a < b->a)
			return -1;
	if (a->a > b->a)
			return -1;
	if (a->b < b->b)
			return -1;
	if (a->b > b->b)
			return -1;
	if (a->c < b->c)
			return -1;
	if (a->c > b->c)
			return -1;
	return 0;
}

static void
FT_basic_usage()
{
	enum { val_count = 26 };
	yeahfifo_meta_t * f = test_malloc(sizeof(yeahfifo_meta_t));
	chfifo_meta_t * c = test_malloc(sizeof(chfifo_meta_t));
	char chvalbuf[val_count * 2];
	yeahfifo_t yeahbuf[val_count], * yeahtmp;
	chfifo_t chbuf[val_count], * chtmp;
	struct f_yeah yeahvalbuf[val_count];
	yeahfifo_t const * cyeahtmp;
	chfifo_t const * cchtmp;

	chfifo_initialize(c);
	yeahfifo_initialize(f);

	assert_true(chfifo_is_empty(c));
	assert_true(yeahfifo_is_empty(f));

	for (int i = 0; i < val_count; ++i)
	{
		chvalbuf[i*2] = 'a' + i;
		chvalbuf[i*2 + 1] = '\0';
		chfifo_set(&chbuf[i], &chvalbuf[i*2]);
		chfifo_push(f, &chbuf[i]);

		yeahvalbuf[i].a = i;
		yeahvalbuf[i].b = 2*i;
		yeahvalbuf[i].c = 3*i;
		yeahfifo_set(&yeahbuf[i], &yeahvalbuf[i]);
		yeahfifo_push(c, &yeahbuf[i]);

		assert_false(chfifo_is_empty(c));
		assert_false(yeahfifo_is_empty(f));

		assert_int_equal(chfifo_get_size(c), i + 1);
		assert_int_equal(yeahfifo_get_size(c), i + 1);
	}

	for (int i = 0; i < val_count; ++i)
	{
		assert_false(chfifo_is_empty(c));
		assert_false(yeahfifo_is_empty(f));

		cyeahtmp = yeahfifo_peek(c);
		yeahcmp(yeahfifo_get(cyeahtmp), &yeahvalbuf[i]);
		assert_int_equal(chfifo_get_size(c), val_count - i);

		cchtmp = chfifo_peek(f);
		assert_string_equal(chfifo_get(cchtmp), &chvalbuf[i*2]);
		assert_int_equal(yeahfifo_get_size(c), val_count - i);

		chtmp = chfifo_pop(f);
		assert_non_null(chtmp);
		assert_string_equal(chfifo_get(chtmp), &chvalbuf[i*2]);

		yeahtmp = yeahfifo_pop(c);
		assert_non_null(yeahtmp);
		yeahcmp(yeahfifo_get(yeahtmp), &yeahvalbuf[i]);

		assert_int_equal(chfifo_get_size(c), val_count - i - 1);
		assert_int_equal(yeahfifo_get_size(c), val_count - i - 1);
	}

	assert_true(chfifo_is_empty(c));
	assert_true(yeahfifo_is_empty(f));

	test_free(f);
	test_free(c);
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
