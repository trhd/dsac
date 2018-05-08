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

#include <string.h>
#include <pthread.h>
#include "cmocka-wrapper.h"
#include "ring_buffer.h"

/************************************************************************/

static void
UT_ring_buffer_initialize()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));
	assert_false(ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_ring_buffer_initialize__NULL()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	expect_assert_failure(ring_buffer_initialize(NULL, NULL, 0));

	expect_assert_failure(ring_buffer_initialize(NULL, NULL, buf_size));
	expect_assert_failure(ring_buffer_initialize(NULL, buf, 0));
	expect_assert_failure(ring_buffer_initialize(ring, NULL, 0));

	expect_assert_failure(ring_buffer_initialize(ring, buf, 0));
	expect_assert_failure(ring_buffer_initialize(ring, NULL, buf_size));
	expect_assert_failure(ring_buffer_initialize(NULL, buf, buf_size));

	test_free(buf);
	test_free(ring);
}

/************************************************************************/

static void
UT_ring_buffer_uninitialize()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));
	assert_false(ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_ring_buffer_uninitialize__NULL()
{
	expect_assert_failure(ring_buffer_uninitialize(NULL));
}

/************************************************************************/

static void
UT_ring_buffer_uninitialize__uninitialized()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));
	assert_false(ring_buffer_uninitialize(ring));

	expect_assert_failure(ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

#ifdef RING_BUFFER_WRITE_ALLOC

/*** #ifdef RING_BUFFER_WRITE_ALLOC ************************************/

#ifndef RING_BUFFER_LOCKLESS

/*** #ifndef RING_BUFFER_LOCKLESS **************************************/
/*** #ifdef RING_BUFFER_WRITE_ALLOC ************************************/

// void ring_buffer_write_alloc(struct ring_buffer *ring, void **data, size_t data_length);
static void
UT_ring_buffer_write_alloc()
{
}

/*** #ifndef RING_BUFFER_LOCKLESS **************************************/
/*** #ifdef RING_BUFFER_WRITE_ALLOC ************************************/

static void
UT_ring_buffer_write_alloc__NULL()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);
	void * p;

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(ring_buffer_write_alloc(NULL, NULL, 0));

	expect_assert_failure(ring_buffer_write_alloc(ring, NULL, 0));
	expect_assert_failure(ring_buffer_write_alloc(NULL, &p, 0));
	expect_assert_failure(ring_buffer_write_alloc(NULL, NULL, buf_size / 4))

	expect_assert_failure(ring_buffer_write_alloc(ring, &p, 0));
	expect_assert_failure(ring_buffer_write_alloc(ring, NULL, buf_size / 4));
	expect_assert_failure(ring_buffer_write_alloc(NULL, &p, buf_size / 4));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/*** #ifndef RING_BUFFER_LOCKLESS **************************************/
/*** #ifdef RING_BUFFER_WRITE_ALLOC ************************************/

#endif

/*** #ifdef RING_BUFFER_WRITE_ALLOC ************************************/

//bool ring_buffer_write_alloc_try(struct ring_buffer *ring, void **data, size_t data_length);
static void
UT_ring_buffer_write_alloc_try()
{
}

/*** #ifdef RING_BUFFER_WRITE_ALLOC ************************************/

static void
UT_ring_buffer_write_alloc_try__NULL()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);
	void * p;

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(ring_buffer_write_alloc_try(NULL, NULL, 0));

	expect_assert_failure(ring_buffer_write_alloc_try(ring, NULL, 0));
	expect_assert_failure(ring_buffer_write_alloc_try(NULL, &p, 0));
	expect_assert_failure(ring_buffer_write_alloc_try(NULL, NULL, buf_size / 4))

	expect_assert_failure(ring_buffer_write_alloc_try(ring, &p, 0));
	expect_assert_failure(ring_buffer_write_alloc_try(ring, NULL, buf_size / 4));
	expect_assert_failure(ring_buffer_write_alloc_try(NULL, &p, buf_size / 4));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/*** #ifdef RING_BUFFER_WRITE_ALLOC ************************************/

//void ring_buffer_write_flush(struct ring_buffer *ring);
static void
UT_ring_buffer_write_flush()
{

}

/*** #ifdef RING_BUFFER_WRITE_ALLOC ************************************/

static void
UT_ring_buffer_write_flush__NULL()
{
	enum { buf_size = 100 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);
	void * p;

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	assert_false(ring_buffer_write_alloc_try(ring, &p, buf_size / 4));

	expect_assert_failure(ring_buffer_write_flush(NULL, NULL, 0));

	expect_assert_failure(ring_buffer_write_flush(ring, NULL, 0));
	expect_assert_failure(ring_buffer_write_flush(NULL, p, 0));
	expect_assert_failure(ring_buffer_write_flush(NULL, NULL, buf_size / 4));

	expect_assert_failure(ring_buffer_write_flush(ring, p, 0));
	expect_assert_failure(ring_buffer_write_flush(ring, NULL, buf_size / 4));
	expect_assert_failure(ring_buffer_write_flush(NULL, p, buf_size / 4));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/*** #ifdef RING_BUFFER_WRITE_ALLOC ************************************/

#endif

/***********************************************************************/

static void
UT_ring_buffer_write()
{
	enum { buf_size = 10 + 3 * sizeof(size_t), tmp_size = 20 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(tmp_size);
	size_t l;

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	assert_false(ring_buffer_write(ring, "foo", 3));
	assert_false(ring_buffer_write(ring, "bar", 3));
	assert_false(ring_buffer_write(ring, "keke", 4));

	assert_true(ring_buffer_write(ring, "fail", 4));

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 3);
	tmp[l] = '\0';
	assert_string_equal(tmp, "foo");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 3);
	tmp[l] = '\0';
	assert_string_equal(tmp, "bar");

	assert_true(ring_buffer_write(ring, "size_t 0 fobar", 8 + 7));
	assert_false(ring_buffer_write(ring, "size_t 0 foba", 8 + 6));
	assert_true(ring_buffer_write(ring, "r", 1));

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 4);
	tmp[l] = '\0';
	assert_string_equal(tmp, "keke");

	l = 5;
	assert_true(ring_buffer_read(ring, tmp, &l));

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 14);
	tmp[l] = '\0';
	assert_string_equal(tmp, "size_t 0 foba");

	l = tmp_size;
	assert_true(ring_buffer_read(ring, tmp, &l));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void
UT_ring_buffer_write__overflow()
{
	enum { buf_size = 12 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(ring_buffer_write(ring, "abcdefghijklmnopqrstuvwxyzåäö", 29));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_ring_buffer_write__NULL()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(ring_buffer_write(NULL, NULL, 0));

	expect_assert_failure(ring_buffer_write(ring, NULL, 0));
	expect_assert_failure(ring_buffer_write(NULL, "foobar", 0));
	expect_assert_failure(ring_buffer_write(NULL, NULL, 6));

	expect_assert_failure(ring_buffer_write(ring, NULL, 6));
	expect_assert_failure(ring_buffer_write(ring, "foobar", 0));
	expect_assert_failure(ring_buffer_write(NULL, "foobar", 6));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}
/************************************************************************/

static void
UT_ring_buffer_write__uninitialized()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));
	assert_false(ring_buffer_uninitialize(ring));

	expect_assert_failure(ring_buffer_write(ring, "foobar", 6));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_ring_buffer_overwrite()
{
	enum { buf_size = 10 + 3 * sizeof(size_t), tmp_size = 20 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(tmp_size);
	size_t l;

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	ring_buffer_overwrite(ring, "foo", 3);
	ring_buffer_overwrite(ring, "bar", 3);
	ring_buffer_overwrite(ring, "keke", 4);

	assert_true(ring_buffer_write(ring, "err", 3));

	l = tmp_size;
	assert_false(ring_buffer_peek(ring, tmp, &l));
	assert_int_equal(l, 3);
	tmp[l] = '\0';
	assert_string_equal(tmp, "foo");

	assert_true(ring_buffer_write(ring, "err", 3));
	ring_buffer_overwrite(ring, "err", 3);

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 3);
	tmp[l] = '\0';
	assert_string_equal(tmp, "bar");

	l = tmp_size;
	assert_false(ring_buffer_peek(ring, tmp, &l));
	assert_int_equal(l, 4);
	tmp[l] = '\0';
	assert_string_equal(tmp, "keke");

	assert_false(ring_buffer_write(ring, "abc", 3));

	ring_buffer_overwrite(ring, "done", 4);
	ring_buffer_overwrite(ring, "!", 1);

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 3);
	tmp[l] = '\0';
	assert_string_equal(tmp, "abc");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 4);
	tmp[l] = '\0';
	assert_string_equal(tmp, "done");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 1);
	tmp[l] = '\0';
	assert_string_equal(tmp, "!");

	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void
UT_ring_buffer_overwrite__overflow()
{
	enum { buf_size = 12 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(ring_buffer_overwrite(ring, "abcdefghijklmnopqrstuvwxyzåäö", 29));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_ring_buffer_overwrite__NULL()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(ring_buffer_overwrite(NULL, NULL, 0));

	expect_assert_failure(ring_buffer_overwrite(ring, NULL, 0));
	expect_assert_failure(ring_buffer_overwrite(NULL, "foobar", 0));
	expect_assert_failure(ring_buffer_overwrite(NULL, NULL, 6));

	expect_assert_failure(ring_buffer_overwrite(ring, NULL, 6));
	expect_assert_failure(ring_buffer_overwrite(ring, "foobar", 0));
	expect_assert_failure(ring_buffer_overwrite(NULL, "foobar", 6));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_ring_buffer_overwrite__uninitialized()
{
	enum { buf_size = 1000 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));
	assert_false(ring_buffer_uninitialize(ring));

	expect_assert_failure(ring_buffer_overwrite(ring, "foobar", 6));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_ring_buffer_read()
{
	enum { buf_size = 5 * (2 + sizeof(size_t)) + 1, tmp_size = 20 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(tmp_size);
	size_t l;

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	assert_true(ring_buffer_read(ring, tmp, &l));

	assert_false(ring_buffer_write(ring, "aa", 2));
	assert_false(ring_buffer_write(ring, "bb", 2));
	assert_false(ring_buffer_write(ring, "cc", 2));
	assert_false(ring_buffer_write(ring, "dd", 2));
	assert_false(ring_buffer_write(ring, "ee", 2));
	assert_true(ring_buffer_write(ring, "ff", 2));

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "aa");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "bb");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "cc");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "dd");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "ee");

	l = tmp_size;
	assert_true(ring_buffer_read(ring, tmp, &l));

	assert_false(ring_buffer_write(ring, "ff", 2));
	assert_false(ring_buffer_write(ring, "gg", 2));
	assert_false(ring_buffer_write(ring, "hh", 2));
	assert_false(ring_buffer_write(ring, "ii", 2));
	assert_false(ring_buffer_write(ring, "jj", 2));
	assert_true(ring_buffer_write(ring, "kk", 2));

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "ff");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "gg");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "hh");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "ii");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "jj");

	l = tmp_size;
	assert_true(ring_buffer_read(ring, tmp, &l));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(tmp);
	test_free(ring);
}

/************************************************************************/

static void
UT_ring_buffer_read__NULL()
{
	enum { buf_size = 100 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l = buf_size;

	assert_false(ring_buffer_initialize(ring, wb, buf_size));

	assert_false(ring_buffer_write(ring, "aa", 2));
	assert_false(ring_buffer_write(ring, "bb", 2));
	assert_false(ring_buffer_write(ring, "cc", 2));

	expect_assert_failure(ring_buffer_read(NULL, NULL, 0));

	expect_assert_failure(ring_buffer_read(NULL, NULL, &l));
	expect_assert_failure(ring_buffer_read(NULL, rb, 0));
	expect_assert_failure(ring_buffer_read(ring, NULL, 0));
		 
	expect_assert_failure(ring_buffer_read(ring, NULL, &l));
	expect_assert_failure(ring_buffer_read(ring, rb, 0));
	expect_assert_failure(ring_buffer_read(NULL, rb, &l));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_ring_buffer_read__uninitialized()
{
	enum { buf_size = 100 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l = buf_size;

	assert_false(ring_buffer_initialize(ring, wb, buf_size));

	assert_false(ring_buffer_write(ring, "aa", 2));
	assert_false(ring_buffer_write(ring, "bb", 2));
	assert_false(ring_buffer_write(ring, "cc", 2));

	assert_false(ring_buffer_uninitialize(ring));

	expect_assert_failure(ring_buffer_read(ring, rb, &l));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_ring_buffer_peek()
{
	enum { buf_size = 100 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l;

	assert_false(ring_buffer_initialize(ring, wb, buf_size));

	l = buf_size;
	assert_true(ring_buffer_peek(ring, rb, &l));

	assert_false(ring_buffer_write(ring, "aa", 2));
	l = buf_size;
	strcpy(rb, "foobar");
	rb[6] = 'x';
	assert_false(ring_buffer_peek(ring, rb, &l));
	assert_int_equal(l, 2);
	rb[2] = '\0';
	assert_string_equal(rb, "aa");

	assert_false(ring_buffer_write(ring, "bb", 2));
	l = buf_size;
	strcpy(rb, "foobar");
	rb[6] = 'x';
	assert_false(ring_buffer_peek(ring, rb, &l));
	assert_int_equal(l, 2);
	rb[2] = '\0';
	assert_string_equal(rb, "aa");

	assert_false(ring_buffer_write(ring, "cc", 2));
	l = buf_size;
	strcpy(rb, "foobar");
	rb[6] = 'x';
	assert_false(ring_buffer_peek(ring, rb, &l));
	assert_int_equal(l, 2);
	rb[2] = '\0';
	assert_string_equal(rb, "aa");

	for (int j = 0 ; j < 3; j++)
	{
		char e[] = "aa\0";
		e[0] += j;
		e[1] += j;

		for (int i = 0 ; i < 10 ; i++)
		{
			l = buf_size;
			assert_false(ring_buffer_peek(ring, rb, &l));
			assert_int_equal(l, 2);
			rb[l] = '\0';
			assert_string_equal(rb, e);
		}

		l = buf_size;
		assert_false(ring_buffer_read(ring, rb, &l));
		assert_int_equal(l, 2);
		rb[l] = '\0';
		assert_string_equal(rb, e);
	}

	l = buf_size;
	assert_true(ring_buffer_peek(ring, rb, &l));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_ring_buffer_peek__NULL()
{
	enum { buf_size = 100 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l = buf_size;

	assert_false(ring_buffer_initialize(ring, wb, buf_size));

	assert_false(ring_buffer_write(ring, "aa", 2));
	assert_false(ring_buffer_write(ring, "bb", 2));
	assert_false(ring_buffer_write(ring, "cc", 2));

	expect_assert_failure(ring_buffer_peek(NULL, NULL, 0));

	expect_assert_failure(ring_buffer_peek(NULL, NULL, &l));
	expect_assert_failure(ring_buffer_peek(NULL, rb, 0));
	expect_assert_failure(ring_buffer_peek(ring, NULL, 0));
		 
	expect_assert_failure(ring_buffer_peek(ring, NULL, &l));
	expect_assert_failure(ring_buffer_peek(ring, rb, 0));
	expect_assert_failure(ring_buffer_peek(NULL, rb, &l));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_ring_buffer_peek__uninitialized()
{
	enum { buf_size = 100 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l = buf_size;

	assert_false(ring_buffer_initialize(ring, wb, buf_size));

	assert_false(ring_buffer_write(ring, "aa", 2));
	assert_false(ring_buffer_write(ring, "bb", 2));
	assert_false(ring_buffer_write(ring, "cc", 2));

	assert_false(ring_buffer_uninitialize(ring));

	expect_assert_failure(ring_buffer_peek(ring, rb, &l));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_ring_buffer_empty()
{
	enum { buf_size = 100 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l;

	assert_false(ring_buffer_initialize(ring, wb, buf_size));

	assert_true(ring_buffer_empty(ring));
	assert_false(ring_buffer_write(ring, "foobar", 6));
	assert_false(ring_buffer_empty(ring));
	assert_false(ring_buffer_write(ring, "foobar2", 7));
	assert_false(ring_buffer_empty(ring));
	l = buf_size;
	assert_false(ring_buffer_read(ring, rb, &l));
	assert_false(ring_buffer_empty(ring));
	l = buf_size;
	assert_false(ring_buffer_read(ring, rb, &l));
	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(wb);
	test_free(ring);
	test_free(rb);
}

/************************************************************************/

static void
UT_ring_buffer_empty__NULL()
{
	expect_assert_failure(ring_buffer_empty(NULL));
}

/************************************************************************/

static void
UT_ring_buffer_empty__uninitialized()
{
	enum { buf_size = 20 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));
	assert_false(ring_buffer_uninitialize(ring));

	expect_assert_failure(ring_buffer_empty(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

/**
 * After writing sizeof(size_t) + 3 to a buffer of size 16, it leaves 5 to
 * the buffer, which is too little to be used -> buffer wraps around
 * early. This test ensures this early wrapping is handled correctly.
 */
static void
FT_wrap_early()
{
	enum { buf_size = 16 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l;

	assert_false(ring_buffer_initialize(ring, wb, buf_size));
	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_write(ring, "foo", 3));
	assert_false(ring_buffer_empty(ring));

	l = buf_size;
	assert_false(ring_buffer_read(ring, rb, &l));
	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(wb);
	test_free(rb);
}

static void
FT_wrap_early__twice()
{
	enum { buf_size = 16 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l;

	assert_false(ring_buffer_initialize(ring, wb, buf_size));

	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_write(ring, "foo", 3));
	assert_false(ring_buffer_empty(ring));

	l = buf_size;
	assert_false(ring_buffer_read(ring, rb, &l));
	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_write(ring, "foo", 3));
	assert_false(ring_buffer_empty(ring));

	l = buf_size;
	assert_false(ring_buffer_read(ring, rb, &l));
	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

static void
FT_wrap_early__double_write()
{
	enum { buf_size = 16 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l;

	assert_false(ring_buffer_initialize(ring, wb, buf_size));

	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_write(ring, "foo", 3));
	assert_false(ring_buffer_empty(ring));

	assert_true(ring_buffer_write(ring, "foo", 3));
	assert_false(ring_buffer_empty(ring));

	l = buf_size;
	assert_false(ring_buffer_read(ring, rb, &l));
	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_write(ring, "foo", 3));
	assert_false(ring_buffer_empty(ring));

	assert_true(ring_buffer_write(ring, "foo", 3));
	assert_false(ring_buffer_empty(ring));

	l = buf_size;
	assert_false(ring_buffer_read(ring, rb, &l));
	assert_true(ring_buffer_empty(ring));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

#define allocate_and_write(r, d, l) \
{ \
	char * t = ring_buffer_allocate(r, l); \
	assert_non_null(t); \
	memcpy(t, d, l); \
}

static void
UT_ring_buffer_allocate()
{
	enum { buf_size = 10 + 3 * sizeof(size_t), tmp_size = 20 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(tmp_size);
	size_t l;

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	allocate_and_write(ring, "foo", 3);
	allocate_and_write(ring, "bar", 3);
	allocate_and_write(ring, "keke", 4);

	assert_true(ring_buffer_write(ring, "fail", 4));

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 3);
	tmp[l] = '\0';
	assert_string_equal(tmp, "foo");

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 3);
	tmp[l] = '\0';
	assert_string_equal(tmp, "bar");

	assert_null(ring_buffer_allocate(ring, 8 + 7));
	allocate_and_write(ring, "size_t 0 foba", 8 + 6);
	assert_null(ring_buffer_allocate(ring, 1));

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 4);
	tmp[l] = '\0';
	assert_string_equal(tmp, "keke");

	l = 5;
	assert_true(ring_buffer_read(ring, tmp, &l));

	l = tmp_size;
	assert_false(ring_buffer_read(ring, tmp, &l));
	assert_int_equal(l, 14);
	tmp[l] = '\0';
	assert_string_equal(tmp, "size_t 0 foba");

	l = tmp_size;
	assert_true(ring_buffer_read(ring, tmp, &l));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

#undef allocate_and_write

/************************************************************************/

static void
UT_ring_buffer_allocate__NULL()
{
	enum { buf_size = 21 };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(ring_buffer_allocate(NULL, 0));
	
	expect_assert_failure(ring_buffer_allocate(ring, 0));
	expect_assert_failure(ring_buffer_allocate(NULL, 4));

	assert_false(ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_ring_buffer_allocate__uninitialized()
{
	enum { buf_size = __LINE__ };
	struct ring_buffer * ring = test_malloc(sizeof(struct ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(ring_buffer_initialize(ring, buf, buf_size));
	assert_false(ring_buffer_uninitialize(ring));

	expect_assert_failure(ring_buffer_allocate(ring, 4));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_ring_buffer_initialize),
		cmocka_unit_test(UT_ring_buffer_initialize__NULL),

		cmocka_unit_test(UT_ring_buffer_uninitialize),
		cmocka_unit_test(UT_ring_buffer_uninitialize__NULL),
		cmocka_unit_test(UT_ring_buffer_uninitialize__uninitialized),

		cmocka_unit_test(UT_ring_buffer_allocate),
		cmocka_unit_test(UT_ring_buffer_allocate__NULL),
		cmocka_unit_test(UT_ring_buffer_allocate__uninitialized),

		cmocka_unit_test(UT_ring_buffer_write),
		cmocka_unit_test(UT_ring_buffer_write__overflow),
		cmocka_unit_test(UT_ring_buffer_write__NULL),
		cmocka_unit_test(UT_ring_buffer_write__uninitialized),

		cmocka_unit_test(UT_ring_buffer_overwrite),
		cmocka_unit_test(UT_ring_buffer_overwrite__overflow),
		cmocka_unit_test(UT_ring_buffer_overwrite__NULL),
		cmocka_unit_test(UT_ring_buffer_overwrite__uninitialized),

		cmocka_unit_test(UT_ring_buffer_read),
		cmocka_unit_test(UT_ring_buffer_read__NULL),
		cmocka_unit_test(UT_ring_buffer_read__uninitialized),

		cmocka_unit_test(UT_ring_buffer_peek),
		cmocka_unit_test(UT_ring_buffer_peek__NULL),
		cmocka_unit_test(UT_ring_buffer_peek__uninitialized),

		cmocka_unit_test(UT_ring_buffer_empty),
		cmocka_unit_test(UT_ring_buffer_empty__NULL),
		cmocka_unit_test(UT_ring_buffer_empty__uninitialized),

		cmocka_unit_test(FT_wrap_early),
		cmocka_unit_test(FT_wrap_early__twice),
		cmocka_unit_test(FT_wrap_early__double_write)
			
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
