# PyFunction_GetAnnotations Test

There seems to be a change in Python 3.10's `PyFunction_GetAnnotations` C API
return value compared to Python 3.9. This repo holds a small program
that can be used to demonstrate the change between versions easily.

# Test Usage

1. Ensure you have both python3.10 and python3.9 installed and on your `$PATH`
3. `make pythonVersion="python3.9"`
4. `./out`
5. `make clean`
6. `make pythonVersion="python3.10"`
7. `./out`

If you happen to have a working [Nix][nix] install you can run `nix-shell` to
have an environment created with both `python3.10` and `python3.9` ready to run
the above steps.

[nix]: https://nixos.org/download.html#nix-quick-install

# Example Output

## Demonstration

```bash
[nix-shell:~/Code/C/pyfunction_getannotations_test]$ make clean
rm -f *~ *.o main

[nix-shell:~/Code/C/pyfunction_getannotations_test]$ make
gcc -Wall -O0 -g -I/nix/store/jizav0w3q9gfszi3g059pm3crh4awnha-python3-3.10.0/include/python3.10 -I/nix/store/jizav0w3q9gfszi3g059pm3crh4awnha-python3-3.10.0/include/python3.10  -Wno-unused-result -Wsign-compare  -DNDEBUG -g -fwrapv -O3 -Wall   -c -o main.o main.c
gcc -Wall -O0 -g -I/nix/store/jizav0w3q9gfszi3g059pm3crh4awnha-python3-3.10.0/include/python3.10 -I/nix/store/jizav0w3q9gfszi3g059pm3crh4awnha-python3-3.10.0/include/python3.10  -Wno-unused-result -Wsign-compare  -DNDEBUG -g -fwrapv -O3 -Wall main.o -o main -lm  -L/nix/store/jizav0w3q9gfszi3g059pm3crh4awnha-python3-3.10.0/lib  -lpthread -ldl -lcrypt -lncurses -lutil -lm -lm  -lpython3.10

[nix-shell:~/Code/C/pyfunction_getannotations_test]$ ./main
PyFunction_GetAnnotations returned PyTuple

[nix-shell:~/Code/C/pyfunction_getannotations_test]$ make clean
rm -f *~ *.o main

[nix-shell:~/Code/C/pyfunction_getannotations_test]$ make pythonVersion="python3.9"
gcc -Wall -O0 -g -I/nix/store/wl02plhc6zf84m6x9984l42wnnnbly5m-python3-3.9.6/include/python3.9 -I/nix/store/wl02plhc6zf84m6x9984l42wnnnbly5m-python3-3.9.6/include/python3.9  -Wno-unused-result -Wsign-compare  -DNDEBUG -g -fwrapv -O3 -Wall   -c -o main.o main.c
gcc -Wall -O0 -g -I/nix/store/wl02plhc6zf84m6x9984l42wnnnbly5m-python3-3.9.6/include/python3.9 -I/nix/store/wl02plhc6zf84m6x9984l42wnnnbly5m-python3-3.9.6/include/python3.9  -Wno-unused-result -Wsign-compare  -DNDEBUG -g -fwrapv -O3 -Wall main.o -o main -lm  -L/nix/store/wl02plhc6zf84m6x9984l42wnnnbly5m-python3-3.9.6/lib  -lpthread -ldl -lcrypt -lncurses -lutil -lm -lm  -lpython3.9

[nix-shell:~/Code/C/pyfunction_getannotations_test]$ ./main
PyFunction_GetAnnotations returned PyDict
```

## Analysis

With the [same C code](main.c) and the [same Python program](test.py) we see two
different results from `PyFunction_GetAnnotations` depending on whether the
program is linked with Python 3.10 or Python 3.9.

In the Python 3.10 case (presented first in the output) the
`PyFunction_GetAnnotations` invocation returns a `PyObject*` that passes
`PyTuple_Check` but not `PyDict_Check`.

In the Python 3.9 case (presented second in the output) the same invocation
produces a `PyObject*` that passes `PyDict_Check` as expected.

The [documentation for `PyFunction_GetAnnotations`][docs] for 3.10 says:
> Return the annotations of the function object op. This can be a mutable
> dictionary or NULL

This seems to indicate it should always be a dictionary or `NULL` and a tuple is
neither of these things. Is it a documentation bug? User error? A regression?

[docs]: https://docs.python.org/3/c-api/function.html#c.PyFunction_GetAnnotationskj

## Root Cause Speculation

The [Python 3.10 "What's New"][whatsnew] announcement mentions an optimization
that may be the cause of this change:
> When using stringized annotations, annotations dicts for functions are no
> longer created when the function is created. Instead, they are stored as
> a tuple of strings, and the function object lazily converts this into the
> annotations dict on demand. This optimization cuts the CPU time needed to
> define an annotated function by half.

See also, [Python issue42202][issue44202].

[issue44202]: https://bugs.python.org/issue42202
[whatsnew]: https://github.com/python/cpython/blob/main/Doc/whatsnew/3.10.rst
