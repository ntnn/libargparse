AM_CFLAGS = -Isrc
AM_LDFLAGS =

if ENABLE_DEBUG
AM_CFLAGS += -g
endif

if ENABLE_COVERAGE
AM_CFLAGS += --coverage
AM_LDFLAGS += --coverage

coverage-reset:
	@find . -iname '*.gcno' -or -iname '*.gcda' -or -iname '*.gcov' -delete

coverage:
	gcov -o src/.libs $(libargparse_la_SOURCES)
endif

lib_LTLIBRARIES = libargparse.la
libargparse_la_SOURCES = src/argparse.c \
			 src/operand.h \
			 src/operand.c \
			 src/option.h \
			 src/option.c \
			 src/parse.c
include_HEADERS = src/argparse.h

if ENABLE_TESTS
TESTS = tests/argparse

check_PROGRAMS = $(TESTS)
CFLAGS += -lcmocka
LDADD = libargparse.la

tests_argparse_SOURCES = tests/argparse.c
endif

@DX_RULES@

clean-doc:
	rm -rf doc
