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
#include "linked_list.h"

#define test_find_head()\
do\
{\
	if (linked_list_find_head(&llmeta) != &llmeta.head)\
	{\
		printf("Failed to find correct head for the linked list.\n");\
		goto err;\
	}\
} while (0)

#define test_find_head_empty()\
do\
{\
	if (*linked_list_find_head(&llmeta) != NULL)\
	{\
		printf("Failed to find correct head (NULL) for the empty linked list.\n");\
		goto err;\
	}\
} while (0)

#define test_next(a, b)\
do\
{\
	struct linked_list * T = b >= bufs_len ? 0 : &ll_bufs[b],\
			**U = linked_list_next(&ll_bufs[a]);\
	\
	if (!U || *U != T)\
	{\
		printf("Linked list \"next\" was wrong.\n");\
		goto err;\
	}\
} while (0)

#define test_size(I)\
do\
{\
	unsigned int T;\
	if ((T = linked_list_size(&llmeta)) != I)\
	{\
		printf("Linked list reported wrong size (Got: %u, Expected %u).\n",\
				T, (unsigned int)I);\
		goto err;\
	}\
} while (0)

#define test_empty()\
do\
{\
	if (!linked_list_empty(&llmeta))\
	{\
		printf("Empty linked list reported as non-empty.\n");\
		goto err;\
	}\
	\
	test_size(0);\
	test_find_head_empty();\
} while (0)

#define test_nonempty()\
do\
{\
	if (linked_list_empty(&llmeta))\
	{\
		printf("Non-empty linked list reported as emtpy.\n");\
		goto err;\
	}\
	\
	test_find_head();\
} while (0)


#define test_add_back(I)\
do\
{\
	if (linked_list_insert_back(&llmeta, &ll_bufs[I]))\
	{\
		printf("Failed to \"insert back\" \"%s\" into the linked list.\n",\
				(char*)linked_list_get(&ll_bufs[I]));\
		goto err;\
	}\
} while (0)

#define test_add_front(I)\
do\
{\
	if (linked_list_insert_front(&llmeta, &ll_bufs[I]))\
	{\
		printf("Failed to \"insert front\" \"%s\" into the linked list.\n",\
				(char*)linked_list_get(&ll_bufs[I]));\
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
	\
	test_find_head();\
} while (0)

#define test_add_existing(I)\
do\
{\
	if (!linked_list_insert_front(&llmeta, &ll_bufs[I]))\
	{\
		printf("Succeeded to insert \"%s\" existing entry to the linked list.\n",\
				(char*)linked_list_get(&ll_bufs[I]));\
		goto err;\
	}\
	\
	if (!linked_list_insert_back(&llmeta, &ll_bufs[I]))\
	{\
		printf("Succeeded to insert \"%s\" existing entry to the linked list.\n",\
				(char*)linked_list_get(&ll_bufs[I]));\
		goto err;\
	}\
} while (0)

#define test_find_nonexisting(I)\
do\
{\
	if (linked_list_find(&llmeta, data_bufs[I]))\
	{\
		printf("Found an entry for nonexisting request.\n");\
		goto err;\
	}\
	test_find_match_nonexisting(I);\
} while (0)

#define test_find_match(I)\
do\
{\
	char buf[20] = { 0 };\
	memcpy(buf, data_bufs[I], strlen(data_bufs[I]));\
	struct linked_list **T = linked_list_find_match(&llmeta, buf,\
		       (int(*)(const void*, const void*))strcmp);\
	\
	if (!T)\
	{\
		printf("Failed to find \"%s\" from the linked list by value.\n", buf);\
		goto err;\
	}\
	if (strcmp((char*)linked_list_get(*T), data_bufs[I]))\
	{\
		printf("Found incorrect data from received (match) entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)linked_list_get(*T), data_bufs[I]);\
		goto err;\
	}\
} while (0)\

#define test_find_match_nonexisting(I)\
do\
{\
	char buf[20] = { 0 };\
	memcpy(buf, data_bufs[I], strlen(data_bufs[I]));\
	struct linked_list **T = linked_list_find_match(&llmeta, buf,\
		       (int(*)(const void*, const void*))strcmp);\
	\
	if (T)\
	{\
		printf("Found nonexisting entry from the linked list (match).\n");\
		goto err;\
	}\
} while (0)\

#define test_find(I)\
do\
{\
	struct linked_list ** T = linked_list_find(&llmeta, data_bufs[I]);\
	\
	if (!T)\
	{\
		printf("Failed to find \"%s\" from the linked list.\n",\
				data_bufs[I]);\
		goto err;\
	}\
	\
	if (strcmp((char*)linked_list_get(*T), data_bufs[I]))\
	{\
		printf("Found incorrect data from received entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)linked_list_get(*T), data_bufs[I]);\
		goto err;\
	}\
	\
	test_find_head();\
	test_find_match(I);\
} while (0)

#define test_remove(I)\
do\
{\
	struct linked_list * T = linked_list_remove(&llmeta, data_bufs[I]);\
	\
	if (!T)\
	{\
		printf("Failed to remove \"%s\" from the linked list.\n",\
				data_bufs[I]);\
		goto err;\
	}\
	\
	if (strcmp((char*)linked_list_get(T), data_bufs[I]))\
	{\
		printf("Found incorrect data from the removed entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)linked_list_get(T), data_bufs[I]);\
		goto err;\
	}\
} while (0)

#define test_remove_head(I)\
do\
{\
	test_find_head();\
	\
	struct linked_list * T = linked_list_remove_head(&llmeta);\
	\
	if (!T)\
	{\
		printf("Failed to remove \"%s\" \"from the front\" of the linked list.\n",\
				data_bufs[I]);\
		goto err;\
	}\
	\
	if (strcmp((char*)linked_list_get(T), data_bufs[I]))\
	{\
		printf("Found incorrect data from the removed entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)linked_list_get(T), data_bufs[I]);\
		goto err;\
	}\
	\
	if (linked_list_empty(&llmeta))\
		test_find_head_empty();\
	else\
		test_find_head();\
} while (0)

#define test_remove_tail(I)\
do\
{\
	test_find_head();\
	\
	struct linked_list * T = linked_list_remove_tail(&llmeta);\
	\
	if (!T)\
	{\
		printf("Failed to remove \"%s\" \"from the back\" of the linked list.\n",\
				data_bufs[I]);\
		goto err;\
	}\
	\
	if (strcmp((char*)linked_list_get(T), data_bufs[I]))\
	{\
		printf("Found incorrect data from the removed entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)linked_list_get(T), data_bufs[I]);\
		goto err;\
	}\
	\
	if (linked_list_empty(&llmeta))\
		test_find_head_empty();\
	else\
		test_find_head();\
} while (0)

#define test_remove_match(I)\
do\
{\
	char buf[20] = { 0 };\
	memcpy(buf, data_bufs[I], strlen(data_bufs[I]));\
	struct linked_list *T = linked_list_remove_match(&llmeta, buf,\
		       (int(*)(const void*, const void*))strcmp);\
	\
	if (!T)\
	{\
		printf("Failed to remove \"%s\" from the linked list by value.\n", buf);\
		goto err;\
	}\
	if (strcmp((char*)linked_list_get(T), data_bufs[I]))\
	{\
		printf("Found incorrect data from removed (match) entry (Got: \"%s\", "\
				"Expected \"%s\").\n", (char*)linked_list_get(T), data_bufs[I]);\
		goto err;\
	}\
} while (0)\

#define test_remove_match_nonexisting(I)\
do\
{\
	char buf[20] = { 0 };\
	memcpy(buf, data_bufs[I], strlen(data_bufs[I]));\
	struct linked_list *T = linked_list_remove_match(&llmeta, buf,\
		       (int(*)(const void*, const void*))strcmp);\
	\
	if (T)\
	{\
		printf("Linked list returned something when removing nonexisting entry (match).\n");\
		goto err;\
	}\
} while (0)\

#define test_remove_nonexisting(I)\
do\
{\
	if (linked_list_remove(&llmeta, data_bufs[I]))\
	{\
		printf("Removing non-existing didn't return an error.\n");\
		goto err;\
	}\
	test_remove_match_nonexisting(I);\
} while (0)

#define test_remove_head_nonexisting()\
do\
{\
	if (linked_list_remove_head(&llmeta))\
	{\
		printf("Removing non-existing \"first\" didn't return an error.\n");\
		goto err;\
	}\
	\
	test_find_head_empty();\
} while (0)


static int
_test_iteration(const struct linked_list *n, int *c)
{
	static const struct linked_list *p = NULL;

	if (*c && n == p)
	{
		printf("Iterating same node more than once?\n");
		return -1;
	}

	p = n;
	++*c;

	return 0;
}

static int
test_iteration(const struct linked_list_meta *m)
{
	int s = linked_list_size(m),
	    c = 0,
	    t = linked_list_iterate(m,
			    (int(*)(const struct linked_list *, void *))_test_iteration, &c);

	if (s != c)
	{
		printf("%s failed.\n", __func__);
		return -1;
	}

	if (t)
	{
		printf("linked_list_iterate returned an error %d.\n", t);
		return -1;
	}

	return 0;
}


static void __attribute__((unused))
print_list(struct linked_list_meta *m)
{
	struct linked_list **l = &m->head;

	printf("\n  Printing liked list:");

	while (*l)
	{
		printf(" <%s>", (char*)linked_list_get(*l));
		l = linked_list_next(*l);
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
	struct linked_list ll_bufs[bufs_len];
	struct linked_list_meta llmeta;

	for (i = bufs_len - 1; i >= 0; --i)
		linked_list_set(&ll_bufs[i], data_bufs[i]);

	printf("Executing basic tests:\n");
	fflush(stdout);

	linked_list_initialize(&llmeta);

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
		if (test_iteration(&llmeta))
			goto err;
		test_find(i);
		test_remove(i);
		test_remove_nonexisting(i);
	}
	printf("OK\n");

	printf("Round 2: ");
	test_empty();
	for (i = 0; i < bufs_len; ++i)
		test_add(i);
	if (test_iteration(&llmeta))
		goto err;
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
	test_remove_match(5);
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
	if (test_iteration(&llmeta))
		goto err;

	test_remove(0);
	test_remove_match(1);
	test_remove(4);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_match(6);
	test_remove(8);
	test_remove_match(9);

	test_empty();

	for (i = 0; i < bufs_len; ++i)
		test_find_nonexisting(i);
	printf("OK\n");

	printf("Round 4: ");
	test_empty();
	test_add_back(5);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(5);
	test_add_front(8);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(8);
	test_add_back(2);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(2);
	test_add_front(6);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(6);
	test_add_back(7);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(7);
	test_add_front(3);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(3);
	test_add_back(1);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(1);
	test_add_front(4);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(4);
	test_add_back(9);
	if (test_iteration(&llmeta))
		goto err;
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
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(4);
	test_remove_head(3);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(3);
	test_remove_head(6);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(6);
	test_remove_head(8);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(8);
	test_remove_head(5);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(5);
	test_remove_head(2);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(2);
	test_remove_head(7);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(7);
	test_remove_head(1);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(1);
	test_remove_head(9);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(9);
	test_empty();
	if (test_iteration(&llmeta))
		goto err;

	test_remove_head_nonexisting();

	for (i = 0; i < bufs_len; ++i)
		test_find_nonexisting(i);
	printf("OK\n");

	printf("Round 5: ");
	test_empty();
	test_add_back(5);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(5);
	test_add_front(8);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(8);
	test_add_back(2);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(2);
	test_add_front(6);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(6);
	test_add_back(7);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(7);
	test_add_front(3);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(3);
	test_add_back(1);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(1);
	test_add_front(4);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(4);
	test_add_back(9);
	if (test_iteration(&llmeta))
		goto err;
	test_add_existing(9);
	test_nonempty();

	test_size(9);

	test_remove_tail(9);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(9);
	test_remove_tail(1);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(1);
	test_remove_tail(7);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(7);
	test_remove_tail(2);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(2);
	test_remove_tail(5);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(5);
	test_remove_tail(8);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(8);
	test_remove_tail(6);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(6);
	test_remove_tail(3);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(3);
	test_remove_tail(4);
	if (test_iteration(&llmeta))
		goto err;
	test_remove_nonexisting(4);
	test_empty();
	if (test_iteration(&llmeta))
		goto err;

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
