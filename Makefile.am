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
			 src/subcommand.h \
			 src/subcommand.c \
			 src/subcommands.h \
			 src/subcommands.c \
			 src/operand.h \
			 src/operand.c \
			 src/option.h \
			 src/option.c \
			 src/parse.c
libargparse_la_LDFLAGS = -version-info 0:1:0
include_HEADERS = src/argparse.h

TESTS = tests/argparse \
	tests/subcommand \
	tests/operand \
	tests/option \
	tests/parse

check_PROGRAMS = $(TESTS)
CFLAGS += -Itests -lcmocka
LDADD = libargparse.la

tests_argparse_SOURCES = tests/common.c tests/argparse.c
tests_subcommand_SOURCES = tests/common.c tests/subcommand.c
tests_operand_SOURCES = tests/common.c tests/operand.c
tests_option_SOURCES = tests/common.c tests/option.c
tests_parse_SOURCES = tests/common.c tests/parse.c

@DX_RULES@

clean-doc:
	rm -rf doc
