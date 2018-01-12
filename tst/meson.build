# dsac -- Data Structures and Alorithms for C
# Copyright (C) 2016-2018 Hemmo Nieminen
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

_cmocka = dependency('cmocka')

_test_cflags = [
	'-DUNIT_TESTING',
	'-isystem', meson.current_source_dir() + '/system'
]

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

_test_deps = [
	_threads,
	_cmocka,
	declare_dependency(
		link_with : [
			static_library(
				'dsac_test_lib_gnu',
				_all_sources_gnu,
				include_directories : dsac_include,
				c_args : _test_cflags + [ '-D_GNU_SOURCE' ],
				dependencies : [_cmocka, _threads]
			),
			static_library(
				'dsac_test_lib',
				_all_sources + files('cmocka-wrapper.c'),
				include_directories : dsac_include,
				c_args : _test_cflags,
				dependencies : [_cmocka, _threads]
			)
		],
		include_directories : dsac_include
	)
]

test('search_structure-avl',
	executable('search_structure-avl',
		'test_search_structure.c',
		dependencies : _test_deps,
		include_directories : dsac_include,
		c_args : [
			'-DSEARCH_STRUCTURE_BACKEND=avl',
			'-DTEST_ITERATION_COUNT=100000',
			'-DTEST_ELEMENT_COUNT=100'
		] + _test_cflags
	)
)

test('search_structure-splay',
	executable('search_structure-splay',
		'test_search_structure.c',
		dependencies : _test_deps,
		include_directories : dsac_include,
		c_args : [
			'-DSEARCH_STRUCTURE_BACKEND=splay',
			'-DTEST_ITERATION_COUNT=100000',
			'-DTEST_ELEMENT_COUNT=100'
		] + _test_cflags
	)
)

foreach t : [
		[
			['test_avl_tree.c'],
			'avl_tree',
			['-D_GNU_SOURCE']
		],
		[
			['test_bitmap.c'],
			'bitmap',
			[]
		],
		[
			['test_debug_flags.c'],
			'debug_flags',
			[]
		],
		[
			['test_blocking_ring_buffer.c'],
			'blocking_ring_buffer',
			['-D_GNU_SOURCE']
		],
		[
			['test_blocking_homogeneous_ring_buffer.c'],
			'blocking_homogeneous_ring_buffer',
			['-D_GNU_SOURCE']
		],
		[
			['test_doubly_linked_list.c'],
			'doubly_linked_list',
			[]
		],
		[
			['test_condition.c'],
			'condition',
			['-D_GNU_SOURCE']
		],
		[
			['test_fifo.c'],
			'fifo',
			[]
		],
		[
			['test_flags.c'],
			'flags',
			[]
		],
		[
			['test_homogeneous_ring_buffer.c'],
			'homogeneous_ring_buffer',
			[]
		],
		[
			['test_lifo.c'],
			'lifo',
			[]
		],
		[
			['test_linked_list.c', '../src/linked_list.c'],
			'linked_list',
			[]
		],
		[
			['test_linked_list.c', '../src/linked_list.c'],
			'linked_list__size',
			['-DLINKED_LIST_CONFIG_SIZE_CACHE']
		],
		[
			['test_linked_list.c', '../src/linked_list.c'],
			'linked_list__notail',
			['-DLINKED_LIST_CONFIG_NO_TAIL_POINTER']
		],
		[
			['test_linked_list.c', '../src/linked_list.c'],
			'linked_list__size_notail',
			['-DLINKED_LIST_CONFIG_SIZE_CACHE', '-DLINKED_LIST_CONFIG_NO_TAIL_POINTER']
		],
		[
			['test_typed_linked_list.c'],
			'typed_linked_list',
			[]
		],
		[
			['test_lock.c'],
			'lock',
			['-D_GNU_SOURCE']
		],
		[
			['test_mergesort.c'],
			'mergesort',
			[]
		],
		[
			['test_quicksort.c'],
			'quicksort',
			[]
		],
		[
			['test_sorting_algorithms.c'],
			'sorting_algorithms',
			[]
		],
		[
			['test_ring_buffer.c'],
			'ring_buffer',
			[]
		],
		[
			['test_radix_tree.c', '../src/radix_tree.c'],
			'radix_tree',
			[]
		],
		[
			['test_radix_tree.c', '../src/radix_tree.c'],
			'radix_tree__no_cached_key_length',
			['-DRADIX_TREE_CONFIG_NO_CAHED_KEY_LENGHT']
		],
		[
			['test_radix_tree.c', '../src/radix_tree.c'],
			'radix_tree__prealloc_one',
			['-DRADIX_TREE_CONFIG_ITERATION_PREALLOCATION=1']
		],
		[
			['test_radix_tree.c', '../src/radix_tree.c'],
			'radix_tree__prealloc_one__no_cached_key_length',
			['-DRADIX_TREE_CONFIG_NO_CAHED_KEY_LENGHT', '-DRADIX_TREE_CONFIG_ITERATION_PREALLOCATION=1']
		],
		[
			['test_radix_tree.c', '../src/radix_tree.c'],
			'radix_tree__prealloc_thousand',
			['-DRADIX_TREE_CONFIG_ITERATION_PREALLOCATION=1000']
		],
		[
			['test_radix_tree.c', '../src/radix_tree.c'],
			'radix_tree__prealloc_thousand__no_cached_key_length',
			['-DRADIX_TREE_CONFIG_NO_CAHED_KEY_LENGHT', '-DRADIX_TREE_CONFIG_ITERATION_PREALLOCATION=1000']
		],
		[
			['test_splay_tree.c', '../src/radix_tree.c'],
			'splay_tree',
			[]
		]
	]
	test(t.get(1),
		executable(t.get(1),
			t.get(0),
			dependencies : _test_deps,
			c_args : _test_cflags + t.get(2),
			include_directories : dsac_include,
		)
	)
endforeach
