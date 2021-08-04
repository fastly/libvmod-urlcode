AM_CPPFLAGS = @VMOD_INCLUDES@

vmoddir = @VMOD_DIR@
vmod_LTLIBRARIES = libvmod_urlcode.la

libvmod_urlcode_la_LDFLAGS = -module -export-dynamic -avoid-version

libvmod_urlcode_la_SOURCES = vmod_urlcode.c

nodist_libvmod_urlcode_la_SOURCES = \
	vcc_if.c \
	vcc_if.h

vmod_urlcode.c: vcc_if.h

vmod_selector.lo: $(nodist_libvmod_selector_la_SOURCES)

vcc_if.h: vcc_if.c

vcc_if.c: vmod_urlcode.vcc
	@VMODTOOL@ $(top_srcdir)/src/vmod_urlcode.vcc

check:
	PATH=@LIBVARNISHAPI_SBINDIR@:$$PATH \
	@VARNISHTEST@ -Dvmod_topbuild=$(abs_top_builddir) $(srcdir)/tests/*.vtc

EXTRA_DIST = \
	vmod_urlcode.vcc \
	$(VMOD_TESTS)

CLEANFILES = $(builddir)/vcc_if.c $(builddir)/vcc_if.h
