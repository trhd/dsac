[![Build Status](https://travis-ci.org/trhd/dsac.svg)](https://travis-ci.org/trhd/dsac)

Data Structures and Algorithms for C
====================================


About
-----

As the title suggests, this is a (small) collection of data structures and
algorithms implemented in and for C.


Why
---

To get implementations for data structures and algorithms that work and
behave just the way I want them to and to educate myself while
implementing them.


Status
------

Even though there are not (at least yet) many data structures and
algorithms implemented here, the ones that are _should_ work (if not,
please let me know).


How-To
------

To use any data structures or algorithms implemented in this collection
one can either:

- Install the shared library (and headers).
- Copy the source(s) to one's own project.
- Use this collection as a Meson submodule.

A-very-quick-guide to Meson based projects:

	$ mkdir build
	$ meson build
	$ ninja -C build test
	$ sudo ninja -C build install

Asserts (see "man 3 assert") may have been used in the implementations so
one might want to define the NDEBUG compiler macro for release builds.


Bugs, Patches etc.
------------------

Any feedback appreciated. Can be e.g. sent to the author via email.


Author
------

[Hemmo Nieminen](mailto:hemmo.nieminen@iki.fi)


License
-------

See the file called LICENSE.
