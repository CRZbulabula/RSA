#include <iostream>
#include <sstream>

#include "utils.h"
#include "rsa.h"
#include "BigInteger.h"

void generateRSAKeys() {
    // Input RSA-number
    int nLength = -1;
    while (nLength == -1) {
        std::cout << "Please select RSA number:" << std::endl;
        std::cout << "\t1. RSA-576(576-bits)" << std::endl;
        std::cout << "\t2. RSA-640(640-bits)" << std::endl;
        std::cout << "\t3. RSA-768(768-bits)" << std::endl;
        std::cout << "\t4. RSA-896(896-bits)" << std::endl;
        std::cout << "\t5. RSA-1024(1024-bits)" << std::endl;
        std::cout << "\t6. RSA-1536(1536-bits)" << std::endl;
        std::cout << "\t7. RSA-2048(2048-bits)" << std::endl;
        std::cout << "Enter RSA number[1, 7]: ";

        int index = readInt(std::cin);
        switch (index) {
            case 1:
                nLength = RSA576;
                break;
            case 2:
                nLength = RSA640;
                break;
            case 3:
                nLength = RSA768;
                break;
            case 4:
                nLength = RSA896;
                break;
            case 5:
                nLength = RSA1024;
                break;
            case 6:
                nLength = RSA1536;
                break;
            case 7:
                nLength = RSA2048;
                break;
            default:
                std::cout << "Unrecognized RSA number, please try again." << std::endl;
                continue;
        }
    }

    // Select exponent
    int isEDefault = -1;
    while (isEDefault == -1) {
        std::cout << "Use default exponent(65537)?" << std::endl;
        std::cout << "\t1. Yes" << std::endl;
        std::cout << "\t2. No(generate random exponent)" << std::endl;
        std::cout << "Enter [1, 2]: ";

        int index = readInt(std::cin);
        switch (index) {
            case 1:
                isEDefault = true;
                break;
            case 2:
                isEDefault = false;
                break;
            default:
                std::cout << "Unrecognized RSA number, please try again." << std::endl;
                continue;
        }
    }

    auto startTime = clock();

    // Generate RSA keys
    BigInteger n, e, d;
    generateRSANumbers(n, nLength, e, isEDefault, d);

    // Write public key
    const static std::string publicKeyFile = "./public_key.txt";
    std::ofstream publicKey(publicKeyFile);
    n.write(publicKey);
    e.write(publicKey);
    publicKey.close();
    std::cout << "Successfully generate RSA public key on: " << publicKeyFile << std::endl;

    // Write private key
    const static std::string privateKeyFile = "./private_key.txt";
    std::ofstream privateKey(privateKeyFile);
    n.write(privateKey);
    d.write(privateKey);
    privateKey.close();
    std::cout << "Successfully generate RSA private key on: " << privateKeyFile << std::endl;

    auto endTime = clock();
    std::cout << "Successfully generate RSA public/private keys for module bit-length of "
              << nLength << ", time-consuming: ";
    printLnSecond(endTime - startTime);
}

void inputPublicKey(BigInteger &n, BigInteger &e) {
    std::ifstream publicKey = openReadFile("Please input your public_key file: ");

    std::string nString = readString(publicKey);
    n = BigInteger(HEXADECIMAL_RADIX, nString);
    std::string eString = readString(publicKey);
    e = BigInteger(HEXADECIMAL_RADIX, eString);

    publicKey.close();
    std::cout << "Successfully read RSA public key." << std::endl;
}

void inputPrivateKey(BigInteger &n, BigInteger &d) {
    std::ifstream privateKey = openReadFile("Please input your private_key file: ");

    std::string nString = readString(privateKey);
    n = BigInteger(HEXADECIMAL_RADIX, nString);
    std::string dString = readString(privateKey);
    d = BigInteger(HEXADECIMAL_RADIX, dString);

    privateKey.close();
    std::cout << "Successfully read RSA private key." << std::endl;
}

std::string inputPlaintext() {
    std::ifstream plain = openReadFile("Please input your plaintext file: ");

    std::ostringstream tmp;
    tmp << plain.rdbuf();
    std::string plaintext = tmp.str();

    plain.close();
    std::cout << "Successfully read plaintext." << std::endl;
    return plaintext;
}

void encryptPlaintext() {
    // Input public key and plaintext
    BigInteger n, e;
    inputPublicKey(n, e);
    std::string plaintext = inputPlaintext();

    // Encrypt and write
    BigInteger *ciphertext = nullptr;
    int ciphertextLength = BigInteger::encryptPlaintext(plaintext, ciphertext, e, n);

    const static std::string ciphertextFile = "./ciphertext.txt";
    std::ofstream ciphertextStream(ciphertextFile);

    ciphertextStream << plaintext.length() << std::endl;
    ciphertextStream << ciphertextLength << std::endl;
    for (int i = 0; i < ciphertextLength; i++) {
        ciphertext[i].write(ciphertextStream);
    }

    ciphertextStream.close();
    std::cout << "Successfully encrypt the plaintext, and the ciphertext is wrote on: " << ciphertextFile << std::endl;
}

void decryptCiphertext() {
    // Input private key
    BigInteger n, d;
    inputPrivateKey(n, d);

    // Input ciphertext
    std::ifstream ciphertextStream = openReadFile("Please input your ciphertext file: ");

    int plaintextLength, ciphertextLength;
    ciphertextStream >> plaintextLength >> ciphertextLength;
    auto *ciphertext = new BigInteger[ciphertextLength];
    for (int i = 0; i < ciphertextLength; i++) {
        ciphertext[i] = BigInteger(HEXADECIMAL_RADIX, readString(ciphertextStream));
    }

    ciphertextStream.close();

    // Decrypt and write
    const static std::string plaintextFile = "./plaintext.txt";
    std::ofstream plaintextStream(plaintextFile);

    std::string plaintext = BigInteger::decryptCiphertext(plaintextLength, ciphertext, ciphertextLength, d, n);
    plaintextStream << plaintext;

    plaintextStream.close();
    std::cout << "Successfully decrypt the ciphertext, and the plaintext is wrote on: " << plaintextFile << std::endl;
}

void signSignature() {
    // Input private key
    BigInteger n, d;
    inputPrivateKey(n, d);

    // Input plaintext and hash
    std::string plaintext = inputPlaintext();
    unsigned int hashcode = BKDRHash(plaintext);

    // Sign and write
    const static std::string signatureFile = "./signature.txt";
    std::ofstream signatureStream(signatureFile);

    BigInteger signature = BigInteger::signSignature(hashcode, d, n);
    signature.write(signatureStream);

    signatureStream.close();
    std::cout << "Successfully generate digital signature on: " << signatureFile << std::endl;
}

void verifySignature() {
    // Input public key
    BigInteger n, e;
    inputPublicKey(n, e);

    // Input plaintext
    std::string plaintext = inputPlaintext();
    unsigned int hashcode = BKDRHash(plaintext);

    // Input signature
    std::ifstream signatureStream = openReadFile("Please input your signature file: ");
    std::string signatureString = readString(signatureStream);
    BigInteger signature = BigInteger(HEXADECIMAL_RADIX, signatureString);

    // Verify
    unsigned int hashcode2 = BigInteger::decryptSignature(signature, e, n);
    if (hashcode == hashcode2) {
        std::cout << "The digital signature is successfully verified!" << std::endl;
    } else {
        std::cout << "The digital signature verification failed!" << std::endl;
    }
}

int main() {

    std::cout << "Welcome to use EasyRSA!" << std::endl;

    while (true) {

        std::cout << "Please select a function:" << std::endl;
        std::cout << "\t1. Generate RSA public/private keys" << std::endl;
        std::cout << "\t2. Encrypt(ascii plaintext -> hexadecimal ciphertext)" << std::endl;
        std::cout << "\t3. Decrypt(hexadecimal ciphertext -> ascii plaintext)" << std::endl;
        std::cout << "\t4. Sign(generate an ascii plaintext's digital signature)" << std::endl;
        std::cout << "\t5. Verify(verify a digital signature with a plaintext)" << std::endl;
        std::cout << "\t6. Quit" << std::endl;

        std::cout << "Please input the function index[1, 6]: ";
        int index = readInt(std::cin);
        switch (index) {
            case 1:
                generateRSAKeys();
                break;
            case 2:
                encryptPlaintext();
                break;
            case 3:
                decryptCiphertext();
                break;
            case 4:
                signSignature();
                break;
            case 5:
                verifySignature();
                break;
            case 6:
                std::cout << "Bye." << std::endl;
                return 0;
            default:
                std::cout << "Unrecognized function index, please try again." << std::endl;
        }
    }

}

