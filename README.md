Image Mapping library
=====================

The Image Mapping library (libimgmap) is a C library for POSIX compatible systems
that operates on images in raw formats by memory mapping.

## Goals
I have written this library because I felt unsatisfied by the complex
bloated old libraries used for opening image files.
I wanted a really straightforward method that permits direct access
and memory sharing.
This library is thread safe and without any global variables (so it does not
requires any init/deinit procedures).
It maps directly the files on memory and gives pointer to the
raw data along with information on its format.
This method is faster than buffered operations, especially modifying
image, and makes sharing of images and their modification between different
processes easier.
It might be the best way to open these kind of files on POSIX 64-bits systems,
as the address space of these system is quite huge.

## Image Formats Supported
* PBM/PGM/PPM with text(read-only) and binary (bitmap, 8-bits and 16-bits)

## License
This library is distributed under an ISC-like license (simplified BSD license).

## Limitations
Windows is not supported as its file mapping methods are quite different from
those used by Unix likes. This library is heavily relying on features provided
by a POSIX environment, so it is not portable to non-Unix operating systems.

