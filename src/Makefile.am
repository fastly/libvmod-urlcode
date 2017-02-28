AM_CFLAGS = $(VARNISHAPI_CFLAGS) -Wall -Werror
AM_LDFLAGS = $(VARNISHAPI_LIBS) $(VMOD_LDFLAGS)

vmod_LTLIBRARIES = libvmod_urlcode.la

libvmod_urlcode_la_SOURCES = \
	vmod_urlcode.c

nodist_libvmod_urlcode_la_SOURCES = \
	vcc_urlcode_if.c \
	vcc_urlcode_if.h

dist_man_MANS = vmod_urlcode.3

@BUILD_VMOD_URLCODE@

AM_TESTS_ENVIRONMENT = \
	PATH="$(VMOD_TEST_PATH)" \
	LD_LIBRARY_PATH="$(VARNISH_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = varnishtest -v
AM_VTC_LOG_FLAGS = -Dvmod_urlcode="$(VMOD_URLCODE)"

TESTS = @VMOD_TESTS@

EXTRA_DIST = \
	vmod_urlcode.vcc \
	$(VMOD_TESTS)
