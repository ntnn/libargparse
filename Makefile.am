AM_CFLAGS = -Isrc
AM_LDFLAGS =

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
TESTS = tests/argparse \
	tests/operand \
	tests/option \
	tests/parse

check_PROGRAMS = $(TESTS)
CFLAGS += -lcmocka
LDADD = libargparse.la

tests_argparse_SOURCES = tests/argparse.c
tests_operand_SOURCES = tests/operand.c
tests_option_SOURCES = tests/option.c
tests_parse_SOURCES = tests/parse.c
endif

@DX_RULES@

clean-doc:
	rm -rf doc
