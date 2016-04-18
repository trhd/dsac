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

#include <stdio.h>
#include "lifo.h"

int
test_1()
{
	printf("Starting test %s.\n", __func__);

	static const unsigned int val_count = 1000;
	unsigned int vals[val_count], i, j;
	lifo_t valbufs[val_count];
	lifo_meta_t meta;

	for (i = 0; i < val_count; ++i)
	{
		vals[i] = i;
		valbufs[i].data = vals + i;
	}

	lifo_initialize(&meta);

	if (!lifo_empty(&meta))
	{
		printf("ERROR: LIFO not reported as empty.\n");
		return 1;
	}

	if (lifo_size(&meta) != 0)
	{
		printf("ERROR: LIFO size reported as non-zero.\n");
		return 1;
	}

	for (i = 0; i < val_count; ++i)
	{
		if (lifo_push(&meta, valbufs + i))
		{
			printf("ERROR: Failed to push into the LIFO.\n");
			return 1;
		}

		if (lifo_size(&meta) != i + 1)
		{
			printf("ERROR: LIFO size reported as not equal to i + 1 (%d).\n", i + 1);
			return 1;
		}
	}

	if (lifo_empty(&meta))
	{
		printf("ERROR: LIFO reported as empty.\n");
		return 1;
	}

	for (i = 0; i < val_count; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			if (lifo_peek(&meta) != valbufs + val_count - i - 1)
			{
				printf("ERROR: Peek returned invalid entry for "
						"index %d (iteration %u).\n", i, j);
				return 1;
			}
		}

		if (lifo_pop(&meta) != valbufs + val_count - i - 1)
		{
			printf("ERROR: Pop returned invalid entry for index %d.\n", i);
			return 1;
		}

		if (lifo_size(&meta) != val_count - i - 1)
		{
			printf("ERROR: LIFO size doesn't match val_count - i - 1 (%d).\n", val_count - i - 1);
			return 1;
		}
	}

	if (!lifo_empty(&meta))
	{
		printf("ERROR: LIFO not reported as empty.\n");
		return 1;
	}

	if (lifo_peek(&meta) != 0)
	{
		printf("ERROR: Emtpy LIFO returned an pointer (on peek).\n");
		return 1;
	}

	if (lifo_pop(&meta) != 0)
	{
		printf("ERROR: Emtpy LIFO returned an pointer (on pop).\n");
		return 1;
	}

	printf("Ending test %s.\n", __func__);
	return 0;
}

int
test_2()
{
	printf("Starting test %s.\n", __func__);

	unsigned int i, j, vals[] =
	{
		1, 1324, 1988432, 11, 13, 15, 165, 184, 151, 154, 168, 186, 198, 1654, 11, 1658, 16844, 121, 165, 184,
		2, 2324, 2988432, 21, 23, 25, 265, 284, 251, 254, 268, 286, 298, 2654, 21, 2658, 26844, 221, 265, 284,
		3, 3324, 3988432, 31, 33, 35, 365, 384, 351, 354, 368, 386, 398, 3654, 31, 3658, 36844, 321, 365, 384,
		4, 4324, 4988432, 41, 43, 45, 465, 484, 451, 454, 468, 486, 498, 4654, 41, 4658, 46844, 421, 465, 484,
		5, 5324, 5988432, 51, 53, 55, 565, 584, 551, 554, 568, 586, 598, 5654, 51, 5658, 56844, 521, 565, 584
	};
	static const size_t val_count = sizeof(vals) / sizeof(unsigned int);
	lifo_t valbufs[val_count];
	lifo_meta_t meta;

	for (i = 0; i < val_count; ++i)
	{
		vals[i] = i;
		valbufs[i].data = vals + i;
	}

	lifo_initialize(&meta);

	if (!lifo_empty(&meta))
	{
		printf("ERROR: LIFO not reported as empty.\n");
		return 1;
	}

	if (lifo_size(&meta) != 0)
	{
		printf("ERROR: LIFO size reported as non-zero.\n");
		return 1;
	}

	for (i = 0; i < val_count; ++i)
		if (lifo_push(&meta, valbufs + i))
		{
			printf("ERROR: Failed to push into the LIFO.\n");
			return 1;
		}

	if (lifo_empty(&meta))
	{
		printf("ERROR: LIFO reported as empty.\n");
		return 1;
	}

	if (lifo_size(&meta) != val_count)
	{
		printf("ERROR: LIFO size reported as not equal to val_count.\n");
		return 1;
	}

	for (i = 0; i < val_count; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			if (lifo_peek(&meta) != valbufs + val_count - 1 - i)
			{
				printf("ERROR: Peek returned invalid entry for "
						"index %zd (iteration %u).\n", val_count - 1 - i, j);
				return 1;
			}
		}

		if (lifo_pop(&meta) != valbufs + val_count - 1 - i)
		{
			printf("ERROR: Pop returned invalid entry for index %zd.\n", val_count - 1 - i);
			return 1;
		}
	}

	if (!lifo_empty(&meta))
	{
		printf("ERROR: LIFO not reported as empty.\n");
		return 1;
	}

	if (lifo_size(&meta) != 0)
	{
		printf("ERROR: LIFO size reported as non-zero.\n");
		return 1;
	}

	if (lifo_peek(&meta) != 0)
	{
		printf("ERROR: Emtpy LIFO returned an pointer (on peek).\n");
		return 1;
	}

	if (lifo_pop(&meta) != 0)
	{
		printf("ERROR: Emtpy LIFO returned an pointer (on pop).\n");
		return 1;
	}

	printf("Ending test %s.\n", __func__);
	return 0;
}

int
test_3()
{
	printf("Starting test %s.\n", __func__);

	static const unsigned int val_count = 1000;
	unsigned int vals[val_count], i, j;
	lifo_t valbufs[val_count];
	lifo_meta_t meta;

	for (i = 0; i < val_count; ++i)
	{
		vals[i] = i;
		valbufs[i].data = vals + i;
	}

	lifo_initialize(&meta);

	for (i = 0; i < val_count; ++i)
	{

		if (!lifo_empty(&meta))
		{
			printf("ERROR: LIFO not reported as empty.\n");
			return 1;
		}

		if (lifo_size(&meta) != 0)
		{
			printf("ERROR: LIFO size reported as non-zero.\n");
			return 1;
		}

		if (lifo_push(&meta, valbufs + i))
		{
			printf("ERROR: Failed to push into the LIFO.\n");
			return 1;
		}

		if (lifo_empty(&meta))
		{
			printf("ERROR: LIFO reported as empty.\n");
			return 1;
		}

		if (lifo_size(&meta) != 1)
		{
			printf("ERROR: LIFO size reported as not equal to i.\n");
			return 1;
		}

		for (j = 0; j < 3; ++j)
		{
			if (lifo_peek(&meta) != valbufs + i)
			{
				printf("ERROR: Peek returned invalid entry for "
						"index %d (iteration %u).\n", i, j);
				return 1;
			}
		}

		if (lifo_pop(&meta) != valbufs + i)
		{
			printf("ERROR: Pop returned invalid entry for index %d.\n", i);
			return 1;
		}

		if (!lifo_empty(&meta))
		{
			printf("ERROR: LIFO not reported as empty.\n");
			return 1;
		}

		if (lifo_size(&meta) != 0)
		{
			printf("ERROR: LIFO size reported as non-zero.\n");
			return 1;
		}

		if (lifo_peek(&meta) != 0)
		{
			printf("ERROR: Emtpy LIFO returned an pointer (on peek).\n");
			return 1;
		}

		if (lifo_pop(&meta) != 0)
		{
			printf("ERROR: Emtpy LIFO returned an pointer (on pop).\n");
			return 1;
		}
	}

	printf("Ending test %s.\n", __func__);
	return 0;
}

int
test_4()
{
	printf("Starting test %s.\n", __func__);

	static const unsigned int val_count = 1000;
	unsigned int vals[val_count], i;
	lifo_t valbufs[val_count];
	lifo_meta_t meta;

	for (i = 0; i < val_count; ++i)
	{
		vals[i] = i;
		valbufs[i].data = vals + i;
	}

	lifo_initialize(&meta);
	i = 0;

	if (!lifo_empty(&meta))
	{
		printf("ERROR: LIFO not reported as empty.\n");
		return 1;
	}

	if (lifo_size(&meta) != i)
	{
		printf("ERROR: LIFO size reported as not equal to %d.\n", i);
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_empty(&meta))
	{
		printf("ERROR: LIFO reported as empty.\n");
		return 1;
	}

	if (lifo_size(&meta) != i)
	{
		printf("ERROR: LIFO size reported as not equal to %d.\n", i);
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #1 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #2 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #3 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #4 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #5 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #6 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (lifo_push(&meta, valbufs + i++))
	{
		printf("ERROR: Failed to push into the LIFO.\n");
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #7 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #8 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #9 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (lifo_peek(&meta) != valbufs + i - 1)
	{
		printf("ERROR: Peek returned invalid entry for index %d.\n", i - 1);
		return 1;
	}

	if (lifo_pop(&meta) != valbufs + --i)
	{
		printf("ERROR: #10 Pop returned invalid entry for index %d.\n", i + 1);
		return 1;
	}

	if (!lifo_empty(&meta))
	{
		printf("ERROR: LIFO not reported as empty.\n");
		return 1;
	}

	if (lifo_size(&meta) != i)
	{
		printf("ERROR: LIFO size reported as not equal to %d.\n", i);
		return 1;
	}

	if (lifo_peek(&meta) != 0)
	{
		printf("ERROR: Emtpy LIFO returned an pointer (on peek).\n");
		return 1;
	}

	if (lifo_pop(&meta) != 0)
	{
		printf("ERROR: Emtpy LIFO returned an pointer (on pop).\n");
		return 1;
	}

	printf("Ending test %s.\n", __func__);
	return 0;
}

int
main()
{
	printf("Starting LIFO test.\n");


	if (
			test_1()
			|| test_2()
			|| test_3()
			|| test_4()
	   )
		goto err;

	printf("LIFO test was a SUCCESS!\n");
	return 0;
err:
	printf("LIFO test was a FAILURE!\n");
	return 1;
}
