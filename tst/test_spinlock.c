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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "cmocka-wrapper.h"
#include "spinlock.h"

static bool VALGRIND = false;

/***********************************************************************/

static void
UT_spinlock_initialize()
{
	struct spinlock *l = test_malloc(sizeof(struct spinlock));

	expect_assert_failure(spinlock_initialize(NULL));

	spinlock_initialize(l);
	spinlock_uninitialize(l);

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_initialize__NULL()
{
	expect_assert_failure(spinlock_initialize(NULL));
}

/***********************************************************************/

static void
UT_spinlock_uninitialize()
{
	struct spinlock *l = test_malloc(sizeof(struct spinlock));

	spinlock_initialize(l);
	spinlock_uninitialize(l);

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_uninitialize__uninitialized()
{
	struct spinlock *l = test_malloc(sizeof(struct spinlock));

	spinlock_initialize(l);
	spinlock_uninitialize(l);

	expect_assert_failure(spinlock_uninitialize(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_uninitialize__NULL()
{
	expect_assert_failure(spinlock_uninitialize(NULL));
}

/***********************************************************************/

static void
UT_spinlock_acquire()
{
	struct spinlock * l = test_malloc(sizeof(struct spinlock));
	spinlock_initialize(l);

	spinlock_acquire(l);
	expect_assert_failure(spinlock_acquire(l));

	spinlock_release(l);
	spinlock_uninitialize(l);

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_acquire__uninitialized()
{
	struct spinlock *l = test_malloc(sizeof(struct spinlock));

	spinlock_initialize(l);
	spinlock_uninitialize(l);

	expect_assert_failure(spinlock_acquire(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_acquire__NULL()
{
	expect_assert_failure(spinlock_acquire(NULL));
}

/***********************************************************************/

static void *
_UT_spinlock_acquire_try__helper_2(void * p)
{
	struct spinlock * l = (struct spinlock *)p;

	return spinlock_acquire_try(l) ? (void*)-1ul : (void*)0ul;
}

static void
_UT_spinlock_acquire_try__helper(struct spinlock * l)
{
	assert(l);

	pthread_t chld;
	void * chldrv;

	assert_false(pthread_create(&chld, NULL, _UT_spinlock_acquire_try__helper_2, l));
	assert_false(pthread_join(chld, &chldrv));
	assert_int_equal((unsigned long)chldrv, -1ul);
}

static void
UT_spinlock_acquire_try()
{
	struct spinlock *l = test_malloc(sizeof(struct spinlock));
	spinlock_initialize(l);

	assert_false(spinlock_acquire_try(l));
	_UT_spinlock_acquire_try__helper(l);
	spinlock_release(l);

	spinlock_acquire(l);
	_UT_spinlock_acquire_try__helper(l);
	spinlock_release(l);

	assert_false(spinlock_acquire_try(l));
	expect_assert_failure(spinlock_acquire_try(l));
	expect_assert_failure(spinlock_acquire(l));
	spinlock_release(l);

	spinlock_uninitialize(l);

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_acquire_try__uninitialized()
{
	struct spinlock *l = test_malloc(sizeof(struct spinlock));

	spinlock_initialize(l);
	spinlock_uninitialize(l);

	expect_assert_failure(spinlock_acquire_try(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_acquire_try__NULL()
{
	expect_assert_failure(spinlock_acquire_try(NULL));
}

/***********************************************************************/

static void
UT_spinlock_release()
{

	struct spinlock *l = test_malloc(sizeof(struct spinlock));
	spinlock_initialize(l);

	expect_assert_failure(spinlock_release(l));

	spinlock_acquire(l);
	spinlock_release(l);

	expect_assert_failure(spinlock_release(l));

	spinlock_uninitialize(l);

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_release__uninitialized()
{

	struct spinlock *l = test_malloc(sizeof(struct spinlock));

	spinlock_initialize(l);

	spinlock_acquire(l);
	spinlock_uninitialize(l);

	expect_assert_failure(spinlock_release(l));

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_release__NULL()
{
	expect_assert_failure(spinlock_release(NULL));
}

/***********************************************************************/

static void
UT_spinlock_assert()
{
	struct spinlock *l = test_malloc(sizeof(struct spinlock));

	spinlock_initialize(l);

	expect_assert_failure(spinlock_assert(l));

	spinlock_acquire(l);
	spinlock_assert(l);

	spinlock_release(l);
	expect_assert_failure(spinlock_assert(l));

	spinlock_uninitialize(l);

	test_free(l);
}

/***********************************************************************/

static void
UT_spinlock_assert__uninitialized()
{
	struct spinlock *l = test_malloc(sizeof(struct spinlock));

	spinlock_initialize(l);
	spinlock_uninitialize(l);

	expect_assert_failure(spinlock_assert(l));

	test_free(l);
}

/***********************************************************************/


static void
UT_spinlock_assert__NULL()
{
	expect_assert_failure(spinlock_assert(NULL));
}

/***********************************************************************/

static void
FT_test_basic_usage()
{
	struct spinlock *l = test_malloc(sizeof(struct spinlock));;

	spinlock_initialize(l);

	for (volatile int i = 0; i < 1000; ++i)
	{
		expect_assert_failure(spinlock_assert(l));
		spinlock_acquire(l);
		expect_assert_failure(spinlock_acquire(l));
		spinlock_assert(l);
		spinlock_release(l);
		expect_assert_failure(spinlock_release(l));
	}

	spinlock_uninitialize(l);

	test_free(l);
}

/***********************************************************************/

static pthread_t concurrent_basic_shared_varialbe;

static void *
_concurrent_basic(struct spinlock *l)
{
	for (int i = 0; i < 20; ++i)
	{
		struct timespec delay = { 0, ( rand() % 1000 ) * 10 * 1000 };

		spinlock_acquire(l);
		spinlock_assert(l);

		concurrent_basic_shared_varialbe = pthread_self();

		assert_false(nanosleep(&delay, NULL));
		assert_int_equal(concurrent_basic_shared_varialbe, pthread_self());

		spinlock_assert(l);
		spinlock_release(l);
	}

	return NULL;
}

static void
concurrent_basic(int thread_count)
{
	pthread_t threads[thread_count];
	struct spinlock *l = test_malloc(sizeof(struct spinlock));

	spinlock_initialize(l);

	for (int i = 0; i < thread_count; i++)
		assert_false(pthread_create(&threads[i], NULL, (void*(*)(void*))_concurrent_basic, l));

	for (int i = 0; i < thread_count; i++)
		assert_false(pthread_join(threads[i], NULL));

	spinlock_uninitialize(l);

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
	if (VALGRIND)
		skip();

	concurrent_basic(10);
}

static void
FT_test_basic_usage_concurrently__20()
{
	if (VALGRIND)
		skip();

	concurrent_basic(20);
}

/***********************************************************************/

static void
FT_SPINLOCK_INITIALIZE()
{
	static struct spinlock static_spinlock = SPINLOCK_INITIALIZER;
	struct spinlock *dynamic_spinlock = test_malloc(sizeof(struct spinlock));

	memset(dynamic_spinlock, 0, sizeof(*dynamic_spinlock));
	spinlock_initialize(dynamic_spinlock);

	assert_memory_equal(dynamic_spinlock, &static_spinlock, sizeof(static_spinlock));

	spinlock_uninitialize(dynamic_spinlock);

	test_free(dynamic_spinlock);
}

/***********************************************************************/

int
main(int argc, char **argv)
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_spinlock_initialize),
		cmocka_unit_test(UT_spinlock_initialize__NULL),
		cmocka_unit_test(UT_spinlock_uninitialize),
		cmocka_unit_test(UT_spinlock_uninitialize__NULL),
		cmocka_unit_test(UT_spinlock_uninitialize__uninitialized),
		cmocka_unit_test(UT_spinlock_acquire),
		cmocka_unit_test(UT_spinlock_acquire__uninitialized),
		cmocka_unit_test(UT_spinlock_acquire__NULL),
		cmocka_unit_test(UT_spinlock_acquire_try),
		cmocka_unit_test(UT_spinlock_acquire_try__uninitialized),
		cmocka_unit_test(UT_spinlock_acquire_try__NULL),
		cmocka_unit_test(UT_spinlock_release),
		cmocka_unit_test(UT_spinlock_release__uninitialized),
		cmocka_unit_test(UT_spinlock_release__NULL),
		cmocka_unit_test(UT_spinlock_assert),
		cmocka_unit_test(UT_spinlock_assert__uninitialized),
		cmocka_unit_test(UT_spinlock_assert__NULL),
		cmocka_unit_test(FT_SPINLOCK_INITIALIZE),
		cmocka_unit_test(FT_test_basic_usage),
		cmocka_unit_test(FT_test_basic_usage_concurrently__1),
		cmocka_unit_test(FT_test_basic_usage_concurrently__2),
		cmocka_unit_test(FT_test_basic_usage_concurrently__3),
		cmocka_unit_test(FT_test_basic_usage_concurrently__10),
		cmocka_unit_test(FT_test_basic_usage_concurrently__20)
	};
	char * vg;

	if (argc > 1)
	{
		printf("Using seed %s.\n", argv[1]);
		srand(atoi(argv[1]));
	}
	else
		printf("Using default seed.\n");

	vg = getenv("VALGRIND");
	if (vg && 0 < atoi(vg))
		VALGRIND = true;

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
