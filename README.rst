libargparse is a C library to handle subcommands, options,
option-arguments and operands conforming to the `XBD Utility
Conventions`_ included in `POSIX.1-2008`_.

For usage check the examples in the ``examples/`` directory and the
header file ``argparse.h``.

Using this code:

.. code:: c
    :number-lines: 1

    #include <argparse.h>

    int main(int argc, char **argv) {
        args *args = args_new();

        args_add_option(args, option_new("f", "feature", "description of feature"));
        args_parse(args, argc--, argv++);

        args_free(args);
    }

The following arguments will be recognized and parsed:

* -f
* +f
* --feature
* --enable-feature
* --disable-feature
* --no-feature
* --with-feature
* --without-feature

Given ``opt->accepts_arguments = true`` or ``opt->requires_arguments
= true`` option-arguments will be parsed in this manner:

* a positive prefix sets a positive ``opt->present``, a negative sets negative
* a negative option-argument inverts ``opt->present``

This behaviour is similar to GNU autoconf's parameter handling.

Examples:

* -f -> positive present
* +f -> negative present
* -f=no -> negative present
* +f=no -> positive present
* --feature -> positive present
* --enable-feature -> positive present
* --with-feature=disable -> negative present
* --without-feature=true -> negative present
* --disable-feature true -> negative present

A passend option-argument like "disable", "true", etc. pp. are still
saved as the first option-argument.

Argparse does not provide a way to link options as mutually exclusive or
to set the same value. It is encouraged to make use of the functionality
above, otherwise this logic has to be implemented separately.

Using the ``_help()`` functions it is possible to print help output
generated from the ``description``, ``short_description``, ``name`` members to
a stream.

.. code:: c

    #include <argparse.h>
    #include <stdio.h>

    int main(int argc, char **argv) {
        args *args = args_new();

        args_add_option(args, option_new("f", "feature", "description of feature"));
        args_help(args, stdout);

        args_free(args);
    }

Results in:

        -f --feature  description of feature


.. _`XBD Utility Conventions`: http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html
.. _`POSIX.1-2008` : http://pubs.opengroup.org/onlinepubs/9699919799/
