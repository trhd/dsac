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
#include <string.h>
#include <sys/types.h>
#include "doubly_linked_list.h"

#define test_find_head()\
do\
{\
	if (doubly_linked_list_find_head(&dlmeta) != dlmeta.head)\
	{\
		printf("Received unexpected list head.\n");\
		goto err;\
	}\
} while (0)

#define test_find_tail()\
do\
{\
	if (doubly_linked_list_find_tail(&dlmeta) != dlmeta.tail)\
	{\
		printf("Received unexpected list tail.\n");\
		goto err;\
	}\
} while (0)

#define test_find_tail_and_head()\
do\
{\
	test_find_head();\
	test_find_tail();\
} while (0)

#define test_next(a, b)\
do\
{\
	struct doubly_linked_list * T = b >= bufs_len ? 0 : &ll_bufs[b],\
			*U = doubly_linked_list_next(&ll_bufs[a]);\
	\
	if (!U || U != T)\
	{\
		printf("Linked list \"next\" was wrong.\n");\
		goto err;\
	}\
} while (0)

#define test_size(I)\
do\
{\
	unsigned int T;\
	if ((T = doubly_linked_list_size(&dlmeta)) != I)\
	{\
		printf("Linked list reported wrong size (Got: %u, Expected %u).\n",\
				T, (unsigned int)I);\
		goto err;\
	}\
} while (0)

#define test_empty()\
do\
{\
	if (!doubly_linked_list_empty(&dlmeta))\
	{\
		printf("Empty linked list reported as non-empty.\n");\
		goto err;\
	}\
	\
	test_size(0);\
} while (0)

#define test_nonempty()\
do\
{\
	if (doubly_linked_list_empty(&dlmeta))\
	{\
		printf("Non-empty linked list reported as emtpy.\n");\
		goto err;\
	}\
} while (0)

#define test_add_back(I)\
do\
{\
	if (doubly_linked_list_insert_back(&dlmeta, &ll_bufs[I]))\
	{\
		printf("Failed to \"insert back\" \"%s\" into the linked list.\n",\
				(char*)doubly_linked_list_get(&ll_bufs[I]));\
		goto err;\
	}\
} while (0)

#define test_add_front(I)\
do\
{\
	if (doubly_linked_list_insert_front(&dlmeta, &ll_bufs[I]))\
	{\
		printf("Failed to \"insert front\" \"%s\" into the linked list.\n",\
				(char*)doubly_linked_list_get(&ll_bufs[I]));\
		goto err;\
	}\
} while (0)

#define test_add(I)\
do\
{\
	if (I % 2)\
		test_add_front(I);\
	else\
		test_add_back(I);\
} while (0)

#define test_add_front_existing(I)\
do\
{\
	if (!doubly_linked_list_insert_front(&dlmeta, &ll_bufs[I]))\
	{\
		printf("Succeeded to insert \"%s\" existing entry to the front of the linked list.\n",\
				(char*)doubly_linked_list_get(&ll_bufs[I]));\
		goto err;\
	}\
} while (0)

#define test_add_back_existing(I)\
do\
{\
	if (!doubly_linked_list_insert_back(&dlmeta, &ll_bufs[I]))\
	{\
		printf("Succeeded to insert \"%s\" existing entry to the back of the linked list.\n",\
				(char*)doubly_linked_list_get(&ll_bufs[I]));\
		goto err;\
	}\
} while (0)

#define test_add_existing(I)\
do\
{\
	test_add_front_existing(I);\
	test_add_back_existing(I);\
	test_find_tail_and_head();\
} while (0)

#define test_find_nonexisting(I)\
do\
{\
	if (doubly_linked_list_find_front(dlmeta.head, data_bufs[I]))\
	{\
		printf("Found an entry for nonexisting request.\n");\
		goto err;\
	}\
	if (doubly_linked_list_find_back(dlmeta.tail, data_bufs[I]))\
	{\
		printf("Found an entry for nonexisting request.\n");\
		goto err;\
	}\
	test_find_tail_and_head();\
} while (0)

#define test_find(I)\
do\
{\
	struct doubly_linked_list * T = doubly_linked_list_find_front(dlmeta.head, data_bufs[I]);\
	\
	if (!T)\
	{\
		printf("Failed to find \"%s\" from the linked list.\n",\
				data_bufs[I]);\
		goto err;\
	}\
	\
	if (strcmp((char*)doubly_linked_list_get(T), data_bufs[I]))\
	{\
		printf("Found incorrect data from received entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)doubly_linked_list_get(T), data_bufs[I]);\
		goto err;\
	}\
	\
	if (doubly_linked_list_find_back(dlmeta.tail, data_bufs[I]) != T)\
	{\
		printf("Failed to find the correct entry when searching backwards.\n");\
		goto err;\
	}\
	test_find_tail_and_head();\
} while (0)

#define test_remove_head(I)\
do\
{\
	struct doubly_linked_list * T = doubly_linked_list_remove_head(&dlmeta);\
	\
	if (!T)\
	{\
		printf("Failed to remove the head \"%s\" from of the linked list.\n",\
				data_bufs[I]);\
		goto err;\
	}\
	\
	if (strcmp((char*)doubly_linked_list_get(T), data_bufs[I]))\
	{\
		printf("Found incorrect data from the removed entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)T->data, data_bufs[I]);\
		goto err;\
	}\
} while (0)

#define test_remove_tail(I)\
do\
{\
	struct doubly_linked_list * T = doubly_linked_list_remove_tail(&dlmeta);\
	\
	if (!T)\
	{\
		printf("Failed to remove \"%s\" \"the tail\" of the linked list.\n",\
				data_bufs[I]);\
		goto err;\
	}\
	\
	if (strcmp((char*)doubly_linked_list_get(T), data_bufs[I]))\
	{\
		printf("Found incorrect data from the removed entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)T->data, data_bufs[I]);\
		goto err;\
	}\
} while (0)

#define test_remove_front(I)\
do\
{\
	struct doubly_linked_list * T = doubly_linked_list_remove_front(&dlmeta, data_bufs[I]);\
	\
	if (!T)\
	{\
		printf("Failed to remove \"%s\" \"from the front\" of the linked list.\n",\
				data_bufs[I]);\
		goto err;\
	}\
	\
	if (strcmp((char*)doubly_linked_list_get(T), data_bufs[I]))\
	{\
		printf("Found incorrect data from the removed entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)T->data, data_bufs[I]);\
		goto err;\
	}\
} while (0)

#define test_remove_back(I)\
do\
{\
	struct doubly_linked_list * T = doubly_linked_list_remove_back(&dlmeta, data_bufs[I]);\
	\
	if (!T)\
	{\
		printf("Failed to remove \"%s\" \"from the back\" of the linked list.\n",\
				data_bufs[I]);\
		goto err;\
	}\
	\
	if (strcmp((char*)doubly_linked_list_get(T), data_bufs[I]))\
	{\
		printf("Found incorrect data from the removed entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)T->data, data_bufs[I]);\
		goto err;\
	}\
} while (0)

#define test_remove(I)\
do\
{\
	if (I % 2)\
		test_remove_front(I);\
	else\
		test_remove_back(I);\
} while (0)

#define test_remove_back_nonexisting(I)\
do\
{\
	if (doubly_linked_list_remove_back(&dlmeta, data_bufs[I]))\
	{\
		printf("Removing non-existing didn't return an error.\n");\
		goto err;\
	}\
} while (0)

#define test_remove_front_nonexisting(I)\
do\
{\
	if (doubly_linked_list_remove_front(&dlmeta, data_bufs[I]))\
	{\
		printf("Removing non-existing didn't return an error.\n");\
		goto err;\
	}\
} while (0)

#define test_remove_nonexisting(I)\
do\
{\
	test_remove_front_nonexisting(I);\
	test_remove_back_nonexisting(I);\
} while(0)

#define test_remove_head_nonexisting()\
do\
{\
	if (doubly_linked_list_remove_head(&dlmeta))\
	{\
		printf("Removing non-existing \"first\" didn't return an error.\n");\
		goto err;\
	}\
} while (0)

#define test_remove_tail_nonexisting()\
do\
{\
	if (doubly_linked_list_remove_head(&dlmeta))\
	{\
		printf("Removing non-existing \"first\" didn't return an error.\n");\
		goto err;\
	}\
} while (0)

static void __attribute__((unused))
print_list(const struct doubly_linked_list * l)
{
	const struct doubly_linked_list *p;

	printf("\n  Printing liked list:  \n");

	while (l)
	{
		printf(" %s >", (char*)doubly_linked_list_get(l));
		p = l;
		l = doubly_linked_list_next(l);
	}
	printf("\n  ");

	l = p;
	while (l)
	{
		printf(" %s <", (char*)doubly_linked_list_get(l));
		l = doubly_linked_list_previous(l);
	}

	printf("\n");
}

int
main(int ac __attribute__((unused)), char **av)
{
	printf("Test %s.\n", av[0]);

	int i;
	const char * data_bufs[] =
	{
		"aa",
		"bb",
		"cc",
		"dd",
		"ee",
		"ff",
		"gg",
		"hh",
		"ii",
		"jj"
	};
	ssize_t bufs_len = sizeof(data_bufs)/sizeof(char*);
	struct doubly_linked_list ll_bufs[bufs_len];
	struct doubly_linked_list_meta dlmeta;

	for (i = bufs_len - 1; i >= 0; --i)
		doubly_linked_list_set(&ll_bufs[i], data_bufs[i]);

	printf("Executing basic tests:\n");
	fflush(stdout);

	doubly_linked_list_initialize(&dlmeta);

	test_empty();

	printf("Round 1: ");
	for (i = 0; i < bufs_len; ++i)
	{
		test_empty();
		test_find_nonexisting(i);
		test_add(i);
		test_add_existing(i);
		test_nonempty();
		test_size(1);
		test_find(i);
		test_remove(i);
		test_remove_nonexisting(i);
	}
	printf("OK\n");

	printf("Round 2: ");
	test_empty();
	for (i = 0; i < bufs_len; ++i)
		test_add(i);
	test_nonempty();
	test_size(bufs_len);
	for (i = 0; i < bufs_len; ++i)
		test_find(i);
	for (i = 0; i < bufs_len; ++i)
		test_remove(i);
	test_empty();
	for (i = 0; i < bufs_len; ++i)
		test_find_nonexisting(i);
	printf("OK\n");

	printf("Round 3: ");
	test_empty();
	test_remove_head_nonexisting();

	for (i = 0; i < bufs_len; ++i)
		test_add(i);

	test_size(bufs_len);
	test_nonempty();

	for (i = 0; i < bufs_len; ++i)
		test_find(i);

	test_remove(2);
	test_size(bufs_len - 1);

	test_nonempty();

	test_find(0);
	test_find(1);
	test_find_nonexisting(2);
	for (i = 3; i < bufs_len; ++i)
		test_find(i);

	test_nonempty();

	test_remove(3);
	test_remove(5);
	test_remove(7);
	test_size(bufs_len - 4);

	test_nonempty();

	test_find(0);
	test_find(1);
	test_find_nonexisting(2);
	test_find_nonexisting(3);
	test_find(4);
	test_find_nonexisting(5);
	test_find(6);
	test_find_nonexisting(7);
	test_find(8);
	test_find(9);

	test_remove(0);
	test_remove(1);
	test_remove(4);
	test_remove(6);
	test_remove(8);
	test_remove(9);

	test_empty();
	test_remove_head_nonexisting();

	for (i = 0; i < bufs_len; ++i)
		test_find_nonexisting(i);
	printf("OK\n");

	printf("Round 4: ");
	test_empty();
	test_add_back(5);
	test_add_existing(5);
	test_add_front(8);
	test_add_existing(8);
	test_add_back(2);
	test_add_existing(2);
	test_add_front(6);
	test_add_existing(6);
	test_add_back(7);
	test_add_existing(7);
	test_add_front(3);
	test_add_existing(3);
	test_add_back(1);
	test_add_existing(1);
	test_add_front(4);
	test_add_existing(4);
	test_add_back(9);
	test_add_existing(9);
	test_nonempty();

	test_next(4, 3);
	test_next(3, 6);
	test_next(6, 8);
	test_next(8, 5);
	test_next(5, 2);
	test_next(2, 7);
	test_next(7, 1);
	test_next(1, 9);

	test_size(9);

	test_remove_head(4);
	test_remove_nonexisting(4);
	test_remove_head(3);
	test_remove_nonexisting(3);
	test_remove_head(6);
	test_remove_nonexisting(6);
	test_remove_head(8);
	test_remove_nonexisting(8);
	test_remove_head(5);
	test_remove_nonexisting(5);
	test_remove_head(2);
	test_remove_nonexisting(2);
	test_remove_head(7);
	test_remove_nonexisting(7);
	test_remove_head(1);
	test_remove_nonexisting(1);
	test_remove_head(9);
	test_remove_nonexisting(9);
	test_empty();
	test_remove_head_nonexisting();


	for (i = 0; i < bufs_len; ++i)
		test_find_nonexisting(i);
	printf("OK\n");

	printf("Round 5: ");
	test_empty();
	test_add_back(5);
	test_add_existing(5);
	test_add_front(8);
	test_add_existing(8);
	test_add_back(2);
	test_add_existing(2);
	test_add_front(6);
	test_add_existing(6);
	test_add_back(7);
	test_add_existing(7);
	test_add_front(3);
	test_add_existing(3);
	test_add_back(1);
	test_add_existing(1);
	test_add_front(4);
	test_add_existing(4);
	test_add_back(9);
	test_add_existing(9);
	test_nonempty();

	test_next(4, 3);
	test_next(3, 6);
	test_next(6, 8);
	test_next(8, 5);
	test_next(5, 2);
	test_next(2, 7);
	test_next(7, 1);
	test_next(1, 9);

	test_size(9);

	test_remove_tail(9);
	test_remove_nonexisting(9);
	test_remove_tail(1);
	test_remove_nonexisting(1);
	test_remove_tail(7);
	test_remove_nonexisting(7);
	test_remove_tail(2);
	test_remove_nonexisting(2);
	test_remove_tail(5);
	test_remove_nonexisting(5);
	test_remove_tail(8);
	test_remove_nonexisting(8);
	test_remove_tail(6);
	test_remove_nonexisting(6);
	test_remove_tail(3);
	test_remove_nonexisting(3);
	test_remove_tail(4);
	test_remove_nonexisting(4);
	test_empty();
	test_remove_head_nonexisting();

	for (i = 0; i < bufs_len; ++i)
		test_find_nonexisting(i);
	printf("OK\n");

	printf("Test %s ended with SUCCESS!\n", av[0]);
	return 0;
err:
	printf("Test %s ended with FAILURE!\n", av[0]);
	return -1;
}
