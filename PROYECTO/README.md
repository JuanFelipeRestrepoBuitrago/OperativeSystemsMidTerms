# Compression and Decompression of files using RSA and Huffman
For the final project, we will implement a program that compresses and decompresses files using the **RSA** algorithm and **Huffman** encoding. The program will be implemented in **C++**.

## Background

### RSA Algorithm
The **RSA (Rivest-Shamir-Adleman)** algorithm is an **asymmetric cryptographic system** used for secure data transmission. It is based on the mathematical difficulty of **factoring large prime numbers**. RSA uses a pair of keys:
- **Public Key (`e, n`)** → Used for encryption.
- **Private Key (`d, n`)** → Used for decryption.

#### **How RSA Works**
1. Choose two large prime numbers, `p` and `q`.
2. Compute `n = p * q` (this forms part of the public key).
3. Compute **Euler’s Totient Function**: `ϕ(n) = (p - 1) * (q - 1)`.
4. Select an integer `e` such that `1 < e < ϕ(n)`, and `gcd(e, ϕ(n)) = 1`.
5. Compute `d` as the modular inverse of `e` modulo `ϕ(n)`, satisfying:  
    ```
    (e * d) ≡ 1 (mod ϕ(n))
    ```
6. **Encryption:** A plaintext message `M` is encrypted using the public key `(e, n)`:
    ```
    C = M^e mod n
    ```
7. **Decryption:** The ciphertext `C` is decrypted using the private key `(d, n)`:
    ```
    M = C^d mod n
    ```
RSA provides **strong security** but generates large ciphertexts, making **compression** useful before or after encryption.

### Huffman Encoding
The Huffman algorithm is a lossless data compression method based on character frequency in a file.
#### ⚙️ **How Huffman Works**
1. The frequency of each character in the file is calculated.
2. A Huffman tree is built, where the least frequent characters have longer codes, and the most frequent ones have shorter codes.
3. A unique binary code is generated for each character based on the tree.
4. The original file is compress by replacing each character with its Huffman code.

The Huffman tree is stored also to allow later decompress.

## 📂 **FileManager: Handling File Operations**
The **FileManager** module is responsible for managing system-level file operations, including reading and writing files securely. It uses **low-level system calls (`open`, `read`, `write`, `close`)** to handle files efficiently.

### ⚙️ **How FileManager Works**
- **Reading a File:**
- Opens the file in read-only mode (`O_RDONLY`).
- Reads data in chunks (4096 bytes at a time).
- Stores the file content in a `std::vector<char>`.

- **Writing to a File:**
- Opens the file in write mode (`O_WRONLY | O_CREAT | O_TRUNC`).
- Writes the provided data to the file.
- Uses `0644` permissions, allowing the owner to read/write and others to read.

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
- `testHuffman`: Runs the tests for the Huffman file.

## How to run

To run the project, you have to follow the next steps:

1. Compile the project.

2. Run one of the following commands:
    ```bash
    ./out/perzip --help
    ```
    ```bash
    ./out/perzip -h
    ```

Or you can run the following command if you are in the out directory:
```bash
./perzip --help
```

### Regex Argument for Decompression

- The regex decompression argument is used to specify the file or files which you want to decompress from the compressed file. The regex matches the file names in the compressed file, you can list them with `./perzip -s <compressed_file>`. Also, if you insert `./` symbol at the beginning of the regex, it will match the files in the current directory (The root directory of the compressed file). Furthermore, if you insert the `/*` symbol at the end of the regex, it will match all the files inside the folder selected.

For example: 
- `./perzip -d <compressed_file> <out_directory> "./folder/*"` will decompress all the files in the `folder` directory.
- `./perzip -d <compressed_file> <out_directory> "folder/*"` will decompress all the files in the `folder` directory in the root directory of the compressed file.
- `./perzip -d <compressed_file> <out_directory> "./folder"` will decompress the folder `folder` and all its contents.
- `./perzip -d <compressed_file> <out_directory> "./folder/file.txt"` will decompress the file `file.txt` in the `folder` directory.
- `./perzip -d <compressed_file> <out_directory> "file.txt"` will decompress the file `file.txt` in the root directory of the compressed file.
- `./perzip -d <compressed_file> <out_directory>` will decompress all the files in the compressed file.`
