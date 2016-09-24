additive and replacing option-arguments
---------------------------------------

.. code::

    command --arg=1 --arg=2

With additive should result in two option-arguments, ``1`` and ``2``.
Replacing should result in ``2``.

Additive should also drop a default value when parsing the first value.

Problems:

* prefix handling
    - additional flag for operands
    - separate struct for option-arguments

refactoring
-----------

Lots of refactoring to do, maybe using pseudo-generics and cpp to
generate explicit functions out of generic code.

rst instead of doxygen
----------------------

Would deduplicate the text from ``src/argparse.dox`` and the readme.
Also, man pages could be generated easier with rst.

parseable output
----------------

Parseable output to generate completions from.
Ideally this'd be available as an option for each level separately. This
way full output could be provided by traversing the tree and specific
output (e.g. the divider for option-arguments) could still be retrieved
only where needed.

split headers
-------------

The header files should be split into at least two logical chunks:
An easy interface which provides the ``_new()``, ``_parse()`` and
``_help()`` functions to a reasonable level.

An advanced interface which provides all functions.

More
----

More TODOs without explanations.

- optional compilation of examples
- getopt replacement
