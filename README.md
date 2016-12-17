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
make all
```
After iash has finished building, `bin/` will contain all of the built files.
Built examples can be found in `bin/example/`, library files can be found
in `bin/lib/`, and individual object files can be found in `bin/obj/`.

# Running

To run the basic example, do the following from the project root:
```
export LD_LIBRARY_PATH=$PWD/bin/lib:$LD_LIBRARY_PATH
bin/example/basic
```
(For the moment, we dynamically link the example, since this is the only linking
option that appears to work. Examples will be statically linked in the future).

# Posting Issues

Before you post an issue to the issue tracker, please read the [rules](https://github.com/hal7df/iash/wiki/Issue-Tracker).

# Help

See the [docs](https://hal7df.github.io/iash/docs/html).
