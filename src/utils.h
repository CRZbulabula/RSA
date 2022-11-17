//
// Created by Yongzao Dan on 2022/11/7.
//

#ifndef RSA_UTILS_H
#define RSA_UTILS_H

#include <random>
#include <iostream>
#include <fstream>

const static int HEXADECIMAL_RADIX = 1 << 4;
const static unsigned int HEXADECIMAL_BITS = 4;
const static unsigned int HEXADECIMAL_MASK = HEXADECIMAL_RADIX - 1;

const static int ASCII_RADIX = 1 << 8;
const static unsigned int ASCII_BITS = 8;
const static unsigned int ASCII_MASK = ASCII_RADIX - 1;

const static int UNSIGNED_INTEGER_BYTES = 4;
const static unsigned int UNSIGNED_INTEGER_BITS = 32;
const static unsigned int UNSIGNED_INTEGER_MASK = 0xffffffff;

const static long long LONG_LONG_MASK = 0xffffffffL;
const static unsigned long long UNSIGNED_LONG_LONG_MASK = 0xffffffffL;

/** @return The bitLength of the given array */
static int calcBitLength(const unsigned int *arr, int length) {
    unsigned int mask = 1 << 31;
    unsigned int head = arr[length - 1];
    int result = UNSIGNED_INTEGER_BITS;
    while (head < mask) {
        mask >>= 1;
        result -= 1;
    }
    return result + (length - 1) * (int) UNSIGNED_INTEGER_BITS;
}

/** Strip the leading zeros of the given array */
static int stripLeadingZeros(const unsigned int *src, unsigned int *&dst, int length) {
    int firstNonZero = length - 1;
    while (firstNonZero >= 0 && src[firstNonZero] == 0) {
        --firstNonZero;
    }
    if (firstNonZero == length - 1) {
        dst = new unsigned int[length];
        std::memcpy(dst, src, length * UNSIGNED_INTEGER_BYTES);
        return length;
    } else {
        dst = new unsigned int[firstNonZero + 1];
        std::memcpy(dst, src, (firstNonZero + 1) * UNSIGNED_INTEGER_BYTES);
        return firstNonZero + 1;
    }
}

/** @return The number of leading zeros in arr[length - 1] */
static int countLeadingZeros(const unsigned int *arr, int length) {
    int result = 0;
    unsigned int head = arr[length - 1];
    unsigned int currBit = 1 << 31;
    while (head < currBit) {
        result += 1;
        currBit >>= 1;
    }
    return result;
}

/** @return The tailing zeros of arr */
static int countTailingZeros(const unsigned int *arr, int length) {
    int result = 0;
    for (int i = 0; i < length; i++) {
        unsigned int tail = arr[i];
        if (!tail) {
            result += UNSIGNED_INTEGER_BITS;
            continue;
        }

        while (!(tail & 1)) {
            ++result;
            tail >>= 1;
        }

        return result;
    }
    return result;
}

/**
 * Left shift the 'src' by 'shift' bits and copy to 'dst'.
 * The 'dst' will have a leading zero ceil for better computing.
 */
static int leftShiftAndAddLeadingZero(const unsigned int *src, unsigned int *&dst, int length, int shift) {
    int result;
    unsigned int shiftOffset = UNSIGNED_INTEGER_BITS - shift;
    unsigned int shiftMask = ((1 << shift) - 1) << shiftOffset;
    if (shift <= countLeadingZeros(src, length)) {
        result = length;
        dst = new unsigned int[length + 1];
        dst[length] = 0;
    } else {
        result = length + 1;
        dst = new unsigned int[length + 2];
        dst[length] = (src[length - 1] & shiftMask) >> shiftOffset;
        dst[length + 1] = 0;
    }

    for (int i = length - 1; i > 0; i--) {
        dst[i] = (src[i] << shift) | ((src[i - 1] & shiftMask) >> shiftOffset);
    }
    dst[0] = src[0] << shift;
    return result;
}

/** Right shift the 'src' by 'shift' bits. */
static int rightShift(const unsigned int *src, unsigned int *&dst, int length, int shift) {
    int block = shift >> 5;
    shift %= UNSIGNED_INTEGER_BITS;
    unsigned int shiftMask = (1 << shift) - 1;
    unsigned int shiftOffset = UNSIGNED_INTEGER_BITS - shift;

    auto *result = new unsigned int[length];
    for (int i = 0; i < length - 1 - block; i++) {
        result[i] = shift ?
                    (((src[i + block + 1] & shiftMask) << shiftOffset) | (src[i + block] >> shift)) :
                    src[i + block];
    }
    result[length - block - 1] = src[length - 1] >> shift;
    for (int i = length - block; i < length - 1; i++) {
        result[i] = 0;
    }

    return stripLeadingZeros(result, dst, length);
}

static std::random_device randomDevice;
static std::default_random_engine randomEngine(randomDevice());
static std::uniform_int_distribution<unsigned int> uniformDistribution(0, UNSIGNED_INTEGER_MASK);

/** @return A uniform distribution random variable within [0, 2^32) */
static unsigned int rd() {
    return uniformDistribution(randomEngine);
}

static int readInt(std::istream &in) {
    int x;
    in >> x;
    return x;
}

static std::string readString(std::istream &in) {
    std::string s;
    in >> s;
    return s;
}

static std::ifstream openReadFile(const std::string &msg) {
    while (true) {
        std::cout << msg;
        std::ifstream in;
        std::string fileName = readString(std::cin);
        in.open(fileName, std::ios::in);

        if (in) {
            std::cout << "Successfully open file: " << fileName << std::endl;
            return in;
        } else {
            std::cout << "The specified file: " << fileName << " doesn't exist, please retry." << std::endl;
        }
    }
}

const static unsigned int bkdrSeed = 131;
static unsigned int BKDRHash(const std::string &s) {
    unsigned int hashcode = 0;
    unsigned int length = s.length();
    for (int i = 0; i < length; i++) {
        hashcode = hashcode * bkdrSeed + s[i];
    }
    return hashcode;
}

static void printLnSecond(clock_t delta) {
    std::cout << std::setprecision(3) << (double) delta / CLOCKS_PER_SEC << "s." << std::endl;
}

#endif //RSA_UTILS_H
