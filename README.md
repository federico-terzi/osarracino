# O(sarracino)

O(sarracino) is a Tablut Engine written in C++ with a strong focus on performance. 
It was written by Federico Terzi and Massimo Schembri for the 2019 [yearly challenge](http://ai.unibo.it/games/boardgamecompetition/1819) 
hosted by Prof. Mello for the Fundamentals of AI course.

O(sarracino) got the 2nd place out of 22 participants.

### Play online

While the original version required a dedicated server to be played, a lighter version has been ported to the web using WebAssembly.

[You can play online here](https://federicoterzi.com/webTablut/).

### Performance

In order to achive the best performance, O(sarracino) took advantage of:

* Multi-threading
* Bit operations
* CPU intrinsics (native processor instructions)

The engine is capable to evaluate ( on my Intel i5-8400 machine ) about 35 million possibile moves each second.

### Installation

To install the required libraries on Debian:

```
sudo apt install build-essential pkg-config libjsoncpp-dev cmake
```

Then to compile the program, first enter the project directory:

```
cmake CMakeLists.txt -DCMAKE_BUILD_TYPE=Release
make -j 4
ln -s ./cli/osarracino ./osarracino
```

### Using the player

To start the player, type:

```
./osarracino COLOR -t [TIMEOUT]

# Examples:

# Launch white player with a timeout of 60 seconds
./osarracino white -t 60

# Launch black player with a timeout of 60 seconds
./osarracino black -t 60

```

To change the number of cores used by osarracino you can use the -j option:

```
# Launch white player with a timeout of 60 seconds using 4 cores
./osarracino white -t 60 -j 4
```
