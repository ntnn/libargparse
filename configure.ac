AC_INIT(argparse, 0.0.1, nelo@wallus.de)
AC_CONFIG_AUX_DIR(etc)

AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL

# libraries
AM_PROG_AR
LT_INIT

# doxygen
DX_DOXYGEN_FEATURE(ON)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_MAN_FEATURE(ON)
DX_INIT_DOXYGEN(argparse, Doxyfile, doc)

AC_CONFIG_FILES(Makefile)

# --enable-coverage
AC_ARG_ENABLE(coverage,
              AS_HELP_STRING(--enable-coverage, set coverage build flags),
              enable_coverage="$enable_coverage",
              enable_coverage="no"
              )
AM_CONDITIONAL(ENABLE_COVERAGE, test "x$enable_coverage" = "xyes")

AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_OUTPUT

