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

#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "cmocka-wrapper.h"
#include "blocking_ring_buffer.h"

/************************************************************************/

static void
UT_blocking_ring_buffer_initialize()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_initialize__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	expect_assert_failure(blocking_ring_buffer_initialize(NULL, NULL, 0));

	expect_assert_failure(blocking_ring_buffer_initialize(NULL, NULL, buf_size));
	expect_assert_failure(blocking_ring_buffer_initialize(NULL, buf, 0));
	expect_assert_failure(blocking_ring_buffer_initialize(ring, NULL, 0));

	expect_assert_failure(blocking_ring_buffer_initialize(ring, buf, 0));
	expect_assert_failure(blocking_ring_buffer_initialize(ring, NULL, buf_size));
	expect_assert_failure(blocking_ring_buffer_initialize(NULL, buf, buf_size));

	test_free(buf);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_uninitialize()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_uninitialize__NULL()
{
	expect_assert_failure(blocking_ring_buffer_uninitialize(NULL));
}

/************************************************************************/

static void
UT_blocking_ring_buffer_uninitialize__uninitialized()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void *
helper__UT_blocking_ring_buffer_write(struct blocking_ring_buffer * b)
{
	assert(b);

	pthread_t p = pthread_self();
	if (blocking_ring_buffer_write(b, &p, sizeof(p)))
		return (void *)1;

	return NULL;
}

static void
UT_blocking_ring_buffer_write()
{
	enum { thread_count = 1000, buf_size = thread_count * (sizeof(pthread_t) + sizeof(size_t)) };
	pthread_t threads[thread_count];
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);
	void * r;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_ring_buffer_write, ring));

	for (int i = 0 ; i < thread_count ; i++)
	{
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	for (int i = 0 ; i < thread_count ; i++)
	{
		pthread_t p;
		size_t l = sizeof(p);
		int j;
		assert_false(blocking_ring_buffer_read_try(ring, &p, &l));
		assert_int_equal(l, sizeof(p));

		for (j = 0 ; j < thread_count; j++)
			if (p == threads[j])
			{
				threads[j] = 0;
				break;
			}

		assert_int_not_equal(j, thread_count);
	}

	assert_true(blocking_ring_buffer_empty(ring) > 0);

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_write__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(blocking_ring_buffer_write(NULL, NULL, 0));

	expect_assert_failure(blocking_ring_buffer_write(ring, NULL, 0));
	expect_assert_failure(blocking_ring_buffer_write(NULL, "foobar", 0));
	expect_assert_failure(blocking_ring_buffer_write(NULL, NULL, 6));

	expect_assert_failure(blocking_ring_buffer_write(ring, NULL, 6));
	expect_assert_failure(blocking_ring_buffer_write(ring, "foobar", 0));
	expect_assert_failure(blocking_ring_buffer_write(NULL, "foobar", 6));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_write__uninitialized()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_write(ring, "foobar", 6));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void *
helper__UT_blocking_ring_buffer_overwrite(struct blocking_ring_buffer * b)
{
	assert(b);

	pthread_t p = pthread_self();

	if (blocking_ring_buffer_overwrite(b, &p, sizeof(p)))
		return (void *)1;

	return NULL;
}

static void
UT_blocking_ring_buffer_overwrite()
{
	enum { thread_count = 1000, buf_size = 200 };
	pthread_t threads[thread_count];
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);
	void * r;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_ring_buffer_overwrite, ring));

	for (int i = 0 ; i < thread_count ; i++)
	{
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	do {
		pthread_t p;
		size_t l = sizeof(p);
		int j;
		if (blocking_ring_buffer_read_try(ring, &p, &l))
			break;
		assert_int_equal(l, sizeof(p));

		for (j = 0 ; j < thread_count; j++)
		{
			if (p == threads[j])
			{
				threads[j] = 0;
				break;
			}
		}

		assert_int_not_equal(j, thread_count);
	} while (true);

	assert_true(blocking_ring_buffer_empty(ring) > 0);

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_overwrite__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(blocking_ring_buffer_overwrite(NULL, NULL, 0));

	expect_assert_failure(blocking_ring_buffer_overwrite(ring, NULL, 0));
	expect_assert_failure(blocking_ring_buffer_overwrite(NULL, "foobar", 0));
	expect_assert_failure(blocking_ring_buffer_overwrite(NULL, NULL, 6));

	expect_assert_failure(blocking_ring_buffer_overwrite(ring, NULL, 6));
	expect_assert_failure(blocking_ring_buffer_overwrite(ring, "foobar", 0));
	expect_assert_failure(blocking_ring_buffer_overwrite(NULL, "foobar", 6));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_overwrite__uninitialized()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_overwrite(ring, "foobar", 6));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

struct tuple__UT_blocking_ring_buffer_read
{
	struct blocking_ring_buffer *r;
	struct lock *l;
	uint8_t res[25][25];
};

static void *
helper__UT_blocking_ring_buffer_read(struct tuple__UT_blocking_ring_buffer_read *t)
{
	assert(t);

	char b[3];
	size_t l = 3;

	if (blocking_ring_buffer_read(t->r, &b, &l))
		return (void *)1;

	if (l != 2)
		return (void *)1;

	if (lock_acquire(t->l))
		return (void *)1;

	t->res[b[0] - 'a'][b[1] - 'a']++;

	lock_release(t->l);

	return NULL;
}

static void
UT_blocking_ring_buffer_read()
{
	enum { thread_count = 25*25, buf_size = thread_count * (2 + sizeof(size_t)) };
	pthread_t threads[thread_count];
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	struct lock * lock = test_malloc(sizeof(struct lock));
	struct tuple__UT_blocking_ring_buffer_read t = {ring, lock, {{0}}};
	char * buf = test_malloc(buf_size);

	lock_initialize(lock);
	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	for (int i = 0 ; i < 25 ; i++)
		for (int j = 0 ; j < 25 ; j++)
			assert_false(blocking_ring_buffer_write(ring, (char []){ 'a' + j, 'a' + i }, 2));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_ring_buffer_read, &t));

	for (int i = 0 ; i < thread_count ; i++)
	{
		void * r;
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	assert_true(blocking_ring_buffer_empty(ring) > 0);

	for (int i = 0 ; i < 25 ; i++)
		for (int j = 0 ; j < 25 ; j++)
			assert_int_equal(t.res[i][j], 1);

	lock_uninitialize(lock);
	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(lock);
	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_read__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * tmp = test_malloc(buf_size);
	size_t l;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	assert_false(blocking_ring_buffer_write(ring, "aa", 2));
	assert_false(blocking_ring_buffer_write(ring, "bb", 2));
	assert_false(blocking_ring_buffer_write(ring, "cc", 2));
	assert_false(blocking_ring_buffer_write(ring, "dd", 2));

	expect_assert_failure(blocking_ring_buffer_read(NULL, NULL, 0));

	expect_assert_failure(blocking_ring_buffer_read(ring, NULL, 0));
	expect_assert_failure(blocking_ring_buffer_read(NULL, tmp, 0));
	l = buf_size;
	expect_assert_failure(blocking_ring_buffer_read(NULL, NULL, &l));

	l = buf_size;
	expect_assert_failure(blocking_ring_buffer_read(ring, NULL, &l));
	expect_assert_failure(blocking_ring_buffer_read(ring, tmp, 0));
	l = buf_size;
	expect_assert_failure(blocking_ring_buffer_read(NULL, tmp, &l));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_read__uninitialized()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(buf_size);
	size_t l;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	assert_false(blocking_ring_buffer_write(ring, "foobar", 6));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	l = buf_size;
	expect_assert_failure(blocking_ring_buffer_read(ring, tmp, &l));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void *
helper__UT_blocking_ring_buffer_peek(struct blocking_ring_buffer * r)
{
	assert(r);

	char b[2][3] = { {0} };
	size_t l[2] = { 2 };

	if (blocking_ring_buffer_peek(r, &b[0], &l[0])
			|| l[0] != 2
			|| blocking_ring_buffer_read(r, &b[1], &l[0])
			|| l[0] != 2
			|| (int)b[0][0] * 255 + (int)b[0][1] > (int)b[1][0] * 255 + (int)b[1][1])
		return (void *)1;
	return NULL;
}

static void
UT_blocking_ring_buffer_peek()
{
	enum { thread_count = 25*25, buf_size = thread_count * (2 + sizeof(size_t)) };
	pthread_t threads[thread_count];
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	for (int i = 0 ; i < 26 ; i++)
		for (int j = 0 ; j < 25 ; j++)
			assert_false(blocking_ring_buffer_overwrite(ring, (char []){ 'a' + i, 'a' + j }, 2));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_ring_buffer_peek, ring));

	for (int i = 0 ; i < thread_count ; i++)
	{
		void * r;
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	assert_true(blocking_ring_buffer_empty(ring) > 0);
	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_peek__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * tmp = test_malloc(buf_size);
	size_t l;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	assert_false(blocking_ring_buffer_write(ring, "aa", 2));
	assert_false(blocking_ring_buffer_write(ring, "bb", 2));
	assert_false(blocking_ring_buffer_write(ring, "cc", 2));
	assert_false(blocking_ring_buffer_write(ring, "dd", 2));

	expect_assert_failure(blocking_ring_buffer_peek(NULL, NULL, 0));

	expect_assert_failure(blocking_ring_buffer_peek(ring, NULL, 0));
	expect_assert_failure(blocking_ring_buffer_peek(NULL, tmp, 0));
	l = buf_size;
	expect_assert_failure(blocking_ring_buffer_peek(NULL, NULL, &l));

	l = buf_size;
	expect_assert_failure(blocking_ring_buffer_peek(ring, NULL, &l));
	expect_assert_failure(blocking_ring_buffer_peek(ring, tmp, 0));
	l = buf_size;
	expect_assert_failure(blocking_ring_buffer_peek(NULL, tmp, &l));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_peek__uninitialized()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * tmp = test_malloc(buf_size);
	size_t l;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	assert_false(blocking_ring_buffer_write(ring, "aa", 2));
	assert_false(blocking_ring_buffer_write(ring, "bb", 2));
	assert_false(blocking_ring_buffer_write(ring, "cc", 2));
	assert_false(blocking_ring_buffer_write(ring, "dd", 2));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	l = buf_size;
	expect_assert_failure(blocking_ring_buffer_peek(ring, tmp, &l));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_write_allocate()
{
	enum { buf_size = 100, read_buf = 20 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * read = test_malloc(read_buf),
	     * tmp;
	size_t l;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	tmp = blocking_ring_buffer_write_allocate(ring, read_buf / 2);
	assert_non_null(tmp);

	expect_assert_failure(blocking_ring_buffer_write_allocate(ring, 1));
	expect_assert_failure(blocking_ring_buffer_write_try(ring, "foobar", 6));

	assert_true(read_buf / 2 >= 7);
	strcpy(tmp, "barfoo");

	blocking_ring_buffer_write_flush(ring);

	l = read_buf;
	assert_false(blocking_ring_buffer_read(ring, read, &l));
	assert_int_equal(l, read_buf / 2);
	assert_string_equal(tmp, "barfoo");

	assert_true(blocking_ring_buffer_empty(ring));

	blocking_ring_buffer_uninitialize(ring);

	test_free(ring);
	test_free(read);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_write_allocate__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(blocking_ring_buffer_write_allocate(NULL, 0));

	expect_assert_failure(blocking_ring_buffer_write_allocate(ring, 0));
	expect_assert_failure(blocking_ring_buffer_write_allocate(NULL, buf_size / 4))

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_write_allocate__uninitialized()
{
	enum { buf_size = 232 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_write_allocate(ring, 10));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_write_allocate_try()
{
	enum { buf_size = 100, read_buf = 20 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * read = test_malloc(read_buf),
	     * tmp;
	size_t l;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	tmp = blocking_ring_buffer_write_allocate_try(ring, read_buf / 2);
	assert_non_null(tmp);

	expect_assert_failure(blocking_ring_buffer_write_allocate_try(ring, 1));
	expect_assert_failure(blocking_ring_buffer_write_try(ring, "foobar", 6));

	assert_true(read_buf / 2 >= 7);
	strcpy(tmp, "barfoo");

	blocking_ring_buffer_write_flush(ring);

	l = read_buf;
	assert_false(blocking_ring_buffer_read(ring, read, &l));
	assert_int_equal(l, read_buf / 2);
	assert_string_equal(tmp, "barfoo");

	assert_true(blocking_ring_buffer_empty(ring));

	assert_false(blocking_ring_buffer_write(ring, "0123456789", 10));
	assert_false(blocking_ring_buffer_write(ring, "0123456789", 10));
	assert_false(blocking_ring_buffer_write(ring, "0123456789", 10));
	assert_false(blocking_ring_buffer_write(ring, "0123456789", 10));
	assert_false(blocking_ring_buffer_write(ring, "0123456789", 10));

	assert_null(blocking_ring_buffer_write_allocate_try(ring, 50));

	blocking_ring_buffer_uninitialize(ring);

	test_free(ring);
	test_free(read);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_write_allocate_try__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(blocking_ring_buffer_write_allocate_try(NULL, 0));

	expect_assert_failure(blocking_ring_buffer_write_allocate_try(ring, 0));
	expect_assert_failure(blocking_ring_buffer_write_allocate_try(NULL, buf_size / 4))

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_write_allocate_try__uninitialized()
{
	enum { buf_size = 232 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_write_allocate_try(ring, 10));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_write_flush()
{
	enum { buf_size = 100 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * tmp;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	assert_false(blocking_ring_buffer_write(ring, "a", 1));

	tmp = blocking_ring_buffer_write_allocate_try(ring, 10);
	assert_non_null(tmp);
	strcpy(tmp, "foo");
	expect_assert_failure(blocking_ring_buffer_write_try(ring, "b", 1));
	blocking_ring_buffer_write_flush(ring);
	assert_false(blocking_ring_buffer_write(ring, "b", 1));

	tmp = blocking_ring_buffer_write_allocate(ring, 10);
	assert_non_null(tmp);
	strcpy(tmp, "bar");
	expect_assert_failure(blocking_ring_buffer_write_try(ring, "c", 1));
	blocking_ring_buffer_write_flush(ring);
	assert_false(blocking_ring_buffer_write(ring, "c", 1));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_write_flush__NULL()
{
	enum { buf_size = 100 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	assert_non_null(blocking_ring_buffer_write_allocate_try(ring, buf_size / 4));

	expect_assert_failure(blocking_ring_buffer_write_flush(NULL));

	blocking_ring_buffer_write_flush(ring);
	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_write_flush__uninitialized()
{
	enum { buf_size = 232 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_write_flush(ring));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_ring_buffer_write_try()
{
	enum { buf_size = 10 + 3 * sizeof(size_t), tmp_size = 20 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(tmp_size);
	size_t l;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	assert_false(blocking_ring_buffer_write_try(ring, "foo", 3));
	assert_false(blocking_ring_buffer_write_try(ring, "bar", 3));
	assert_false(blocking_ring_buffer_write_try(ring, "keke", 4));

	assert_true(blocking_ring_buffer_write_try(ring, "fail", 4));

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 3);
	tmp[l] = '\0';
	assert_string_equal(tmp, "foo");

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 3);
	tmp[l] = '\0';
	assert_string_equal(tmp, "bar");

	assert_true(blocking_ring_buffer_write_try(ring, "size_t 0 fobar", 8 + 7));
	assert_false(blocking_ring_buffer_write_try(ring, "size_t 0 foba", 8 + 6));
	assert_true(blocking_ring_buffer_write_try(ring, "r", 1));

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 4);
	tmp[l] = '\0';
	assert_string_equal(tmp, "keke");

	l = 5;
	assert_true(blocking_ring_buffer_read_try(ring, tmp, &l));

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 14);
	tmp[l] = '\0';
	assert_string_equal(tmp, "size_t 0 foba");

	l = tmp_size;
	assert_true(blocking_ring_buffer_read_try(ring, tmp, &l));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_write_try__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	expect_assert_failure(blocking_ring_buffer_write_try(NULL, NULL, 0));

	expect_assert_failure(blocking_ring_buffer_write_try(ring, NULL, 0));
	expect_assert_failure(blocking_ring_buffer_write_try(NULL, "foobar", 0));
	expect_assert_failure(blocking_ring_buffer_write_try(NULL, NULL, 6));

	expect_assert_failure(blocking_ring_buffer_write_try(ring, NULL, 6));
	expect_assert_failure(blocking_ring_buffer_write_try(ring, "foobar", 0));
	expect_assert_failure(blocking_ring_buffer_write_try(NULL, "foobar", 6));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_write_try__uninitialized()
{
	enum { buf_size = 1000 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_write_try(ring, "foobar", 6));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_read_try()
{
	enum { buf_size = 5 * (2 + sizeof(size_t)) + 1, tmp_size = 20 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(tmp_size);
	size_t l;

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	assert_true(blocking_ring_buffer_read_try(ring, tmp, &l));

	assert_false(blocking_ring_buffer_write_try(ring, "aa", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "bb", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "cc", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "dd", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "ee", 2));
	assert_true(blocking_ring_buffer_write_try(ring, "ff", 2));

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "aa");

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "bb");

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "cc");

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "dd");

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "ee");

	l = tmp_size;
	assert_true(blocking_ring_buffer_read_try(ring, tmp, &l));

	assert_false(blocking_ring_buffer_write_try(ring, "ff", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "gg", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "hh", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "ii", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "jj", 2));
	assert_true(blocking_ring_buffer_write_try(ring, "kk", 2));

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "ff");

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "gg");

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "hh");

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "ii");

	l = tmp_size;
	assert_false(blocking_ring_buffer_read_try(ring, tmp, &l));
	assert_int_equal(l, 2);
	tmp[l] = '\0';
	assert_string_equal(tmp, "jj");

	l = tmp_size;
	assert_true(blocking_ring_buffer_read_try(ring, tmp, &l));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(tmp);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_read_try__NULL()
{
	enum { buf_size = 100 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l = buf_size;

	assert_false(blocking_ring_buffer_initialize(ring, wb, buf_size));

	assert_false(blocking_ring_buffer_write_try(ring, "aa", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "bb", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "cc", 2));

	expect_assert_failure(blocking_ring_buffer_read_try(NULL, NULL, 0));

	expect_assert_failure(blocking_ring_buffer_read_try(NULL, NULL, &l));
	expect_assert_failure(blocking_ring_buffer_read_try(NULL, rb, 0));
	expect_assert_failure(blocking_ring_buffer_read_try(ring, NULL, 0));
		 
	expect_assert_failure(blocking_ring_buffer_read_try(ring, NULL, &l));
	expect_assert_failure(blocking_ring_buffer_read_try(ring, rb, 0));
	expect_assert_failure(blocking_ring_buffer_read_try(NULL, rb, &l));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_read_try__uninitialized()
{
	enum { buf_size = 100 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l = buf_size;

	assert_false(blocking_ring_buffer_initialize(ring, wb, buf_size));

	assert_false(blocking_ring_buffer_write_try(ring, "aa", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "bb", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "cc", 2));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_read_try(ring, rb, &l));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_peek_try()
{
	enum { buf_size = 100 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l;

	assert_false(blocking_ring_buffer_initialize(ring, wb, buf_size));

	l = buf_size;
	assert_true(blocking_ring_buffer_peek_try(ring, rb, &l));

	assert_false(blocking_ring_buffer_write_try(ring, "aa", 2));
	l = buf_size;
	strncpy(rb, "foobar", 6);
	assert_false(blocking_ring_buffer_peek_try(ring, rb, &l));
	assert_int_equal(l, 2);
	rb[2] = '\0';
	assert_string_equal(rb, "aa");

	assert_false(blocking_ring_buffer_write_try(ring, "bb", 2));
	l = buf_size;
	strncpy(rb, "foobar", 6);
	assert_false(blocking_ring_buffer_peek_try(ring, rb, &l));
	assert_int_equal(l, 2);
	rb[2] = '\0';
	assert_string_equal(rb, "aa");

	assert_false(blocking_ring_buffer_write_try(ring, "cc", 2));
	l = buf_size;
	strncpy(rb, "foobar", 6);
	assert_false(blocking_ring_buffer_peek_try(ring, rb, &l));
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
			assert_false(blocking_ring_buffer_peek_try(ring, rb, &l));
			assert_int_equal(l, 2);
			rb[l] = '\0';
			assert_string_equal(rb, e);
		}

		l = buf_size;
		assert_false(blocking_ring_buffer_read_try(ring, rb, &l));
		assert_int_equal(l, 2);
		rb[l] = '\0';
		assert_string_equal(rb, e);
	}

	l = buf_size;
	assert_true(blocking_ring_buffer_peek_try(ring, rb, &l));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_peek_try__NULL()
{
	enum { buf_size = 100 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l = buf_size;

	assert_false(blocking_ring_buffer_initialize(ring, wb, buf_size));

	assert_false(blocking_ring_buffer_write_try(ring, "aa", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "bb", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "cc", 2));

	expect_assert_failure(blocking_ring_buffer_peek_try(NULL, NULL, 0));

	expect_assert_failure(blocking_ring_buffer_peek_try(NULL, NULL, &l));
	expect_assert_failure(blocking_ring_buffer_peek_try(NULL, rb, 0));
	expect_assert_failure(blocking_ring_buffer_peek_try(ring, NULL, 0));
		 
	expect_assert_failure(blocking_ring_buffer_peek_try(ring, NULL, &l));
	expect_assert_failure(blocking_ring_buffer_peek_try(ring, rb, 0));
	expect_assert_failure(blocking_ring_buffer_peek_try(NULL, rb, &l));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_peek_try__uninitialized()
{
	enum { buf_size = 100 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);
	size_t l = buf_size;

	assert_false(blocking_ring_buffer_initialize(ring, wb, buf_size));

	assert_false(blocking_ring_buffer_write_try(ring, "aa", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "bb", 2));
	assert_false(blocking_ring_buffer_write_try(ring, "cc", 2));

	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_peek_try(ring, rb, &l));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

struct tuple__UT_blocking_ring_buffer_empty
{
	struct blocking_ring_buffer * ring;
	int count;
	struct lock * lock;
};

static void *
helper__UT_blocking_ring_buffer_empty(struct tuple__UT_blocking_ring_buffer_empty * t)
{
	assert(t);

	char buf[3];
	size_t l = sizeof(buf);

	if (lock_acquire(t->lock))
		return (void *)-1;

	if ((bool)t->count == (blocking_ring_buffer_empty(t->ring) > 0))
		goto err;

	if (!blocking_ring_buffer_write_try(t->ring, "foo", 3))
		t->count++;

	lock_release(t->lock);

	if (blocking_ring_buffer_empty(t->ring) > 0)
		goto err;

	pthread_yield();

	if (blocking_ring_buffer_empty(t->ring) > 0)
		goto err;

	if (lock_acquire(t->lock))
		return (void *)-1;

	if (!blocking_ring_buffer_read_try(t->ring, buf, &l))
		t->count--;

	if ((bool)t->count == (blocking_ring_buffer_empty(t->ring) > 0))
		goto err;

	lock_release(t->lock);
	return NULL;
err:
	lock_release(t->lock);
	return (void*)1;
}

static void
UT_blocking_ring_buffer_empty()
{
	enum { thread_count = 1, buf_size = thread_count * (sizeof(pthread_t) + sizeof(size_t)) };
	pthread_t threads[thread_count];
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	struct lock * l = test_malloc(sizeof(struct lock));
	char * buf = test_malloc(buf_size);
	void * r;
	struct tuple__UT_blocking_ring_buffer_empty tuple = {ring, 0, l};

	assert_false(lock_initialize(l));
	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_ring_buffer_empty, &tuple));

	for (int i = 0 ; i < thread_count ; i++)
	{
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	assert_false(blocking_ring_buffer_uninitialize(ring));
	assert_false(lock_uninitialize(l));

	test_free(buf);
	test_free(ring);
	test_free(l);
}

/************************************************************************/

static void
UT_blocking_ring_buffer_empty__NULL()
{
	expect_assert_failure(blocking_ring_buffer_empty(NULL));
}

/************************************************************************/

static void
UT_blocking_ring_buffer_empty__uninitialized()
{
	enum { buf_size = 100 };
	struct blocking_ring_buffer * ring = test_malloc(sizeof(struct blocking_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_ring_buffer_initialize(ring, buf, buf_size));
	assert_false(blocking_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_ring_buffer_empty(ring));

	test_free(buf);
	test_free(ring);
}

/************************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_blocking_ring_buffer_initialize),
		cmocka_unit_test(UT_blocking_ring_buffer_initialize__NULL),

		cmocka_unit_test(UT_blocking_ring_buffer_uninitialize),
		cmocka_unit_test(UT_blocking_ring_buffer_uninitialize__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_uninitialize__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_write),
		cmocka_unit_test(UT_blocking_ring_buffer_write__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_write__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_write_try),
		cmocka_unit_test(UT_blocking_ring_buffer_write_try__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_write_try__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_overwrite),
		cmocka_unit_test(UT_blocking_ring_buffer_overwrite__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_overwrite__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_read),
		cmocka_unit_test(UT_blocking_ring_buffer_read__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_read__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_read_try),
		cmocka_unit_test(UT_blocking_ring_buffer_read_try__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_read_try__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_peek),
		cmocka_unit_test(UT_blocking_ring_buffer_peek__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_peek__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_peek_try),
		cmocka_unit_test(UT_blocking_ring_buffer_peek_try__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_peek_try__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_write_allocate),
		cmocka_unit_test(UT_blocking_ring_buffer_write_allocate__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_write_allocate__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_write_allocate_try),
		cmocka_unit_test(UT_blocking_ring_buffer_write_allocate_try__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_write_allocate_try__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_write_flush),
		cmocka_unit_test(UT_blocking_ring_buffer_write_flush__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_write_flush__uninitialized),

		cmocka_unit_test(UT_blocking_ring_buffer_empty),
		cmocka_unit_test(UT_blocking_ring_buffer_empty__NULL),
		cmocka_unit_test(UT_blocking_ring_buffer_empty__uninitialized)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
