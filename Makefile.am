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

TESTS = tests/argparse \
	tests/operand \
	tests/option \
	tests/parse

check_PROGRAMS = $(TESTS)
CFLAGS += -lcmocka
LDADD = libargparse.la

tests_argparse_SOURCES = tests/common.c tests/argparse.c
tests_operand_SOURCES = tests/common.c tests/operand.c
tests_option_SOURCES = tests/common.c tests/option.c
tests_parse_SOURCES = tests/common.c tests/parse.c

examplesdir = examples
examples_PROGRAMS = examples/echo
examples_echo_SOURCES = examples/echo.c
examples_echo_LDADD = libargparse.la

@DX_RULES@

clean-doc:
	rm -rf doc
