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

#include "cmocka-wrapper.h"
#include "homogeneous_ring_buffer.h"

/************************************************************************/

static void
UT_homogeneous_ring_buffer_initialize()
{
	enum { buf_len = 234, entry_len = 8 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len);

	assert_non_null(ring);
	assert_non_null(buf);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	test_free(buf);
	test_free(ring);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_initialize__too_big_element()
{
	enum { buf_len = 234, entry_len = buf_len + 1 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len);

	assert_non_null(ring);
	assert_non_null(buf);

	expect_assert_failure(homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len));

	test_free(buf);
	test_free(ring);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_initialize__NULL()
{
	enum { buf_len = 234, entry_len = 8 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len);

	assert_non_null(ring);
	assert_non_null(buf);

	expect_assert_failure(homogeneous_ring_buffer_initialize(ring, buf, buf_len, 0));
	expect_assert_failure(homogeneous_ring_buffer_initialize(ring, buf, 0, entry_len));
	expect_assert_failure(homogeneous_ring_buffer_initialize(ring, NULL, buf_len, entry_len));
	expect_assert_failure(homogeneous_ring_buffer_initialize(NULL, buf, buf_len, entry_len));

	expect_assert_failure(homogeneous_ring_buffer_initialize(ring, buf, buf_len, 0));
	expect_assert_failure(homogeneous_ring_buffer_initialize(ring, buf, 0, 0));
	expect_assert_failure(homogeneous_ring_buffer_initialize(ring, NULL, buf_len, 0));
	expect_assert_failure(homogeneous_ring_buffer_initialize(NULL, buf, buf_len, 0));

	expect_assert_failure(homogeneous_ring_buffer_initialize(ring, NULL, 0, entry_len));
	expect_assert_failure(homogeneous_ring_buffer_initialize(NULL, buf, 0, entry_len));

	expect_assert_failure(homogeneous_ring_buffer_initialize(NULL, NULL, buf_len, entry_len));

	expect_assert_failure(homogeneous_ring_buffer_initialize(ring, NULL, 0, 0));
	expect_assert_failure(homogeneous_ring_buffer_initialize(NULL, buf, 0, 0));
	expect_assert_failure(homogeneous_ring_buffer_initialize(NULL, NULL, buf_len, 0));
	expect_assert_failure(homogeneous_ring_buffer_initialize(NULL, NULL, 0, entry_len));

	test_free(buf);
	test_free(ring);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_write()
{
	enum { buf_len = 8, entry_len = 1 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
		 * tmp = test_malloc(entry_len + 1);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(tmp);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);
	tmp[entry_len] = '\0';

	assert_false(homogeneous_ring_buffer_write(ring, "0"));
	assert_false(homogeneous_ring_buffer_write(ring, "1"));
	assert_false(homogeneous_ring_buffer_write(ring, "2"));
	assert_false(homogeneous_ring_buffer_write(ring, "3"));
	assert_false(homogeneous_ring_buffer_write(ring, "4"));
	assert_false(homogeneous_ring_buffer_write(ring, "5"));
	assert_false(homogeneous_ring_buffer_write(ring, "6"));
	assert_false(homogeneous_ring_buffer_write(ring, "7"));

	assert_true(homogeneous_ring_buffer_write(ring, "8"));
	assert_true(homogeneous_ring_buffer_write(ring, "9"));
	assert_true(homogeneous_ring_buffer_write(ring, "a"));

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "0");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "1");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "2");

	assert_false(homogeneous_ring_buffer_write(ring, "8"));
	assert_false(homogeneous_ring_buffer_write(ring, "9"));
	assert_false(homogeneous_ring_buffer_write(ring, "a"));

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "3");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "4");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "5");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "6");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "7");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "8");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "9");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "a");

	assert_true(homogeneous_ring_buffer_read(ring, tmp));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_write__NULL()
{
	enum { buf_len = 41, entry_len = 4 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len);

	assert_non_null(ring);
	assert_non_null(buf);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	expect_assert_failure(homogeneous_ring_buffer_write(ring, NULL));
	expect_assert_failure(homogeneous_ring_buffer_write(NULL, "abba"));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_overwrite()
{
	enum { buf_len = 8, entry_len = 1 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
		 * tmp = test_malloc(entry_len + 1);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(tmp);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);
	tmp[entry_len] = '\0';

	homogeneous_ring_buffer_overwrite(ring, "0");
	homogeneous_ring_buffer_overwrite(ring, "1");
	homogeneous_ring_buffer_overwrite(ring, "2");
	homogeneous_ring_buffer_overwrite(ring, "3");
	homogeneous_ring_buffer_overwrite(ring, "4");
	homogeneous_ring_buffer_overwrite(ring, "5");
	homogeneous_ring_buffer_overwrite(ring, "6");
	homogeneous_ring_buffer_overwrite(ring, "7");

	assert_true(homogeneous_ring_buffer_write(ring, "8"));

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "0");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "1");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "2");

	homogeneous_ring_buffer_overwrite(ring, "8");
	homogeneous_ring_buffer_overwrite(ring, "9");
	homogeneous_ring_buffer_overwrite(ring, "a");

	assert_true(homogeneous_ring_buffer_write(ring, "b"));

	homogeneous_ring_buffer_overwrite(ring, "b");
	homogeneous_ring_buffer_overwrite(ring, "c");
	homogeneous_ring_buffer_overwrite(ring, "d");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "6");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "7");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "8");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "9");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "a");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "b");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "c");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "d");

	assert_true(homogeneous_ring_buffer_read(ring, tmp));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_overwrite__NULL()
{
	enum { buf_len = 41, entry_len = 4 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len);

	assert_non_null(ring);
	assert_non_null(buf);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	expect_assert_failure(homogeneous_ring_buffer_overwrite(ring, NULL));
	expect_assert_failure(homogeneous_ring_buffer_overwrite(NULL, "abba"));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_read()
{
	enum { buf_len = 11, entry_len = 2 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
	     * tmp = test_malloc(entry_len + 1);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(tmp);

	tmp[entry_len] = '\0';
	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	assert_true(homogeneous_ring_buffer_read(ring, tmp));

	assert_false(homogeneous_ring_buffer_write(ring, "aa"));
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "aa");

	assert_false(homogeneous_ring_buffer_write(ring, "bb"));
	assert_false(homogeneous_ring_buffer_write(ring, "cc"));
	assert_false(homogeneous_ring_buffer_write(ring, "dd"));
	assert_false(homogeneous_ring_buffer_write(ring, "ee"));
	assert_false(homogeneous_ring_buffer_write(ring, "ff"));

	assert_true(homogeneous_ring_buffer_write(ring, "gg"));

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "bb");

	assert_false(homogeneous_ring_buffer_write(ring, "gg"));
	assert_true(homogeneous_ring_buffer_write(ring, "hh"));

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "cc");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "dd");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "ee");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "ff");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "gg");

	assert_true(homogeneous_ring_buffer_read(ring, tmp));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_peek()
{
	enum { buf_len = 6, entry_len = 2 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
	     * tmp = test_malloc(entry_len + 1);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(tmp);

	tmp[entry_len] = '\0';
	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	assert_true(homogeneous_ring_buffer_peek(ring, tmp));

	assert_false(homogeneous_ring_buffer_write(ring, "aa"));
	assert_false(homogeneous_ring_buffer_peek(ring, tmp));
	assert_string_equal(tmp, "aa");
	assert_false(homogeneous_ring_buffer_peek(ring, tmp));
	assert_string_equal(tmp, "aa");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "aa");

	assert_false(homogeneous_ring_buffer_write(ring, "bb"));
	assert_false(homogeneous_ring_buffer_write(ring, "cc"));
	assert_false(homogeneous_ring_buffer_write(ring, "dd"));

	assert_true(homogeneous_ring_buffer_write(ring, "ee"));
	assert_false(homogeneous_ring_buffer_peek(ring, tmp));
	assert_string_equal(tmp, "bb");
	homogeneous_ring_buffer_overwrite(ring, "ee");

	assert_false(homogeneous_ring_buffer_peek(ring, tmp));
	assert_string_equal(tmp, "cc");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "cc");

	assert_false(homogeneous_ring_buffer_peek(ring, tmp));
	assert_string_equal(tmp, "dd");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "dd");

	assert_false(homogeneous_ring_buffer_peek(ring, tmp));
	assert_string_equal(tmp, "ee");
	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "ee");

	assert_true(homogeneous_ring_buffer_peek(ring, tmp));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_peek__NULL()
{
	enum { buf_len = 41, entry_len = 7 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
	     * tmp = test_malloc(entry_len);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(tmp);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	assert_false(homogeneous_ring_buffer_write(ring, "foobar1"));
	assert_false(homogeneous_ring_buffer_write(ring, "foobar2"));
	assert_false(homogeneous_ring_buffer_write(ring, "foobar3"));

	expect_assert_failure(homogeneous_ring_buffer_peek(ring, NULL));
	expect_assert_failure(homogeneous_ring_buffer_peek(NULL, tmp));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_read__NULL()
{
	enum { buf_len = 41, entry_len = 7 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
	     * tmp = test_malloc(entry_len);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(tmp);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	assert_false(homogeneous_ring_buffer_write(ring, "foobar1"));
	assert_false(homogeneous_ring_buffer_write(ring, "foobar2"));
	assert_false(homogeneous_ring_buffer_write(ring, "foobar3"));

	expect_assert_failure(homogeneous_ring_buffer_read(ring, NULL));
	expect_assert_failure(homogeneous_ring_buffer_read(NULL, tmp));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/************************************************************************/

static void
FT_max_allowed_size_entry()
{
	enum { buf_len = 10, entry_len = buf_len };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
	     * in =  test_malloc(entry_len),
	     * out = test_malloc(entry_len);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(in);

	memset(in, 0x1337, entry_len);
	memset(out, 0, entry_len);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	assert_false(homogeneous_ring_buffer_write(ring, in));
	assert_true(homogeneous_ring_buffer_write(ring, in));
	assert_false(homogeneous_ring_buffer_read(ring, out));

	assert_memory_equal(in, out, entry_len);

	test_free(buf);
	test_free(in);
	test_free(out);
	test_free(ring);
}

/************************************************************************/

static void
FT_keep_on_rolling_rolling_rolling()
{
	enum { buf_len = 10, entry_len = 2 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
	     * in =  test_malloc(entry_len),
	     * out = test_malloc(entry_len);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(in);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	for (int i = 0 ; i < 1000000 ; i++)
	{
		in[0] = i % 256;
		in[1] = i % 256;
		homogeneous_ring_buffer_overwrite(ring, in);
		assert_false(homogeneous_ring_buffer_peek(ring, out));
		if (i > buf_len / entry_len)
		{
			in[0] = (i - (buf_len / entry_len - 1)) % 256;
			in[1] = (i - (buf_len / entry_len - 1)) % 256;
			assert_memory_equal(in, out, entry_len);
		}
	}

	test_free(buf);
	test_free(in);
	test_free(out);
	test_free(ring);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_empty()
{
	enum { buf_len = 10, entry_len = 1 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
	     * tmp = test_malloc(entry_len);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(tmp);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);

	assert_true(homogeneous_ring_buffer_empty(ring));

	assert_false(homogeneous_ring_buffer_write(ring, "a"));
	assert_false(homogeneous_ring_buffer_empty(ring));

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_true(homogeneous_ring_buffer_empty(ring));

	test_free(ring);
	test_free(tmp);
	test_free(buf);
}

/************************************************************************/

static void
UT_homogeneous_ring_buffer_empty__NULL()
{
	expect_assert_failure(homogeneous_ring_buffer_empty(NULL));
}

/************************************************************************/

#define allocate_and_write(r, d) \
{ \
	char * t = homogeneous_ring_buffer_allocate(r); \
	assert_non_null(t); \
	memcpy(t, d, entry_len); \
}

static void
UT_homogeneous_ring_buffer_allocate()
{
	enum { buf_len = 8, entry_len = 1 };
	struct homogeneous_ring_buffer * ring = test_malloc(sizeof(struct homogeneous_ring_buffer));
	char * buf = test_malloc(buf_len),
		 * tmp = test_malloc(entry_len + 1);

	assert_non_null(ring);
	assert_non_null(buf);
	assert_non_null(tmp);

	homogeneous_ring_buffer_initialize(ring, buf, buf_len, entry_len);
	tmp[entry_len] = '\0';

	allocate_and_write(ring, "0");
	allocate_and_write(ring, "1");
	allocate_and_write(ring, "2");
	allocate_and_write(ring, "3");
	allocate_and_write(ring, "4");
	allocate_and_write(ring, "5");
	allocate_and_write(ring, "6");
	allocate_and_write(ring, "7");

	assert_null(homogeneous_ring_buffer_allocate(ring));
	assert_null(homogeneous_ring_buffer_allocate(ring));
	assert_null(homogeneous_ring_buffer_allocate(ring));

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "0");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "1");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "2");

	allocate_and_write(ring, "8");
	allocate_and_write(ring, "9");
	allocate_and_write(ring, "a");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "3");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "4");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "5");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "6");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "7");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "8");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "9");

	assert_false(homogeneous_ring_buffer_read(ring, tmp));
	assert_string_equal(tmp, "a");

	assert_true(homogeneous_ring_buffer_read(ring, tmp));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

#undef allocate_and_write

/************************************************************************/

static void
UT_homogeneous_ring_buffer_allocate__NULL()
{
	expect_assert_failure(homogeneous_ring_buffer_allocate(NULL));
}

/************************************************************************/

int
main()
{
	struct CMUnitTest const tests[] =
	{
		cmocka_unit_test(UT_homogeneous_ring_buffer_initialize),
		cmocka_unit_test(UT_homogeneous_ring_buffer_initialize__NULL),
		cmocka_unit_test(UT_homogeneous_ring_buffer_initialize__too_big_element),

		cmocka_unit_test(UT_homogeneous_ring_buffer_write),
		cmocka_unit_test(UT_homogeneous_ring_buffer_write__NULL),

		cmocka_unit_test(UT_homogeneous_ring_buffer_overwrite),
		cmocka_unit_test(UT_homogeneous_ring_buffer_overwrite__NULL),

		cmocka_unit_test(UT_homogeneous_ring_buffer_allocate),
		cmocka_unit_test(UT_homogeneous_ring_buffer_allocate__NULL),

		cmocka_unit_test(UT_homogeneous_ring_buffer_read),
		cmocka_unit_test(UT_homogeneous_ring_buffer_read__NULL),

		cmocka_unit_test(UT_homogeneous_ring_buffer_peek),
		cmocka_unit_test(UT_homogeneous_ring_buffer_peek__NULL),

		cmocka_unit_test(UT_homogeneous_ring_buffer_empty),
		cmocka_unit_test(UT_homogeneous_ring_buffer_empty__NULL),

		cmocka_unit_test(FT_max_allowed_size_entry),
		cmocka_unit_test(FT_keep_on_rolling_rolling_rolling)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
