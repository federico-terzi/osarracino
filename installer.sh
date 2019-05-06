#!/bin/bash



if test $(dpkg-query -l 'libjsoncpp-dev' > /dev/null)
then
	echo "DO: sudo apt install libjsoncpp-dev"
	exit 1
fi

if test $(dpkg-query -l 'cmake' > /dev/null)
then
	echo "DO: sudo apt install cmake"
	exit 1
fi

if test $(dpkg-query -l 'pkg-config' > /dev/null)
then
	echo "DO: sudo apt install pkg-config"
	exit 1
fi

echo "OK YOU HAVE ALL THE LIBRARIES"
cmake . -DCMAKE_BUILD_TYPE=Release
make -j 4
ln -s ./cli/osarracino ./osarracino
