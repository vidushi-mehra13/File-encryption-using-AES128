#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BLOCK_SIZE = 16; // 128 bits

// Dummy AES: XOR encryption
void xorBlock(uint8_t* block, const uint8_t* key) {
    for (int i = 0; i < BLOCK_SIZE; ++i)
        block[i] ^= key[i];
}

void encryptBlock(uint8_t* block, const uint8_t* key) {
    xorBlock(block, key);
}

void decryptBlock(uint8_t* block, const uint8_t* key) {
    xorBlock(block, key);
}

// Print a block in hex
void printHexBlock(const uint8_t* block) {
    for (int i = 0; i < BLOCK_SIZE; ++i)
        cout << hex << setw(2) << setfill('0') << (int)block[i] << " ";
    cout << dec << endl;
}

void padBlock(vector<uint8_t>& block) {
    while (block.size() < BLOCK_SIZE)
        block.push_back(0);
}

int main() {
    string inputFilename, outputFilename;
    cout << "Enter input text file: ";
    cin >> inputFilename;

    ifstream inFile(inputFilename, ios::binary);
    if (!inFile) {
        cerr << "❌ Failed to open input file!" << endl;
        return 1;
    }

    vector<uint8_t> fileData((istreambuf_iterator<char>(inFile)), {});
    inFile.close();

    // Generate AES-like key
    uint8_t key[BLOCK_SIZE];
    srand(time(0));
    for (int i = 0; i < BLOCK_SIZE; ++i)
        key[i] = rand() % 256;

    // Encrypt blocks
    vector<vector<uint8_t>> encryptedBlocks;
    for (size_t i = 0; i < fileData.size(); i += BLOCK_SIZE) {
        vector<uint8_t> block(fileData.begin() + i, fileData.begin() + min(i + BLOCK_SIZE, fileData.size()));
        padBlock(block);
        encryptBlock(block.data(), key);
        encryptedBlocks.push_back(block);
    }

    // MENU
    int choice;
    do {
        cout << "\n================== MENU ==================\n";
        cout << "1. Show AES-128 Encryption Key\n";
        cout << "2. Show Encrypted Text (Hex Blocks)\n";
        cout << "3. Decrypt and Save Output to File\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\n🔐 AES-128 Key:\n";
                printHexBlock(key);
                break;

            case 2:
                cout << "\n📦 Encrypted Blocks (Hex):\n";
                for (size_t i = 0; i < encryptedBlocks.size(); ++i) {
                    cout << "Block " << i << ": ";
                    printHexBlock(encryptedBlocks[i].data());
                }
                break;

            case 3: {
                cout << "Enter output filename for decrypted text: ";
                cin >> outputFilename;

                vector<uint8_t> decryptedData;
                for (auto& block : encryptedBlocks) {
                    decryptBlock(block.data(), key);
                    decryptedData.insert(decryptedData.end(), block.begin(), block.end());
                }

                ofstream outFile(outputFilename, ios::binary);
                if (!outFile) {
                    cerr << "❌ Failed to open output file!" << endl;
                    break;
                }

                outFile.write(reinterpret_cast<const char*>(decryptedData.data()), decryptedData.size());
                outFile.close();
                cout << "✅ Decrypted data written to '" << outputFilename << "'\n";
                break;
            }

            case 0:
                cout << "👋 Exiting program.\n";
                break;

            default:
                cout << "❗ Invalid choice. Try again.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
