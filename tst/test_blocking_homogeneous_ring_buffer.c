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
#include <unistd.h>
#include "cmocka-wrapper.h"
#include "blocking_homogeneous_ring_buffer.h"

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_initialize()
{
	enum { buf_size = 1000, element_size = 10 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, element_size));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_initialize__NULL()
{
	enum { buf_size = 1000, element_size = 10 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(NULL, NULL, 0, 0));

	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(NULL, NULL, 0, element_size));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(NULL, NULL, buf_size, 0));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(NULL, buf, 0, 0));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(ring, NULL, 0, 0));

	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(NULL, NULL, buf_size, element_size));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(NULL, buf, 0, element_size));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(ring, NULL, 0, element_size));

	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(ring, buf, 0, 0));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(ring, NULL, buf_size, 0));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(NULL, buf, buf_size, 0));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(ring, buf, 0, element_size));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(ring, NULL, buf_size, element_size));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(NULL, buf, buf_size, element_size));

	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(NULL, buf, buf_size, element_size));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(ring, NULL, buf_size, element_size));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(ring, buf, 0, element_size));
	expect_assert_failure(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 0));

	test_free(buf);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_uninitialize()
{
	enum { buf_size = 1000, element_size = 1 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, element_size));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_uninitialize__NULL()
{
	expect_assert_failure(blocking_homogeneous_ring_buffer_uninitialize(NULL));
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_uninitialize__uninitialized()
{
	enum { buf_size = 1000, element_size = buf_size };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, element_size));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void *
helper__UT_blocking_homogeneous_ring_buffer_write(struct blocking_homogeneous_ring_buffer * b)
{
	assert(b);

	pthread_t p = pthread_self();
	if (blocking_homogeneous_ring_buffer_write(b, &p))
		return (void *)1;

	return NULL;
}

static void
UT_blocking_homogeneous_ring_buffer_write()
{
	enum { thread_count = 1000, buf_size = thread_count * (sizeof(pthread_t) + sizeof(size_t)) };
	pthread_t threads[thread_count];
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);
	void * r;

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, sizeof(pthread_t)));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_homogeneous_ring_buffer_write, ring));

	for (int i = 0 ; i < thread_count ; i++)
	{
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	for (int i = 0 ; i < thread_count ; i++)
	{
		pthread_t p;
		int j;
		assert_false(blocking_homogeneous_ring_buffer_read_try(ring, &p));

		for (j = 0 ; j < thread_count; j++)
			if (p == threads[j])
			{
				threads[j] = 0;
				break;
			}

		assert_int_not_equal(j, thread_count);
	}

	assert_true(blocking_homogeneous_ring_buffer_empty(ring) > 0);

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write__NULL()
{
	enum { buf_size = 1000, element_size = 3 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, element_size));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write(NULL, NULL));
	expect_assert_failure(blocking_homogeneous_ring_buffer_write(ring, NULL));
	expect_assert_failure(blocking_homogeneous_ring_buffer_write(NULL, "abc"));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write__uninitialized()
{
	enum { buf_size = 1000, element_size = 7 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, element_size));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write(ring, "foobar\0"));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void *
helper__UT_blocking_homogeneous_ring_buffer_overwrite(struct blocking_homogeneous_ring_buffer * b)
{
	assert(b);

	pthread_t p = pthread_self();

	if (blocking_homogeneous_ring_buffer_overwrite(b, &p))
		return (void *)1;

	return NULL;
}

static void
UT_blocking_homogeneous_ring_buffer_overwrite()
{
	enum { thread_count = 1000, buf_size = 200 };
	pthread_t threads[thread_count];
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);
	void * r;

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, sizeof(pthread_t)));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_homogeneous_ring_buffer_overwrite, ring));

	for (int i = 0 ; i < thread_count ; i++)
	{
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	do {
		pthread_t p;
		int j;
		if (blocking_homogeneous_ring_buffer_read_try(ring, &p))
			break;

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

	assert_true(blocking_homogeneous_ring_buffer_empty(ring) > 0);

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_overwrite__NULL()
{
	enum { buf_size = 1000, element_size = 11 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, element_size));

	expect_assert_failure(blocking_homogeneous_ring_buffer_overwrite(NULL, NULL));

	expect_assert_failure(blocking_homogeneous_ring_buffer_overwrite(ring, NULL));
	expect_assert_failure(blocking_homogeneous_ring_buffer_overwrite(NULL, "01234567890\0"));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_overwrite__uninitialized()
{
	enum { buf_size = 1000, element_size = 6 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, element_size));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_overwrite(ring, "foobar"));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

struct tuple__UT_blocking_homogeneous_ring_buffer_read
{
	struct blocking_homogeneous_ring_buffer *r;
	struct lock *l;
	uint8_t res[25][25];
};

static void *
helper__UT_blocking_homogeneous_ring_buffer_read(struct tuple__UT_blocking_homogeneous_ring_buffer_read *t)
{
	assert(t);

	char b[3];

	if (blocking_homogeneous_ring_buffer_read(t->r, &b))
		return (void *)1;

	if (lock_acquire(t->l))
		return (void *)1;

	t->res[b[0] - 'a'][b[1] - 'a']++;

	if (lock_release(t->l))
		return (void *)1;

	return NULL;
}

static void
UT_blocking_homogeneous_ring_buffer_read()
{
	enum { thread_count = 25*25, buf_size = thread_count * (2 + sizeof(size_t)) };
	pthread_t threads[thread_count];
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	struct lock * lock = test_malloc(sizeof(struct lock));
	struct tuple__UT_blocking_homogeneous_ring_buffer_read t = {ring, lock, {{0}}};
	char * buf = test_malloc(buf_size);

	lock_initialize(lock);
	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 2));

	for (int i = 0 ; i < 25 ; i++)
		for (int j = 0 ; j < 25 ; j++)
			assert_false(blocking_homogeneous_ring_buffer_write(ring, (char []){ 'a' + j, 'a' + i }));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_homogeneous_ring_buffer_read, &t));

	for (int i = 0 ; i < thread_count ; i++)
	{
		void * r;
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	assert_true(blocking_homogeneous_ring_buffer_empty(ring) > 0);

	for (int i = 0 ; i < 25 ; i++)
		for (int j = 0 ; j < 25 ; j++)
			assert_int_equal(t.res[i][j], 1);

	lock_uninitialize(lock);
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(lock);
	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_read__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * tmp = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 2));

	assert_false(blocking_homogeneous_ring_buffer_write(ring, "aa"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "bb"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "cc"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "dd"));

	expect_assert_failure(blocking_homogeneous_ring_buffer_read(NULL, NULL));
	expect_assert_failure(blocking_homogeneous_ring_buffer_read(ring, NULL));
	expect_assert_failure(blocking_homogeneous_ring_buffer_read(NULL, tmp));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_read__uninitialized()
{
	enum { buf_size = 1000 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(10);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 6));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "foobar"));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_read(ring, tmp));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void *
helper__UT_blocking_homogeneous_ring_buffer_peek(struct blocking_homogeneous_ring_buffer * r)
{
	assert(r);

	char b[2][3] = { {0} };

	if (blocking_homogeneous_ring_buffer_peek(r, &b[0])
			|| blocking_homogeneous_ring_buffer_read(r, &b[1])
			|| (int)b[0][0] * 255 + (int)b[0][1] > (int)b[1][0] * 255 + (int)b[1][1])
		return (void *)1;
	return NULL;
}

static void
UT_blocking_homogeneous_ring_buffer_peek()
{
	enum { thread_count = 25*25, buf_size = thread_count * (2 + sizeof(size_t)) };
	pthread_t threads[thread_count];
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 2));

	for (int i = 0 ; i < 26 ; i++)
		for (int j = 0 ; j < 25 ; j++)
			assert_false(blocking_homogeneous_ring_buffer_overwrite(ring, (char []){ 'a' + i, 'a' + j }));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_homogeneous_ring_buffer_peek, ring));

	for (int i = 0 ; i < thread_count ; i++)
	{
		void * r;
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_peek__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * tmp = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 2));

	assert_false(blocking_homogeneous_ring_buffer_write(ring, "aa"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "bb"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "cc"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "dd"));

	expect_assert_failure(blocking_homogeneous_ring_buffer_peek(NULL, NULL));

	expect_assert_failure(blocking_homogeneous_ring_buffer_peek(ring, NULL));
	expect_assert_failure(blocking_homogeneous_ring_buffer_peek(NULL, tmp));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_peek__uninitialized()
{
	enum { buf_size = 1000 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * tmp = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 2));

	assert_false(blocking_homogeneous_ring_buffer_write(ring, "aa"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "bb"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "cc"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "dd"));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_peek(ring, tmp));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_allocate()
{
	enum { buf_size = 100, read_buf = 20 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * read = test_malloc(read_buf),
	     * tmp;

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 6));

	tmp = blocking_homogeneous_ring_buffer_write_allocate(ring);
	assert_non_null(tmp);

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_allocate(ring));
	expect_assert_failure(blocking_homogeneous_ring_buffer_write_try(ring, "xxxxxx"));

	strcpy(tmp, "foobar");

	assert_false(blocking_homogeneous_ring_buffer_write_flush(ring));

	assert_false(blocking_homogeneous_ring_buffer_read(ring, read));
	assert_string_equal(tmp, "foobar");

	assert_true(blocking_homogeneous_ring_buffer_empty(ring) > 0);

	blocking_homogeneous_ring_buffer_uninitialize(ring);

	test_free(ring);
	test_free(read);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_allocate__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 0xf));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_allocate(NULL));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_allocate__uninitialized()
{
	enum { buf_size = 232 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 5));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_allocate(ring));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_allocate_try()
{
	enum { buf_size = 55, read_buf = 20 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * read = test_malloc(read_buf),
	     * tmp;

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 10));

	tmp = blocking_homogeneous_ring_buffer_write_allocate_try(ring);
	assert_non_null(tmp);

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_allocate_try(ring));
	expect_assert_failure(blocking_homogeneous_ring_buffer_write_try(ring, "abcdefghij"));

	strcpy(tmp, "0123456789");

	blocking_homogeneous_ring_buffer_write_flush(ring);

	assert_false(blocking_homogeneous_ring_buffer_read(ring, read));
	assert_string_equal(tmp, "0123456789");

	assert_true(blocking_homogeneous_ring_buffer_empty(ring) > 0);

	assert_false(blocking_homogeneous_ring_buffer_write(ring, "0123456789"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "0123456789"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "0123456789"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "0123456789"));
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "0123456789"));

	assert_null(blocking_homogeneous_ring_buffer_write_allocate_try(ring));

	blocking_homogeneous_ring_buffer_uninitialize(ring);

	test_free(ring);
	test_free(read);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_allocate_try__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 22));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_allocate_try(NULL));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_allocate_try__uninitialized()
{
	enum { buf_size = 232 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 7));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_allocate_try(ring));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_flush()
{
	enum { buf_size = 100 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size),
	     * tmp;

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 1));

	assert_false(blocking_homogeneous_ring_buffer_write(ring, "a"));

	tmp = blocking_homogeneous_ring_buffer_write_allocate_try(ring);
	assert_non_null(tmp);
	strcpy(tmp, "A");
	expect_assert_failure(blocking_homogeneous_ring_buffer_write_try(ring, "b"));
	blocking_homogeneous_ring_buffer_write_flush(ring);
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "B"));

	tmp = blocking_homogeneous_ring_buffer_write_allocate(ring);
	assert_non_null(tmp);
	strcpy(tmp, "C");
	expect_assert_failure(blocking_homogeneous_ring_buffer_write_try(ring, "c"));
	blocking_homogeneous_ring_buffer_write_flush(ring);
	assert_false(blocking_homogeneous_ring_buffer_write(ring, "D"));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(ring);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_flush__NULL()
{
	enum { buf_size = 100 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 4));

	assert_non_null(blocking_homogeneous_ring_buffer_write_allocate_try(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_flush(NULL));

	blocking_homogeneous_ring_buffer_write_flush(ring);
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_flush__uninitialized()
{
	enum { buf_size = 232 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 6));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_flush(ring));

	test_free(ring);
	test_free(buf);
}

/***********************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_try()
{
	enum { buf_size = 9, tmp_size = 20 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(tmp_size);
	tmp[3] = '\0';

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 3));

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "foo"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "bar"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "kek"));

	assert_true(blocking_homogeneous_ring_buffer_write_try(ring, "err"));

	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "foo");

	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "bar");

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "abc"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "def"));
	assert_true(blocking_homogeneous_ring_buffer_write_try(ring, "err"));

	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "kek");
	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "abc");
	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "def");

	assert_true(blocking_homogeneous_ring_buffer_empty(ring) > 0);

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
	test_free(tmp);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_try__NULL()
{
	enum { buf_size = 1000 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 5));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_try(NULL, NULL));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_try(ring, NULL));
	expect_assert_failure(blocking_homogeneous_ring_buffer_write_try(NULL, "foobar"));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_write_try__uninitialized()
{
	enum { buf_size = 1000 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 6));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_write_try(ring, "foobar"));

	test_free(ring);
	test_free(buf);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_read_try()
{
	enum { buf_size = 5 * 2 + 1, tmp_size = 20 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size),
		 * tmp = test_malloc(tmp_size);

	tmp[2] = '\0';

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 2));

	assert_true(blocking_homogeneous_ring_buffer_read_try(ring, tmp));

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "aa"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "bb"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "cc"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "dd"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "ee"));
	assert_true(blocking_homogeneous_ring_buffer_write_try(ring, "ff"));

	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "aa");

	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "bb");

	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "cc");

	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "dd");

	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "ee");


	assert_true(blocking_homogeneous_ring_buffer_read_try(ring, tmp));

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "ff"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "gg"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "hh"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "ii"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "jj"));
	assert_true(blocking_homogeneous_ring_buffer_write_try(ring, "kk"));

	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "ff");
	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "gg");
	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "hh");
	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "ii");
	assert_false(blocking_homogeneous_ring_buffer_read_try(ring, tmp));
	assert_string_equal(tmp, "jj");

	assert_true(blocking_homogeneous_ring_buffer_read_try(ring, tmp));

	assert_true(blocking_homogeneous_ring_buffer_empty(ring) > 0);

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(buf);
	test_free(tmp);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_read_try__NULL()
{
	enum { buf_size = 100 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, wb, buf_size, 2));

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "aa"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "bb"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "cc"));

	expect_assert_failure(blocking_homogeneous_ring_buffer_read_try(NULL, NULL));

	expect_assert_failure(blocking_homogeneous_ring_buffer_read_try(NULL, rb));
	expect_assert_failure(blocking_homogeneous_ring_buffer_read_try(ring, NULL));
		 
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_read_try__uninitialized()
{
	enum { buf_size = 100 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, wb, buf_size, 2));

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "aa"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "bb"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "cc"));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_read_try(ring, rb));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_peek_try()
{
	enum { buf_size = 100 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, wb, buf_size, 2));

	assert_true(blocking_homogeneous_ring_buffer_peek_try(ring, rb));

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "aa"));
	strcpy(rb, "xx");
	assert_false(blocking_homogeneous_ring_buffer_peek_try(ring, rb));
	assert_string_equal(rb, "aa");

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "bb"));
	strcpy(rb, "xx");
	assert_false(blocking_homogeneous_ring_buffer_peek_try(ring, rb));
	assert_string_equal(rb, "aa");

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "cc"));
	strcpy(rb, "xx");
	assert_false(blocking_homogeneous_ring_buffer_peek_try(ring, rb));
	assert_string_equal(rb, "aa");

	for (int j = 0 ; j < 3; j++)
	{
		char e[] = "aa\0";
		e[0] += j;
		e[1] += j;

		for (int i = 0 ; i < 10 ; i++)
		{
			assert_false(blocking_homogeneous_ring_buffer_peek_try(ring, rb));
			assert_string_equal(rb, e);
		}

		assert_false(blocking_homogeneous_ring_buffer_read_try(ring, rb));
		assert_string_equal(rb, e);
	}

	assert_true(blocking_homogeneous_ring_buffer_peek_try(ring, rb));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_peek_try__NULL()
{
	enum { buf_size = 100 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, wb, buf_size, 2));

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "aa"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "bb"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "cc"));

	expect_assert_failure(blocking_homogeneous_ring_buffer_peek_try(NULL, NULL));

	expect_assert_failure(blocking_homogeneous_ring_buffer_peek_try(NULL, rb));
	expect_assert_failure(blocking_homogeneous_ring_buffer_peek_try(ring, NULL));
		 
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_peek_try__uninitialized()
{
	enum { buf_size = 100 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * rb = test_malloc(buf_size),
	     * wb = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, wb, buf_size, 2));

	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "aa"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "bb"));
	assert_false(blocking_homogeneous_ring_buffer_write_try(ring, "cc"));

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_peek_try(ring, rb));

	test_free(rb);
	test_free(wb);
	test_free(ring);
}

/************************************************************************/

struct tuple__UT_blocking_homogeneous_ring_buffer_empty
{
	struct blocking_homogeneous_ring_buffer * ring;
	int count;
	struct lock * lock;
};

static void *
helper__UT_blocking_homogeneous_ring_buffer_empty(struct tuple__UT_blocking_homogeneous_ring_buffer_empty * t)
{
	assert(t);

	char buf[3];

	if (lock_acquire(t->lock))
		return (void *)-1;

	if ((bool)t->count == (blocking_homogeneous_ring_buffer_empty(t->ring) > 0))
		goto err;

	if (!blocking_homogeneous_ring_buffer_write_try(t->ring, "foo"))
		t->count++;

	if (lock_release(t->lock))
		goto err;

	if (blocking_homogeneous_ring_buffer_empty(t->ring) > 0)
		goto err;

	pthread_yield();

	if (blocking_homogeneous_ring_buffer_empty(t->ring) > 0)
		goto err;

	if (lock_acquire(t->lock))
		return (void *)-1;

	if (!blocking_homogeneous_ring_buffer_read_try(t->ring, buf))
		t->count--;

	if ((bool)t->count == (blocking_homogeneous_ring_buffer_empty(t->ring) > 0))
		goto err;

	return lock_release(t->lock) ? (void*)-1 : NULL;
err:
	lock_release(t->lock);
	return (void*)1;
}

static void
UT_blocking_homogeneous_ring_buffer_empty()
{
	enum { thread_count = 1, buf_size = thread_count * (sizeof(pthread_t) + sizeof(size_t)) };
	pthread_t threads[thread_count];
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	struct lock * l = test_malloc(sizeof(struct lock));
	char * buf = test_malloc(buf_size);
	void * r;
	struct tuple__UT_blocking_homogeneous_ring_buffer_empty tuple = {ring, 0, l};

	assert_false(lock_initialize(l));
	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 3));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&threads[i], 0, (void * (*) (void *))helper__UT_blocking_homogeneous_ring_buffer_empty, &tuple));

	for (int i = 0 ; i < thread_count ; i++)
	{
		assert_false(pthread_join(threads[i], &r));
		assert_null(r);
	}

	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));
	assert_false(lock_uninitialize(l));

	test_free(buf);
	test_free(ring);
	test_free(l);
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_empty__NULL()
{
	expect_assert_failure(blocking_homogeneous_ring_buffer_empty(NULL));
}

/************************************************************************/

static void
UT_blocking_homogeneous_ring_buffer_empty__uninitialized()
{
	enum { buf_size = 100 };
	struct blocking_homogeneous_ring_buffer * ring = test_malloc(sizeof(struct blocking_homogeneous_ring_buffer));
	char * buf = test_malloc(buf_size);

	assert_false(blocking_homogeneous_ring_buffer_initialize(ring, buf, buf_size, 2));
	assert_false(blocking_homogeneous_ring_buffer_uninitialize(ring));

	expect_assert_failure(blocking_homogeneous_ring_buffer_empty(ring));

	test_free(buf);
	test_free(ring);
}

/************************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_initialize),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_initialize__NULL),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_uninitialize),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_uninitialize__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_uninitialize__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_try),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_try__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_try__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_overwrite),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_overwrite__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_overwrite__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_read),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_read__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_read__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_read_try),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_read_try__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_read_try__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_peek),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_peek__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_peek__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_peek_try),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_peek_try__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_peek_try__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_allocate),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_allocate__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_allocate__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_allocate_try),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_allocate_try__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_allocate_try__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_flush),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_flush__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_write_flush__uninitialized),

		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_empty),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_empty__NULL),
		cmocka_unit_test(UT_blocking_homogeneous_ring_buffer_empty__uninitialized)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
