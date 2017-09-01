# Scenegraph Demo

A bare-bones scenegraph demo using OpenGL.

## Building and Running

You must have `meson` and `ninja` installed to build the project unlike the
traditional `make`. In addition the dependencies `glew`, `sdl2`, and `boost`
should be installed. Your C++ compiler should support also C++14.

```sh
$ meson builddir
$ cd builddir
$ ninja
```

To run you need to set an environment variable to tell the program where the
graphical assets are located before running.

```sh
$ export SCENEGRAPHDEMO_RESOURCE_DIR=$YOUR_PROJECT_DIR/resources
```

After that's done you can run it like any other program.

```sh
$ ./scenegraph-demo
```

## License

[![CC0](https://i.creativecommons.org/p/zero/1.0/88x31.png)](https://creativecommons.org/publicdomain/zero/1.0/)

To the extent possible under law, [Walter Kuppens](https://walterkuppens.com)
has waived all copyright and related or neighboring rights to this work.
