# Mid-Term 2: Buddy System Allocator, Image Rotation and Scaling
For the exam of operating systems subject we created a system function that uses system calls to rotate and scale images with or without buddy system allocator. We give the user the general information about the program execution

## Background

### Buddy System Allocator
The **Buddy System** is a **memory allocation technique** that efficiently manages dynamic memory by splitting blocks into power-of-two sizes. It provides fast allocation and deallocation while minimizing fragmentation.

#### **How the Buddy System Works**
1. **Initialization:** The allocator starts with a single large block of memory.
2. **Allocation:**
   - If a requested size is not a power of two, round it up.
   - Find the smallest available block that can accommodate the request.
   - If the block is larger than needed, split it into two equal "buddies."
3. **Deallocation:**
   - Freed blocks attempt to merge with their adjacent buddy if it is also free.
   - Merging continues recursively up to the largest block size.
4. **Addressing & Buddy Computation:**
   - Each block is indexed in a power-of-two system.
   - A buddy of block `B` at address `A` with size `S` is calculated as:
     ``` 
     Buddy(A) = A ⊕ S
     ```
   - The XOR operation (`⊕`) ensures correct buddy pairing.

#### **Advantages of the Buddy System**
- **Efficient splitting & merging** reduces fragmentation.
- **Fast allocation and deallocation** due to predictable memory patterns.
- **Simple implementation** with minimal overhead.

The **Buddy System** is widely used in **operating systems** and **real-time applications** where memory management performance is critical.


### Image Rotation & Scaling
Image rotation and scaling are common operations in image processing that transform images for various purposes, such as display, printing, or analysis.

## Requirements

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
- `testHuffman`: Runs the tests for the Huffman file.

## How to run

To run the project, you have to follow the next steps:

1. Compile the project.

2. Run one of the following commands:
    ```bash
    ./out/program --help
    ```
    ```bash
    ./out/program -h
    ```

Or you can run the following command if you are in the out directory:
```bash
./program --help
```
```bash
./program -h
```
