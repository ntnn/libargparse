AM_CFLAGS = -Isrc
AM_LDFLAGS =

if ENABLE_DEBUG
AM_CFLAGS += -g
endif

if ENABLE_COVERAGE
AM_CFLAGS += --coverage
AM_LDFLAGS += --coverage

coverage: $(bin_PROGRAMS) $(lib_LTLIBRARIES)
	@gcov -o src/.libs src/argparse.c
endif

if ENABLE_TESTS
TESTS = 
bin_PROGRAMS =
endif

@DX_RULES@

clean-doc:
	rm -r doc/*
