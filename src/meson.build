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

_threads = dependency('threads')
_all_deps = []
_all_sources = []
_all_sources_gnu = []

_header_only = declare_dependency(
	include_directories : dsac_include
)

_header_only_gnu = declare_dependency(
	include_directories : dsac_include,
	compile_args : '-D_GNU_SOURCE'
)

flags = _header_only
debug_flags = _header_only
refcount = _header_only

condition = _header_only_gnu

atomic_fifo_file = files('atomic_fifo.c')
atomic_fifo = declare_dependency(
	link_with : static_library(
		'atomic_fifo',
		atomic_fifo_file,
		include_directories : dsac_include
	),
	include_directories : dsac_include,
	version : meson.project_version()
)
_all_deps += atomic_fifo
_all_sources += atomic_fifo_file

typed_atomic_fifo = atomic_fifo

atomic_lifo_file = files('atomic_lifo.c')
atomic_lifo = declare_dependency(
	link_with : static_library(
		'atomic_lifo',
		atomic_lifo_file,
		include_directories : dsac_include
	),
	include_directories : dsac_include,
	version : meson.project_version()
)
_all_deps += atomic_lifo
_all_sources += atomic_lifo_file

typed_atomic_lifo = atomic_lifo

avl_tree = declare_dependency(
	link_with : static_library(
		'avl_tree',
		'avl_tree.c',
		include_directories : dsac_include,
		c_args : '-D_GNU_SOURCE'
	),
	include_directories : dsac_include,
	version : meson.project_version()
)
_all_deps += avl_tree
_all_sources_gnu += files('avl_tree.c')

search_structure_avl = avl_tree

lock = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'lock',
		'lock.c',
		include_directories : dsac_include,
		dependencies : _threads,
		c_args : '-D_GNU_SOURCE'
	),
	dependencies : _threads,
	version : meson.project_version()
)
_all_deps += lock
_all_sources_gnu += files('lock.c')

bitmap = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'bitmap',
		'bitmap.c',
		include_directories : dsac_include
	),
	version : meson.project_version()
)

_all_deps += bitmap
_all_sources += files('bitmap.c')

blocking_ring_buffer = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'blocking_ring_buffer',
		'blocking_ring_buffer.c',
		c_args : '-D_GNU_SOURCE',
		include_directories : dsac_include,
		dependencies : [lock, condition]
	),
	version : meson.project_version(),
)
_all_deps += blocking_ring_buffer
_all_sources_gnu += files('blocking_ring_buffer.c')

blocking_homogeneous_ring_buffer = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'blocking_homogeneous_ring_buffer',
		'blocking_homogeneous_ring_buffer.c',
		include_directories : dsac_include,
		dependencies : [lock, condition],
		c_args : '-D_GNU_SOURCE'
	),
	version : meson.project_version()
)
_all_deps += blocking_homogeneous_ring_buffer
_all_sources_gnu += files('blocking_homogeneous_ring_buffer.c')

doubly_linked_list = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'doubly_linked_list',
		'doubly_linked_list.c',
		include_directories : dsac_include
	),
	version : meson.project_version()
)
_all_deps += doubly_linked_list
_all_sources += files('doubly_linked_list.c')

fifo = doubly_linked_list
typed_fifo = fifo
lifo = doubly_linked_list

homogeneous_ring_buffer = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'homogeneous_ring_buffer',
		'homogeneous_ring_buffer.c',
		include_directories : dsac_include
	),
	version : meson.project_version()
)
_all_deps += homogeneous_ring_buffer
_all_sources += files('homogeneous_ring_buffer.c')

typed_ring_buffer = homogeneous_ring_buffer

linked_list = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'linked_list',
		'linked_list.c',
		include_directories : dsac_include
	),
	version : meson.project_version()
)
_all_deps += linked_list
_all_sources += files('linked_list.c')

typed_linked_list = linked_list

mergesort = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'mergesort',
		'mergesort.c',
		include_directories : dsac_include
	),
	version : meson.project_version()
)
_all_deps += mergesort
_all_sources += files('mergesort.c')

ring_buffer = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'ring_buffer',
		'ring_buffer.c',
		include_directories : dsac_include
	),
	version : meson.project_version()
)
_all_deps += ring_buffer
_all_sources += files('ring_buffer.c')

radix_tree = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'radix_tree',
		'radix_tree.c',
		include_directories : dsac_include
	),
	version : meson.project_version()
)
_all_deps += radix_tree
_all_sources += files('radix_tree.c')

quicksort = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'quicksort',
		'quicksort.c',
		include_directories : dsac_include
	),
	version : meson.project_version()
)
_all_deps += quicksort
_all_sources += files('quicksort.c')

splay_tree = declare_dependency(
	include_directories : dsac_include,
	link_with : static_library(
		'splay_tree',
		'splay_tree.c',
		include_directories : dsac_include
	),
	version : meson.project_version()
)
_all_deps += splay_tree
_all_sources += files('splay_tree.c')

search_structure_splay = splay_tree

dsac_lib_all = library(
	'dsac',
	[],
	dependencies : _all_deps,
	version : meson.project_version(),
	install : true
)
