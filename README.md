Rover Lang
==========

Language created for the [2022 Unofficial Lang Jam by matan1905](https://github.com/matan1905/unofficial-lang-jam).

The theme of the lang jam was: it's not a bug, it's a feature.

About the language
------------------

**Disclaimer:** while this is a working language, it is not meant to be taken
seriously. It's a bad idea to use it for any serious application, and this is
by design.

We created a language that can be used for programming Mars rovers (or other
rovers), and out of the box it handles some of the common problems you could
find in such a low-memory environment that needs to deal with accurate measurements.
This way, you don't have to worry about these issues, they are handled for you!

### Metric-imperial conversions

We know that converting between metric and imperial measurement systems might
be problematic. For that reason, any comparison assumes that the left-hand side
is in imperial, and the right-hand side in metric. Left-hand side is automatically
multiplied by 1.6, so that you don't have to worry about conversions!

### Integers

While we suggest using floating-point numbers for most calculations (they
come with the measurement conversions built in!), we are aware that sometimes
you might need integers, for example to store date. However, in order to lower
the memory usage, we thought: what is the largest integer that you might need?
We came to a conclusion that the highest integers would be used in date 
calculations, so we limited the value of an integer to 99. After that, it rolls
back over to 0, making those date calculations so much easier!

### User-defined functions

We know stack overflows can be a problem in systems where memory is limited,
such as a Mars rover. For that reason, we do not allow the programmers to
define their own functions. No recursive functions, no risk of a stack overflow!

Syntax
------

Ok, so how does the code actually look like? The language itself is a simple
imperative language, with C-like syntax. You can have a look at the example
programs in [`test_code/`](test_code/). The language supports:

* printing stuff using the built-in `printf` function
* variables and constants
* conditionals and loops
* arrays, with useful built-in functions: `length`, `push` and `pop`

Please note, that:

* a variable/constant definition always requires an initial value to be provided
* strings can only be used for printing stuff, they cannot be stored 
in variables or otherwise interacted with (as ASCII characters will not fit
into Rover integers)
* no breaks, continues or any other type of early return is available

Building and running the interpreter
------------------------------------

The interpreter is written using C++17, and you can build it easily using 
CMake (>= 3.14). We have verified that GCC 9.3.0 and clang 11 successfully
build the project, we cannot guarantee that an earlier version of either of
these will work.

If you don't know how to use CMake, you can do the following in the project
directory:

```
mkdir build
cd build
cmake ..
cd -
cmake --build build
```

After that an executable will be present under `build/rover`. You can run
a program by providing a path to the source code as a parameter, e.g.:

```
rover test_code/simple.🚲
```

**Note:** we are using a bicycle emoji as the file extension (because the Polish
word for bicycle is the same as the English word for rover). We have not 
encountered any issues using this extension. However, the interpreter itself
does not check for the extension, so if you do run into any issues, you can
change it to whatever you want (we recommend `.rvr`).

Acknowledgements
----------------

See contributor list for project authors. Note that the actual contributions
might not match the commit authors, since we did some pair/group coding.
