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

#define TYPED_ATOMIC_LIFO_CONFIG_TYPE char
#define TYPED_ATOMIC_LIFO_CONFIG_PREFIX chlifo
#include "typed_atomic_lifo.h"

#define TYPED_ATOMIC_LIFO_CONFIG_TYPE long
#define TYPED_ATOMIC_LIFO_CONFIG_PREFIX longlifo
#include "typed_atomic_lifo.h"

/***********************************************************************/

static void
FT_basic_usage()
{
	enum { lifo_element_count = 200 };

	chlifo_t * chfs = test_malloc(sizeof(chlifo_t) * (lifo_element_count + 1));
	longlifo_t * longfs = test_malloc(sizeof(longlifo_t) * (lifo_element_count + 1));

	chlifo_initialize(&chfs[0]);
	longlifo_initialize(&longfs[0]);

	assert_null(chlifo_pop(&chfs[0]));
	assert_null(longlifo_pop(&longfs[0]));

	for (long i = 0 ; i < lifo_element_count ; i++)
	{
		chlifo_set(&chfs[1 + i], (char*)i);
		longlifo_set(&longfs[1 + i], (long*)i);

		chlifo_push(&chfs[0], &chfs[1 + i]);
		longlifo_push(&longfs[0], &longfs[1 + i]);
	}

	for (long i = lifo_element_count - 1 ; i >= 0 ; i--)
	{
		chlifo_t * cht = chlifo_pop(&chfs[0]);
		assert_int_equal((long)chlifo_get(cht), i);

		longlifo_t * longt = longlifo_pop(&longfs[0]);
		assert_int_equal((long)longlifo_get(longt), i);
	}

	assert_null(chlifo_pop(&chfs[0]));
	assert_null(longlifo_pop(&longfs[0]));

	test_free(chfs);
	test_free(longfs);
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
