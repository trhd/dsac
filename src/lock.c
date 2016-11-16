#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "lock.h"

bool
lock_initialize(struct lock * l)
{
	assert(l);

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	pthread_mutexattr_t t;

	if ((errno = pthread_mutexattr_init(&t))
			|| (errno = pthread_mutexattr_settype(&t, PTHREAD_MUTEX_ERRORCHECK))
			|| (errno = pthread_mutex_init(&l->mutex, &t)))
#else
	if ((errno = pthread_mutex_init(&l->mutex, NULL)))
#endif
		return true;

	debug_flags_initialize(l);
	debug_flags_set(l, LOCK_DEBUG_FLAG_INITIALIZED);

	return false;
}

bool
lock_uninitialize(struct lock * l)
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	errno = pthread_mutex_destroy(&l->mutex);

	if (errno)
		return true;

	debug_flags_clear(l, LOCK_DEBUG_FLAG_INITIALIZED);

	return false;
}

bool
lock_acquire(struct lock * l)
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	errno = pthread_mutex_lock(&l->mutex);

	assert(errno != EDEADLK);

	return errno;
}

bool
lock_acquire_try(struct lock * l)
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	errno = pthread_mutex_trylock(&l->mutex);

	assert(errno != EDEADLK);

	return errno;
}

bool
lock_release(struct lock * l)
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	return (errno = pthread_mutex_unlock(&l->mutex));
}


#if !defined(NDEBUG) || defined(UNIT_TESTING)
void
lock_assert(const struct lock * l __attribute__((unused)))
{
	assert(l);
	debug_flags_assert(l, LOCK_DEBUG_FLAG_INITIALIZED);

	int e = pthread_mutex_lock(&((struct lock *)l)->mutex);

	if (e != EDEADLK)
	{
		if (!e)
			lock_release((struct lock *)l);
		abort();
	}
}
#endif
