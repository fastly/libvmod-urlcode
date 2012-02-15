============
vmod_urlcode
============

----------------------
Varnish URLcode Module
----------------------

:Author: Rogier "DocWilco" Mulhuijzen
:Date: 2012-02-15
:Version: 1.0
:Manual section: 3

SYNOPSIS
========

::

	import urlcode;
	urlcode.encode(<string>);
	urlcode.decode(<string>);

DESCRIPTION
===========

Varnish Module (vmod) for encoding or decoding to/from "percent encoding" as 
per RFC3986.

For backward compatibility, a + will be decoded to a space.

FUNCTIONS
=========

Example VCL::

	backend foo { ... };

	import urlcode;

	sub vcl_recv {
		set req.url = "/example?url=" + urlcode.encode("http://" +
			req.http.host + req.url);
	}

encode
------

Prototype
        ::

		urlcode.encode(STRING_LIST input)

Return value
	STRING
Description
	Returns a percent encoded version of input. Or NULL if sess_workspace
	does not have enough space for the operation.
Example
	::

		set resp.http.foo = urlcode.encode("hello world!");

decode
------

Prototype
        ::

		urlcode.decode(STRING_LIST input)

Return value
	STRING
Description
	Returns a percent decoded version of input. Or NULL if either the 
	encoding is invalid or sess_workspace does not have enough space for
	the operation.
Example
	::

		set resp.http.foo = urlcode.decode("hello%20world%21");


INSTALLATION
============

The source tree is based on autotools to configure the building, and
does also have the necessary bits in place to do functional unit tests
using the varnishtest tool.

Usage::

 ./configure VARNISHSRC=DIR [VMODDIR=DIR]

`VARNISHSRC` is the directory of the Varnish source tree for which to
compile your vmod. Both the `VARNISHSRC` and `VARNISHSRC/include`
will be added to the include search paths for your module.

Optionally you can also set the vmod install directory by adding
`VMODDIR=DIR` (defaults to the pkg-config discovered directory from your
Varnish installation).

Make targets:

* make - builds the vmod
* make install - installs your vmod in `VMODDIR`
* make check - runs the unit tests in ``src/tests/*.vtc``


HISTORY
=======

Version 1.0: Initial version.

COPYRIGHT
=========

This document is licensed under the same license as the
libvmod-urlcode project. See LICENSE for details.

* Copyright (c) 2012 Fastly Inc.
