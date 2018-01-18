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

#include "cmocka-wrapper.h"

#define TYPED_RING_BUFFER_CONFIG_TYPE unsigned long
#define TYPED_RING_BUFFER_CONFIG_PREFIX ulongbuf
#include "typed_ring_buffer.h"

#define TYPED_RING_BUFFER_CONFIG_TYPE char
#define TYPED_RING_BUFFER_CONFIG_PREFIX charbuf
#include "typed_ring_buffer.h"


/***********************************************************************/

static void
FT_basic_usage()
{
	enum { char_count = 10, ulong_count = 2000 };

	ulongbuf_t * ulong_ring = test_malloc(sizeof(ulongbuf_t));
	charbuf_t * char_ring = test_malloc(sizeof(charbuf_t));

	char * char_buf = test_malloc(sizeof(char) * char_count);
	unsigned long * ulong_buf = test_malloc(sizeof(unsigned long) * ulong_count);

	char t[2] = { '\0' };
	unsigned long u;

	assert_non_null(ulong_ring);
	assert_non_null(char_ring);
	assert_non_null(char_buf);
	assert_non_null(ulong_buf);

	ulongbuf_initialize(ulong_ring, ulong_buf, ulong_count);
	charbuf_initialize(char_ring, char_buf, char_count);

	assert_true(ulongbuf_empty(ulong_ring));
	assert_true(charbuf_empty(char_ring));

	assert_false(charbuf_write(char_ring, "a"));
	assert_false(charbuf_write(char_ring, "a"));
	assert_false(charbuf_write(char_ring, "b"));
	assert_false(charbuf_write(char_ring, "b"));
	assert_false(charbuf_write(char_ring, "c"));
	assert_false(charbuf_write(char_ring, "c"));

	assert_false(charbuf_empty(char_ring));

	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "a");

	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "a");

	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "b");

	assert_false(charbuf_write(char_ring, "x"));
	assert_false(charbuf_write(char_ring, "x"));
	assert_false(charbuf_write(char_ring, "x"));
	memcpy(charbuf_allocate(char_ring), "X", 1);
	assert_false(charbuf_write(char_ring, "x"));
	charbuf_overwrite(char_ring, "x");
	assert_false(charbuf_write(char_ring, "x"));
	assert_true(charbuf_write(char_ring, "z"));
	charbuf_overwrite(char_ring, "y");

	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "c");
	assert_false(charbuf_peek(char_ring, t));
	assert_string_equal(t, "c");
	assert_false(charbuf_peek(char_ring, t));
	assert_string_equal(t, "c");
	assert_false(charbuf_peek(char_ring, t));
	assert_string_equal(t, "c");
	assert_false(charbuf_peek(char_ring, t));
	assert_string_equal(t, "c");
	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "c");
	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "x");
	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "x");
	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "x");
	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "X");
	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "x");
	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "x");
	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "x");
	assert_false(charbuf_read(char_ring, t));
	assert_string_equal(t, "y");

	assert_true(charbuf_empty(char_ring));
	assert_true(charbuf_read(char_ring, t));

	for (unsigned long i = 0 ; i < ulong_count ; i++)
		assert_false(ulongbuf_write(ulong_ring, &i));
	u = 3;
	assert_true(ulongbuf_write(ulong_ring, &u));
	for (unsigned long i = 0 ; i < ulong_count ; i++)
	{
		assert_false(ulongbuf_read(ulong_ring, &u));
		assert_int_equal(u, i);
	}

	test_free(ulong_ring);
	test_free(char_ring);
	test_free(ulong_buf);
	test_free(char_buf);
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
