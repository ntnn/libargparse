AM_CFLAGS = -Isrc
AM_LDFLAGS =

if ENABLE_COVERAGE
AM_CFLAGS += --coverage
AM_LDFLAGS += --coverage
GCOV ?= gcov --relative-only --object-directory src/.libs

coverage-reset:
	@find . -name '*.gcno' -or -name '*.gcda' -or -name '*.gcov' -delete

coverage: $(libargparse_la_SOURCES)
	$(GCOV) $^

coverage-func: $(libargparse_la_SOURCES)
	$(GCOV) --function-summaries $^

endif # ENABLE_COVERAGE

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

if ENABLE_CHECKS
TESTS = tests/argparse \
	tests/subcommands \
	tests/subcommand \
	tests/operand \
	tests/option \
	tests/parse

check_PROGRAMS = $(TESTS)
CFLAGS += -Itests -lcmocka
LDADD = libargparse.la

tests_argparse_SOURCES = tests/common.c tests/argparse.c
tests_subcommands_SOURCES = tests/common.c tests/subcommands.c
tests_subcommand_SOURCES = tests/common.c tests/subcommand.c
tests_operand_SOURCES = tests/common.c tests/operand.c
tests_option_SOURCES = tests/common.c tests/option.c
tests_parse_SOURCES = tests/common.c tests/parse.c

endif # ENABLE_CHECKS

@DX_RULES@

clean-doc:
	rm -rf doc
