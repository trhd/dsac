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

#define TYPED_ATOMIC_FIFO_CONFIG_TYPE char
#define TYPED_ATOMIC_FIFO_CONFIG_PREFIX chfifo
#include "typed_atomic_fifo.h"

#define TYPED_ATOMIC_FIFO_CONFIG_TYPE long
#define TYPED_ATOMIC_FIFO_CONFIG_PREFIX longfifo
#include "typed_atomic_fifo.h"

/***********************************************************************/

static void
FT_basic_usage()
{
	enum { fifo_element_count = 200 };

	chfifo_t * chfs = test_malloc(sizeof(chfifo_t) * (fifo_element_count + 1));
	longfifo_t * longfs = test_malloc(sizeof(longfifo_t) * (fifo_element_count + 1));

	chfifo_initialize(&chfs[0]);
	longfifo_initialize(&longfs[0]);

	assert_null(chfifo_pop(&chfs[0]));
	assert_null(longfifo_pop(&longfs[0]));

	for (long i = 0 ; i < fifo_element_count ; i++)
	{
		chfifo_set(&chfs[1 + i], (char*)i);
		longfifo_set(&longfs[1 + i], (long*)i);

		chfifo_push(&chfs[0], &chfs[1 + i]);
		longfifo_push(&longfs[0], &longfs[1 + i]);
	}

	for (long i = 0 ; i < fifo_element_count ; i++)
	{
		chfifo_t * cht = chfifo_pop(&chfs[0]);
		assert_int_equal((long)chfifo_get(cht), i);

		longfifo_t * longt = longfifo_pop(&longfs[0]);
		assert_int_equal((long)longfifo_get(longt), i);
	}

	assert_null(chfifo_pop(&chfs[0]));
	assert_null(longfifo_pop(&longfs[0]));

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
