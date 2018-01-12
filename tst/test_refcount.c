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

#include <pthread.h>
#include "cmocka-wrapper.h"
#include "refcount.h"

/***********************************************************************/

static void
UT_refcount_initialize_to()
{
	struct refcount *r = test_malloc(sizeof(struct refcount));

	refcount_initialize_to(r, 0);
	refcount_initialize_to(r, 1);
	refcount_initialize_to(r, 2);
	refcount_initialize_to(r, 1000);
	refcount_initialize_to(r, (uint16_t)-1);

	expect_assert_failure(refcount_initialize_to(NULL, 0));
	expect_assert_failure(refcount_initialize_to(NULL, 1));

	test_free(r);
}

/***********************************************************************/

static void
UT_refcount_initialize()
{
	struct refcount *r = test_malloc(sizeof(struct refcount));

	refcount_initialize(r);
	expect_assert_failure(refcount_initialize(NULL));

	test_free(r);
}

/***********************************************************************/

static void
UT_refcount_acquire()
{
	struct refcount *r = test_malloc(sizeof(struct refcount));

	refcount_initialize(r);

	for (int i = 0; i < (uint16_t) - 1; ++i)
		assert_int_equal(refcount_acquire(r), i);
	expect_assert_failure(refcount_acquire(r));

	test_free(r);
}

/***********************************************************************/

static void
UT_refcount_acquire__NULL()
{
	expect_assert_failure(refcount_acquire(NULL));
}

/***********************************************************************/

static void
UT_refcount_acquire__uninitialized()
{
	struct refcount * r = test_malloc(sizeof(struct refcount));

	memset(r, 0, sizeof(*r));

	expect_assert_failure(refcount_acquire(r));

	test_free(r);
}

/***********************************************************************/

static void
UT_refcount_release()
{
	struct refcount *r = test_malloc(sizeof(struct refcount));

	refcount_initialize_to(r, (uint16_t)-1);

	for (int i = (uint16_t)-1; i > 0; --i)
		assert_int_equal(refcount_release(r), i);
	expect_assert_failure(refcount_release(r));

	test_free(r);
}

/***********************************************************************/

static void
UT_refcount_release__NULL()
{
	expect_assert_failure(refcount_release(NULL));
}

/***********************************************************************/

static void
UT_refcount_release__uninitialized()
{
	struct refcount * r = test_malloc(sizeof(struct refcount));

	memset(r, 0, sizeof(*r));

	expect_assert_failure(refcount_release(r));

	test_free(r);
}

/***********************************************************************/

static void
UT_refcount_check()
{
	struct refcount *r = test_malloc(sizeof(struct refcount));

	refcount_initialize(r);

	assert_int_equal(refcount_check(r), 0);
	assert_int_equal(refcount_acquire(r), 0);
	assert_int_equal(refcount_check(r), 1);
	assert_int_equal(refcount_acquire(r), 1);
	assert_int_equal(refcount_check(r), 2);
	assert_int_equal(refcount_acquire(r), 2);
	assert_int_equal(refcount_check(r), 3);
	assert_int_equal(refcount_release(r), 3);
	assert_int_equal(refcount_check(r), 2);
	assert_int_equal(refcount_release(r), 2);
	assert_int_equal(refcount_check(r), 1);
	assert_int_equal(refcount_acquire(r), 1);
	assert_int_equal(refcount_check(r), 2);
	assert_int_equal(refcount_release(r), 2);
	assert_int_equal(refcount_check(r), 1);
	assert_int_equal(refcount_release(r), 1);
	assert_int_equal(refcount_check(r), 0);

	test_free(r);
}

/***********************************************************************/

static void
UT_refcount_check__NULL()
{
	expect_assert_failure(refcount_release(NULL));
}

/***********************************************************************/

static void
UT_refcount_check__uninitialized()
{
	struct refcount * r = test_malloc(sizeof(struct refcount));

	memset(r, 0, sizeof(*r));

	expect_assert_failure(refcount_check(r));

	test_free(r);
}

/***********************************************************************/

static void
test_basic_reference_counting()
{
	int i;
	struct refcount *r = test_malloc(sizeof(struct refcount));

	refcount_initialize_to(r, 5);

	for (i = 5 ; i < 250 ; i++)
		assert_int_equal(refcount_acquire(r), i);

	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);
	assert_int_equal(refcount_acquire(r), i++);
	assert_int_equal(refcount_release(r), i--);

	for ( ; i ; i--)
		assert_int_equal(refcount_release(r), i);

	test_free(r);
}

/***********************************************************************/

static void *
_inc(struct refcount *r)
{
	assert(r);

	refcount_acquire(r);
	refcount_acquire(r);

	return NULL;
}

static void *
_dec(struct refcount *r)
{
	assert(r);

	refcount_release(r);
	refcount_release(r);

	return NULL;
}

static void
FT_refcount_acquire_n_release_in_parallel()
{
	enum consts { thread_count = 100 };
	struct refcount *r = test_malloc(sizeof(struct refcount));
	pthread_t ts[thread_count];

	refcount_initialize(r);

	for (int i = 0; i < thread_count ; ++i)
		pthread_create(&ts[i], NULL, (void*(*)(void*))_inc, r);

	for (int i = 0; i < thread_count ; ++i)
		pthread_join(ts[i], NULL);

	assert_int_equal(refcount_release(r), 2 * thread_count);

	for (int i = 0; i < thread_count - 1; ++i)
		pthread_create(&ts[i], NULL, (void*(*)(void*))_dec, r);

	for (int i = 0; i < thread_count - 1; ++i)
		pthread_join(ts[i], NULL);
	refcount_release(r);

	assert_int_equal(refcount_acquire(r), 0);

	test_free(r);
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_refcount_initialize),
		cmocka_unit_test(UT_refcount_initialize_to),
		cmocka_unit_test(UT_refcount_acquire),
		cmocka_unit_test(UT_refcount_acquire__NULL),
		cmocka_unit_test(UT_refcount_acquire__uninitialized),
		cmocka_unit_test(UT_refcount_release),
		cmocka_unit_test(UT_refcount_release__NULL),
		cmocka_unit_test(UT_refcount_release__uninitialized),
		cmocka_unit_test(UT_refcount_check),
		cmocka_unit_test(UT_refcount_check__NULL),
		cmocka_unit_test(UT_refcount_check__uninitialized),
		cmocka_unit_test(FT_refcount_acquire_n_release_in_parallel),
		cmocka_unit_test(test_basic_reference_counting)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
