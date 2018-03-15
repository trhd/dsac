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

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "cmocka-wrapper.h"
#include "sema.h"

static int VALGRIND = 0;

/***********************************************************************/

static void
UT_semaphore_initialize()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));
	assert_non_null(s);

	semaphore_initialize(s, 0);
	semaphore_uninitialize(s);
	semaphore_initialize(s, 1);
	semaphore_uninitialize(s);
	semaphore_initialize(s, 2);
	semaphore_uninitialize(s);

	test_free(s);
}

/***********************************************************************/

static void
UT_semaphore_uninitialize()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));
	assert_non_null(s);

	semaphore_initialize(s, 0);
	semaphore_uninitialize(s);

	test_free(s);
}

/***********************************************************************/

static struct semaphore * _UT_semaphore_acquire__holder = NULL;

static void _UT_semaphore_acquire__handler(int sig)
{
	assert(sig == SIGALRM);

	assert(_UT_semaphore_acquire__holder);
	semaphore_release(_UT_semaphore_acquire__holder);
	_UT_semaphore_acquire__holder = NULL;
}

static void
UT_semaphore_acquire()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));
	struct sigaction * sa = test_malloc(sizeof(struct sigaction));
	assert_non_null(s);
	assert_non_null(sa);

	semaphore_initialize(s, 2);
	memset(sa, 0, sizeof(*sa));
	sa->sa_handler = _UT_semaphore_acquire__handler;
	sigemptyset(&sa->sa_mask);

	semaphore_acquire(s);
	semaphore_acquire(s);

	assert_false(sigaction(SIGALRM, sa, NULL));
	_UT_semaphore_acquire__holder = s;
	alarm(1);
	semaphore_acquire(s);
	assert_null(_UT_semaphore_acquire__holder);

	assert_false(sigaction(SIGALRM, sa, NULL));
	_UT_semaphore_acquire__holder = s;
	alarm(1);
	semaphore_acquire(s);
	assert_null(_UT_semaphore_acquire__holder);

	semaphore_release(s);
	semaphore_acquire(s);

	assert_false(sigaction(SIGALRM, sa, NULL));
	_UT_semaphore_acquire__holder = s;
	alarm(1);
	semaphore_acquire(s);
	assert_null(_UT_semaphore_acquire__holder);

	semaphore_release(s);
	semaphore_release(s);

	semaphore_uninitialize(s);

	test_free(s);
	test_free(sa);
}

/***********************************************************************/

static void _UT_semaphore_acquire_interruptible__handler(int sig)
{
	assert(sig == SIGALRM);
}

static void
UT_semaphore_acquire_interruptible()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));
	struct sigaction * sa = test_malloc(sizeof(struct sigaction));
	assert_non_null(s);
	assert_non_null(sa);

	semaphore_initialize(s, 0);
	memset(sa, 0, sizeof(*sa));
	sa->sa_handler = _UT_semaphore_acquire_interruptible__handler;
	sigemptyset(&sa->sa_mask);

	assert_false(sigaction(SIGALRM, sa, NULL));
	alarm(1);
	assert_true(semaphore_acquire_interruptible(s));
	assert_int_equal(errno, EINTR);

	semaphore_release(s);
	assert_false(semaphore_acquire_interruptible(s));

	semaphore_uninitialize(s);

	test_free(s);
	test_free(sa);
}

/***********************************************************************/

static void
UT_semaphore_acquire_try()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));
	assert_non_null(s);

	semaphore_initialize(s, 2);

	assert_false(semaphore_acquire_try(s));
	assert_false(semaphore_acquire_try(s));
	assert_true(semaphore_acquire_try(s));
	assert_int_equal(errno, EWOULDBLOCK);
	assert_true(semaphore_acquire_try(s));
	semaphore_release(s);
	assert_false(semaphore_acquire_try(s));
	assert_true(semaphore_acquire_try(s));
	assert_int_equal(errno, EWOULDBLOCK);
	semaphore_release(s);
	semaphore_release(s);

	semaphore_uninitialize(s);

	test_free(s);
}

/***********************************************************************/

static void
UT_semaphore_acquire_timed()
{
	struct timespec * d = test_malloc(sizeof(struct timespec)), a, b;
	struct semaphore * s = test_malloc(sizeof(struct semaphore));
	assert_non_null(s);
	assert_non_null(d);

	semaphore_initialize(s, 2);
	d->tv_sec = 0;
	d->tv_nsec = 1e4;

	assert_false(clock_gettime(CLOCK_MONOTONIC, &b));
	assert_false(semaphore_acquire_timed(s, d));
	assert_false(clock_gettime(CLOCK_MONOTONIC, &a));
	assert_true((a.tv_sec - b.tv_sec) * 1e9 + (a.tv_nsec - b.tv_nsec) < 1e6 + (VALGRIND * 1e8));

	assert_false(clock_gettime(CLOCK_MONOTONIC, &b));
	assert_false(semaphore_acquire_timed(s, d));
	assert_false(clock_gettime(CLOCK_MONOTONIC, &a));
	assert_true((a.tv_sec - b.tv_sec) * 1e9 + (a.tv_nsec - b.tv_nsec) < 1e6 + (VALGRIND * 1e8));

	assert_false(clock_gettime(CLOCK_MONOTONIC, &b));
	assert_true(semaphore_acquire_timed(s, d));
	assert_int_equal(errno, ETIMEDOUT);
	assert_false(clock_gettime(CLOCK_MONOTONIC, &a));
	assert_true((a.tv_sec - b.tv_sec) * 1e9 + (a.tv_nsec - b.tv_nsec) >= 1e4);

	assert_false(clock_gettime(CLOCK_MONOTONIC, &b));
	assert_true(semaphore_acquire_timed(s, d));
	assert_int_equal(errno, ETIMEDOUT);
	assert_false(clock_gettime(CLOCK_MONOTONIC, &a));
	assert_true((a.tv_sec - b.tv_sec) * 1e9 + (a.tv_nsec - b.tv_nsec) >= 1e4);

	semaphore_release(s);

	assert_false(clock_gettime(CLOCK_MONOTONIC, &b));
	assert_false(semaphore_acquire_timed(s, d));
	assert_false(clock_gettime(CLOCK_MONOTONIC, &a));
	assert_true((a.tv_sec - b.tv_sec) * 1e9 + (a.tv_nsec - b.tv_nsec) < 1e6 + (VALGRIND * 1e8));

	assert_false(clock_gettime(CLOCK_MONOTONIC, &b));
	assert_true(semaphore_acquire_timed(s, d));
	assert_int_equal(errno, ETIMEDOUT);
	assert_false(clock_gettime(CLOCK_MONOTONIC, &a));
	assert_true((a.tv_sec - b.tv_sec) * 1e9 + (a.tv_nsec - b.tv_nsec) >= 1e4);

	semaphore_uninitialize(s);

	test_free(s);
	test_free(d);
}

/***********************************************************************/

static void
UT_semaphore_release()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));
	assert_non_null(s);

	semaphore_initialize(s, 2);

	semaphore_acquire(s);
	semaphore_acquire(s);
	assert_true(semaphore_acquire_try(s));

	semaphore_release(s);
	semaphore_release(s);
	semaphore_release(s);
	semaphore_release(s);

	semaphore_acquire(s);
	semaphore_acquire(s);
	semaphore_acquire(s);
	semaphore_acquire(s);
	assert_true(semaphore_acquire_try(s));

	semaphore_uninitialize(s);

	test_free(s);
}

/***********************************************************************/

static void
UT_semaphore_initialize__NULL()
{
	expect_assert_failure(semaphore_initialize(NULL, 0));
	expect_assert_failure(semaphore_initialize(NULL, 1));
}

/***********************************************************************/

static void
UT_semaphore_uninitialize__NULL()
{
	expect_assert_failure(semaphore_uninitialize(NULL));
}

/***********************************************************************/

static void
UT_semaphore_acquire__NULL()
{
	expect_assert_failure(semaphore_acquire(NULL));
}

/***********************************************************************/

static void
UT_semaphore_acquire_try__NULL()
{
	expect_assert_failure(semaphore_acquire_try(NULL));
}

/***********************************************************************/

static void
UT_semaphore_acquire_timed__NULL()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));
	struct timespec * d = test_malloc(sizeof(struct timespec));
	assert_non_null(s);
	assert_non_null(d);

	semaphore_initialize(s, 1);
	d->tv_sec = 1;
	d->tv_nsec = 0;

	expect_assert_failure(semaphore_acquire_timed(NULL, NULL));
	expect_assert_failure(semaphore_acquire_timed(s, NULL));
	expect_assert_failure(semaphore_acquire_timed(NULL, d));

	semaphore_uninitialize(s);

	test_free(s);
	test_free(d);
}

/***********************************************************************/

static void
UT_semaphore_release__NULL()
{
	expect_assert_failure(semaphore_release(NULL));
}

/***********************************************************************/

static void
UT_semaphore_uninitialize__uninitialized()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));

	semaphore_initialize(s, 1);
	semaphore_uninitialize(s);

	expect_assert_failure(semaphore_uninitialize(s));

	test_free(s);
}

/***********************************************************************/

static void
UT_semaphore_acquire__uninitialized()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));

	semaphore_initialize(s, 1);
	semaphore_uninitialize(s);

	expect_assert_failure(semaphore_acquire(s));

	test_free(s);
}

/***********************************************************************/

static void
UT_semaphore_acquire_try__uninitialized()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));

	semaphore_initialize(s, 1);
	semaphore_uninitialize(s);

	expect_assert_failure(semaphore_acquire_try(s));

	test_free(s);
}

/***********************************************************************/

static void
UT_semaphore_acquire_timed__uninitialized()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));
	struct timespec * d = test_malloc(sizeof(struct timespec));
	assert_non_null(s);
	assert_non_null(d);

	d->tv_sec = 1;
	d->tv_nsec = 0;
	semaphore_initialize(s, 1);
	semaphore_uninitialize(s);

	expect_assert_failure(semaphore_acquire_timed(s, d));

	test_free(s);
	test_free(d);
}

/***********************************************************************/

static void
UT_semaphore_release__uninitialized()
{
	struct semaphore * s = test_malloc(sizeof(struct semaphore));

	semaphore_initialize(s, 1);
	semaphore_uninitialize(s);

	expect_assert_failure(semaphore_release(s));

	test_free(s);
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_semaphore_initialize),
		cmocka_unit_test(UT_semaphore_uninitialize),
		cmocka_unit_test(UT_semaphore_acquire),
		cmocka_unit_test(UT_semaphore_acquire_interruptible),
		cmocka_unit_test(UT_semaphore_acquire_try),
		cmocka_unit_test(UT_semaphore_acquire_timed),
		cmocka_unit_test(UT_semaphore_release),
		cmocka_unit_test(UT_semaphore_initialize__NULL),
		cmocka_unit_test(UT_semaphore_uninitialize__NULL),
		cmocka_unit_test(UT_semaphore_acquire__NULL),
		cmocka_unit_test(UT_semaphore_acquire_try__NULL),
		cmocka_unit_test(UT_semaphore_acquire_timed__NULL),
		cmocka_unit_test(UT_semaphore_release__NULL),
		cmocka_unit_test(UT_semaphore_uninitialize__uninitialized),
		cmocka_unit_test(UT_semaphore_acquire__uninitialized),
		cmocka_unit_test(UT_semaphore_acquire_try__uninitialized),
		cmocka_unit_test(UT_semaphore_acquire_timed__uninitialized),
		cmocka_unit_test(UT_semaphore_release__uninitialized)
	};
	char const * valg = getenv("VALGRIND");

	if (valg && atoi(valg))
		VALGRIND = 1;

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
