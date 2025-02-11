# First Exam

## Requierements

### Google Test
This is a library that allows you to create and run tests for C++ programs. It is a part of the Google C++ Testing Framework.

To install Google Test, you can use the following commands:
```bash
sudo apt update && sudo apt install libgtest-dev
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp lib/*.a /usr/lib
```

### CMake

To install CMake, you can use the following commands:
```bash
sudo apt update && sudo apt install cmake
```

### G++

To install G++, you can use the following commands:
```bash
sudo apt update && sudo apt install g++
```

### Make

To install Make, you can use the following commands:
```bash
sudo apt update && sudo apt install make
```

## How to compile

To compile the project, you can use the following commands:
```bash
make $OPTION
```

Where `$OPTION` can be:
- `all` or ``: Compiles the project.
- `clean`: Removes the build directory.
- `run`: Runs the project.
- `test`: Runs the tests.
- `testUtils`: Runs the tests for the Utils file.
