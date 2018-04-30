# iash

Internal Application SHell -- a faux bash shell within an application.

# Download

_iash does not currently have a system package build._

See the [Releases](https://github.com/hal7df/iash/releases) page.

# Building

iash is presently only known to build on Linux (Mac support coming soon,
it could possibly build on Windows, but this is not known).
From the root of the project directory, run:

```
make
```
This will build iash as a static library, located in bin/libiash.a. To build
a dynamic library instead, run:
```
make dynamic
```
After iash has finished building, you will find either libiash.a or libiash.so
under `bin/`.

## Building examples

To build all examples and link them statically, run:
```
make static-example
```
Similarly, to build all examples and link them dynamically, run:
```
make dynamic-example
```
After the examples have finished building, you can find them in `bin/example`,
with the suffix `.static` or `.dynamic` depending on what linking method you
chose.

# Running

To run any example, you can just call the executable in the `bin/example`
directory:
```
bin/example/basic.static
```
If you linked the examples dynamically, you will have to set the 
`LD_LIBRARY_PATH` environment variable before running the executable:
```
export LD_LIBRARY_PATH=$PWD/bin/lib:$LD_LIBRARY_PATH
bin/example/basic.dynamic
```

# Posting Issues

Before you post an issue to the issue tracker, please read the [rules](https://github.com/hal7df/iash/wiki/Issue-Tracker).

# Help

See the [docs](https://hal7df.github.io/iash/docs/html).
