#!/bin/bash
if [ ! -d build ];then mkdir build;fi
cd build
cmake -G"Eclipse CDT4 - Unix Makefiles" -DECLIPSE_CDT4_GENERATE_SOURCE_PROJECT=TRUE ..
cp .project ..
cp .cproject ..
make
cp mU ..
cp *.dll ../Package/
cp *.so ../Package/