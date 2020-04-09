# wmbusmeters-lib-example

First off install wmbusmeters with cmake in a temporary directory:

```
git clone git@github.com:WattSense/wmbusmeters.git
cd wmbusmeters
git checkout cmake_build
mkdir build
cd build
cmake ..
DESTDIR=/tmp/wmbusmeters_install make install
```

Then get this repository and build the example:

```
mkdir build
cd build
cmake ..
PKG_CONFIG_LIBDIR=/tmp/wmbusmeters_install/usr/local/lib/pkgconfig PKG_CONFIG_SYSROOT_DIR=/tmp/wmbusmeters_install cmake ..
make
```

Finally run it:

```
LD_LIBRARY_PATH=/tmp/wmbusmeters_install/usr/local/lib ./main 2544460637178014051b72371780144606051b900010002f2f02653e084265000002fd170200
```
