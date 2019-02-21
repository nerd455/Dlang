# Dlang

## Current feature set

* positive integers (no other types yet)
* integer constants
* addition (`foo + bar`)
* subtraction (`foo - bar` binary only)
* multiplication (`foo * bar`)
* comments (`// foo` and `/* foo */`)
* return statements
* local variables (`int` only, function scope only, must be
  initialised)
* variable assignment (`int` only)

## Usage

You will need `clang`, `lex` and `yacc` installed.

Compiling Dlang:

    # Compile the compiler.
    $ make

Usage:

    # To produce an assembly file.
    $ dlang test/foo.c
    # To assemble and link.
    $ ./link
