# RSA Encryption with Run-Length Encoding (RLE)
For the exam of operating systems subject we created a system function that uses system calls to encrypt files using RSA and compress them using RLE.

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

### Run-Length Encoding (RLE)
**Run-Length Encoding (RLE)** is a simple **lossless compression algorithm** that replaces sequences of repeated characters with a count followed by the character itself.

#### ⚙️ **How RLE Works**
- **Example:**
    ```
    Input: "AAABBBCCDAA" Output: "3A3B2C1D2A"
    ```
- RLE is **efficient for data with repeated patterns**, but **not effective for highly random data**.

## **How RLE is Used in RSA**
Since RSA produces **long ciphertexts**, we use **RLE compression** to optimize the encrypted file size. The workflow is as follows:

1. **Encryption Phase:**
 - Convert plaintext to a numerical representation.
 - Encrypt the message using **RSA**.
 - Apply **RLE compression** to reduce the ciphertext size.
 - Store the compressed ciphertext.

2. **Decryption Phase:**
 - Retrieve the compressed ciphertext.
 - **Decompress** it using **RLE**.
 - Decrypt the data using **RSA** to obtain the original plaintext.

This integration **optimizes storage** and **reduces transmission overhead** without compromising security.

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
- `testRLE`: Runs the tests for the RLE file.

## How to run

To run the project, you have to follow the next steps:

1. Compile the project.

2. Run one of the following commands:
    ```bash
    ./out/rsa --help
    ```
    ```bash
    ./out/rsa -h
    ```

Or you can run the following command if you are in the out directory:
```bash
./rsa --help
```