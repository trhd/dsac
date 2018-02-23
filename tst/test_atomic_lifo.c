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

#include <pthread.h>
#include "cmocka-wrapper.h"
#include "atomic_lifo.h"

static int VALGRIND = 0;

/***********************************************************************/

static void
UT_atomic_lifo_initialize()
{
	struct atomic_lifo * lifo = test_malloc(sizeof(struct atomic_lifo));
	assert_non_null(lifo);

	atomic_lifo_initialize(lifo);

	test_free(lifo);
}

/***********************************************************************/

static void
UT_atomic_lifo_initialize__NULL()
{
	expect_assert_failure(atomic_lifo_initialize(NULL));
}

/***********************************************************************/

static void
UT_atomic_lifo_push()
{
	enum { element_count = 10 };

	struct atomic_lifo * lifo = test_malloc(sizeof(struct atomic_lifo));
	struct atomic_lifo * es = test_malloc(element_count * sizeof(struct atomic_lifo));

	atomic_lifo_initialize(lifo);

	atomic_lifo_set(&es[0], "a");
	atomic_lifo_set(&es[1], "b");
	atomic_lifo_set(&es[2], "c");
	atomic_lifo_set(&es[3], "d");
	atomic_lifo_set(&es[4], "e");
	atomic_lifo_set(&es[5], "f");
	atomic_lifo_set(&es[6], "g");
	atomic_lifo_set(&es[7], "h");
	atomic_lifo_set(&es[8], "i");
	atomic_lifo_set(&es[9], "j");

	for (int i = 0 ; i < element_count ; i++)
		atomic_lifo_push(lifo, &es[i]);

	assert_non_null(lifo);

	test_free(lifo);
	test_free(es);
}

/***********************************************************************/

struct _UT_atomic_lifo_push__multithread__shared_stuff
{
	struct atomic_lifo * lifo;
	struct atomic_lifo * els;
	unsigned long count;
};

void *
_UT_atomic_lifo_push__multithread__helper(void * arg)
{
	struct _UT_atomic_lifo_push__multithread__shared_stuff * s
		= (struct _UT_atomic_lifo_push__multithread__shared_stuff *)arg;

	for (unsigned long i = 0 ; i < s->count ; i++)
		atomic_lifo_push(s->lifo, &s->els[i]);

	return NULL;
}

static void
UT_atomic_lifo_push__multithread()
{
	unsigned int thread_count = VALGRIND ? 4 : 8, element_count = 3000;
	struct _UT_atomic_lifo_push__multithread__shared_stuff * stuff
		= test_malloc(thread_count * sizeof(struct _UT_atomic_lifo_push__multithread__shared_stuff));
	struct atomic_lifo * f = test_malloc(sizeof(struct atomic_lifo));
	pthread_t threads[thread_count];

	atomic_lifo_initialize(f);

	for (unsigned long i = 0 ; i < thread_count ; i++)
	{
		stuff[i].lifo = f;
		stuff[i].count = element_count / thread_count;
		stuff[i].els = test_malloc(element_count * sizeof(struct atomic_lifo));

		for (unsigned long j = 0 ; j < stuff[i].count ; j++)
			atomic_lifo_set(&stuff[i].els[j], (void*)(i*stuff[i].count + j));
	}

	for (unsigned int i = 0 ; i < thread_count ; i++)
		assert_int_equal(0, pthread_create(&threads[i], NULL, _UT_atomic_lifo_push__multithread__helper, &stuff[i]));

	for (unsigned int i = 0 ; i < thread_count ; i++)
		assert_int_equal(0, pthread_join(threads[i], NULL));

	for (unsigned long i = 0 ; i < element_count ; i++)
		assert_non_null(atomic_lifo_pop(f));

	for (unsigned long i = 0 ; i < thread_count ; i++)
		test_free(stuff[i].els);
	test_free(stuff);
	test_free(f);
}

/***********************************************************************/

static void
UT_atomic_lifo_push__NULL()
{
	expect_assert_failure(atomic_lifo_push(NULL, NULL));
}

/***********************************************************************/

static void
UT_atomic_lifo_push__uninitialized()
{
	struct atomic_lifo * lifo = test_malloc(sizeof(struct atomic_lifo));
	assert_non_null(lifo);

	memset(lifo, 0, sizeof(*lifo));

	expect_assert_failure(atomic_lifo_push(lifo, NULL));

	test_free(lifo);
}

/***********************************************************************/

static void
UT_atomic_lifo_pop()
{
	enum { element_count = 10 };

	struct atomic_lifo * lifo = test_malloc(sizeof(struct atomic_lifo));
	struct atomic_lifo * es = test_malloc(element_count * sizeof(struct atomic_lifo));

	atomic_lifo_initialize(lifo);

	atomic_lifo_set(&es[0], "a");
	atomic_lifo_set(&es[1], "b");
	atomic_lifo_set(&es[2], "c");
	atomic_lifo_set(&es[3], "d");
	atomic_lifo_set(&es[4], "e");
	atomic_lifo_set(&es[5], "f");
	atomic_lifo_set(&es[6], "g");
	atomic_lifo_set(&es[7], "h");
	atomic_lifo_set(&es[8], "i");
	atomic_lifo_set(&es[9], "j");

	assert_null(atomic_lifo_pop(lifo));
	atomic_lifo_push(lifo, &es[0]);
	atomic_lifo_push(lifo, &es[1]);
	atomic_lifo_push(lifo, &es[2]);
	atomic_lifo_push(lifo, &es[3]);
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "d");
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "c");
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "b");
	atomic_lifo_push(lifo, &es[4]);
	atomic_lifo_push(lifo, &es[5]);
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "f");
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "e");
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "a");
	assert_null(atomic_lifo_pop(lifo));
	atomic_lifo_push(lifo, &es[6]);
	atomic_lifo_push(lifo, &es[7]);
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "h");
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "g");
	atomic_lifo_push(lifo, &es[8]);
	atomic_lifo_push(lifo, &es[9]);
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "j");
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "i");
	assert_null(atomic_lifo_pop(lifo));

	atomic_lifo_push(lifo, &es[0]);
	atomic_lifo_push(lifo, &es[1]);
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "b");
	atomic_lifo_push(lifo, &es[1]);
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "b");
	atomic_lifo_push(lifo, &es[1]);
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "b");
	assert_string_equal((char*)atomic_lifo_pop(lifo)->data, "a");

	assert_non_null(lifo);

	test_free(lifo);
	test_free(es);
}

/***********************************************************************/

struct _UT_atomic_lifo_pop__multithread__shared_stuff
{
	struct atomic_lifo lifo;
	int count;
};

void *
_UT_atomic_lifo_pop__multithread__helper(void * arg)
{
	struct _UT_atomic_lifo_pop__multithread__shared_stuff * s
		= (struct _UT_atomic_lifo_pop__multithread__shared_stuff *)arg;
	struct atomic_lifo * cur, * prev;

	prev = NULL;
	for (int i = 0 ; i < s->count ; i++)
	{
		cur = atomic_lifo_pop(&s->lifo);
		if (!cur)
			return (void*)0x1;
		if (prev && (unsigned long)atomic_lifo_get(prev) <= (unsigned long)atomic_lifo_get(cur))
			return (void*)0x2;
		prev = cur;
	}

	return NULL;
}

static void
UT_atomic_lifo_pop__multithread()
{
	int thread_count = VALGRIND ? 4 : 8;
	int element_count = VALGRIND ? 240 : 1600;

	struct _UT_atomic_lifo_pop__multithread__shared_stuff * stuff
		= test_malloc(sizeof(struct _UT_atomic_lifo_pop__multithread__shared_stuff));
	struct atomic_lifo * els[element_count];
	pthread_t threads[thread_count];

	atomic_lifo_initialize(&stuff->lifo);
	stuff->count = element_count / thread_count;

	for (long i = 0 ; i < element_count ; i++)
	{
		els[i] = test_malloc(sizeof(struct atomic_lifo));
		assert_non_null(els[i]);
		atomic_lifo_set(els[i], (void*)i);
		atomic_lifo_push(&stuff->lifo, els[i]);
	}

	for (int i = 0 ; i < thread_count ; i++)
		assert_int_equal(0, pthread_create(&threads[i], NULL, _UT_atomic_lifo_pop__multithread__helper, stuff));

	for (int i = 0 ; i < thread_count ; i++)
	{
		void * rv;
		assert_int_equal(0, pthread_join(threads[i], &rv));
		assert_null(rv);
	}

	assert_null(atomic_lifo_pop(&stuff->lifo));

	for (int i = 0 ; i < element_count ; i++)
		test_free(els[i]);
	test_free(stuff);
}

/***********************************************************************/

static void
UT_atomic_lifo_pop__NULL()
{
	expect_assert_failure(atomic_lifo_pop(NULL));
}

/***********************************************************************/

static void
UT_atomic_lifo_pop__uninitialized()
{
	struct atomic_lifo * lifo = test_malloc(sizeof(struct atomic_lifo));
	assert_non_null(lifo);

	memset(lifo, 0, sizeof(*lifo));

	expect_assert_failure(atomic_lifo_pop(lifo));

	test_free(lifo);
}

/***********************************************************************/

struct _FT_concurrent_access__shared_stuff
{
	struct atomic_lifo lifo;
	int count;
};

void *
_FT_concurrent_access__helper(void * arg)
{
	struct _FT_concurrent_access__shared_stuff * s
		= (struct _FT_concurrent_access__shared_stuff *)arg;
	struct atomic_lifo * f;

	for (int i = 0 ; i < s->count ; i++)
	{
		f = atomic_lifo_pop(&s->lifo);
		if (!f)
			return (void*)0x1;
		atomic_lifo_push(&s->lifo, f);
	}

	return NULL;
}

static void
FT_concurrent_access()
{
	unsigned long thread_count = VALGRIND ? 3 : 6, element_count = 20, cycle_count = VALGRIND ? 300 : 2000;
	struct _FT_concurrent_access__shared_stuff * stuff
		= test_malloc(sizeof(struct _FT_concurrent_access__shared_stuff));
	struct atomic_lifo * els[element_count];
	pthread_t threads[thread_count];

	atomic_lifo_initialize(&stuff->lifo);
	stuff->count = cycle_count;

	for (unsigned long i = 0 ; i < element_count ; i++)
	{
		els[i] = test_malloc(sizeof(struct atomic_lifo));
		assert_non_null(els[i]);
		atomic_lifo_set(els[i], (void*)i);
		atomic_lifo_push(&stuff->lifo, els[i]);
	}

	for (unsigned long i = 0 ; i < thread_count ; i++)
		assert_int_equal(0, pthread_create(&threads[i], NULL, _FT_concurrent_access__helper, stuff));

	for (unsigned long i = 0 ; i < thread_count ; i++)
	{
		void * rv;
		assert_int_equal(0, pthread_join(threads[i], &rv));
		assert_null(rv);
	}

	for (unsigned long i = 0 ; i < element_count ; i++)
		assert_non_null(atomic_lifo_pop(&stuff->lifo));

	for (unsigned long i = 0 ; i < element_count ; i++)
		test_free(els[i]);

	test_free(stuff);
}

/***********************************************************************/

static void
FT_single_thread_random_access()
{

}

/***********************************************************************/

static void
FT_assert_locklessness()
{
	struct atomic_lifo f;

	assert_true(atomic_is_lock_free(&f.next));
}

/***********************************************************************/

static void
UT_atomic_lifo_set()
{
	struct atomic_lifo * f = test_malloc(sizeof(struct atomic_lifo));
	void * foo = &f;

	atomic_lifo_set(f, "foobar");
	assert_string_equal(f->data, "foobar");

	atomic_lifo_set(f, foo);
	assert_ptr_equal(f->data, &f);

	test_free(f);
}

/***********************************************************************/

static void
UT_atomic_lifo_set__NULL()
{
	expect_assert_failure(atomic_lifo_set(NULL, NULL));
	expect_assert_failure(atomic_lifo_set(NULL, (char*)1));
}

/***********************************************************************/

static void
UT_atomic_lifo_get()
{
	struct atomic_lifo * f = test_malloc(sizeof(struct atomic_lifo));
	void * foo = &f;

	atomic_lifo_set(f, "foobar");
	assert_string_equal(atomic_lifo_get(f), "foobar");

	atomic_lifo_set(f, foo);
	assert_ptr_equal(atomic_lifo_get(f), &f);

	test_free(f);
}

/***********************************************************************/

static void
UT_atomic_lifo_get__NULL()
{
	expect_assert_failure(atomic_lifo_get(NULL));
}

/***********************************************************************/

int
main()
{
	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test(UT_atomic_lifo_initialize),
		cmocka_unit_test(UT_atomic_lifo_initialize__NULL),
		cmocka_unit_test(UT_atomic_lifo_push),
		cmocka_unit_test(UT_atomic_lifo_push__NULL),
		cmocka_unit_test(UT_atomic_lifo_push__uninitialized),
		cmocka_unit_test(UT_atomic_lifo_push__multithread),
		cmocka_unit_test(UT_atomic_lifo_pop),
		cmocka_unit_test(UT_atomic_lifo_pop__multithread),
		cmocka_unit_test(UT_atomic_lifo_pop__NULL),
		cmocka_unit_test(UT_atomic_lifo_pop__uninitialized),
		cmocka_unit_test(UT_atomic_lifo_set),
		cmocka_unit_test(UT_atomic_lifo_set__NULL),
		cmocka_unit_test(UT_atomic_lifo_get),
		cmocka_unit_test(UT_atomic_lifo_get__NULL),
		cmocka_unit_test(FT_concurrent_access),
		cmocka_unit_test(FT_single_thread_random_access),
		cmocka_unit_test(FT_assert_locklessness)
	};

	if (getenv("VALGRIND") && atoi(getenv("VALGRIND")) > 0)
		VALGRIND = 1;

	return cmocka_run_group_tests(tests, NULL, NULL);
}

/***********************************************************************/
