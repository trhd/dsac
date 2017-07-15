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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cmocka-wrapper.h"
#include "lock.h"

/***********************************************************************/

static void
UT_lock_initialize()
{
	struct lock *l = test_malloc(sizeof(struct lock));

	expect_assert_failure(lock_initialize(NULL));

	assert_false(lock_initialize(l));
	assert_false(lock_uninitialize(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_lock_initialize__NULL()
{
	expect_assert_failure(lock_initialize(NULL));
}

/***********************************************************************/

static void
UT_lock_uninitialize()
{
	struct lock *l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(lock_uninitialize(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_lock_uninitialize__uninitialized()
{
	struct lock *l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(lock_uninitialize(l));

	expect_assert_failure(lock_uninitialize(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_lock_uninitialize__NULL()
{
	expect_assert_failure(lock_uninitialize(NULL));
}

/***********************************************************************/

static void
UT_lock_acquire()
{
	struct lock *l = test_malloc(sizeof(struct lock));
	assert_false(lock_initialize(l));

	assert_false(lock_acquire(l));
	expect_assert_failure(lock_acquire(l));

	assert_false(lock_release(l));
	assert_false(lock_uninitialize(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_lock_acquire__uninitialized()
{
	struct lock *l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(lock_uninitialize(l));

	expect_assert_failure(lock_acquire(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_lock_acquire__NULL()
{
	expect_assert_failure(lock_acquire(NULL));
}

/***********************************************************************/

static void
UT_lock_acquire_try()
{
	struct lock *l = test_malloc(sizeof(struct lock));
	assert_false(lock_initialize(l));

	assert_false(lock_acquire_try(l));
	assert_true(lock_acquire_try(l));
	assert_false(lock_release(l));

	assert_false(lock_acquire(l));
	assert_true(lock_acquire_try(l));
	assert_false(lock_release(l));

	assert_false(lock_acquire_try(l));
	expect_assert_failure(lock_acquire(l));
	assert_false(lock_release(l));

	assert_false(lock_uninitialize(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_lock_acquire_try__uninitialized()
{
	struct lock *l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(lock_uninitialize(l));

	expect_assert_failure(lock_acquire_try(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_lock_acquire_try__NULL()
{
	expect_assert_failure(lock_acquire_try(NULL));
}

/***********************************************************************/

static void
UT_lock_release()
{

	struct lock *l = test_malloc(sizeof(struct lock));
	assert_false(lock_initialize(l));

	assert_true(lock_release(l));

	assert_false(lock_acquire(l));
	assert_false(lock_release(l));

	assert_true(lock_release(l));

	assert_false(lock_uninitialize(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_lock_release__uninitialized()
{

	struct lock *l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));

	assert_false(lock_acquire(l));
	assert_true(lock_uninitialize(l));

	assert_false(lock_release(l));

	assert_false(lock_uninitialize(l));

	expect_assert_failure(lock_release(l));

	test_free(l);
}

/***********************************************************************/


static void
UT_lock_release__NULL()
{
	expect_assert_failure(lock_release(NULL));
}

/***********************************************************************/

static void
UT_lock_assert()
{
	struct lock *l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));

	expect_assert_failure(lock_assert(l));

	assert_false(lock_acquire(l));
	lock_assert(l);

	assert_false(lock_release(l));
	expect_assert_failure(lock_assert(l));

	assert_false(lock_uninitialize(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_lock_assert__uninitialized()
{
	struct lock *l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(lock_uninitialize(l));

	expect_assert_failure(lock_assert(l));

	test_free(l);
}

/***********************************************************************/


static void
UT_lock_assert__NULL()
{
	expect_assert_failure(lock_assert(NULL));
}

/***********************************************************************/

static void
FT_test_basic_usage()
{
	struct lock *l = test_malloc(sizeof(struct lock));;

	assert_false(lock_initialize(l));

	for (volatile int i = 0; i < 1000; ++i)
	{
		expect_assert_failure(lock_assert(l));
		assert_false(lock_acquire(l));
		expect_assert_failure(lock_acquire(l));
		lock_assert(l);
		assert_false(lock_release(l));
		assert_true(lock_release(l));
	}

	assert_false(lock_uninitialize(l));

	test_free(l);
}

/***********************************************************************/

static pthread_t concurrent_basic_shared_varialbe;

static void *
_concurrent_basic(struct lock *l)
{
	for (int i = 0; i < 20; ++i)
	{
		struct timespec delay = { 0, ( rand() % 1000 ) * 10 * 1000 };

		assert_false(lock_acquire(l));
		lock_assert(l);

		concurrent_basic_shared_varialbe = pthread_self();

		assert_false(nanosleep(&delay, NULL));
		assert_int_equal(concurrent_basic_shared_varialbe, pthread_self());

		lock_assert(l);
		assert_false(lock_release(l));
	}

	return NULL;
}

static void
concurrent_basic(int thread_count)
{
	pthread_t threads[thread_count];
	struct lock *l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));

	for (int i = 0; i < thread_count; i++)
		assert_false(pthread_create(&threads[i], NULL, (void*(*)(void*))_concurrent_basic, l));

	for (int i = 0; i < thread_count; i++)
		assert_false(pthread_join(threads[i], NULL));

	assert_false(lock_uninitialize(l));

	test_free(l);
}

static void
FT_test_basic_usage_concurrently__1()
{
	concurrent_basic(1);
}

static void
FT_test_basic_usage_concurrently__2()
{
	concurrent_basic(2);
}

static void
FT_test_basic_usage_concurrently__3()
{
	concurrent_basic(3);
}

static void
FT_test_basic_usage_concurrently__10()
{
	concurrent_basic(10);
}

static void
FT_test_basic_usage_concurrently__20()
{
	concurrent_basic(20);
}

/***********************************************************************/

static void
FT_LOCK_INITIALIZE()
{
	static struct lock static_lock = LOCK_INITIALIZER;
	struct lock *dynamic_lock = test_malloc(sizeof(struct lock));

	memset(dynamic_lock, 0, sizeof(*dynamic_lock));
	assert_false(lock_initialize(dynamic_lock));

	assert_memory_equal(dynamic_lock, &static_lock, sizeof(static_lock));

	assert_false(lock_uninitialize(dynamic_lock));

	test_free(dynamic_lock);
}

/***********************************************************************/

int
main(int argc, char **argv)
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_lock_initialize),
		cmocka_unit_test(UT_lock_initialize__NULL),
		cmocka_unit_test(UT_lock_uninitialize),
		cmocka_unit_test(UT_lock_uninitialize__NULL),
		cmocka_unit_test(UT_lock_uninitialize__uninitialized),
		cmocka_unit_test(UT_lock_acquire),
		cmocka_unit_test(UT_lock_acquire__uninitialized),
		cmocka_unit_test(UT_lock_acquire__NULL),
		cmocka_unit_test(UT_lock_acquire_try),
		cmocka_unit_test(UT_lock_acquire_try__uninitialized),
		cmocka_unit_test(UT_lock_acquire_try__NULL),
		cmocka_unit_test(UT_lock_release),
		cmocka_unit_test(UT_lock_release__uninitialized),
		cmocka_unit_test(UT_lock_release__NULL),
		cmocka_unit_test(UT_lock_assert),
		cmocka_unit_test(UT_lock_assert__uninitialized),
		cmocka_unit_test(UT_lock_assert__NULL),
		cmocka_unit_test(FT_LOCK_INITIALIZE),
		cmocka_unit_test(FT_test_basic_usage),
		cmocka_unit_test(FT_test_basic_usage_concurrently__1),
		cmocka_unit_test(FT_test_basic_usage_concurrently__2),
		cmocka_unit_test(FT_test_basic_usage_concurrently__3),
		cmocka_unit_test(FT_test_basic_usage_concurrently__10),
		cmocka_unit_test(FT_test_basic_usage_concurrently__20)
	};

	if (argc > 1)
	{
		printf("Using seed %s.\n", argv[1]);
		srand(atoi(argv[1]));
	}
	else
		printf("Using default seed.\n");

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
