#include "cmocka-wrapper.h"

/***********************************************************************/

bool do_mock_realloc = false;

void *
_mock_realloc(void *p, size_t z)
{
	if (do_mock_realloc)
		return (void*)mock();
	else
		return test_realloc(p, z);
}

/***********************************************************************/

bool do_mock_malloc = false;

void *
_mock_malloc(size_t z)
{
	if (do_mock_malloc)
		return (void*)mock();
	else
		return test_malloc(z);
}

/***********************************************************************/

int
mock_reset_all(void **p __attribute__((unused)))
{
	do_mock_realloc = false;
	do_mock_malloc = false;
	return 0;
}

/***********************************************************************/
