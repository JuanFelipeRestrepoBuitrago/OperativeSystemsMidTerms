# First Exam

## Requierements


### Open SSL

To install Open SSL, you can use the following commands:
```bash
sudo apt update && sudo apt install libssl-dev
```

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
- `all`, `compile` or ``: Compiles the project. In a new out file called `out/rsa`.
- `clean`: Removes the build directory.
- `test`: Runs the tests.
- `testUtils`: Runs the tests for the Utils file.
- `testRSA`: Runs the tests for the RSA file.

## How to run

To run the project, you have to follow the next steps:

1. Compile the project.

2. Run the following command:
```bash
./out/rsa --help
```

Or you can run the following command if you are in the out directory:
```bash
./rsa --help
```
