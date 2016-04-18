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

#include <errno.h>
#include "splay_tree.h"

#define NULL ((void*)0)

static int
dummy_min_compare(const void *a __attribute__((unused)),
		const void *b __attribute__((unused)))
{
	return -1;
}

static int
dummy_max_compare(const void *a __attribute__((unused)),
		const void *b __attribute__((unused)))
{
	return 1;
}

static struct splay_node **
find_max(struct splay_node **root)
{
	assert(root);
	assert(*root);

	while ((*root)->right)
		root = &(*root)->right;
	return root;
}

static void
rotate_right(struct splay_node **root)
{
	assert(root);
	assert(*root);
	assert((*root)->left);

	struct splay_node *n = (*root)->left;
	(*root)->left = n->right;
	n->right = *root;
	*root = n;
}

static void
rotate_left(struct splay_node **root)
{
	assert(root);
	assert(*root);
	assert((*root)->right);

	struct splay_node *n = (*root)->right;
	(*root)->right = n->left;
	n->left = *root;
	*root = n;
}

static void
zig_right(struct splay_node **root, struct splay_node **right_tail)
{
	assert(root);
	assert(right_tail);

	if (*right_tail)
	{
		(*right_tail)->left = *root;
		*right_tail = (*right_tail)->left;
	}
	else
		*right_tail = *root;

	*root = (*root)->left;
	(*right_tail)->left = NULL;
}

static void
zig_left(struct splay_node **root, struct splay_node **left_tail)
{
	assert(root);
	assert(left_tail);

	if (*left_tail)
	{
		(*left_tail)->right = *root;
		*left_tail = (*left_tail)->right;
	}
	else
		*left_tail = *root;

	*root = (*root)->right;
	(*left_tail)->right = NULL;
}

static int
insert(struct splay_tree *tree, struct splay_node *node, int c)
{
	assert(tree);
	assert(node);

	if (!tree->root)
		node->left = node->right = NULL;
	else if (!c)
		return -EEXIST;
	else if (c < 0)
	{
		node->left = tree->root->left;
		tree->root->left = NULL;
		node->right = tree->root;
	}
	else
	{
		node->right = tree->root->right ;
		tree->root->right = NULL;
		node->left = tree->root;
	}

	tree->root = node;
	tree->size++;

	return 0;
}

static struct splay_node *
remove_root_node(struct splay_tree *tree)
{
	assert(tree);
	assert(tree->root);

	struct splay_node **n;
	struct splay_node * const rv = tree->root;

	if (!tree->root->left && !tree->root->right)
		tree->root = NULL;
	else if (!tree->root->left)
		tree->root = tree->root->right;
	else if (!tree->root->right)
		tree->root = tree->root->left;
	else
	{
		n = find_max(&tree->root->left);

		tree->root = *n;
		*n = (*n)->left;

		tree->root->left = rv->left;
		tree->root->right = rv->right;

		rv->left = NULL;
		rv->right = NULL;
	}

	tree->size--;
	return rv;
}

void
splay_initialize(struct splay_tree *tree, int (*cmp)(const void*, const void *))
{
	assert(tree);
	assert(cmp);

	tree->size = 0;
	tree->root = 0;
	tree->compare = cmp;
}

static void
reassemble(struct splay_node **middle,
		struct splay_node **left_head,
		struct splay_node **left_tail,
		struct splay_node **right_head,
		struct splay_node **right_tail)
{
	assert(middle);
	assert(left_head);
	assert(left_tail);
	assert(right_head);
	assert(right_tail);

	if (*left_head)
	{
		assert(*middle);

		if ((*middle)->left)
			(*left_tail)->right = (*middle)->left;

		(*middle)->left = *left_head;
	}

	if (*right_head)
	{
		assert(*middle);

		if ((*middle)->right)
			(*right_tail)->left = (*middle)->right;

		(*middle)->right = *right_head;
	}
}

static int
splay(int (*cmp) (const void *, const void *), struct splay_node **root, const void *key)
{
	assert(cmp);
	assert(root);
	assert(key);

	struct splay_node *right_head = NULL, *right_tail = NULL,
			  *left_head = NULL, *left_tail = NULL;
	int c = 1;

	if (!*root)
		return 0;

	for (;;)
	{
		c = cmp(key, splay_get(*root));

		if (!c)
			break;

		if (c < 0)
		{
			if (!(*root)->left)
			       break;

			if ((c = cmp(key, splay_get((*root)->left))) < 0)
			{
				rotate_right(root);

				if (!(*root)->left)
					break;
			}

			zig_right(root, &right_tail);

			if (!right_head)
				right_head = right_tail;
		}
		else
		{
			if (!(*root)->right)
				break;

			if ((c = cmp(key, splay_get((*root)->right))) > 0)
			{
				rotate_left(root);

				if (!(*root)->right)
					break;
			}

			zig_left(root, &left_tail);

			if (!left_head)
				left_head = left_tail;
		}
	}

	reassemble(root, &left_head, &left_tail, &right_head, &right_tail);

	return c;
}

int
splay_insert(struct splay_tree *tree, struct splay_node *node)
{
	assert(tree);
	assert(node);

	int c = splay(tree->compare, &tree->root, splay_get(node));

	return insert(tree, node, c);
}

const struct splay_node *
splay_find(struct splay_tree *tree, const void *key)
{
	assert(tree);
	assert(key);

	if (!splay(tree->compare, &tree->root, key))
		return tree->root;

	return NULL;
}

static const struct splay_node *
splay_min(struct splay_node **root)
{
	assert(root);

	splay(dummy_min_compare, root, (void*)1);

	return *root;
}

const struct splay_node *
splay_find_min(struct splay_tree *tree)
{
	assert(tree);

	if (!splay_size(tree))
		return NULL;

	return splay_min(&tree->root);
}

static const struct splay_node *
splay_max(struct splay_node **root)
{
	assert(root);

	splay(dummy_max_compare, root, (void*)1);

	return *root;
}

const struct splay_node *
splay_find_max(struct splay_tree *tree)
{
	assert(tree);

	if (!splay_size(tree))
		return NULL;

	return splay_max(&tree->root);
}

const struct splay_node *
splay_find_next(struct splay_tree *tree, const void *key)
{
	assert(tree);
	assert(key);

	const struct splay_node *n = splay_find(tree, key);

	if (!n)
		return NULL;

	assert(n == tree->root);

	if (tree->root->right)
	{
		splay_min(&tree->root->right);
		return splay_find(tree, splay_get(tree->root->right));
	}

	return NULL;
}

const struct splay_node *
splay_find_prev(struct splay_tree *tree, const void *key)
{
	assert(tree);
	assert(key);

	const struct splay_node *n = splay_find(tree, key);

	if (!n)
		return NULL;

	assert(n == tree->root);

	if (tree->root->left)
	{
		splay_max(&tree->root->left);
		return splay_find(tree, splay_get(tree->root->left));
	}

	return NULL;
}

struct splay_node *
splay_remove_min(struct splay_tree *tree)
{
	assert(tree);

	if (!splay_size(tree))
		return NULL;

	splay_min(&tree->root);

	return remove_root_node(tree);
}

struct splay_node *
splay_remove_max(struct splay_tree *tree)
{
	assert(tree);

	if (!splay_size(tree))
		return NULL;

	splay_max(&tree->root);

	return remove_root_node(tree);
}

struct splay_node *
splay_remove_next(struct splay_tree *tree, const void *key)
{
	assert(tree);
	assert(key);

	const struct splay_node *n = splay_find(tree, key);

	if (!n)
		return NULL;

	assert(n == tree->root);

	if (tree->root->right)
	{
		splay_min(&tree->root->right);
		return splay_remove(tree, splay_get(tree->root->right));
	}

	return NULL;
}

struct splay_node *
splay_remove_prev(struct splay_tree *tree, const void *key)
{
	assert(tree);
	assert(key);

	const struct splay_node *n = splay_find(tree, key);

	if (!n)
		return NULL;

	assert(n == tree->root);

	if (tree->root->left)
	{
		splay_max(&tree->root->left);
		return splay_remove(tree, splay_get(tree->root->left));
	}

	return NULL;
}

struct splay_node *
splay_remove_any(struct splay_tree *tree)
{
	assert(tree);

	if (!tree->root)
		return NULL;

	return remove_root_node(tree);
}

struct splay_node *
splay_remove(struct splay_tree *tree, const void *key)
{
	assert(tree);
	assert(key);

	if (!splay_size(tree))
		return NULL;

	if (!splay(tree->compare, &tree->root, key))
		return remove_root_node(tree);

	return NULL;
}

static int
iteration_recursion(const struct splay_node *node, int (*cb)(const struct splay_node *, void *), void *arg)
{
	assert(node);
	assert(cb);

	int r = 0;

	if (node->left && (r = iteration_recursion(node->left, cb, arg)) != 0)
		return r;

	if ((r = cb(node, arg)) != 0)
		return r;

	if (node->right)
		r = iteration_recursion(node->right, cb, arg);

	return r;
}

int
splay_iterate(const struct splay_tree *tree, int(*cb)(const struct splay_node *, void *), void *arg)
{
	assert(tree);
	assert(cb);

	if (splay_is_empty(tree))
		return 0;
	else
		return iteration_recursion(tree->root, cb, arg);
}

#ifndef NDEBUG

#include <stdio.h>

struct verification_variables
{
	size_t counter, err;
	const void *prev;
	int (*compare)(const void *, const void *);
};

static void
print_inner(const struct splay_node *node, int lvl, char *pf)
{
	int i;

	if (lvl == 0)
		printf("/==============================\\\n");

	if (node->right)
		print_inner(node->right, lvl + 1, "/ ");
	for (i = 0; i < lvl; ++i)
		printf("    ");
	printf("%s%s\n", pf, (char*)node->data);
	if (node->left)
		print_inner(node->left, lvl + 1, "\\ ");

	if (lvl == 0)
		printf("\\==============================/\n");
}

void
debug_print(const struct splay_node *m,
		const struct splay_node *lvl,
		const struct splay_node *r)
{
	if (lvl)
	{
		printf("\tLEFT\n");
		print_inner(lvl, 0, "-");
	}
	if (m)
	{
		printf("\tMIDDLE\n");
		print_inner(m, 0, "-");
	}
	if (r)
	{
		printf("\tRIGHT\n");
		print_inner(r, 0, "-");
	}
}

void
splay_print(const struct splay_tree *tree)
{
	assert(tree);

	if (tree->root)
		print_inner(tree->root, 0, "-");
}

static int
ensure_preorder(const struct splay_node *node, struct verification_variables *vars)
{
        static const struct splay_node *prev;

        if (node)
        {
		vars->counter++;

                if (prev && vars->compare(splay_get(prev), splay_get(node)) >= 0)
		{
			fprintf(stderr, "ERROR: Preorder condition not met.\n");
			vars->err++;
		}
        }

        prev = node;

        return 0;
}

int
splay_verify(const struct splay_tree *tree)
{
	struct verification_variables vars = { 0, 0, NULL, NULL };

	vars.compare = tree->compare;

	ensure_preorder(NULL, NULL);

	vars.err += splay_iterate(tree,
			(int(*)(const struct splay_node *, void *))ensure_preorder,
			&vars);

	if (vars.counter != splay_size(tree))
	{
		fprintf(stderr, "ERROR: splay_size() and iteration counter don't match (ic=%zd, z=%zd).\n",
				vars.counter, splay_size(tree));
		vars.err++;
	}

	if (vars.err)
		fprintf(stderr, "INFO: Errors encountered. Verification fails.\n");

	return -vars.err;
}

#endif
