# AES-128 Encryption Tool

A simple file encryption and decryption tool with both **C++ command-line** and **web-based frontend** implementations.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C++-11-00599C.svg)
![HTML5](https://img.shields.io/badge/HTML5-E34F26.svg)

## 🔒 Features

- **File Encryption** - Encrypt any file type with AES-128 XOR cipher
- **File Decryption** - Decrypt files using the encryption key
- **Key Management** - Save and view encryption keys
- **Cross-Platform** - C++ CLI tool and web-based interface
- **User-Friendly** - Beautiful modern UI with dark theme
- **Secure** - 128-bit encryption keys

## 📁 Project Structure

```
aes-encryption-tool/
├── aes_tool.cpp          # C++ command-line implementation
├── aes_tool.html         # Web-based frontend (standalone)
└── README.md             # This file
```

## 🚀 Getting Started

### C++ Version

#### Prerequisites
- C++ compiler (g++, clang, or MSVC)
- C++11 or higher

#### Compilation
```bash
g++ -o aes_tool aes_tool.cpp -std=c++11
```

#### Usage
```bash
./aes_tool
```

Follow the interactive menu:
1. **Encrypt a File** - Choose a file to encrypt, generates encryption key
2. **Decrypt a File** - Provide encrypted file and key to decrypt
3. **View Key from File** - Display contents of a key file
0. **Exit**

### Web Version

#### Prerequisites
- Modern web browser (Chrome, Firefox, Safari, Edge)

#### Usage
1. Open `aes_tool.html` in any web browser
2. Choose between three modes:
   - **Encrypt File** - Upload file, get encrypted file + key
   - **Decrypt File** - Upload encrypted file, provide key, get original file
   - **View Key** - Load and display key files

## 🔑 How It Works

### Encryption Process
1. File is read and divided into 16-byte blocks
2. Random 128-bit (16-byte) key is generated
3. Each block is encrypted using XOR cipher with the key
4. Encrypted file and key are saved separately

### Decryption Process
1. Encrypted file is loaded
2. User provides the 128-bit encryption key
3. Each block is decrypted using XOR cipher with the key
4. Original file is restored

### Key Format
Keys are 32 hexadecimal characters (128 bits):
```
Example: a1b2c3d4e5f6071829304b5c6d7e8f90
```

## 📝 Example Workflow

### Encrypting a File
```bash
# C++ Version
./aes_tool
> Choose option 1 (Encrypt a File)
> Enter: document.txt
> Save encrypted as: document.txt.enc
> Save key as: document.key
```

### Decrypting a File
```bash
# C++ Version
./aes_tool
> Choose option 2 (Decrypt a File)
> Enter encrypted file: document.txt.enc
> Enter key: a1b2c3d4e5f6071829304b5c6d7e8f90
> Save decrypted as: document_recovered.txt
```

## 🎨 Frontend Screenshots

### Encrypt Mode
Upload files and generate encryption keys with a beautiful dark-themed interface.

### Decrypt Mode
Load encrypted files and provide keys through manual entry or file upload.

### View Key Mode
Display and manage your encryption keys securely.

## ⚠️ Important Security Notes

- **Educational Purpose**: This implementation uses XOR cipher for demonstration. It is NOT secure for production use.
- **Key Storage**: Store your encryption keys securely. Lost keys cannot be recovered.
- **File Backup**: Always keep backups of important files before encryption.
- **Production Use**: For real-world applications, use established encryption libraries like OpenSSL or libsodium.

## 🔧 Technical Details

### Algorithm
- **Cipher**: XOR-based block cipher
- **Block Size**: 16 bytes (128 bits)
- **Key Size**: 16 bytes (128 bits)
- **Padding**: Zero-padding for incomplete blocks

### Compatibility
- C++ and web versions are **fully compatible**
- Files encrypted with C++ can be decrypted with web interface and vice versa
- Keys are interchangeable between both implementations

## 🛠️ Development

### Building from Source

**C++ Version:**
```bash
# Linux/Mac
g++ -o aes_tool aes_tool.cpp -std=c++11

# Windows (MinGW)
g++ -o aes_tool.exe aes_tool.cpp -std=c++11

# Windows (MSVC)
cl /EHsc aes_tool.cpp
```

**Web Version:**
- No build process required
- Open `aes_tool.html` directly in browser
- All dependencies loaded from CDN

## 📄 File Formats

### Encrypted File (`.enc`)
Binary file containing encrypted data in 16-byte blocks.

### Key File (`.key`)
Plain text file containing:
- Encryption key (32 hex characters)
- Original filename
- File size and block count

Example:
```
AES-128 Encryption Key
======================
a1b2c3d4e5f6071829304b5c6d7e8f90

Original file: document.txt
Encrypted file: document.txt.enc
File size: 1024 bytes
Blocks: 64
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

### Areas for Improvement
- Implement actual AES encryption (AES-128, AES-256)
- Add password-based key derivation (PBKDF2)
- Support for large files (streaming encryption)
- Progress indicators
- Drag-and-drop file upload
- Multiple file encryption

## 📜 License

This project is licensed under the MIT License - see below for details:

```
MIT License

Copyright (c) 2024

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```


---

**⚠️ Disclaimer**: This tool is for educational purposes only. Do not use for protecting sensitive data in production environments. Always use industry-standard encryption libraries for real-world applications.
