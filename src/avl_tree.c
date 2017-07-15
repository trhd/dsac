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

#include <errno.h>
#include <string.h>
#include "avl_tree.h"

#define MAX(a, b) ((a) >= (b) ? (a) : (b))

#include <stdio.h>

enum direction
{
	MIN,
	MAX
};

#if !defined(NDEBUG) || defined(UNIT_TESTING)

# include <stdio.h>

static int
verify_inner(struct avl_node const * n)
{
	assert(n);
	debug_flags_assert(n, AVL_NODE_DEBUG_FLAG_SET);

	int l = 0, r = 0, d = 0;

	if (n->left)
	{
		l = n->left->height;
		d += verify_inner(n->left);
	}

	if (n->right)
	{
		r = n->right->height;
		d += verify_inner(n->right);
	}

	if (l < r - 1 || l > r + 1)
	{
		printf("\n%s: l=%d, r=%d ", (char*)n->data, l, r);
		++d;
	}

	if (n->height > l + 1 && n->height > r + 1)
	{
		printf("Inconsistent heights (l=%d, r=%d, h=%d).\n",
				l, r, n->height);
		++d;
	}


	return d;
}

int
avl_verify(struct avl_tree const * m)
{
	assert(m);
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	int rv = 0;

	if (m->root)
		rv = verify_inner(m->root);

	if (rv)
	{
		printf("AVL tree verification FAILED!\n");
		avl_print(m);
	}

	return rv;
}

static void
print_inner(struct avl_node const * n, int l, char * pf)
{
	assert(n);
	debug_flags_assert(n, AVL_NODE_DEBUG_FLAG_SET);

	int i;

	if (l == 0)
		fprintf(stdout, "/==============================\\\n");

	if (n->left)
		print_inner(n->left, l + 1, "/ ");
	for (i = 0; i < l; ++i)
		fprintf(stdout, "    ");
	fprintf(stdout, "%s%s(%d)\n", pf, (char*)n->data, n->height);
	if (n->right)
		print_inner(n->right, l + 1, "\\ ");

	if (l == 0)
		fprintf(stdout, "\\==============================/\n");
}

void
avl_print(struct avl_tree const * m)
{
	assert(m);
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	if (m->root)
		print_inner(m->root, 0, "-");
}

#else
# define avl_verify(...)
#endif

void
avl_initialize(struct avl_tree * t, int (*cmp)(void const *, void const *))
{
	assert(t);
	assert(cmp);

	t->size = 0;
	t->compare = cmp;
	t->root = NULL;
	debug_flags_initialize(t);

	debug_flags_set(t, AVL_TREE_DEBUG_FLAG_INITIALIZED);
}

static void
initialize_leaf(struct avl_node * l)
{
	assert(l);

	l->left = 0;
	l->right = 0;
	l->height = 0;
}

static int
update_height(struct avl_node * n)
{
	if (!n)
		return 0;

	debug_flags_assert(n, AVL_NODE_DEBUG_FLAG_SET);

	if (!n->right && !n->left)
		return n->height = 0;
	else if (n->right && n->left)
		return n->height = MAX(n->left->height, n->right->height) + 1;
	else if (n->right)
		return n->height = n->right->height + 1;
	else
		return n->height = n->left->height + 1;
}

static int
get_balance_factor(struct avl_node const * n)
{
	if (!n)
		return 0;

	debug_flags_assert(n, AVL_NODE_DEBUG_FLAG_SET);

	return update_height(n->left) - update_height(n->right);
}

static void
rotate_left(struct avl_node ** p)
{
	assert(p);
	assert(*p);
	assert((*p)->right);

	struct avl_node * a = *p;

	debug_flags_assert(a, AVL_NODE_DEBUG_FLAG_SET);

	*p = a->right;
	a->right =(*p)->left;
	(*p)->left = a;

	update_height((*p)->left);
	update_height((*p)->right);
	update_height(*p);
}

static void
rotate_right(struct avl_node ** p)
{
	assert(p);
	assert(*p);
	assert((*p)->left);

	struct avl_node * a = *p;

	debug_flags_assert(a, AVL_NODE_DEBUG_FLAG_SET);

	*p = a->left;
	a->left = (*p)->right;
	(*p)->right = a;

	update_height((*p)->right);
	update_height((*p)->left);
	update_height(*p);
}

static void
balance(struct avl_node ** n)
{
	assert(n);
	assert(*n);
	debug_flags_assert(*n, AVL_NODE_DEBUG_FLAG_SET);

	struct avl_node ** c;
	int b = get_balance_factor(*n);

	if (b == 2)
	{
		c = &(*n)->left;
		if (get_balance_factor(*c) < 0)
			rotate_left(c);

		rotate_right(n);
	}
	else if (b == -2)
	{
		c = &(*n)->right;
		if (get_balance_factor(*c) > 0)
			rotate_right(c);

		rotate_left(n);
	}

	if (b*b == 4)
		get_balance_factor(*n);

	update_height(*n);
}

static int
insert_recurse(struct avl_tree * t,
		struct avl_node ** r,
		struct avl_node * n)
{
	assert(t);
	assert(r);
	assert(n);
	debug_flags_assert(t, AVL_TREE_DEBUG_FLAG_INITIALIZED);
	debug_flags_assert(n, AVL_NODE_DEBUG_FLAG_SET);

	int c, rv;

	if (!*r)
	{
		*r = n;
		update_height(*r);
		return 0;
	}

	c = t->compare(n->data, (*r)->data);

	if (c < 0)
		rv = insert_recurse(t, &(*r)->left, n);
	else if (c > 0)
		rv = insert_recurse(t, &(*r)->right, n);
	else
		return -ENOTUNIQ;

	if (rv)
		return rv;
	else
		balance(r);

	return 0;
}

int
avl_insert(struct avl_tree * t, struct avl_node * n)
{
	assert(t);
	assert(n);
	debug_flags_assert(t, AVL_TREE_DEBUG_FLAG_INITIALIZED);
	debug_flags_assert(n, AVL_NODE_DEBUG_FLAG_SET);

	int rv;

	initialize_leaf(n);

	rv = insert_recurse(t, &t->root, n);

	if (!rv)
	{
		update_height(t->root);
		t->size++;
	}

	avl_verify(t);

	return rv;
}

struct avl_node *
remove_rightmost(struct avl_node ** t)
{
	assert(t);
	assert(*t);

	struct avl_node * h;

	if ((*t)->right)
		h = remove_rightmost(&(*t)->right);
	else
	{
		h = *t;
		*t = (*t)->left;
	}

	if (*t)
		balance(t);

	return h;
}

static struct avl_node *
remove_node(struct avl_node ** r)
{
	assert(r);
	assert(*r);
	debug_flags_assert(*r, AVL_NODE_DEBUG_FLAG_SET);

	struct avl_node *t = *r, *tt;

	if (!(*r)->left && !(*r)->right)
		*r = NULL;
	else if ((*r)->left && (*r)->right)
	{
		tt = remove_rightmost(&(*r)->left);
		tt->left = (*r)->left;
		tt->right = (*r)->right;
		tt->height = (*r)->height;
		*r = tt;
	}
	else if ((*r)->left)
		*r = (*r)->left;
	else
		*r = (*r)->right;

	t->height = 0;
	t->left = 0;
	t->right = 0;

	return t;
}

struct avl_node *
remove_leaf_r(struct avl_tree * m, struct avl_node ** r, enum direction const d)
{
	assert(m);
	assert(r);
	assert(*r);
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);
	debug_flags_assert(*r, AVL_NODE_DEBUG_FLAG_SET);

	struct avl_node * rv;

	if (d == MIN && (*r)->left)
		rv = remove_leaf_r(m, &(*r)->left, d);
	else if (d == MAX && (*r)->right)
		rv = remove_leaf_r(m, &(*r)->right, d);
	else
		rv = remove_node(r);

	if (*r)
		balance(r);

	return rv;
}

struct avl_node *
avl_remove_min(struct avl_tree * m)
{
	assert(m);
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	struct avl_node * r;

	if (avl_empty(m))
		return NULL;

	r = remove_leaf_r(m, &m->root, MIN);
	assert(r);
	--m->size;

	avl_verify(m);

	return r;
}

struct avl_node *
avl_remove_max(struct avl_tree * m)
{
	assert(m);
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	struct avl_node * r;

	if (avl_empty(m))
		return NULL;

	r = remove_leaf_r(m, &m->root, MAX);
	assert(r);
	--m->size;

	avl_verify(m);

	return r;
}

struct avl_node const *
avl_find_min(struct avl_tree const * m)
{
	assert(m);
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	struct avl_node * r = m->root;

	if (r)
		while (r->left)
			r = r->left;

	avl_verify(m);

	return r;
}

struct avl_node const *
avl_find_max(struct avl_tree const * m)
{
	assert(m);
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	struct avl_node * r = m->root;

	if (r)
		while (r->right)
			r = r->right;

	return r;
}

struct avl_node const *
avl_find(struct avl_tree const * m, void const * d)
{
	assert(m);
	assert(m->compare);
	/** d might be null/zero if pretending pointers to be numbers. */
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	int cmp;
	struct avl_node * t = m->root;

	while (t != NULL)
	{
		cmp = m->compare(d, t->data);

		if (cmp == 0)
			return t;

		if (cmp < 0)
			t = t->left;
		else
			t = t->right;
	}

	return NULL;
}

static struct avl_node *
remove_recurse(struct avl_tree const * m, struct avl_node ** r, void const * d)
{
	assert(m);
	assert(r);
	assert(*r);
	/** d might be null/zero if pretending pointers to be numbers. */
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	int cmp;
	struct avl_node **t, *rv;

	cmp = m->compare(d, (*r)->data);

	if (cmp == 0)
		rv = remove_node(r);
	else
	{
		t = cmp < 0 ? &(*r)->left : &(*r)->right;

		if (!*t)
			return NULL;

		rv = remove_recurse(m, t, d);
	}

	if (*r)
		balance(r);

	return rv;
}

struct avl_node *
avl_remove(struct avl_tree * m, void const * d)
{
	assert(m);
	/** d might be null/zero if pretending pointers to be numbers. */
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	struct avl_node * r;

	if (!m->root)
		return NULL;

	r = remove_recurse(m, &m->root, d);

	if (r)
		--m->size;

	avl_verify(m);

	return r;
}

static int
avl_iterate_recurse(struct avl_node const * n, int (*cb)(struct avl_node const *, void *), void * a)
{
	assert(n);
	assert(cb);
	debug_flags_assert(n, AVL_NODE_DEBUG_FLAG_SET);

	int r = 0;

	if (n->left && (r = avl_iterate_recurse(n->left, cb, a)) != 0)
		return r;

	if ((r = cb(n, a)) != 0)
		return r;

	if (n->right)
		r = avl_iterate_recurse(n->right, cb, a);

	return r;
}

int
avl_iterate(struct avl_tree const * m, int(*cb)(struct avl_node const *, void *), void * a)
{
	assert(m);
	assert(cb);
	debug_flags_assert(m, AVL_TREE_DEBUG_FLAG_INITIALIZED);

	if (avl_empty(m))
		return 0;
	else
		return avl_iterate_recurse(m->root, cb, a);
}

#ifdef avl_verify
# undef avl_verify
#endif
