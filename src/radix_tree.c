#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include "debug_flags.h"
#include "radix_tree.h"

#ifndef SEARCH_STRUCTURE_CONFIG_BACKEND
# define SEARCH_STRUCTURE_BACKEND splay
#endif

#ifndef RADIX_TREE_CONFIG_ITERATION_PREALLOCATION
# define RADIX_TREE_CONFIG_ITERATION_PREALLOCATION 50
#endif

#include "search_structure.h"

#if !defined(NDEBUG) || defined(UNIT_TESTING)
# define SEARCH_STRUCTURE_INSERT_FAILSAFE(...) (assert(!SEARCH_STRUCTURE_INSERT(__VA_ARGS__)))
#else
# define SEARCH_STRUCTURE_INSERT_FAILSAFE(...) (SEARCH_STRUCTURE_INSERT(__VA_ARGS__))
#endif

DEBUG_FLAGS_ENUM(radix_tree_debug_flag,
		RADIX_TREE_DEBUG_FLAGS_INITIALIZED,
		_RADIX_TREE_DEBUG_FLAGS_COUNT)

DEBUG_FLAGS_ENUM(radix_node_debug_flag,
		RADIX_NODE_DEBUG_FLAGS_INITIALIZED,
		_RADIX_NODE_DEBUG_FLAGS_COUNT)

struct radix_node
{
	SEARCH_STRUCTURE appendixes;
	char * key;
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	size_t key_len;
#endif
	void * value;
	DEBUG_FLAGS(_RADIX_NODE_DEBUG_FLAGS_COUNT)
};

struct radix_tree
{
	void * (*memalloc) (size_t);
	void (*memfree) (void *);
	void * (*memrealloc) (void *, size_t);
	size_t size;
	SEARCH_STRUCTURE keys;
	DEBUG_FLAGS(_RADIX_TREE_DEBUG_FLAGS_COUNT)
};

/**
 * The element must be the first field in the struct to allow deducing the
 * addreess of the combo from it.
 */
struct radix_node_combo
{
	SEARCH_STRUCTURE_ELEMENT element;
	struct radix_node node;
};

struct argument_holder
{
	struct radix_tree * tree;
	char const * const original_key;
	char const * key;
	void * value;
};

struct lookup_result
{
	SEARCH_STRUCTURE * tree;
	SEARCH_STRUCTURE_ELEMENT * element;
	SEARCH_STRUCTURE * parent_tree;
	SEARCH_STRUCTURE_ELEMENT * parent_element;
};

struct iteration_arguments
{
	int (*callback) (char const * key, void * value, void * callback_argument);
	void * callback_argument;
	char * base;
	size_t baselen;
	size_t base_buflen;
	void * (*memrealloc) (void *, size_t);
};

static int
keycmp(void const * va, void const * vb)
{
	struct radix_node const * ra = (struct radix_node *)va,
	                        * rb = (struct radix_node *)vb;
	char const * a = ra->key,
	           * b = rb->key;

	if (*a < *b)
		return -1;
	else if (*a > *b)
		return 1;
	else
		return 0;
}

static struct radix_node *
getnode(SEARCH_STRUCTURE_ELEMENT * e)
{
	return ((struct radix_node *)SEARCH_STRUCTURE_ELEMENT_VALUE(e));
}

static struct radix_node const *
getnode_const(SEARCH_STRUCTURE_ELEMENT const * e)
{
	return ((struct radix_node const *)SEARCH_STRUCTURE_ELEMENT_VALUE(e));
}

#if !defined(NDEBUG) || defined(UNIT_TESTING)

#include <stdio.h>

int
_radix_tree_dump(SEARCH_STRUCTURE_ELEMENT const * n, void * psib)
{
	int * sib = (int *)psib, nsib = 0;
	struct radix_node const * r = (struct radix_node const *)SEARCH_STRUCTURE_ELEMENT_VALUE(n);

	if (!*sib)
		printf("%s", r->key);
	else
		printf("+%s", r->key);

	++*sib;

	if (SEARCH_STRUCTURE_GET_ENTRY_COUNT(&r->appendixes))
	{
		printf("(");
		assert(!SEARCH_STRUCTURE_ITERATE(&r->appendixes, _radix_tree_dump, &nsib));
		printf(")");
	}

	return 0;
}

void
radix_tree_dump(struct radix_tree const * t)
{
	assert(t);

	int sib = 0;

	printf("(");
	assert(!SEARCH_STRUCTURE_ITERATE(&t->keys, _radix_tree_dump, &sib));
	printf(")\n");
}


static int
integrity_assertion_recursion(SEARCH_STRUCTURE_ELEMENT const * e, void * pa __attribute__((unused)))
{
	assert(e);

	if (!getnode_const(e)->value)
		assert(SEARCH_STRUCTURE_GET_ENTRY_COUNT(&getnode_const(e)->appendixes) > 1);

	SEARCH_STRUCTURE_ITERATE(&getnode_const(e)->appendixes, integrity_assertion_recursion, NULL);

	return 0;
}

static void
assert_integrity(struct radix_tree const * t)
{
	assert(t);

	SEARCH_STRUCTURE_ITERATE(&t->keys, integrity_assertion_recursion, NULL);
}

#else

#define assert_integrity(...)

#endif

struct radix_tree *
radix_tree_initialize(void * (*m) (size_t),
		void (*f) (void *),
		void * (*r) (void *, size_t))
{
	assert(m);
	assert(f);
	assert(r);

	struct radix_tree * t = m(sizeof(struct radix_tree));

	if (!t)
	{
		errno = ENOMEM;
		return NULL;
	}

	SEARCH_STRUCTURE_INITIALIZE(&t->keys, keycmp);
	debug_flags_initialize(t);

	t->memalloc = m;
	t->memfree = f;
	t->memrealloc = r;

	t->size = 0;

	debug_flags_set(t, RADIX_TREE_DEBUG_FLAGS_INITIALIZED);

	return t;
}

static void
empty_tree(struct radix_tree * t, SEARCH_STRUCTURE * ss)
{
	assert(t);
	assert(ss);

	for (SEARCH_STRUCTURE_ELEMENT * e = SEARCH_STRUCTURE_REMOVE_ANY(ss) ;
			e ; e = SEARCH_STRUCTURE_REMOVE_ANY(ss))
	{
		struct radix_node * n = (struct radix_node *)SEARCH_STRUCTURE_ELEMENT_VALUE(e);

		empty_tree(t, &n->appendixes);
		t->memfree(n->key);
		t->memfree((struct radix_node_combo *)e);
	}
}

void
radix_tree_uninitialize(struct radix_tree ** t)
{
	assert(t);
	assert(*t);
	debug_flags_assert(*t, RADIX_TREE_DEBUG_FLAGS_INITIALIZED);

	void (*f) (void *) = (*t)->memfree;
	empty_tree(*t, &(*t)->keys);

	debug_flags_clear(*t, RADIX_TREE_DEBUG_FLAGS_INITIALIZED);
	f(*t);
	*t = NULL;
}

void
radix_node_initialize(struct radix_node * n, char * k, size_t kl, void * v)
{
	assert(n);
	assert(k);
	assert(v);

	debug_flags_initialize(n);
	n->key = k;
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	n->key_len = kl;
#endif
	n->value = v;
	SEARCH_STRUCTURE_INITIALIZE(&n->appendixes, keycmp);

	debug_flags_set(n, RADIX_NODE_DEBUG_FLAGS_INITIALIZED);
}

static void
swap_node_contents(struct radix_node * a, struct radix_node * b)
{
	struct radix_node c = *a;
	*a = *b;
	*b = c;
}

static int
new_node_init(struct radix_tree const * t, struct radix_node * n, char const * k, void * v)
{
	assert(n);
	assert(v);

	size_t const l = strlen(k);
	char * kc = t->memalloc(l + 1);

	if (!kc)
		return ENOMEM;

	strcpy(kc, k);
	radix_node_initialize(n, kc, l, v);

	return 0;
}

static void
swap_appendixes(SEARCH_STRUCTURE * a, SEARCH_STRUCTURE * b)
{
	assert(a);
	assert(b);

	SEARCH_STRUCTURE s = *a;
	*a = *b;
	*b = s;
}

static bool
radix_node_combo_initialize(struct radix_tree const * t, struct radix_node_combo * c, char const * k, void * v)
{
	assert(t);
	assert(c);
	assert(k);

	int e = new_node_init(t, &c->node, k, v);

	if (e)
	{
		errno = ENOMEM;
		return true;
	}

	SEARCH_STRUCTURE_ELEMENT_DEFINE(&c->element, &c->node);

	return false;
}

static struct radix_node_combo *
get_new_combo(struct radix_tree const * t, char const * k , void * v)
{
	assert(t);
	assert(k);

	struct radix_node_combo * c = t->memalloc(sizeof(struct radix_node_combo));

	if (!c)
		errno = ENOMEM;
	else if (radix_node_combo_initialize(t, c, k, v))
	{
		assert(errno == ENOMEM);
		t->memfree(c);
		errno = ENOMEM;
	}
	else
		return c;
	return NULL;
}

static bool
fork_existing_key(struct argument_holder * args, SEARCH_STRUCTURE_ELEMENT * n, unsigned int i)
{
	assert(args);
	assert(n);
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	assert((unsigned long)i < getnode(n)->key_len);
#else
	assert((unsigned long)i < strlen(getnode(n)->key));
#endif

	struct radix_node * o = getnode(n);
	struct radix_node_combo * c = get_new_combo(args->tree, o->key + i, o->value);

	if (!c)
	{
		errno = ENOMEM;
		return true;
	}

	swap_appendixes(&c->node.appendixes, &o->appendixes);
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	o->key_len = i;
#endif
	o->key[i] = '\0';
	o->value = NULL;
	SEARCH_STRUCTURE_INSERT_FAILSAFE(&o->appendixes, &c->element);

	return false;
}

static SEARCH_STRUCTURE_ELEMENT const *
find_by_key(SEARCH_STRUCTURE * ss, char const * k)
{
	assert(ss);

	struct radix_node n;

	if (!k)
		return SEARCH_STRUCTURE_FIND_ANY(ss);
	else
	{
		radix_node_initialize(&n, (char*)k, strlen(k), (void*)1);
		return SEARCH_STRUCTURE_FIND(ss, &n);
	}
}

static void
shift_key(struct radix_node * n, unsigned int s)
{
	assert(n);
	assert(s);
	assert(n->key_len - s > 0);

#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	int j = n->key_len - s,
#else
	int j = strlen(n->key) - s,
#endif
	    i = -1;

	while (++i < j)
		n->key[i] = n->key[i + s];

	n->key[i] = '\0';
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	n->key_len -= s;
#endif
}

static bool
create_leaf(struct argument_holder * args, SEARCH_STRUCTURE * ss)
{
	assert(args);
	assert(ss);

	struct radix_node_combo * c = get_new_combo(args->tree, args->key, args->value);

	if (!c)
	{
		assert(errno = ENOMEM);
		return true;
	}

	// FIXME: This lookup shouldn't be needed as we already sought for
	// this element...
	SEARCH_STRUCTURE_INSERT_FAILSAFE(ss, &c->element);
	args->tree->size++;

	return false;
}

static bool
create_new_branch(struct radix_tree * t, struct argument_holder * args,
		SEARCH_STRUCTURE_ELEMENT * e, int i)
{
	assert(t);
	assert(args);
	assert(e);
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	assert(getnode(e)->key_len > (unsigned long)i);
#else
	assert(strlen(getnode(e)->key) > (unsigned long)i);
#endif

	struct radix_node_combo * c = get_new_combo(args->tree, args->key, args->value);

	if (!c)
	{
		assert(errno == ENOMEM);
		return true;
	}

	shift_key(getnode(e), i);
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	getnode(e)->key = t->memrealloc(getnode(e)->key, getnode(e)->key_len + 1);
#else
	getnode(e)->key = t->memrealloc(getnode(e)->key, strlen(getnode(e)->key) + 1);
#endif
	if (!getnode_const(e)->key)
	{
		/**
		 * Should never happen as we are reducing the amount of memory we
		 * are using, but nevertheless, this should be handled better.
		 * FIXME.
		 */
		abort();
	}
	SEARCH_STRUCTURE_INSERT_FAILSAFE(&c->node.appendixes, &c->element);
	swap_node_contents(&c->node, getnode(e));

	args->tree->size++;

	return false;
}

static int
find_difference_index(char const * a, const char * b)
{
	assert(a);
	assert(b);

	int i;

	for (i = 0 ; a[i] == b[i] && a[i] != '\0' ; i++);

	return i;
}

static bool
remove_nonvalue_node(struct argument_holder * args, SEARCH_STRUCTURE_ELEMENT * e)
{
	assert(args);
	assert(e);

	assert(SEARCH_STRUCTURE_GET_ENTRY_COUNT(&getnode_const(e)->appendixes) == 1);

	struct radix_node * p = getnode(e);
	SEARCH_STRUCTURE_ELEMENT * r = SEARCH_STRUCTURE_REMOVE_ANY(&p->appendixes);
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	char * k = args->tree->memrealloc(p->key, p->key_len + getnode_const(r)->key_len + 1);
#else
	char * k = args->tree->memrealloc(p->key, strlen(p->key) + strlen(getnode_const(r)->key) + 1);
#endif

	if (!k)
	{
		errno = ENOMEM;
		return true;
	}

	p->key = k;
	strcat(p->key, getnode_const(r)->key);
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	p->key_len += getnode_const(r)->key_len;
#endif
	p->value = getnode(r)->value;

	swap_appendixes(&p->appendixes, &getnode(r)->appendixes);

	args->tree->memfree(getnode(r)->key);
	args->tree->memfree((struct radix_node_combo *)r);

	return false;
}


bool
radix_tree_insert(struct radix_tree * t, char const * k, void * v)
{
	assert(t);
	assert(k);
	assert(v);
	debug_flags_assert(t, RADIX_TREE_DEBUG_FLAGS_INITIALIZED);

	struct argument_holder args = {t, k, k, v};
	SEARCH_STRUCTURE_ELEMENT const * fork = NULL;
	bool rv = true;
	SEARCH_STRUCTURE * ss = &t->keys;

	while (true)
	{
		SEARCH_STRUCTURE_ELEMENT const * cur = find_by_key(ss, args.key);
		unsigned int i;

		if (!cur)
		{
			rv = create_leaf(&args, ss);
			break;
		}

		i = find_difference_index(args.key, getnode_const(cur)->key);

		if (args.key[i] == '\0')
		{
			if (getnode_const(cur)->key[i] == '\0')
			{
				errno = EEXIST;
				/**
				 * Not possible that we have made a fork during insertion
				 * and end up with EEXIST.
				 */
				assert(!fork);
			}
			else
				rv = create_new_branch(t, &args, (SEARCH_STRUCTURE_ELEMENT *)cur, i);
			break;
		}
		else if (getnode_const(cur)->key[i] != '\0')
		{
			assert(!fork);

			if (fork_existing_key(&args, (SEARCH_STRUCTURE_ELEMENT *)cur, i))
				break;

			fork = cur;
		}

		ss = &getnode((SEARCH_STRUCTURE_ELEMENT *)cur)->appendixes;
		args.key += i;
	}

	if (rv && fork)
	{
		int e = errno;
		remove_nonvalue_node(&args, (SEARCH_STRUCTURE_ELEMENT *)fork);
		errno = e;
	}

	assert_integrity(t);
	return rv;
}

static unsigned int
index_of_diff(char const * a, char const * b)
{
	assert(a);
	assert(b);

	unsigned int i = 0;

	while (a[i] == b[i] && a[i] != '\0')
		i++;

	return i;
}

static bool
lookup_key(struct radix_tree * t, char const * k, struct lookup_result * rv)
{
	assert(t);
	debug_flags_assert(t, RADIX_TREE_DEBUG_FLAGS_INITIALIZED);

	int i;
	rv->tree = &t->keys;
	rv->element = NULL;

again:
	rv->parent_element = rv->element;
	rv->parent_tree = rv->tree;
	rv->element = (SEARCH_STRUCTURE_ELEMENT *)find_by_key(rv->tree, k);

	if (!rv->element)
		return true;

	if (!k)
	{
		/** An non-valued node shoul never be a leaf node... */
		if (!getnode_const(rv->element)->value)
		{
			rv->tree = &getnode(rv->element)->appendixes;
			goto again;
		}
		return false;
	}

	i = index_of_diff(getnode(rv->element)->key, k);

	if (k[i] == getnode(rv->element)->key[i])
	{
		assert(getnode(rv->element)->key[i] == '\0');
		assert(k[i] == '\0');
		return false;
	}

	if (getnode(rv->element)->key[i] == '\0')
	{
		k += i;
		rv->tree = &getnode(rv->element)->appendixes;
		goto again;
	}

	return true;
}

static void *
remove_leaf_node(struct radix_tree * t, SEARCH_STRUCTURE * ss, SEARCH_STRUCTURE_ELEMENT * e)
{
	assert(ss);
	assert(e);

	void * rv = getnode(e)->value;

	/** FIXME: Redoing the lookup (we already have the correct element). */

#if !defined(NDEBUG) || defined(UNIT_TESTING)
	assert(e == SEARCH_STRUCTURE_REMOVE(ss, SEARCH_STRUCTURE_ELEMENT_VALUE(e)));
#else
	SEARCH_STRUCTURE_REMOVE(ss, SEARCH_STRUCTURE_ELEMENT_VALUE(e));
#endif

	t->memfree(((struct radix_node *)SEARCH_STRUCTURE_ELEMENT_VALUE(e))->key);
	t->memfree((struct radix_node_combo *)e);

	return rv;
}

static bool
realloc_key(struct radix_tree * t, char ** key, size_t z)
{
	assert(t);
	assert(key);

	char * tmp = t->memrealloc(*key, z);

	if (tmp)
	{
		*key = tmp;
		return false;
	}

	return true;
}

static void *
disown_node(struct radix_node * n)
{
	assert(n);

	void * rv = n->value;
	n->value = NULL;

	return rv;
}

void *
remove_node(struct radix_tree * t, struct radix_node * n)
{
	assert(t);
	assert(n);

	void * rv = n->value;

	SEARCH_STRUCTURE_ELEMENT * e = SEARCH_STRUCTURE_REMOVE_ANY(&n->appendixes);
	assert(SEARCH_STRUCTURE_IS_EMPTY(&n->appendixes));

	if (realloc_key(t, &n->key, strlen(n->key) + strlen(getnode(e)->key) + 1))
	{
		SEARCH_STRUCTURE_INSERT_FAILSAFE(&n->appendixes, e);
		errno = ENOMEM;
		return NULL;
	}

	strcat(n->key, getnode(e)->key);
	n->value = getnode(e)->value;
#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	n->key_len += getnode(e)->key_len;
#endif

	swap_appendixes(&n->appendixes, &getnode(e)->appendixes);

	t->memfree(getnode(e)->key);
	t->memfree((struct radix_node_combo *)e);

	return rv;
}

static unsigned int
get_node_child_count(struct radix_node const * n)
{
	assert(n);

	return SEARCH_STRUCTURE_GET_ENTRY_COUNT(&n->appendixes);
}

void *
radix_tree_remove(struct radix_tree * t, char const * k)
{
	assert(t);

	struct lookup_result r;
	struct argument_holder args = {t, k, k, NULL};
	void * rv;

	if (lookup_key(t, args.key, &r))
		return NULL;

	switch (get_node_child_count(getnode(r.element)))
	{
		case 0:
			rv = remove_leaf_node(t, r.tree, r.element);
			if (r.parent_element && !getnode_const(r.parent_element)->value && SEARCH_STRUCTURE_GET_ENTRY_COUNT(r.tree) == 1)
				if (remove_nonvalue_node(&args, r.parent_element))
					return NULL;
			break;
		case 1:
			rv = remove_node(t, getnode(r.element));
			break;
		default:
			rv = disown_node(getnode(r.element));
			break;
	}

	if (rv)
		t->size--;

	assert_integrity(t);
	return rv;
}

size_t
radix_tree_get_key_count(struct radix_tree const * t)
{
	assert(t);
	debug_flags_assert(t, RADIX_TREE_DEBUG_FLAGS_INITIALIZED);

	return t->size;
}

void *
radix_tree_find(struct radix_tree * t, char const * k)
{
	assert(t);

	struct lookup_result r;
	struct argument_holder args = {t, k, k, NULL};

	if (lookup_key(t, args.key, &r))
		return NULL;
	else
		return getnode(r.element)->value;
}

static int
iteration_recursion(SEARCH_STRUCTURE_ELEMENT const * e, void * pa)
{
	assert(e);
	assert(pa);

	struct iteration_arguments * args = (struct iteration_arguments *)pa;
	int rv = 0;

#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	args->baselen += getnode_const(e)->key_len;
#else
	args->baselen += strlen(getnode_const(e)->key);
#endif

	if (args->base_buflen < args->baselen + 1)
	{
		args->base_buflen = args->baselen + 1;
		args->base = args->memrealloc(args->base, args->base_buflen);

		if (!args->base)
		{
			errno = ENOMEM;
			return -1;
		}
	}

	strcat(args->base, getnode_const(e)->key);

	if (getnode_const(e)->value)
		rv = args->callback(args->base, getnode_const(e)->value, args->callback_argument);

	if (!rv)
		rv = SEARCH_STRUCTURE_ITERATE(&getnode_const(e)->appendixes, iteration_recursion, args);

#ifndef RADIX_TREE_CONFIG_NO_CACHED_KEY_LENGTH
	args->baselen -= getnode_const(e)->key_len;
#else
	args->baselen -= strlen(getnode_const(e)->key);
#endif
	args->base[args->baselen] = '\0';

	return rv;
}

int
radix_tree_iterate(struct radix_tree * t, int (*f) (char const *, void *, void *), void * a)
{
	assert(t);
	assert(f);

	int rv, e;
	struct iteration_arguments args =
	{
		f,
		a,
		t->memalloc(RADIX_TREE_CONFIG_ITERATION_PREALLOCATION),
		0,
		RADIX_TREE_CONFIG_ITERATION_PREALLOCATION,
		t->memrealloc
	};

	if (!args.base)
	{
		errno = ENOMEM;
		return -1;
	}

	args.base[0] = '\0';

	rv = SEARCH_STRUCTURE_ITERATE(&t->keys, iteration_recursion, &args);
	e = errno;

	t->memfree(args.base);

	errno = e;
	return rv;
}
