# O(sarracino)

Installation instructions

```
sudo apt install -y pkg-config
sudo apt install libjsoncpp-dev
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
