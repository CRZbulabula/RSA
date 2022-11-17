//
// Created by Yongzao Dan on 2022/11/14.
//

#ifndef RSA_RSA_H
#define RSA_RSA_H

#include "BigInteger.h"

const static int RSA576 = 576;
const static int RSA640 = 640;
const static int RSA768 = 768;
const static int RSA896 = 896;
const static int RSA1024 = 1024;
const static int RSA1536 = 1536;
const static int RSA2048 = 2048;

/**
 * Generate RSA numbers, which satisfied:
 *      1. n = p * q, where p and q are two big primes
 *      2. phi(n) = e * d
 *
 * @param nLength The expected bit length of n
 * @param isEDefault The e will be set to 65537 if True, a random big prime otherwise
 */
static void generateRSANumbers(
        BigInteger &n,
        int nLength,
        BigInteger &e,
        bool isEDefault,
        BigInteger &d) {

    int pLength = nLength >> 1;
    int qLength = nLength - pLength;
    BigInteger p = BigInteger::generateBigPrime(pLength);
    BigInteger q = BigInteger::generateBigPrime(qLength);

    n = p * q;
    BigInteger phiN = (p - 1) * (q - 1);

    int delta = phiN.getBitLength() - pLength;
    e = isEDefault ?
        BigInteger(BigInteger::E_DEFAULT) :
        BigInteger::generateBigPrime(pLength + (int) (rd() % delta));
    d = e.multiplicativeInverse(phiN);

    // Ensure gcd(e, phiN) == 1
    while (d.isZero()) {
        int eLength = pLength + (int) (rd() % delta);
        e = BigInteger::generateBigPrime(eLength);
        d = e.multiplicativeInverse(phiN);
    }
}

#endif //RSA_RSA_H
