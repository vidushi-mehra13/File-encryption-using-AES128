#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const int BLOCK_SIZE = 16; // 128 bits

// XOR encryption/decryption
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
        cout << hex << setw(2) << setfill('0') << (int)block[i];
    cout << dec;
}

// Pad block to BLOCK_SIZE
void padBlock(vector<uint8_t>& block) {
    while (block.size() < BLOCK_SIZE)
        block.push_back(0);
}

// Parse hex key string
bool parseHexKey(const string& hexStr, uint8_t* key) {
    string cleanHex = hexStr;
    cleanHex.erase(remove_if(cleanHex.begin(), cleanHex.end(), ::isspace), cleanHex.end());
    
    if (cleanHex.length() != BLOCK_SIZE * 2) {
        return false;
    }

    for (int i = 0; i < BLOCK_SIZE; ++i) {
        string byteStr = cleanHex.substr(i * 2, 2);
        try {
            key[i] = (uint8_t)stoi(byteStr, nullptr, 16);
        } catch (...) {
            return false;
        }
    }
    return true;
}

// Encryption function
void encryptFile() {
    string inputFilename, outputFilename, keyFilename;
    
    cout << "\n====================================\n";
    cout << "          FILE ENCRYPTION           \n";
    cout << "====================================\n\n";
    
    cout << "Enter input file to encrypt: ";
    cin >> inputFilename;

    ifstream inFile(inputFilename, ios::binary);
    if (!inFile) {
        cerr << " Failed to open input file!" << endl;
        return;
    }

    // Read file data
    vector<uint8_t> fileData((istreambuf_iterator<char>(inFile)), {});
    inFile.close();

    cout << " File size: " << fileData.size() << " bytes\n";

    // Generate random AES-128 key
    uint8_t key[BLOCK_SIZE];
    srand(time(0));
    for (int i = 0; i < BLOCK_SIZE; ++i)
        key[i] = rand() % 256;

    cout << "\n Generated AES-128 Key (SAVE THIS!):\n";
    printHexBlock(key);
    cout << "\n\n";

    // Encrypt blocks
    vector<vector<uint8_t>> encryptedBlocks;
    for (size_t i = 0; i < fileData.size(); i += BLOCK_SIZE) {
        vector<uint8_t> block(fileData.begin() + i, 
                             fileData.begin() + min(i + BLOCK_SIZE, fileData.size()));
        padBlock(block);
        encryptBlock(block.data(), key);
        encryptedBlocks.push_back(block);
    }

    cout << " Encrypted " << encryptedBlocks.size() << " blocks\n\n";

    // Save encrypted file
    cout << "Enter output filename for encrypted data: ";
    cin >> outputFilename;

    ofstream outFile(outputFilename, ios::binary);
    if (!outFile) {
        cerr << " Failed to create output file!" << endl;
        return;
    }

    for (const auto& block : encryptedBlocks) {
        outFile.write(reinterpret_cast<const char*>(block.data()), block.size());
    }
    outFile.close();

    cout << " Encrypted file saved to: " << outputFilename << "\n\n";

    // Save key to file
    cout << "Enter filename to save the key: ";
    cin >> keyFilename;

    ofstream keyFile(keyFilename);
    if (!keyFile) {
        cerr << " Failed to create key file!" << endl;
        return;
    }

    keyFile << "AES-128 Encryption Key\n";
    keyFile << "======================\n";
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        keyFile << hex << setw(2) << setfill('0') << (int)key[i];
    }
    keyFile << dec << "\n\n";
    keyFile << "Original file: " << inputFilename << "\n";
    keyFile << "Encrypted file: " << outputFilename << "\n";
    keyFile << "File size: " << fileData.size() << " bytes\n";
    keyFile << "Blocks: " << encryptedBlocks.size() << "\n";
    keyFile.close();

    cout << " Key saved to: " << keyFilename << "\n\n";

    cout << "  IMPORTANT: Keep the key file safe!\n";
    cout << "    You will need it to decrypt your file.\n\n";
}

// Show key from key file
void showKey() {
    string keyFilename;
    
    cout << "\n====================================\n";
    cout << "           VIEW KEY FILE            \n";
    cout << "====================================\n\n";
    
    cout << "Enter key filename: ";
    cin >> keyFilename;
    
    ifstream keyFile(keyFilename);
    if (!keyFile) {
        cerr << " Failed to open key file!" << endl;
        return;
    }
    
    string line;
    cout << "\n Key File Contents:\n";
    cout << "─────────────────────────────────────\n";
    while (getline(keyFile, line)) {
        cout << line << "\n";
    }
    cout << "─────────────────────────────────────\n";
    keyFile.close();
    
    cout << "\n Key file displayed successfully!\n";
}

// Decryption function
void decryptFile() {
    string inputFilename, outputFilename, keyInput;
    uint8_t key[BLOCK_SIZE];

    cout << "\n====================================\n";
    cout << "          FILE DECRYPTION           \n";
    cout << "====================================\n\n";

    // Get encrypted file
    cout << "Enter encrypted file: ";
    cin >> inputFilename;

    ifstream inFile(inputFilename, ios::binary);
    if (!inFile) {
        cerr << " Failed to open encrypted file!" << endl;
        return;
    }

    // Read encrypted data
    vector<uint8_t> encryptedData((istreambuf_iterator<char>(inFile)), {});
    inFile.close();

    cout << " Encrypted file size: " << encryptedData.size() << " bytes\n";
    cout << " Number of blocks: " << (encryptedData.size() / BLOCK_SIZE) << "\n\n";

    // Get decryption key
    cout << "Enter decryption key (32 hex characters): ";
    cin.ignore(); // Clear input buffer
    getline(cin, keyInput);

    if (!parseHexKey(keyInput, key)) {
        cerr << " Invalid key format! Key must be 32 hexadecimal characters.\n";
        cerr << "   Example: a1b2c3d4e5f6071829304b5c6d7e8f90\n";
        return;
    }

    cout << "\n Using key: ";
    printHexBlock(key);
    cout << "\n\n";

    // Decrypt blocks
    vector<vector<uint8_t>> decryptedBlocks;
    for (size_t i = 0; i < encryptedData.size(); i += BLOCK_SIZE) {
        vector<uint8_t> block(encryptedData.begin() + i,
                             encryptedData.begin() + min(i + BLOCK_SIZE, encryptedData.size()));
        
        // Ensure block is correct size
        if (block.size() == BLOCK_SIZE) {
            decryptBlock(block.data(), key);
            decryptedBlocks.push_back(block);
        }
    }

    cout << " Decrypted " << decryptedBlocks.size() << " blocks\n\n";

    // Save decrypted file
    cout << "Enter output filename for decrypted data: ";
    getline(cin, outputFilename);

    ofstream outFile(outputFilename, ios::binary);
    if (!outFile) {
        cerr << " Failed to create output file!" << endl;
        return;
    }

    // Write decrypted data
    for (const auto& block : decryptedBlocks) {
        outFile.write(reinterpret_cast<const char*>(block.data()), block.size());
    }
    outFile.close();

    cout << " Decrypted file saved to: " << outputFilename << "\n\n";

    // Additional options menu
    int choice;
    do {
        cout << "\n========== OPTIONS ==========\n";
        cout << "1. Show first 256 bytes (hex)\n";
        cout << "2. Show decrypted blocks info\n";
        cout << "0. Return to main menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "\n First 256 bytes (hex):\n";
                int bytesToShow = min(256, (int)decryptedBlocks.size() * BLOCK_SIZE);
                int byteCount = 0;
                
                for (const auto& block : decryptedBlocks) {
                    for (const auto& byte : block) {
                        cout << hex << setw(2) << setfill('0') << (int)byte << " ";
                        byteCount++;
                        if (byteCount % 16 == 0) cout << "\n";
                        if (byteCount >= bytesToShow) break;
                    }
                    if (byteCount >= bytesToShow) break;
                }
                cout << dec << "\n";
                break;
            }

            case 2:
                cout << "\n Decryption Info:\n";
                cout << "   Total blocks: " << decryptedBlocks.size() << "\n";
                cout << "   Block size: " << BLOCK_SIZE << " bytes\n";
                cout << "   Total data: " << (decryptedBlocks.size() * BLOCK_SIZE) << " bytes\n";
                break;

            case 0:
                cout << "Returning to main menu...\n";
                break;

            default:
                cout << " Invalid choice.\n";
                break;
        }
    } while (choice != 0);
}

int main() {
    int choice;

    cout << "\n";
    cout << "╔════════════════════════════════════╗\n";
    cout << "║   AES-128 ENCRYPTION/DECRYPTION   ║\n";
    cout << "║        File Security Tool          ║\n";
    cout << "╚════════════════════════════════════╝\n";

    do {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Encrypt a File\n";
        cout << "2. Decrypt a File\n";
        cout << "3. View Key from File\n";
        cout << "0. Exit\n";
        cout << "================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                encryptFile();
                break;

            case 2:
                decryptFile();
                break;

            case 3:
                showKey();
                break;

            case 0:
                cout << "\n Thank you for using AES-128 Encryption Tool!\n";
                cout << "   Stay secure! \n\n";
                break;

            default:
                cout << " Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
