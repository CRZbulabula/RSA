//
// Created by Yongzao Dan on 2022/11/7.
//

#include "SmallPrimeSieve.h"

// The length of SMALL_SIEVE is reference from java.math.SmallPrimeSieve
const int SmallPrimeSieve::SMALL_SIEVE_LENGTH = 150 * 64;
const int SmallPrimeSieve::SMALL_PRIMES_COUNT = getSmallPrimesCount();
const int *SmallPrimeSieve::SMALL_PRIMES = sieveSmallPrimes();

int SmallPrimeSieve::getSmallPrimesCount() {
    int smallPrimesCount = 0;
    bool *isComposite = new bool[SMALL_SIEVE_LENGTH];
    memset(isComposite, false, SMALL_SIEVE_LENGTH);
    for (int i = 2; i < SMALL_SIEVE_LENGTH; i++) {
        if (!isComposite[i]) {
            ++smallPrimesCount;
            for (int j = i * 2; j < SMALL_SIEVE_LENGTH; j += i) {
                isComposite[j] = true;
            }
        }
    }
    return smallPrimesCount;
}

int *SmallPrimeSieve::sieveSmallPrimes() {
    int smallPrimesCount = 0;
    bool *isComposite = new bool[SMALL_SIEVE_LENGTH];
    memset(isComposite, false, SMALL_SIEVE_LENGTH);
    for (int i = 2; i < SMALL_SIEVE_LENGTH; i++) {
        if (!isComposite[i]) {
            ++smallPrimesCount;
            for (int j = i * 2; j < SMALL_SIEVE_LENGTH; j += i) {
                isComposite[j] = true;
            }
        }
    }

    int index = 0;
    auto *z = new int[smallPrimesCount];
    for (int i = 2; i < SMALL_SIEVE_LENGTH; i++) {
        if (!isComposite[i]) {
            z[index++] = i;
        }
    }
    return z;
}

SmallPrimeSieve::SmallPrimeSieve(const BigInteger &base) {
    this->candidate = true;
    this->remainders = new int[SMALL_PRIMES_COUNT];
    for (int i = 0; i < SMALL_PRIMES_COUNT; i++) {
        this->remainders[i] = (int) (base % SmallPrimeSieve::SMALL_PRIMES[i]);
        if (!this->remainders[i]) {
            this->candidate = false;
        }
    }
}

void SmallPrimeSieve::selfAddByTwo() {
    this->candidate = true;
    for (int i = 0; i < SMALL_PRIMES_COUNT; i++) {
        this->remainders[i] = (this->remainders[i] + 2);
        this->remainders[i] = this->remainders[i] >= SmallPrimeSieve::SMALL_PRIMES[i] ?
                              this->remainders[i] - SmallPrimeSieve::SMALL_PRIMES[i] :
                              this->remainders[i];
        if (!this->remainders[i]) {
            this->candidate = false;
        }
    }
}

bool SmallPrimeSieve::isCandidate() const {
    return this->candidate;
}

