#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <algorithm>
#include <chrono>
#include <iomanip>

class HuffmanNode {
public:
    char character;
    unsigned frequency;
    HuffmanNode *left, *right;

    HuffmanNode(char c, unsigned f) : character(c), frequency(f), left(nullptr), right(nullptr) {}

    bool operator>(const HuffmanNode& other) const {
        return frequency > other.frequency;
    }
};

class FileCompressor {
private:
    std::unordered_map<char, std::string> huffmanCodes;
    std::unordered_map<std::string, char> reverseHuffmanCodes;

    void buildHuffmanCodes(HuffmanNode* root, std::string code = "") {
        if (!root) return;
        if (root->character != '\0') {
            huffmanCodes[root->character] = code;
            reverseHuffmanCodes[code] = root->character;
        }
        buildHuffmanCodes(root->left, code + "0");
        buildHuffmanCodes(root->right, code + "1");
    }

    HuffmanNode* buildHuffmanTree(const std::unordered_map<char, unsigned>& charFreq) {
        std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, std::greater<HuffmanNode*>> minHeap;

        for (const auto& pair : charFreq) {
            minHeap.push(new HuffmanNode(pair.first, pair.second));
        }

        while (minHeap.size() > 1) {
            HuffmanNode* left = minHeap.top(); minHeap.pop();
            HuffmanNode* right = minHeap.top(); minHeap.pop();

            HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
            parent->left = left;
            parent->right = right;

            minHeap.push(parent);
        }

        return minHeap.top();
    }

    std::unordered_map<char, unsigned> countCharFrequencies(const std::string& filename) {
        std::unordered_map<char, unsigned> charFreq;
        std::ifstream file(filename, std::ios::binary);
        char c;
        while (file.get(c)) {
            charFreq[c]++;
        }
        return charFreq;
    }

public:
    void compress(const std::string& inputFilename, const std::string& outputFilename) {
        auto startTime = std::chrono::high_resolution_clock::now();

        std::unordered_map<char, unsigned> charFreq = countCharFrequencies(inputFilename);
        HuffmanNode* root = buildHuffmanTree(charFreq);
        buildHuffmanCodes(root);

        std::ifstream inputFile(inputFilename, std::ios::binary);
        std::ofstream outputFile(outputFilename, std::ios::binary);

        // Write Huffman codes to the output file
        outputFile << huffmanCodes.size() << '\n';
        for (const auto& pair : huffmanCodes) {
            outputFile << pair.first << ' ' << pair.second << '\n';
        }

        // Compress and write data
        char c;
        std::string buffer;
        while (inputFile.get(c)) {
            buffer += huffmanCodes[c];
            while (buffer.length() >= 8) {
                char byte = static_cast<char>(std::bitset<8>(buffer.substr(0, 8)).to_ulong());
                outputFile.put(byte);
                buffer = buffer.substr(8);
            }
        }

        // Write remaining bits
        if (!buffer.empty()) {
            buffer.append(8 - buffer.length(), '0');
            char byte = static_cast<char>(std::bitset<8>(buffer).to_ulong());
            outputFile.put(byte);
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << "Compression completed in " << duration.count() << " ms." << std::endl;

        // Calculate and display compression ratio
        std::ifstream originalFile(inputFilename, std::ios::binary | std::ios::ate);
        std::ifstream compressedFile(outputFilename, std::ios::binary | std::ios::ate);
        double originalSize = originalFile.tellg();
        double compressedSize = compressedFile.tellg();
        double compressionRatio = (1 - compressedSize / originalSize) * 100;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Original size: " << originalSize << " bytes" << std::endl;
        std::cout << "Compressed size: " << compressedSize << " bytes" << std::endl;
        std::cout << "Compression ratio: " << compressionRatio << "%" << std::endl;
    }

    void decompress(const std::string& inputFilename, const std::string& outputFilename) {
        auto startTime = std::chrono::high_resolution_clock::now();

        std::ifstream inputFile(inputFilename, std::ios::binary);
        std::ofstream outputFile(outputFilename, std::ios::binary);

        // Read Huffman codes
        size_t codeCount;
        inputFile >> codeCount;
        inputFile.ignore(); // Consume newline

        for (size_t i = 0; i < codeCount; ++i) {
            char c;
            std::string code;
            inputFile.get(c);
            inputFile >> code;
            inputFile.ignore(); // Consume newline
            reverseHuffmanCodes[code] = c;
        }

        // Decompress data
        std::string currentCode;
        char byte;
        while (inputFile.get(byte)) {
            std::bitset<8> bits(byte);
            for (int i = 7; i >= 0; --i) {
                currentCode += (bits[i] ? '1' : '0');
                if (reverseHuffmanCodes.find(currentCode) != reverseHuffmanCodes.end()) {
                    outputFile.put(reverseHuffmanCodes[currentCode]);
                    currentCode.clear();
                }
            }
        }

        auto endTime = std://chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << "Decompression completed in " << duration.count() << " ms." << std::endl;
    }
};

int main() {
    FileCompressor compressor;
    std::string inputFilename, outputFilename;
    char choice;

    std::cout << "File Compression Utility" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "1. Compress" << std::endl;
    std::cout << "2. Decompress" << std::endl;
    std::cout << "Enter your choice (1/2): ";
    std::cin >> choice;

    std::cout << "Enter input filename: ";
    std::cin >> inputFilename;
    std::cout << "Enter output filename: ";
    std::cin >> outputFilename;

    try {
        if (choice == '1') {
            compressor.compress(inputFilename, outputFilename);
        } else if (choice == '2') {
            compressor.decompress(inputFilename, outputFilename);
        } else {
            std::cout << "Invalid choice. Please run the program again." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}