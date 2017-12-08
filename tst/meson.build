# dsac -- Data Structures and Alorithms for C
# Copyright (C) 2016-2017 Hemmo Nieminen
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CMOCKA = meson.get_compiler('c').find_library('cmocka')
TEST_TIMEOUT = 900

test_deps = [ CMOCKA, THREADS ]
test_cflags = [
	'-DUNIT_TESTING',
	'-D_GNU_SOURCE',
	'-isystem', meson.current_source_dir() + '/system'
]

if get_option('buildtype') == 'release'
	test_cflags += [ '-DNDEBUG' ]
endif

add_test_setup(
	'valgrind',
	exe_wrapper : [
		'valgrind',
			'--leak-check=full',
			'--track-origins=yes',
			'-v',
			'--show-leak-kinds=all',
			'--error-exitcode=1'
	],
	env : [ 'VALGRIND=1' ],
	timeout_multiplier : 20
)

TEST_LIB = library(
	'dsactest',
	DSAC_LIB_SOURCES + [ 'cmocka-wrapper.c' ],
	include_directories : DSAC_INCLUDE,
	c_args : test_cflags,
	dependencies : test_deps
)

test('search_structure-avl',
	executable('search_structure-avl',
		'test_search_structure.c',
		link_with : TEST_LIB,
		dependencies : test_deps,
		include_directories : DSAC_INCLUDE,
		c_args : [
			'-DSEARCH_STRUCTURE_BACKEND=avl',
			'-DTEST_ITERATION_COUNT=100000',
			'-DTEST_ELEMENT_COUNT=100'
		] + test_cflags
	)
)

test('search_structure-splay',
	executable('search_structure-splay',
		'test_search_structure.c',
		link_with : TEST_LIB,
		dependencies : test_deps,
		include_directories : DSAC_INCLUDE,
		c_args : [
			'-DSEARCH_STRUCTURE_BACKEND=splay',
			'-DTEST_ITERATION_COUNT=100000',
			'-DTEST_ELEMENT_COUNT=100'
		] + test_cflags
	)
)

foreach t : [
		[['test_avl_tree.c'], 'avl_tree', []],
		[['test_bitmap.c'], 'bitmap', []],
		[['test_debug_flags.c'], 'debug_flags', []],
		[['test_blocking_ring_buffer.c'], 'blocking_ring_buffer', []],
		[['test_blocking_homogeneous_ring_buffer.c'], 'blocking_homogeneous_ring_buffer', []],
		[['test_doubly_linked_list.c'], 'doubly_linked_list', []],
		[['test_condition.c'], 'condition', []],
		[['test_fifo.c'], 'fifo', []],
		[['test_flags.c'], 'flags', []],
		[['test_homogeneous_ring_buffer.c'], 'homogeneous_ring_buffer', []],
		[['test_lifo.c'], 'lifo', []],
		[['test_linked_list.c', meson.source_root() + '/src/linked_list.c'], 'linked_list', []],
		[['test_linked_list.c', meson.source_root() + '/src/linked_list.c'], 'linked_list__size', ['-DLINKED_LIST_CONFIG_SIZE_CACHE']],
		[['test_linked_list.c', meson.source_root() + '/src/linked_list.c'], 'linked_list__notail', ['-DLINKED_LIST_CONFIG_NO_TAIL_POINTER']],
		[['test_linked_list.c', meson.source_root() + '/src/linked_list.c'], 'linked_list__size_notail', ['-DLINKED_LIST_CONFIG_SIZE_CACHE', '-DLINKED_LIST_CONFIG_NO_TAIL_POINTER']],
		[['test_typed_linked_list.c'], 'typed_linked_list', []],
		[['test_lock.c'], 'lock', []],
		[['test_quicksort.c'], 'quicksort', []],
		[['test_ring_buffer.c'], 'ring_buffer', []],
		[['test_radix_tree.c', meson.source_root() + '/src/radix_tree.c'], 'radix_tree', []],
		[['test_radix_tree.c', meson.source_root() + '/src/radix_tree.c'], 'radix_tree__no_cached_key_length', ['-DRADIX_TREE_CONFIG_NO_CAHED_KEY_LENGHT']],
		[['test_radix_tree.c', meson.source_root() + '/src/radix_tree.c'], 'radix_tree__prealloc_one', ['-DRADIX_TREE_CONFIG_ITERATION_PREALLOCATION=1']],
		[['test_radix_tree.c', meson.source_root() + '/src/radix_tree.c'], 'radix_tree__prealloc_one__no_cached_key_length', ['-DRADIX_TREE_CONFIG_NO_CAHED_KEY_LENGHT', '-DRADIX_TREE_CONFIG_ITERATION_PREALLOCATION=1']],
		[['test_radix_tree.c', meson.source_root() + '/src/radix_tree.c'], 'radix_tree__prealloc_thousand', ['-DRADIX_TREE_CONFIG_ITERATION_PREALLOCATION=1000']],
		[['test_radix_tree.c', meson.source_root() + '/src/radix_tree.c'], 'radix_tree__prealloc_thousand__no_cached_key_length', ['-DRADIX_TREE_CONFIG_NO_CAHED_KEY_LENGHT', '-DRADIX_TREE_CONFIG_ITERATION_PREALLOCATION=1000']],
		[['test_splay_tree.c', meson.source_root() + '/src/radix_tree.c'], 'splay_tree', []]
		]
	test(t.get(1),
		executable(t.get(1),
			t.get(0),
			link_with : TEST_LIB,
			c_args : test_cflags + t.get(2),
			dependencies : test_deps,
			include_directories : DSAC_INCLUDE,
		)
	)
endforeach
