# Pathfinding Visualiser

Pathfinding algorithm visualisation using C++ and SFML.

So far it can show BFS, Greedy BFS, Dijkstra's, and A*.

![Peek 2021-12-27 10-22](https://user-images.githubusercontent.com/13512660/147462469-f9c44f81-9b72-4545-a0cd-e0ad4dd229a3.gif)


### Windows (Visual Studio)

The easiest way to build is to use [vcpkg](https://vcpkg.io/en/index.html) and install SFML through this.

```bash
vcpkg install sfml
vcpkg integrate install
```

Create a new visual studio C++ empty project, git clone the files, and copy them into the project directory.

Select the "Show All Files" options in Solution Explorer, and right-click on the src/ and deps/ directory, and choose the "include in project options"

Go into the project properies and under `C/C++ > General`, add the deps/ directory as an additional include directy.


Finally, under `Linker > Input`, add OpenGL32.lib as an additional dependancy. 

### Linux

Requires conan and cmake.

```sh
python3 -m pip install conan
```

To build, at the root of the project:

```sh
sh scripts/build.sh install
```

The `install` argument is only needed for the first time compilation as this is what grabs the libraries from Conan, future builds can omit the `install` arg.

To run, at the root of the project:

```sh
sh scripts/run.sh
```

To build and run in release mode, simply add the `release` suffix:

```sh
sh scripts/build.sh release
sh scripts/run.sh release
```

[]: https://www.sfml-dev.org/download/sfml/2.5.1/
