//
// Created by Yongzao Dan on 2022/11/7.
//

#ifndef RSA_SMALLPRIMESIEVE_H
#define RSA_SMALLPRIMESIEVE_H

#include <iostream>

#include "utils.h"
#include "BigInteger.h"

/** Pre-calculate a batch of small primes, in order to speed up big prime detection. */
class SmallPrimeSieve {

private:

    static const int SMALL_SIEVE_LENGTH;

    static const int SMALL_PRIMES_COUNT;
    static int getSmallPrimesCount();

    static const int *SMALL_PRIMES;
    static int *sieveSmallPrimes();

    int *remainders;
    bool candidate;

public:

    /** Let remainders[i] = base % SMALL_PRIMES[i] */
    explicit SmallPrimeSieve(const BigInteger &base);

    /** Let remainders[i] = (remainders[i] + 2) % SMALL_PRIMES[i] */
    void selfAddByTwo();

    /** @return True iff for all i, remainders[i] > 0 */
    bool isCandidate() const;
};



#endif //RSA_SMALLPRIMESIEVE_H
