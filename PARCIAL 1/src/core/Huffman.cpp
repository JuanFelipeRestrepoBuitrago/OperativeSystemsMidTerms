#include "Huffman.h"

Huffman::Huffman() : root(nullptr) {}

Huffman::~Huffman() {
    deleteTree(root);
}

void Huffman::deleteTree(Node *node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void Huffman::buildTree(const std::unordered_map<char, int> &freqMap) {
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
    if (!node) return;
    if (!node->left && !node->right) {
        huffmanCodes[node->ch] = code;
        reverseCodes[code] = node->ch;
    }
    
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::vector<char> Huffman::compress(const std::vector<char> &data) {
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
    return reverseCodes;
}