#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

class Huffman {
private:
    struct Node {
        char ch;
        int freq;
        Node *left, *right;
        Node(char c, int f, Node *l = nullptr, Node *r = nullptr) : ch(c), freq(f), left(l), right(r) {}
    };

    struct Compare {
        bool operator()(Node *l, Node *r) {
            return l->freq > r->freq;
        }
    };
    std::unordered_map<char, std::string> huffmanCodes;
    std::unordered_map<std::string, char> reverseCodes;
    Node *root;
public:

    void buildTree(const std::unordered_map<char, int> &freqMap);
    void generateCodes(Node *node, const std::string &code);
    void deleteTree(Node *node);
    Huffman();
    ~Huffman();
    std::vector<char> compress(const std::vector<char> &data);
    std::vector<char> uncompress(const std::vector<char> &data);
};

#endif
