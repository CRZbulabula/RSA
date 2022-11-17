//
// Created by Yongzao Dan on 2022/11/11.
//

#include <fstream>

#include "gtest/gtest.h"

#include "BigInteger.h"
#include "rsa.h"

class FunctionalTests: public::testing::Test {

protected:

    static const int TEST_CASES = 100;

    void SetUp() override {

    }

    void TearDown() override {

    }

    static void readTestCase(std::ifstream &in, BigInteger &A, BigInteger &B, BigInteger &C) {
        std::string a;
        in >> a;
        A = BigInteger(HEXADECIMAL_RADIX, a);
        std::string b;
        in >> b;
        B = BigInteger(HEXADECIMAL_RADIX, b);
        std::string c;
        in >> c;
        C = BigInteger(HEXADECIMAL_RADIX, c);
    }

};

TEST_F(FunctionalTests, addTest) {
    std::ifstream in("../test/data/addTest.txt");
    for (int i = 0; i < TEST_CASES; i++) {
        BigInteger A, B, C;
        readTestCase(in, A, B, C);
        BigInteger sum = A + B;
        EXPECT_EQ(0, sum.compareAbsolute(C));
    }
    in.close();
}

TEST_F(FunctionalTests, subtractTest) {
    std::ifstream in("../test/data/subtractTest.txt");
    for (int i = 0; i < TEST_CASES; i++) {
        BigInteger A, B, C;
        readTestCase(in, A, B, C);
        BigInteger difference = A - B;
        EXPECT_EQ(0, difference.compareAbsolute(C));
    }
    in.close();
}

TEST_F(FunctionalTests, multiplyTest) {
    std::ifstream in("../test/data/multiplyTest.txt");
    for (int i = 0; i < TEST_CASES; i++) {
        BigInteger A, B, C;
        readTestCase(in, A, B, C);
        BigInteger proc = A * B;
        EXPECT_EQ(0, proc.compareAbsolute(C));
    }
    in.close();
}

TEST_F(FunctionalTests, divideTest) {
    std::ifstream in("../test/data/divideTest.txt");
    for (int i = 0; i < TEST_CASES; i++) {
        BigInteger A, B, C;
        readTestCase(in, A, B, C);
        BigInteger quotient = A / B;
        EXPECT_EQ(0, quotient.compareAbsolute(C));
    }
    in.close();
}

TEST_F(FunctionalTests, smallModTest) {
    std::ifstream in("../test/data/smallModTest.txt");
    for (int i = 0; i < TEST_CASES; i++) {
        std::string a;
        in >> a;
        BigInteger A = BigInteger(HEXADECIMAL_RADIX, a);

        unsigned int b, c;
        in >> b >> c;

        unsigned int remainder = A % b;

        EXPECT_EQ(c, remainder);
    }
    in.close();
}

TEST_F(FunctionalTests, modTest) {
    std::ifstream in("../test/data/modTest.txt");
    for (int i = 0; i < TEST_CASES; i++) {
        BigInteger A, B, C;
        readTestCase(in, A, B, C);
        BigInteger remainder = A % B;
        EXPECT_EQ(0, remainder.compareAbsolute(C));
    }
    in.close();
}

TEST_F(FunctionalTests, powModTest) {
    std::ifstream in("../test/data/powModTest.txt");
    for (int i = 0; i < TEST_CASES; i++) {
        std::string a;
        in >> a;
        BigInteger A = BigInteger(HEXADECIMAL_RADIX, a);
        std::string b;
        in >> b;
        BigInteger B = BigInteger(HEXADECIMAL_RADIX, b);
        std::string c;
        in >> c;
        BigInteger C = BigInteger(HEXADECIMAL_RADIX, c);
        std::string d;
        in >> d;
        BigInteger D = BigInteger(HEXADECIMAL_RADIX, d);

        BigInteger result = A.bigPowMod(B, C);
        EXPECT_EQ(0, result.compareAbsolute(D));
    }
    in.close();
}

TEST_F(FunctionalTests, inverseTest) {
    std::ifstream in("../test/data/inverseTest.txt");
    for (int i = 0; i < TEST_CASES; i++) {
        BigInteger A, B, C;
        readTestCase(in, A, B, C);
        BigInteger inverse = A.multiplicativeInverse(B);
        EXPECT_EQ(0, inverse.compareAbsolute(C));
    }
    in.close();
}

TEST_F(FunctionalTests, isPrimeTest) {
    std::ifstream in("../test/data/isPrimeTest.txt");
    for (int i = 0; i < TEST_CASES; i++) {
        std::string a;
        in >> a;
        BigInteger A = BigInteger(HEXADECIMAL_RADIX, a);
        EXPECT_TRUE(A.isPrime());
    }
    in.close();
}

const static int TEST_PLAIN_TEXT_LENGTH = 300;

static int randomRSANumber() {
    unsigned int rsaNumber = rd() % 3;
    switch (rsaNumber) {
        case 0:
            return RSA576;
        case 1:
            return RSA640;
        case 2:
        default:
            return RSA768;
    }
}

static bool randomIsEDefault() {
    return rd() % 2;
}

static std::string randomPlaintext() {
    std::string plaintext;
    for (int j = 0; j < TEST_PLAIN_TEXT_LENGTH; j++) {
        plaintext += (char) (rd() % 128);
    }
    return plaintext;
}

TEST_F(FunctionalTests, cipherTest) {
    for (int i = 0; i < TEST_CASES; i++) {
        int rsaNumber = randomRSANumber();
        bool isEDefault = randomIsEDefault();
        BigInteger n, e, d;
        generateRSANumbers(n, rsaNumber, e, isEDefault, d);
        std::string plaintext = randomPlaintext();

        BigInteger *ciphertext = nullptr;
        int ciphertextLength = BigInteger::encryptPlaintext(plaintext, ciphertext, e, n);
        std::string decryptText = BigInteger::decryptCiphertext(
                TEST_PLAIN_TEXT_LENGTH,
                ciphertext,
                ciphertextLength,
                d,
                n);

        EXPECT_EQ(plaintext, decryptText);
    }
}

TEST_F(FunctionalTests, signatureTest) {
    for (int i = 0; i < TEST_CASES; i++) {
        int rsaNumber = randomRSANumber();
        bool isEDefault = randomIsEDefault();

        BigInteger n, e, d;
        generateRSANumbers(n, rsaNumber, e, isEDefault, d);
        std::string plaintext = randomPlaintext();
        unsigned int hashcode = BKDRHash(plaintext);

        BigInteger signature = BigInteger::signSignature(hashcode, d, n);
        unsigned int decode = BigInteger::decryptSignature(signature, e, n);
        EXPECT_EQ(hashcode, decode);
    }
}