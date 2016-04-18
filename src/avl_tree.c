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
#include <string.h>
#include "avl_tree.h"

#define MAX(a, b) ((a) >= (b) ? (a) : (b))

#include <stdio.h>

enum direction
{
	MIN,
	MAX
};

void
avl_initialize(struct avl_tree *t, int (*cmp)(const void*, const void *))
{
	assert(t);
	assert(cmp);

	t->size = 0;
	t->compare = cmp;
	t->root = 0;
}

static void
initialize_leaf(struct avl_node *l)
{
	assert(l);

	l->left = 0;
	l->right = 0;
	l->height = 0;
}

static int
update_height(struct avl_node *n)
{
	if (!n)
		return 0;

	if (!n->right && !n->left)
		return n->height = 1;
	else if (n->right && n->left)
		return n->height = MAX(n->left->height, n->right->height) + 1;
	else if (n->right)
		return n->height = n->right->height + 1;
	else
		return n->height = n->left->height + 1;
}

static int
get_balance_factor(const struct avl_node *n)
{
	if (!n)
		return 0;

	return update_height(n->left) - update_height(n->right);
}

static void
rotate_left(struct avl_node **p)
{
	assert(p);

	struct avl_node *a = *p;

	*p = a->right;
	a->right = (*p)->left;
	(*p)->left = a;

	update_height((*p)->left);
	update_height((*p)->right);
	update_height(*p);
}

static void
rotate_right(struct avl_node **p)
{
	assert(p);

	struct avl_node *a = *p;

	*p = a->left;
	a->left = (*p)->right;
	(*p)->right = a;

	update_height((*p)->right);
	update_height((*p)->left);
	update_height(*p);
}

static void
balance(struct avl_node **n)
{
	assert(n);
	assert(*n);

	struct avl_node **c;
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
insert_recurse(struct avl_tree *t,
		struct avl_node **r,
		struct avl_node *n)
{
	assert(t);
	assert(r);
	assert(n);

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
		return ENOTUNIQ;

	if (rv)
		return rv;
	else
		balance(r);

	return 0;
}

int
avl_insert(struct avl_tree *t, struct avl_node *n)
{
	assert(t);
	assert(n);

	int rv;

	initialize_leaf(n);

	rv = insert_recurse(t, &t->root, n);

	if (!rv)
	{
		update_height(t->root);
		t->size++;
	}

	return rv;
}

struct avl_node *
remove_rightmost(struct avl_node **t)
{
	assert(t);
	assert(*t);

	struct avl_node *h;

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
remove_node(struct avl_node **r)
{
	assert(r);
	assert(*r);

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
remove_leaf_r(struct avl_tree *m, struct avl_node **r, const enum direction d)
{
	assert(m);
	assert(r);
	assert(*r);

	struct avl_node *rv;

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
	assert(m->size > 0);

	struct avl_node * r = remove_leaf_r(m, &m->root, MIN);

	if (r)
		--m->size;

	return r;
}

struct avl_node *
avl_remove_max(struct avl_tree * m)
{
	assert(m);
	assert(m->size > 0);

	struct avl_node * r = remove_leaf_r(m, &m->root, MAX);

	if (r)
		--m->size;

	return r;
}

const struct avl_node *
avl_find_min(const struct avl_tree *m)
{
	assert(m);

	struct avl_node *r = m->root;

	if (r)
		while (r->left)
			r = r->left;

	return r;
}

const struct avl_node *
avl_find_max(const struct avl_tree *m)
{
	assert(m);

	struct avl_node *r = m->root;

	if (r)
		while (r->right)
			r = r->right;

	return r;
}

const struct avl_node *
avl_find(const struct avl_tree *m, const void *d)
{
	assert(m);
	assert(d);
	assert(m->compare);

	int cmp;
	struct avl_node *t = m->root;

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
remove_recurse(const struct avl_tree *m, struct avl_node **r, const void * d)
{
	assert(m);
	assert(r);
	assert(*r);
	assert(d);

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
avl_remove(struct avl_tree *m, const void *d)
{
	assert(m);
	assert(d);

	struct avl_node *r;

	if (!m->root)
		return NULL;

	r = remove_recurse(m, &m->root, d);

	if (r)
		--m->size;

	return r;
}

static int
avl_iterate_recurse(const struct avl_node *n, int (*cb)(const struct avl_node *, void *), void *a)
{
	assert(n);
	assert(cb);

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
avl_iterate(const struct avl_tree *m, int(*cb)(const struct avl_node *, void *), void *a)
{
	assert(m);
	assert(cb);

	if (avl_empty(m))
		return 0;
	else
		return avl_iterate_recurse(m->root, cb, a);
}

#ifndef NDEBUG

#include <stdio.h>

static int
verify_inner(const struct avl_node * n)
{
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
avl_verify(const struct avl_tree *m)
{
	assert(m);

	int rv = 0;

	printf("Verifying AVL search tree's integrity: ");

	if (m->root)
		rv = verify_inner(m->root);

	if (!rv)
		printf("PASSED\n");
	else
		printf("FAILED\n");

	return rv;
}

static void
print_inner(const struct avl_node *n, int l, char *pf)
{
	int i;

	if (l == 0)
		printf("/==============================\\\n");

	if (n->left)
		print_inner(n->left, l + 1, "/ ");
	for (i = 0; i < l; ++i)
		printf("    ");
	printf("%s%s(%d)\n", pf, (char*)n->data, n->height);
	if (n->right)
		print_inner(n->right, l + 1, "\\ ");

	if (l == 0)
		printf("\\==============================/\n");
}

void
avl_print(const struct avl_tree *m)
{
	assert(m);

	if (m->root)
		print_inner(m->root, 0, "-");
}

#endif
