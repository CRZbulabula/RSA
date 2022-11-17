//
// Created by Yongzao Dan on 2022/11/7.
//

#ifndef RSA_BIGINTEGER_H
#define RSA_BIGINTEGER_H

#include <map>

#include "utils.h"

class BigInteger {

private:

    static const BigInteger ZERO;
    static const BigInteger ONE;

    static const std::map<unsigned int, char> HEXADECIMAL_MAP;
    static const std::map<char, unsigned int> HEXADECIMAL_REFLECT;

    static std::map<unsigned int, char> generateHexadecimalMap();
    static std::map<char, unsigned int> generateHexadecimalReflect();

    // The sign of this
    // this is positive if sign == 1,
    // this is zero if sign == 0,
    // this is negative if sign == -1.
    int sign;
    // The length of number array
    int length;
    // The bitLength of number array
    int bitLength;
    unsigned int *number;

    /**
     * The inner addition implementation of BigInteger.
     *
     * @return The length of z, z = x + y
     */
    static int add(
            const unsigned int *x,
            int xLength,
            const unsigned int *y,
            int yLength,
            unsigned int *&z);

    /** Let this = this + 2, prime search only and this is positive */
    void selfAddByTwo();

    /**
     * The inner subtraction implementation of BigInteger.
     *
     * Notice: Always ensure that |x| > |y|.
     *
     * @return The length of z, z = x - y
     */
    static int subtract(
            const unsigned int *x,
            int xLength,
            const unsigned int *y,
            int yLength,
            unsigned int *&z);

    /**
     * The inner multiplication implementation of BigInteger.
     *
     * @return The length of z, z = x * y
     */
    static int multiply(
            const unsigned int *x,
            int xLength,
            const unsigned int *y,
            int yLength,
            unsigned int *&z);

    /**
     * The inner mod implementation of BigInteger.
     *
     * @return remainder = x % y, and q = x / y
     */
    static unsigned int modOneWord(
            const unsigned int *x,
            int xLength,
            unsigned int y,
            unsigned int *&q,
            int &qLength);

    /**
     * The inner mod implementation of BigInteger
     *
     * @return The length of z,
     *         z = x % y when mark == 1,
     *         z = x / y when mark == -1.
     */
    static int mod(
            const unsigned int *x,
            int xLength,
            const unsigned int *y,
            int yLength,
            int mark,
            unsigned int *&z);

    static void extendGCD(const BigInteger &a, const BigInteger &b, BigInteger &x, BigInteger &y);

public:

    static const BigInteger E_DEFAULT;

    /** Default constructor, default is 0 */
    BigInteger();

    virtual ~BigInteger();

    /** Copy constructor */
    BigInteger(const BigInteger &other);

    /** Constructor for custom BigInteger */
    BigInteger(int sign, unsigned int* number, int length);

    /** Construct this from the given value */
    explicit BigInteger(int value);

    /** Construct this from the given value */
    explicit BigInteger(unsigned int value);

    /** Construct this from the given string */
    explicit BigInteger(int radix, std::string value);

    /**
     * Constructor for generating a random BigInteger.
     *
     * Notice: the highest bit of the new BigInteger is always markComposite to 1.
     *
     * @param bitLength The length of bits in the new BigInteger.
     */
    static BigInteger randomBigInteger(int bitLength);

    /**
     * Compare the absolute value of this and an integer.
     *
     * @return 1 if (|this| > |x|),
     *         -1 if (|this| < |x|),
     *         0 if (|this| == |x|).
     */
    int compareAbsolute(unsigned int x) const;

    /**
     * Compare the absolute value of this and other.
     *
     * @return 1 if (|this| > |other|),
     *         -1 if (|this| < |other|),
     *         0 if (|this| == |other|).
     */
    int compareAbsolute(const BigInteger &other) const;

    /**
     * Overload + operator of BigInteger.
     *
     * @return BigInteger(this + other)
     */
    BigInteger operator+(const BigInteger &other) const;

    /**
     * Overload - operator of BigInteger.
     *
     * @return BigInteger(this - other)
     */
    BigInteger operator-(const BigInteger &other) const;

    /**
     * Overload - operator of BigInteger and int.
     *
     * @return BigInteger(this - x)
     */
    BigInteger operator-(int x) const;

    /**
     * Overload * operator of BigInteger.
     *
     * @return BigInteger(this * other)
     */
    BigInteger operator*(const BigInteger &other) const;

    /**
     * Overload % operator of BigInteger and int.
     *
     * @return (this % divisor)
     */
    unsigned int operator%(unsigned int divisor) const;

    /**
     * Overload % operator of BigInteger.
     *
     * @return (this % other) which is always positive.
     */
    BigInteger operator%(const BigInteger &other) const;

    /**
     * Overload / operator of BigInteger.
     *
     * @return BigInteger(this / other) which is always positive.
     */
    BigInteger operator/(const BigInteger &other) const;

    /** @return this^-1 such that this * this^-1 == 1 (mod mod) */
    BigInteger multiplicativeInverse(const BigInteger &mod) const;

    /** @return z = this^pow % mod */
    BigInteger bigPowMod(const BigInteger &pow, const BigInteger &mod) const;

    static BigInteger generateBigPrime(int bitLength);

    /** @return Ture iff this is probably a prime. */
    bool isPrime() const;

    /** @return The length of ciphertext and ciphertext(plaintext^e (mod n)) */
    static int encryptPlaintext(
            const std::string &plaintext,
            BigInteger *&ciphertext,
            const BigInteger &e,
            const BigInteger &n);

    /** @return Plaintext(ciphertext^d (mod n)) */
    static std::string decryptCiphertext(
            int plaintextLength,
            const BigInteger *ciphertext,
            int ciphertextLength,
            const BigInteger &d,
            const BigInteger &n);

    /** @return hashcode^d (mod n) */
    static BigInteger signSignature(unsigned int hashcode, const BigInteger &d, const BigInteger &n);

    /** @return signature^e (mod n) */
    static unsigned int decryptSignature(const BigInteger &signature, const BigInteger &e, const BigInteger &n);

    /** @param radix: Only 16(hexadecimal) or 256(ascii) is supported. */
    std::string toString(int radix) const;

    void write(std::ofstream &out) const;

    /** @return Ture iff this is zero. */
    bool isZero() const;

    int getBitLength() const;
};


#endif //RSA_BIGINTEGER_H
