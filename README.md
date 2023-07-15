# hexamine
hexamine is a graphical viewer for binary files using [SFML](https://github.com/SFML/SFML) and [Dear ImGui](https://github.com/ocornut/imgui)\
SFML backend support for ImGui is archieved with [ImGui SFML](https://github.com/SFML/imgui-sfml). The cmake files were taken from this [ImGui Fetchcontent example](https://github.com/eliasdaler/imgui-sfml-fetchcontent)

## How to use
Run the `hexamine.exe` from your command line and pass the path to your binary file
```sh
.\hexamine.exe <path-to-bin>
```

## How to build
You can add and build your own changes. Thanks to the the cmake dependencies everything is downloaded automatically. \
Simply run the following commands to build:

```sh
mkdir build
cd build
cmake <path-to-cloned-repo> -D LINK_DEPS_STATIC=ON
cmake --build .
.\src\Debug\hexamine.exe
```
