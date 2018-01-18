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

#include "cmocka-wrapper.h"
#include "condition.h"
#include "lock.h"

struct tuple
{
	struct lock * lock;
	struct condition * cond;
	int counter;
	int iters;
};

static bool valgrind = false;

/***********************************************************************/

static void
UT_condition_initialize()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(condition_uninitialize(c));
	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void
UT_condition_initialize__NULL()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));

	expect_assert_failure(condition_initialize(NULL, NULL));
	expect_assert_failure(condition_initialize(NULL, l));

	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void
UT_condition_uninitialize()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(condition_uninitialize(c));
	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void
UT_condition_uninitialize__NULL()
{
	expect_assert_failure(condition_uninitialize(NULL));
}

/***********************************************************************/

static void
UT_condition_uninitialize__uninitialized()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(condition_uninitialize(c));
	expect_assert_failure(condition_uninitialize(c));

	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void *
helper__UT_condition_wait(struct tuple * t)
{
	if (lock_acquire(t->lock))
		return (void*)1;

	for (int i = 0 ; i < t->iters; i++)
	{
		if (t->counter++ != i * 2
				|| condition_signal(t->cond)
				|| condition_wait(t->cond)
				|| t->counter != 2 * i + 2)
			return (void*)1;
	}

	lock_release(t->lock);

	return NULL;
}

static void
UT_condition_wait()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));
	pthread_t t;
	struct tuple tt = { l, c, 0, 450 };
	void * r;

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(lock_acquire(tt.lock));

	assert_false(pthread_create(&t, 0, (void*(*)(void*))helper__UT_condition_wait, &tt));

	for (int i = 0; i < tt.iters; i++)
	{
		assert_false(condition_wait(c));
		assert_int_equal(tt.counter, 2 * i + 1);
		tt.counter++;
		assert_false(condition_signal(c));
	}

	lock_release(l);

	assert_false(pthread_join(t, &r));
	assert_null(r);

	assert_false(condition_uninitialize(c));
	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void
UT_condition_wait__NULL()
{
	expect_assert_failure(condition_wait(NULL));
}

/***********************************************************************/

static void
UT_condition_wait__uninitialized()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(condition_uninitialize(c));

	assert_false(lock_acquire(l));
	expect_assert_failure(condition_wait(c));
	lock_release(l);

	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static struct timespec helper__UT_condition_timedwait__delay = { 0, 10 * 1000 * 1000 };

static void *
helper__UT_condition_timedwait(struct tuple * t)
{
	if (lock_acquire(t->lock))
		return (void*)1;

	for (int i = 0 ; i < t->iters; i++)
	{
		if (t->counter++ != i * 2
				|| condition_signal(t->cond)
				|| condition_timedwait(t->cond, &helper__UT_condition_timedwait__delay)
				|| t->counter != 2 * i + 2)
			return (void*)1;
	}

	lock_release(t->lock);

	return NULL;
}

static void
UT_condition_timedwait()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));
	pthread_t t;
	struct tuple tt = { l, c, 0, 450 };
	void * r;

	if (valgrind)
		helper__UT_condition_timedwait__delay.tv_sec = 3;

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(lock_acquire(tt.lock));

	assert_false(pthread_create(&t, 0, (void*(*)(void*))helper__UT_condition_timedwait, &tt));

	for (int i = 0; i < tt.iters; i++)
	{
		assert_false(condition_timedwait(c, &helper__UT_condition_timedwait__delay));
		assert_int_equal(tt.counter, 2 * i + 1);
		tt.counter++;
		assert_false(condition_signal(c));
	}

	lock_release(l);

	assert_false(pthread_join(t, &r));
	assert_null(r);

	assert_false(condition_uninitialize(c));
	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void
UT_condition_timedwait__expire()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));
	struct timespec delay = { valgrind ? 3 : 0, 1000 };

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(lock_acquire(l));
	assert_true(condition_timedwait(c, &delay));
	lock_release(l);

	assert_false(condition_uninitialize(c));
	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void
UT_condition_timedwait__NULL()
{
	struct lock * l = test_malloc(sizeof(struct lock));
	struct condition * c = test_malloc(sizeof(struct condition));

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	expect_assert_failure(condition_timedwait(NULL, NULL));
	expect_assert_failure(condition_timedwait(NULL, &helper__UT_condition_timedwait__delay));
	expect_assert_failure(condition_timedwait(c, NULL));

	assert_false(condition_uninitialize(c));
	assert_false(lock_uninitialize(l));

	test_free(l);
	test_free(c);
}

/***********************************************************************/

static void
UT_condition_timedwait__uninitialized()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));
	struct timespec d = { 0, 10 * 1000 * 1000 };

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(condition_uninitialize(c));

	assert_false(lock_acquire(l));
	expect_assert_failure(condition_timedwait(c, &d));
	lock_release(l);

	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void*
helper__UT_condition_signal(struct tuple * t)
{
	assert(t);

	if (lock_acquire(t->lock))
		return (void*)1;

	t->counter = 1000;
	if (condition_signal(t->cond))
		return (void*)1;

	while (t->counter)
		condition_wait(t->cond);

	lock_release(t->lock);

	return NULL;
}

static void
UT_condition_signal()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));
	pthread_t t;
	struct tuple tt = { l, c, 0, 0 };
	void * r;

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(pthread_create(&t, 0, (void*(*)(void*))helper__UT_condition_signal, &tt));

	assert_false(lock_acquire(tt.lock));

	while (!tt.counter)
		condition_wait(tt.cond);

	tt.counter = 0;
	assert_false(condition_signal(tt.cond));

	lock_release(l);

	assert_false(pthread_join(t, &r));
	assert_null(r);

	assert_false(condition_uninitialize(c));
	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void
UT_condition_signal__NULL()
{
	expect_assert_failure(condition_signal(NULL));
}

/***********************************************************************/

static void
UT_condition_signal__uninitialized()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(condition_uninitialize(c));

	expect_assert_failure(condition_signal(c));

	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void *
helper__UT_condition_broadcast(struct tuple * t)
{
	assert(t);

	if (lock_acquire(t->lock))
		return (void*)1;

	t->counter++;

	if (condition_broadcast(t->cond))
		return (void*)1;

	while (t->counter)
		if (condition_wait(t->cond))
			return (void*)1;

	lock_release(t->lock);

	return NULL;
}

static void
UT_condition_broadcast()
{
	enum { thread_count = 450 };
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));
	pthread_t t[thread_count];
	struct tuple tt = { l, c, 0, 0 };
	void * r;

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(lock_acquire(tt.lock));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&t[i], 0, (void*(*)(void*))helper__UT_condition_broadcast, &tt));

	while (tt.counter < thread_count)
		assert_false(condition_wait(c));

	tt.counter = 0;
	assert_false(condition_broadcast(c));

	lock_release(l);

	for (int i = 0 ; i < thread_count ; i++)
	{
		assert_false(pthread_join(t[i], &r));
		assert_null(r);
	}

	assert_false(condition_uninitialize(c));
	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void
UT_condition_broadcast__NULL()
{
	expect_assert_failure(condition_broadcast(NULL));
}

/***********************************************************************/

static void
UT_condition_broadcast__uninitialized()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(condition_uninitialize(c));

	expect_assert_failure(condition_broadcast(c));

	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_condition_initialize),
		cmocka_unit_test(UT_condition_initialize__NULL),
		cmocka_unit_test(UT_condition_uninitialize),
		cmocka_unit_test(UT_condition_uninitialize__NULL),
		cmocka_unit_test(UT_condition_uninitialize__uninitialized),
		cmocka_unit_test(UT_condition_wait),
		cmocka_unit_test(UT_condition_wait__NULL),
		cmocka_unit_test(UT_condition_wait__uninitialized),
		cmocka_unit_test(UT_condition_timedwait),
		cmocka_unit_test(UT_condition_timedwait__expire),
		cmocka_unit_test(UT_condition_timedwait__NULL),
		cmocka_unit_test(UT_condition_timedwait__uninitialized),
		cmocka_unit_test(UT_condition_signal),
		cmocka_unit_test(UT_condition_signal__NULL),
		cmocka_unit_test(UT_condition_signal__uninitialized),
		cmocka_unit_test(UT_condition_broadcast),
		cmocka_unit_test(UT_condition_broadcast__NULL),
		cmocka_unit_test(UT_condition_broadcast__uninitialized)
	};

	char const * e = getenv("VALGRIND");

	valgrind = e && atoi(e) > 0;

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
