/**
 * dsac -- Data Structures and Algorithms for C.
 * Copyright (C) 2016 Hemmo Nieminen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

static int ref_id = 0;

static int
compare(const unsigned char buf[9])
{
	static const unsigned char refs[][9] =
	{
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x15, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x15, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x15, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x15, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80},
		{0x15, 0x05, 0x80, 0x00, 0xff, 0x00, 0x00, 0x00, 0x80},
		{0x15, 0x05, 0x80, 0x00, 0xff, 0x00, 0x00, 0x00, 0x80},
		{0x15, 0x05, 0x80, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x80},
		{0x15, 0x05, 0x80, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x80},
	};
	int i, j;

	assert((size_t)ref_id <= sizeof(refs)/(sizeof(char)*9));

	if (memcmp(buf, refs[ref_id], sizeof(refs[ref_id])))
	{
		fprintf(stderr, "Contents don't match with ref_id %d.\n", ref_id);
		goto err;
	}

	for (i = 0 ; i < 9 ; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			if (bitmap_get(buf, i * 8 + j) != (refs[ref_id][i] & (1 << j)) >> j)
			{
				fprintf(stderr, "bitmap_get() returned inconsistent value"
						" for i=%d, j=%d, ref_id=%d\n", i, j, ref_id);
				goto err;
			}
		}
	}

	printf("Compare OK with ref_id %d.\n", ref_id);
	ref_id++;
	return 0;
err:
	fprintf(stderr, "ref: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
			refs[ref_id][0], refs[ref_id][1], refs[ref_id][2],
			refs[ref_id][3], refs[ref_id][4], refs[ref_id][5],
			refs[ref_id][6], refs[ref_id][7], refs[ref_id][8]);
	fprintf(stderr, "buf: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
			buf[0], buf[1], buf[2],
			buf[3], buf[4], buf[5],
			buf[6], buf[7], buf[8]);
	return 1;
}

bool
bitmap_test()
{
	unsigned char buf[9];

	printf("-> Starting %s...\n", __func__);

	ref_id = 0;
	bitmap_initialize(buf, sizeof(buf));
	if (compare(buf)) return 1;
	if (bitmap_set(buf, 0 * 8 + 0))
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_set(buf, 0 * 8 + 2))
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_set(buf, 0 * 8 + 4))
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_set(buf, 1 * 8 + 0))
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_set(buf, 1 * 8 + 2))
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_set(buf, 2 * 8 + 7))
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_set(buf, 8 * 8 + 7))
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_set(buf, 4 * 8 + 0)
			|| bitmap_set(buf, 4 * 8 + 1)
			|| bitmap_set(buf, 4 * 8 + 2)
			|| bitmap_set(buf, 4 * 8 + 3)
			|| bitmap_set(buf, 4 * 8 + 4)
			|| bitmap_set(buf, 4 * 8 + 5)
			|| bitmap_set(buf, 4 * 8 + 6)
			|| bitmap_set(buf, 4 * 8 + 7))
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (!bitmap_set(buf, 4 * 8 + 0)
			|| !bitmap_set(buf, 4 * 8 + 1)
			|| !bitmap_set(buf, 4 * 8 + 2)
			|| !bitmap_set(buf, 4 * 8 + 3)
			|| !bitmap_set(buf, 4 * 8 + 4)
			|| !bitmap_set(buf, 4 * 8 + 5)
			|| !bitmap_set(buf, 4 * 8 + 6)
			|| !bitmap_set(buf, 4 * 8 + 7))
	{
		fprintf(stderr, "Expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (!bitmap_clear(buf, 4 * 8 + 1))
	{
		fprintf(stderr, "Expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_clear(buf, 4 * 8 + 1))
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;

	printf("<- %s OK\n", __func__);
	return false;
}

bool
bitmap_guard_test()
{
	unsigned char buf[9];
	struct bitmap_guard grd;

	printf("-> Starting %s...\n", __func__);

	ref_id = 0;
	bitmap_guard_initialize(&grd, buf, sizeof(buf));
	if (compare(buf)) return 1;
	if (bitmap_guard_set(&grd, 0 * 8 + 0) != 0)
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_set(&grd, 0 * 8 + 2) != 0)
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_set(&grd, 0 * 8 + 4) != 0)
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_set(&grd, 1 * 8 + 0) != 0)
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_set(&grd, 1 * 8 + 2) != 0)
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_set(&grd, 2 * 8 + 7) != 0)
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_set(&grd, 8 * 8 + 7) != 0)
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_set(&grd, 4 * 8 + 0) != 0
			|| bitmap_guard_set(&grd, 4 * 8 + 1) != 0
			|| bitmap_guard_set(&grd, 4 * 8 + 2) != 0
			|| bitmap_guard_set(&grd, 4 * 8 + 3) != 0
			|| bitmap_guard_set(&grd, 4 * 8 + 4) != 0
			|| bitmap_guard_set(&grd, 4 * 8 + 5) != 0
			|| bitmap_guard_set(&grd, 4 * 8 + 6) != 0
			|| bitmap_guard_set(&grd, 4 * 8 + 7) != 0)
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_set(&grd, 4 * 8 + 0) <= 0
			|| bitmap_guard_set(&grd, 4 * 8 + 1) <= 0
			|| bitmap_guard_set(&grd, 4 * 8 + 2) <= 0
			|| bitmap_guard_set(&grd, 4 * 8 + 3) <= 0
			|| bitmap_guard_set(&grd, 4 * 8 + 4) <= 0
			|| bitmap_guard_set(&grd, 4 * 8 + 5) <= 0
			|| bitmap_guard_set(&grd, 4 * 8 + 6) <= 0
			|| bitmap_guard_set(&grd, 4 * 8 + 7) <= 0)
	{
		fprintf(stderr, "Expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_clear(&grd, 4 * 8 + 1) <= 0)
	{
		fprintf(stderr, "Expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;
	if (bitmap_guard_clear(&grd, 4 * 8 + 1) != 0)
	{
		fprintf(stderr, "Didn't expect the bit to be already set on line %d.\n", __LINE__);
		return 1;
	}
	if (compare(buf)) return 1;

	if (bitmap_guard_get(&grd, 9 * 8) >= 0)
	{
		fprintf(stderr, "Expected bitmap_guard_get() to return <0 for invalid index.\n");
		return 1;
	}

	if (bitmap_guard_set(&grd, 9 * 8) >= 0)
	{
		fprintf(stderr, "Expected bitmap_guard_set() to return <0 for invalid index.\n");
		return 1;
	}

	if (bitmap_guard_clear(&grd, 9 * 8) >= 0)
	{
		fprintf(stderr, "Expected bitmap_guard_clear() to return <0 for invalid index.\n");
		return 1;
	}

	printf("<- %s OK\n", __func__);
	return false;
}

static int
bitmap_loop_test()
{
	unsigned char buf[10];
	unsigned int i;

	printf("-> Starting %s...\n", __func__);

	bitmap_initialize(buf, sizeof(buf));

	for (i = 0; i < sizeof(buf) * 8; ++i)
	{
		if (bitmap_get(buf, i))
			fprintf(stderr, "Didn't expect bit to be set #1.\n");
		else if (bitmap_set(buf, i))
			fprintf(stderr, "Didn't expect bit to be set #2.\n");
		else if (!bitmap_get(buf, i))
			fprintf(stderr, "Expect bit to be set #1.\n");
		else if (!bitmap_set(buf, i))
			fprintf(stderr, "Expect bit to be set #2.\n");
		else if (!bitmap_get(buf, i))
			fprintf(stderr, "Expect bit to be set #3.\n");
		else if (!bitmap_clear(buf, i))
			fprintf(stderr, "Expect bit to be set #4.\n");
		else if (bitmap_get(buf, i))
			fprintf(stderr, "Didn't expect bit to be set #3.\n");
		else if (bitmap_clear(buf, i))
			fprintf(stderr, "Didn't expect bit to be set #4.\n");
		else if (bitmap_get(buf, i))
			fprintf(stderr, "Didn't expect bit to be set #5.\n");
		else
			continue;
		return true;
	}
	printf("<- %s OK\n", __func__);
	return false;
}

static int
bitmap_guard_loop_test()
{
	unsigned char buf[10];
	unsigned int i;
	struct bitmap_guard grd;

	printf("-> Starting %s...\n", __func__);

	bitmap_guard_initialize(&grd, buf, sizeof(buf));

	for (i = 0; i < sizeof(buf) * 8; ++i)
	{
		if (bitmap_guard_get(&grd, i))
			fprintf(stderr, "Didn't expect bit to be set #1.\n");
		else if (bitmap_guard_set(&grd, i))
			fprintf(stderr, "Didn't expect bit to be set #2.\n");
		else if (!bitmap_guard_get(&grd, i))
			fprintf(stderr, "Expect bit to be set #1.\n");
		else if (!bitmap_guard_set(&grd, i))
			fprintf(stderr, "Expect bit to be set #2.\n");
		else if (!bitmap_guard_get(&grd, i))
			fprintf(stderr, "Expect bit to be set #3.\n");
		else if (!bitmap_guard_clear(&grd, i))
			fprintf(stderr, "Expect bit to be set #4.\n");
		else if (bitmap_guard_get(&grd, i))
			fprintf(stderr, "Didn't expect bit to be set #3.\n");
		else if (bitmap_guard_clear(&grd, i))
			fprintf(stderr, "Didn't expect bit to be set #4.\n");
		else if (bitmap_guard_get(&grd, i))
			fprintf(stderr, "Didn't expect bit to be set #5.\n");
		else
			continue;
		return true;
	}
	printf("<- %s OK\n", __func__);
	return false;
}

int
main()
{
	int rv;

	if (bitmap_loop_test()
			|| bitmap_guard_loop_test()
			|| bitmap_test()
			|| bitmap_guard_test())
	{
		rv = EXIT_FAILURE;
		printf("Testing FAILED!!! :(\n");
	}
	else
	{
		rv = EXIT_SUCCESS;
		printf("Testing PASSED. (^_^) \\m/\n");
	}

	return rv;
}
