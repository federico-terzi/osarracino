O(SARRACINO)

Installation

To install the required libraries on Debian:

    sudo apt install build-essential pkg-config libjsoncpp-dev cmake

Then to compile the program, first enter the project directory:

    cmake CMakeLists.txt -DCMAKE_BUILD_TYPE=Release
    make -j 4
    ln -s ./cli/osarracino ./osarracino

Using the player

To start the player, type:

    ./osarracino COLOR -t [TIMEOUT]

    # Examples:

    # Launch white player with a timeout of 60 seconds
    ./osarracino white -t 60

    # Launch black player with a timeout of 60 seconds
    ./osarracino black -t 60

To change the number of cores used by osarracino you can use the -j option:

    # Launch white player with a timeout of 60 seconds using 4 cores
    ./osarracino white -t 60 -j 4