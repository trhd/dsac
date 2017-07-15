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
