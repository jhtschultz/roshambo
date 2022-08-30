# bot classes

These C++ classes wrap up the original C code to avoid global variables, so that multiple threads can allocate their own bot instances. They also fix a couple uninitialised memory accesses and bots that do not entirely reset their state at the beginning of a new match. As a result, not all bots have identical behaviour to the original C code: halbot, inocencio and shofar in particular have statistically different behaviour from the original implementation.
