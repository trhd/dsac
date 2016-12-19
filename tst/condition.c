/**
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This file is part of dsac (Data Structures and Alorithms for C).
 *
 * dsac is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * dsac is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dsac. If not, see <http://www.gnu.org/licenses/>.
 */

#include "cmocka-wrapper.h"
#include "condition.h"
#include "lock.h"

struct tuple
{
	struct condition *cond;
	int counter;
	int iters;
};

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
	if (lock_acquire(t->cond->lock))
		return (void*)1;

	for (int i = 0 ; i < t->iters; i++)
	{
		if (t->counter++ != i * 2
				|| condition_signal(t->cond)
				|| condition_wait(t->cond)
				|| t->counter != 2 * i + 2)
			return (void*)1;
	}

	if (lock_release(t->cond->lock))
		return (void*)1;

	return NULL;
}

static void
UT_condition_wait()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));
	pthread_t t;
	struct tuple tt = { c, 0, 2000 };
	void * r;

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(lock_acquire(tt.cond->lock));

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

	expect_assert_failure(condition_wait(c));

	assert_false(lock_uninitialize(l));

	test_free(c);
	test_free(l);
}

/***********************************************************************/

static void*
helper__UT_condition_signal(struct tuple * t)
{
	assert(t);

	if (lock_acquire(t->cond->lock))
		return (void*)1;

	t->counter = 1000;
	if (condition_signal(t->cond))
		return (void*)1;

	while (t->counter)
		condition_wait(t->cond);

	if (lock_release(t->cond->lock))
		return (void*)1;

	return NULL;
}

static void
UT_condition_signal()
{
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));
	pthread_t t;
	struct tuple tt = { c, 0, 0 };
	void * r;

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(pthread_create(&t, 0, (void*(*)(void*))helper__UT_condition_signal, &tt));

	assert_false(lock_acquire(tt.cond->lock));

	while (!tt.counter)
		condition_wait(tt.cond);

	tt.counter = 0;
	assert_false(condition_signal(tt.cond));

	assert_false(lock_release(l));

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

	if (lock_acquire(t->cond->lock))
		return (void*)1;

	t->counter++;

	if (condition_broadcast(t->cond))
		return (void*)1;

	while (t->counter)
		if (condition_wait(t->cond))
			return (void*)1;

	if (lock_release(t->cond->lock))
		return (void*)1;

	return NULL;
}

static void
UT_condition_broadcast()
{
	enum { thread_count = 2000 };
	struct condition * c = test_malloc(sizeof(struct condition));
	struct lock * l = test_malloc(sizeof(struct lock));
	pthread_t t[thread_count];
	struct tuple tt = { c, 0, 0 };
	void * r;

	assert_false(lock_initialize(l));
	assert_false(condition_initialize(c, l));

	assert_false(lock_acquire(tt.cond->lock));

	for (int i = 0 ; i < thread_count ; i++)
		assert_false(pthread_create(&t[i], 0, (void*(*)(void*))helper__UT_condition_broadcast, &tt));

	while (tt.counter < thread_count)
		assert_false(condition_wait(c));

	tt.counter = 0;
	assert_false(condition_broadcast(c));

	assert_false(lock_release(l));

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

static void
FT_COND_INITIALIZE()
{
	static struct lock static_lock = LOCK_INITIALIZER;
	static struct condition static_condition = CONDITION_INITIALIZER(&static_lock);
	struct condition * dynamic_condition = test_malloc(sizeof(struct condition));

	memset(dynamic_condition, 0, sizeof(* dynamic_condition));
	assert_false(condition_initialize(dynamic_condition, &static_lock));

	assert_memory_equal(dynamic_condition, &static_condition, sizeof(static_condition));

	assert_false(condition_uninitialize(dynamic_condition));

	test_free(dynamic_condition);
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
		cmocka_unit_test(UT_condition_signal),
		cmocka_unit_test(UT_condition_signal__NULL),
		cmocka_unit_test(UT_condition_signal__uninitialized),
		cmocka_unit_test(UT_condition_broadcast),
		cmocka_unit_test(UT_condition_broadcast__NULL),
		cmocka_unit_test(UT_condition_broadcast__uninitialized),
		cmocka_unit_test(FT_COND_INITIALIZE)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
