# O(sarracino)

Installation instructions

```
sudo apt install -y pkg-config
sudo apt install libjsoncpp-dev

If there are some errors with jsoncpp do:
sudo ln -s /usr/lib/x86_64-linux-gnu/your_jsoncpp.so.xx /usr/lib/x86_64-linux-gnu/required_jsoncpp.so.xx
```

Install gtest for testing

```
sudo apt-get install libgtest-dev
sudo apt-get install cmake # install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
 
# copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
sudo cp *.a /usr/lib
```

Install pyGame for the Editor

```
python3 -m pip install -U pygame
```

Note that pip must be installed.
