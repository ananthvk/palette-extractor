# palette-extractor
A C++ application to extract dominant colors from an image to create a palette or a theme.

## How to use?

## Building
Install Meson, and a backend (such as Ninja) and run the following commands:

If you have clang, along with sanitizers installed
```
$ CXX=clang++ meson setup -Db_sanitize=address -Ddevelopment=true -Db_lundef=false --reconfigure builddir
```
Otherwise
```
$ meson setup --reconfigure builddir
```

```
$ cd builddir
$ meson  -v
```