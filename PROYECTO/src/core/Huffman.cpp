#include "Huffman.h"
#include <omp.h>
#include <sstream>
#include <string>
#include <chrono>

Huffman::Huffman() : root(nullptr) {
    /**
     * Constructor for the Huffman class
     * 
     * @return: None
     */
}

Huffman::~Huffman() {
    /**
     * Destructor for the Huffman class
     * 
     * @return: None
     */
    deleteTree(root);
}

void Huffman::deleteTree(Node *node) {
    /**
     * Function to delete the Huffman tree recursively
     * 
     * @param node: The root node of the tree to delete
     * 
     * @return: None
     */
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void Huffman::buildTree(const std::unordered_map<char, int> &freqMap) {
    /**
     * Function to build the Huffman tree based on character frequencies, using OpenMP
     * to parallelize the node creation phase.
     * 
     * @param freqMap: A map containing characters and their corresponding frequencies
     */
    
     // Vector to hold nodes
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Node*> nodes(freqMap.size());

    // Parallelize the node creation
    std::vector<std::pair<char, int>> entries(freqMap.begin(), freqMap.end());

    int numThreads = omp_get_max_threads();
    printf("\033[1;36m🔵 [OpenMP (Huffman)] Threads used for building tree: %d\033[0m\n", numThreads);


    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(entries.size()); ++i) {
        const auto& [ch, freq] = entries[i];
        nodes[i] = new Node(ch, freq);
    }

    // Create a priority queue to hold the nodes
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto node : nodes) {
        pq.push(node);
    }

    // Construir el árbol de Huffman (parte secuencial)
    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        Node *merged = new Node('\0', left->freq + right->freq, left, right);
        pq.push(merged);
    }

    root = pq.top();
    #pragma omp parallel
    {
        #pragma omp single
        generateCodes(root, "");
    }
    auto end = std::chrono::high_resolution_clock::now();

    printf("\033[1;32m🟢 [Timing] Building tree and generating codes time: %lld ms\033[0m\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

}

void Huffman::generateCodes(Node *node, const std::string &code) {
    /**
     * Function to generate Huffman codes from the tree
     * 
     * @param node: The current node in the Huffman tree
     * @param code: The Huffman code corresponding to the node
     * 
     * @return: None
     */
    if (!node) return;
    if (!node->left && !node->right) {
        #pragma omp critical
        {
            huffmanCodes[node->ch] = code;
            reverseCodes[code] = node->ch;
        }
        return;
    }
    #pragma omp task shared(huffmanCodes, reverseCodes)
    generateCodes(node->left, code + "0");

    #pragma omp task shared(huffmanCodes, reverseCodes)
    generateCodes(node->right, code + "1");

    #pragma omp taskwait
}

std::vector<char> Huffman::compress(const std::vector<char> &data) {
    /**
     * Function to compress a given set of data using Huffman encoding
     * 
     * @param data: The input data to be compressed
     * 
     * @return: A vector containing the compressed data
     */

    auto start = std::chrono::high_resolution_clock::now();
    int numThreads = omp_get_max_threads();
    printf("\033[1;36m🔵 [OpenMP (Huffman)] Threads used for compress: %d\033[0m\n", numThreads);

    std::vector<std::string> partialEncoded(numThreads);
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        std::stringstream ss;
        #pragma omp for
        for (int i = 0; i < static_cast<int>(data.size()); ++i) {
            ss << huffmanCodes[data[i]];
        }
        partialEncoded[tid] = ss.str();
    }

    // Concatenate the partial encoded strings
    std::string encodedStr;
    for (const auto& part : partialEncoded) {
        encodedStr += part;
    }

    // Convert the encoded string to a vector of chars
    std::vector<char> compressedData(encodedStr.begin(), encodedStr.end());

    auto end = std::chrono::high_resolution_clock::now();
    printf("\033[1;32m🟢 [Timing] Compress time: %lld ms\033[0m\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    return compressedData;
}

std::vector<char> Huffman::uncompress(const std::vector<char> &data, 
    const std::unordered_map<std::string, char>* externalReverseCodes) {
    /**
     * Function to decompress Huffman encoded data
     * 
     * @param data: The compressed input data
     * @param externalReverseCodes: A pointer to an external Huffman table, if provided
     * 
     * @return: A vector containing the decompressed data
     */
    auto start = std::chrono::high_resolution_clock::now();
    const std::unordered_map<std::string, char>& codesToUse = 
        (externalReverseCodes) ? *externalReverseCodes : reverseCodes;

    std::string encodedStr(data.begin(), data.end());
    std::string decodedStr;
    std::string currentCode;

    for (char bit : encodedStr) {
        currentCode += bit;
        if (codesToUse.count(currentCode)) {
            decodedStr += codesToUse.at(currentCode);
            currentCode.clear();
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    printf("\033[1;32m🟢 [Timing] Uncompress time: %lld ms\033[0m\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    return std::vector<char>(decodedStr.begin(), decodedStr.end());
}

std::unordered_map<std::string, char> Huffman::getReverseCodes() {
    /**
     * Function to retrieve the reverse Huffman table
     * 
     * @return: A map containing Huffman codes and their corresponding characters
     */
    return reverseCodes;
}