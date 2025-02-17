#include "Huffman.h"

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
     * Function to build the Huffman tree based on character frequencies
     * 
     * @param freqMap: A map containing characters and their corresponding frequencies
     * 
     * @return: None
     */
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto &[ch, freq] : freqMap) {
        pq.push(new Node(ch, freq));
    }
    
    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        Node *merged = new Node('\0', left->freq + right->freq, left, right);
        pq.push(merged);
    }
    root = pq.top();
    generateCodes(root, "");
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
        huffmanCodes[node->ch] = code;
        reverseCodes[code] = node->ch;
    }
    
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::vector<char> Huffman::compress(const std::vector<char> &data) {
    /**
     * Function to compress a given set of data using Huffman encoding
     * 
     * @param data: The input data to be compressed
     * 
     * @return: A vector containing the compressed data
     */
    std::unordered_map<char, int> freqMap;
    for (char ch : data) {
        freqMap[ch]++;
    }
    buildTree(freqMap);
    
    std::string encodedStr;
    for (char ch : data) {
        encodedStr += huffmanCodes[ch];
    }
    
    std::vector<char> compressedData(encodedStr.begin(), encodedStr.end());
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