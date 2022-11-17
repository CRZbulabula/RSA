//
// Created by Yongzao Dan on 2022/11/7.
//

#include "BigInteger.h"
#include "SmallPrimeSieve.h"

const BigInteger BigInteger::ZERO = BigInteger(0);
const BigInteger BigInteger::ONE = BigInteger(1);
const BigInteger BigInteger::E_DEFAULT = BigInteger(65537);

const std::map<unsigned int, char> BigInteger::HEXADECIMAL_MAP = generateHexadecimalMap();
const std::map<char, unsigned int> BigInteger::HEXADECIMAL_REFLECT = generateHexadecimalReflect();

std::map<unsigned int, char> BigInteger::generateHexadecimalMap() {
    std::map<unsigned int, char> result;
    result.clear();

    for (int i = 0; i < 10; i++) {
        result.insert(std::pair<unsigned int, char>(i, '0' + i));
    }

    for (int i = 10; i < 16; i++) {
        result.insert(std::pair<unsigned int, char>(i, 'a' - 10 + i));
    }

    return result;
}

std::map<char, unsigned int> BigInteger::generateHexadecimalReflect() {
    std::map<char, unsigned int> result;
    result.clear();

    for (int i = 0; i < 10; i++) {
        result.insert(std::pair<char, unsigned int>('0' + i, i));
    }

    for (int i = 10; i < 16; i++) {
        result.insert(std::pair<char, unsigned int>('a' - 10 + i, i));
    }

    return result;
}

// ========================================
// Begin of BigInteger constructors
// ========================================

BigInteger::BigInteger() {
    this->sign = 0;
    this->length = 0;
    this->bitLength = 0;
    this->number = new unsigned int[0];
}

BigInteger::~BigInteger() = default;

BigInteger::BigInteger(const BigInteger &other) {
    this->sign = other.sign;
    this->length = other.length;
    this->bitLength = other.bitLength;

    this->number = new unsigned int[this->length];
    std::memcpy(this->number, other.number, this->length * UNSIGNED_INTEGER_BYTES);
}

BigInteger::BigInteger(int value) {
    if (value > 0) {
        this->sign = 1;
        this->number = new unsigned int[1];
        this->number[0] = value;
        this->length = 1;
        this->bitLength = calcBitLength(this->number, this->length);
    } else if (value == 0) {
        this->sign = 0;
        this->length = 0;
        this->bitLength = 0;
        this->number = new unsigned int[0];
    } else {
        this->sign = -1;
        this->number = new unsigned int[1];
        this->number[0] = -value;
        this->length = 1;
        this->bitLength = calcBitLength(this->number, this->length);
    }
}

BigInteger::BigInteger(unsigned int value) {
    if (value) {
        this->sign = 1;
        this->number = new unsigned int[1];
        this->number[0] = value;
        this->length = 1;
        this->bitLength = calcBitLength(this->number, this->length);
    } else {
        this->sign = 0;
        this->length = 0;
        this->bitLength = 0;
        this->number = new unsigned int[0];
    }
}

BigInteger::BigInteger(int radix, std::string value) {
    if (value.length() == 1 && value[0] == '0') {
        this->sign = 0;
        this->length = 0;
        this->bitLength = 0;
        this->number = new unsigned int[0];
        return;
    }

    int offset = radix == HEXADECIMAL_RADIX && value[0] == '-' ? 1 : 0;
    int valLength = (int) value.length();
    int valBits = valLength - offset;
    switch (radix) {
        case HEXADECIMAL_RADIX:
            valBits *= HEXADECIMAL_BITS;
            break;
        case ASCII_RADIX:
        default:
            valBits *= ASCII_BITS;
            break;
    }

    this->sign = offset ? -1 : 1;
    this->length = ((valBits - 1) >> 5) + 1;
    this->number = new unsigned int[this->length];

    int index = 0;
    unsigned int curNum = 0;
    for (int i = valLength - 1, j = 0; i >= offset; i--) {
        if (j == UNSIGNED_INTEGER_BITS) {
            this->number[index++] = curNum;
            curNum = j = 0;
        }

        switch (radix) {
            case HEXADECIMAL_RADIX:
                curNum |= (HEXADECIMAL_REFLECT.find(value[i])->second << j);
                j += HEXADECIMAL_BITS;
                break;
            case ASCII_RADIX:
            default:
                curNum |= (value[i] & UNSIGNED_INTEGER_MASK) << j;
                j += ASCII_BITS;
                break;
        }

    }
    if (index < this->length) {
        this->number[index++] = curNum;
    }

    this->bitLength = calcBitLength(this->number, this->length);
}

BigInteger::BigInteger(int sign, unsigned int *number, int length) {
    this->sign = sign;
    this->number = number;
    this->length = length;
    this->bitLength = calcBitLength(number, length);

    if (this->length == 0 || this->bitLength == 0) {
        this->sign = 0;
    }
}

BigInteger BigInteger::randomBigInteger(int bitLength) {
    BigInteger result;

    result.sign = 1;
    result.bitLength = bitLength;

    result.length = ((bitLength - 1) >> 5) + 1;
    result.number = new unsigned int[result.length];
    for (int i = 0; i < result.length; i++) {
        result.number[i] = rd();
    }

    unsigned long long header = bitLength % UNSIGNED_INTEGER_BITS;
    header = header ? header : 32;
    result.number[result.length - 1] &= ((1L << header) - 1);
    result.number[result.length - 1] |= 1 << (header - 1);

    return result;
}
// ========================================
// End of BigInteger constructors
// ========================================


int BigInteger::compareAbsolute(unsigned int x) const {
    if (this->length > 1) {
        return 1;
    }
    if (this->number[0] == x) {
        return 0;
    } else if (this->number[0] > x) {
        return 1;
    } else {
        return -1;
    }
}

int BigInteger::compareAbsolute(const BigInteger &other) const {
    // Compare the length first
    if (this->length > other.length) {
        return 1;
    }
    if (this->length < other.length) {
        return -1;
    }

    // Compare each value
    for (int i = this->length - 1; i >= 0; i--) {
        unsigned int x = this->number[i];
        unsigned int y = other.number[i];
        if (x != y) {
            return x > y ? 1 : -1;
        }
    }

    return 0;
}


// ========================================
// Begin of BigInteger addition
// ========================================

int BigInteger::add(
        const unsigned int *x,
        int xLength,
        const unsigned int *y,
        int yLength,
        unsigned int *&z) {

    // Ensure a is bigger than b
    const unsigned int *a, *b;
    int aLength, bLength;
    if (xLength < yLength) {
        a = y;
        aLength = yLength;
        b = x;
        bLength = xLength;
    } else {
        a = x;
        aLength = xLength;
        b = y;
        bLength = yLength;
    }

    auto *result = new unsigned int[aLength];

    // Do addition and reserve the overflow value
    unsigned long long sum = 0;
    for (int i = 0; i < bLength; i++) {
        sum = (a[i] & UNSIGNED_LONG_LONG_MASK) +
              (b[i] & UNSIGNED_LONG_LONG_MASK) +
              (sum >> UNSIGNED_INTEGER_BITS);
        result[i] = sum & UNSIGNED_INTEGER_MASK;
    }

    // Check and carry the overflow value
    int i = bLength;
    for (; sum > UNSIGNED_INTEGER_MASK && i < aLength; i++) {
        sum = (a[i] & UNSIGNED_LONG_LONG_MASK) + 1;
        result[i] = sum & UNSIGNED_INTEGER_MASK;
    }

    // Copy the remained value
    for (; i < aLength; i++) {
        result[i] = a[i];
    }

    // Extend result if necessary
    if (sum > UNSIGNED_INTEGER_MASK) {
        z = new unsigned int[aLength + 1];
        std::memcpy(z, result, aLength * UNSIGNED_INTEGER_BYTES);
        (z)[aLength] = 1;
        return aLength + 1;
    }

    z = new unsigned int[aLength];
    std::memcpy(z, result, aLength * UNSIGNED_INTEGER_BYTES);
    return aLength;
}

BigInteger BigInteger::operator+(const BigInteger &other) const {
    // Skip zero cases
    if (other.sign == 0) {
        return BigInteger{*this};
    }
    if (this->sign == 0) {
        return BigInteger{other};
    }
    
    int zLength;
    unsigned int *z = nullptr;
    
    // Use addition when this and other have the same sign
    if (this->sign == other.sign) {
        zLength = add(this->number, this->length, other.number, other.length, z);
        return BigInteger{this->sign, z, zLength};
    }
    
    int compare = this->compareAbsolute(other);
    if (compare == 0) {
        // Skip zero case
        return BigInteger{ZERO};
    }
    
    // Use subtraction otherwise
    zLength = compare > 0 ?
              subtract(this->number, this->length, other.number, other.length, z) :
              subtract(other.number, other.length, this->number, this->length, z);
    return BigInteger{compare > 0 ? this->sign : other.sign, z, zLength};
}

void BigInteger::selfAddByTwo() {
    static const int x = 2;
    unsigned long long sum = (this->number[0] & UNSIGNED_LONG_LONG_MASK) + x;
    this->number[0] = sum & UNSIGNED_INTEGER_MASK;
    for (int i = 1; i < length; i++) {
        if (sum <= UNSIGNED_INTEGER_BITS) {
            break;
        }
        sum = (sum >> UNSIGNED_INTEGER_BITS) + this->number[i];
        this->number[i] = sum & UNSIGNED_INTEGER_MASK;
    }

    // Extend itself if necessary
    if (sum > UNSIGNED_INTEGER_MASK) {
        auto *z = new unsigned int[length + 1];
        std::memcpy(z, this->number, length * UNSIGNED_INTEGER_BYTES);
        z[length] = 1;
        this->number = z;
    }
}

// ========================================
// End of BigInteger addition
// ========================================


// ========================================
// Begin of BigInteger subtraction
// ========================================

int BigInteger::subtract(
        const unsigned int *x,
        int xLength,
        const unsigned int *y,
        int yLength,
        unsigned int *&z) {

    auto *result = new unsigned int[xLength];

    // Do subtraction and reserve the insufficient value
    long long difference = 0;
    for (int i = 0; i < yLength; i++) {
        difference = (x[i] & LONG_LONG_MASK) -
                     (y[i] & LONG_LONG_MASK) +
                     // Add -1's complement if there exists borrow before
                     (difference >> UNSIGNED_INTEGER_BITS);
        result[i] = difference & UNSIGNED_INTEGER_MASK;
    }

    // Check and borrow the insufficient value
    int i = yLength;
    for (; difference < 0 && i < xLength; i++) {
        difference = (x[i] & LONG_LONG_MASK) + (difference >> UNSIGNED_INTEGER_BITS);
        result[i] = difference & UNSIGNED_INTEGER_MASK;
    }

    // Copy the remained value
    for (; i < xLength; i++) {
        result[i] = x[i];
    }

    return stripLeadingZeros(result, z, xLength);
}

BigInteger BigInteger::operator-(const BigInteger &other) const {
    // Skip zero cases
    if (other.sign == 0) {
        return BigInteger{*this};
    }
    if (this->sign == 0) {
        BigInteger result = BigInteger{other};
        result.sign *= -1;
        return result;
    }

    int zLength;
    unsigned int *z = nullptr;

    // Use addition when this and other have different sign
    if (this->sign != other.sign) {
        zLength = add(this->number, this->length, other.number, other.length, z);
        return BigInteger{this->sign, z, zLength};
    }

    int compare = this->compareAbsolute(other);
    if (compare == 0) {
        // Skip zero case
        return BigInteger{ZERO};
    }

    // Use subtraction when this and other have the same sign
    zLength = compare > 0 ?
              subtract(this->number, this->length, other.number, other.length, z) :
              subtract(other.number, other.length, this->number, this->length, z);

    return BigInteger{compare > 0 ? this->sign : -this->sign, z, zLength};
}

BigInteger BigInteger::operator-(int x) const {
    BigInteger result = BigInteger{*this};

    int index = 1;
    bool borrow = result.number[0] == 0;
    result.number[0] -= x;
    while (borrow) {
        borrow = result.number[index] == 0;
        --result.number[index];
    }

    result.length = stripLeadingZeros(result.number, result.number, result.length);
    result.bitLength = calcBitLength(result.number, result.length);
    return result;
}

// ========================================
// End of BigInteger subtraction
// ========================================


// ========================================
// Begin of BigInteger multiplication
// ========================================

int BigInteger::multiply(
        const unsigned int *x,
        int xLength,
        const unsigned int *y,
        int yLength,
        unsigned int *&z) {

    auto *result = new unsigned int[xLength + yLength];
    std::memset(result, 0, (xLength + yLength) * UNSIGNED_INTEGER_BYTES);

    for (int i = 0; i < xLength; i++) {
        unsigned long long prod = 0;
        for (int j = 0; j < yLength; j++) {
            prod = (x[i] & UNSIGNED_LONG_LONG_MASK) *
                   y[j] +
                   result[i + j] +
                   (prod >> UNSIGNED_INTEGER_BITS);
            result[i + j] = prod & UNSIGNED_INTEGER_MASK;
        }
        if (prod > UNSIGNED_INTEGER_MASK) {
            result[i + yLength] += (prod >> UNSIGNED_INTEGER_BITS);
        }
    }

    return stripLeadingZeros(result, z, xLength + yLength);
}

BigInteger BigInteger::operator*(const BigInteger &other) const {
    // Skip zero cases
    if (this->length == 0 || other.length == 0) {
        return BigInteger{ZERO};
    }

    unsigned int *z = nullptr;
    int zLength = multiply(this->number, this->length, other.number, other.length, z);
    return BigInteger{this->sign * other.sign, z, zLength};
}

// ========================================
// End of BigInteger multiplication
// ========================================


// ========================================
// Begin of BigInteger mod
// ========================================

unsigned int BigInteger::modOneWord(
        const unsigned int *x,
        int xLength,
        unsigned int y,
        unsigned int *&q,
        int &qLength) {

    // Solve this special case by QinJiushao's algorithm

    auto *quotient = new unsigned int[xLength];
    unsigned long long remainder = 0;
    for (int i = xLength - 1; i >= 0; i--) {
        remainder = (remainder << UNSIGNED_INTEGER_BITS) | x[i];
        quotient[i] = remainder / y;
        remainder = remainder - (quotient[i] & UNSIGNED_LONG_LONG_MASK) * y;
    }

    qLength = stripLeadingZeros(quotient, q, xLength);
    return remainder;
}

unsigned int BigInteger::operator%(const unsigned int divisor) const {
    int qLength;
    unsigned int *quotient = nullptr;
    return modOneWord(this->number, this->length, divisor, quotient, qLength);
}

int BigInteger::mod(
        const unsigned int *x,
        int xLength,
        const unsigned int *y,
        int yLength,
        int mark,
        unsigned int *&z) {

    // Implementation of long division algorithm in Knuth's
    // 'The Art of Computer Programming', Vol 2. section 4.3.1

    /* D1. Normalize, ensure that y[0] is not less than 2^31 */
    int shift = countLeadingZeros(y, yLength);
    unsigned int *remainder = nullptr;
    int nAddM = leftShiftAndAddLeadingZero(x, remainder, xLength, shift);
    unsigned int *divisor = nullptr;
    int n = leftShiftAndAddLeadingZero(y, divisor, yLength, shift);

    /* D2. Initialize iterator j and quotient array */
    int m = nAddM - n;
    auto *quotient = new unsigned int[m + 1];
    unsigned int vFirst = divisor[n - 1];
    unsigned int vSecond = divisor[n - 2];
    unsigned long long upperBound = (vFirst & UNSIGNED_LONG_LONG_MASK) << UNSIGNED_INTEGER_BITS;
    for (int j = m; j >= 0; j--) {
        /* D3. Calculate qHat */
        unsigned int uFirst = remainder[j + n];
        unsigned int uSecond = remainder[j + n - 1];
        unsigned long long uDividend = (((uFirst & LONG_LONG_MASK) << UNSIGNED_INTEGER_BITS) | uSecond);
        bool skipCorrect = uDividend >= upperBound;
        unsigned long long qHat = skipCorrect ?
                                  UNSIGNED_INTEGER_MASK :
                                  uDividend / vFirst;
        if (qHat == 0) {
            // Always have qHat >= q >= 0
            quotient[j] = 0;
            continue;
        }
        if (!skipCorrect) {
            unsigned int uThird = remainder[j + n - 2];
            unsigned long long rHat = uDividend - qHat * vFirst;
            if (qHat * vSecond > ((rHat << UNSIGNED_INTEGER_BITS) | uThird)) {
                --qHat;
            }
        }

        /* D4. Multiplication and subtraction */
        auto *vHat = new unsigned int[n + 1];
        unsigned long long sum = 0;
        for (int i = 0; i < n; i++) {
            sum = qHat * divisor[i] + (sum >> UNSIGNED_INTEGER_BITS);
            vHat[i] = sum & UNSIGNED_INTEGER_MASK;
        }
        vHat[n] = sum >> UNSIGNED_INTEGER_BITS;

        long long difference = 0;
        for (int i = 0; i <= n; i++) {
            difference = (remainder[j + i] & LONG_LONG_MASK) -
                         vHat[i] +
                         (difference >> UNSIGNED_INTEGER_BITS);
            remainder[j + i] = difference & UNSIGNED_INTEGER_MASK;
        }

        /* D5. Test remainder */
        if (difference < 0) {
            /* D6. Add back */
            --qHat;
            sum = 0;
            for (int i = 0; i <= n; i++) {
                sum = (remainder[j + i] & LONG_LONG_MASK) +
                      divisor[i] +
                      (sum >> UNSIGNED_INTEGER_BITS);
                remainder[j + i] = sum & UNSIGNED_INTEGER_MASK;
            }
        }

        quotient[j] = qHat;
    } /* D7. Loop on j */

    /* D8. Denormalize */
    if (mark == 1) {
        // Return remainder
        return rightShift(remainder, z, nAddM, shift);
    } else {
        // Return quotient
        return stripLeadingZeros(quotient, z, m + 1);
    }
}

BigInteger BigInteger::operator%(const BigInteger &other) const {
    // Ensure this > other
    int compare = this->compareAbsolute(other);
    if (compare < 0) {
        return BigInteger{*this};
    } else if (compare == 0) {
        return BigInteger{ZERO};
    }

    if (other.length == 1) {
        int qLength;
        unsigned int *quotient = nullptr;
        return BigInteger{modOneWord(
                this->number,
                this->length,
                other.number[0],
                quotient,
                qLength)};
    }

    unsigned int *z = nullptr;
    int zLength = mod(this->number, this->length, other.number, other.length, 1, z);
    return BigInteger{1, z, zLength};
}

BigInteger BigInteger::operator/(const BigInteger &other) const {
    // Ensure this > other
    int compare = this->compareAbsolute(other);
    if (compare < 0) {
        return BigInteger{ZERO};
    } else if (compare == 0) {
        return BigInteger{ONE};
    }

    if (other.length == 1) {
        int qLength;
        unsigned int *quotient = nullptr;
        modOneWord(this->number, this->length, other.number[0], quotient, qLength);
        return BigInteger{1, quotient, qLength};
    }

    unsigned int *z = nullptr;
    int zLength = mod(this->number, this->length, other.number, other.length, -1, z);
    return BigInteger{1, z, zLength};
}

// ========================================
// End of BigInteger mod
// ========================================


// ========================================
// Begin of BigInteger inverse
// ========================================

void BigInteger::extendGCD(const BigInteger &a, const BigInteger &b, BigInteger &x, BigInteger &y) {
    if (b.isZero()) {
        x = BigInteger(1);
        y = BigInteger(0);
        return;
    }

    extendGCD(b, a % b, y, x);
    y = y - (a / b * x);
}

BigInteger BigInteger::multiplicativeInverse(const BigInteger &mod) const {
    BigInteger result, y;
    extendGCD(*this, mod, result, y);
    while (result.sign == -1) {
        result = result + mod;
    }
    return result;
}

// ========================================
// End of BigInteger inverse
// ========================================


// ========================================
// Begin of BigInteger prime sieve
// ========================================

BigInteger BigInteger::generateBigPrime(int bitLength) {
    BigInteger p = randomBigInteger(bitLength);
    p.number[0] |= 1;
    SmallPrimeSieve sieve = SmallPrimeSieve{p};
    while (true) {
        if (sieve.isCandidate() && p.isPrime()) {
            return p;
        }

        p.selfAddByTwo();
        if (p.bitLength > bitLength) {
            p = randomBigInteger(bitLength);
            p.number[0] |= 1;
        }
        sieve.selfAddByTwo();
    }
}

bool BigInteger::isPrime() const {
    // Implementation of Miller-Rabin algorithm in Wikipedia
    // https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test

    // Find s > 0 and d odd > 0 such that this - 1 = 2^s * d
    const BigInteger thisMinusOne = *this - 1;
    BigInteger d = BigInteger{thisMinusOne};
    int s = countTailingZeros(d.number, d.length);
    d.length = rightShift(d.number, d.number, d.length, s);
    d.bitLength = calcBitLength(d.number, d.length);

    const static int iteration = 10;
    for (int i = 0; i < iteration; i++) {
        // Generate a in range [2, n - 1)
        BigInteger a = randomBigInteger(this->bitLength);
        while (a.compareAbsolute(thisMinusOne) >= 0 || a.compareAbsolute(2) < 0) {
            a = randomBigInteger(this->bitLength);
        }

        BigInteger y;
        BigInteger x = a.bigPowMod(d, *this);
        for (int j = 0; j < s; j++) {
            y = x * x % *this;
            // this is composite if y == 1 and x != 1 and x != n - 1
            if (y.compareAbsolute(1) == 0 &&
                x.compareAbsolute(1) != 0 &&
                x.compareAbsolute(thisMinusOne) != 0) {
                return false;
            }
            x = y;
        }

        // this is composite if y != 1
        if (y.compareAbsolute(1) != 0) {
            return false;
        }
    }

    return true;
}

BigInteger BigInteger::bigPowMod(const BigInteger &pow, const BigInteger &mod) const {
    BigInteger base = BigInteger{*this};
    BigInteger result = BigInteger{1};

    for (int i = 0; i < pow.length; i++) {
        unsigned int curPow = pow.number[i];
        for (int j = 0; j < UNSIGNED_INTEGER_BITS; j++) {
            if (curPow & 1) {
                result = result * base % mod;
            }
            base = base * base % mod;
            curPow >>= 1;
        }
    }

    return result;
}

// ========================================
// End of BigInteger prime sieve
// ========================================


// ========================================
// Begin of BigInteger I/O
// ========================================

std::string BigInteger::toString(const int radix) const {
    std::string result;

    int charPerBlock;
    switch (radix) {
        case HEXADECIMAL_RADIX:
            charPerBlock = UNSIGNED_INTEGER_BITS / HEXADECIMAL_BITS;
            break;
        case ASCII_RADIX:
        default:
            charPerBlock = UNSIGNED_INTEGER_BITS / ASCII_BITS;
            break;
    }

    for (int i = 0; i < this->length; i++) {
        unsigned int tail = this->number[i];
        for (int j = 0; j < charPerBlock; j++) {
            char tailChar;
            switch (radix) {
                case HEXADECIMAL_RADIX:
                    tailChar = HEXADECIMAL_MAP.find(tail & HEXADECIMAL_MASK)->second;
                    tail >>= HEXADECIMAL_BITS;
                    break;
                case ASCII_RADIX:
                default:
                    tailChar = (char) (tail & ASCII_MASK);
                    tail >>= ASCII_BITS;
                    break;
            }

            result += tailChar;
        }
    }

    std::reverse(result.begin(), result.end());
    return result;
}

void BigInteger::write(std::ofstream &out) const {
    out << this->toString(HEXADECIMAL_RADIX) << std::endl;
}

// ========================================
// End of BigInteger I/O
// ========================================


// ========================================
// Begin of BigInteger cipher
// ========================================

int BigInteger::encryptPlaintext(
        const std::string &plaintext,
        BigInteger *&ciphertext,
        const BigInteger &e,
        const BigInteger &n) {

    int charPerBigInteger = (int) ((n.bitLength - 1) / ASCII_BITS);
    int ciphertextLength = (int) ((plaintext.length() - 1) / charPerBigInteger + 1);
    ciphertext = new BigInteger[ciphertextLength];

    // Split and encrypt
    for (int i = 0; i < ciphertextLength; i++) {
        int plainHead = i * charPerBigInteger;
        int plainLength = std::min(charPerBigInteger, (int) (plaintext.length() - plainHead));
        BigInteger plain = BigInteger(ASCII_RADIX, plaintext.substr(plainHead, plainLength));
        ciphertext[i] = plain.bigPowMod(e, n);
    }

    return ciphertextLength;
}

std::string BigInteger::decryptCiphertext(
        const int plaintextLength,
        const BigInteger *ciphertext,
        const int ciphertextLength,
        const BigInteger &d,
        const BigInteger &n) {

    std::string plaintext;

    // Decrypt and join
    int remainChar = plaintextLength;
    int charPerBigInteger = (int) ((n.bitLength - 1) / ASCII_BITS);
    for (int i = 0; i < ciphertextLength; i++) {
        std::string plain = ciphertext[i].bigPowMod(d, n).toString(ASCII_RADIX);
        if (remainChar >= charPerBigInteger) {
            plain = plain.substr(plain.length() - charPerBigInteger, charPerBigInteger);
            remainChar -= charPerBigInteger;
        } else {
            plain = plain.substr(plain.length() - remainChar, remainChar);
            remainChar = 0;
        }
        plaintext += plain;
    }

    return plaintext;
}

BigInteger BigInteger::signSignature(const unsigned int hashcode, const BigInteger &d, const BigInteger &n) {
    return BigInteger(hashcode).bigPowMod(d, n);
}

unsigned int BigInteger::decryptSignature(const BigInteger &signature, const BigInteger &e, const BigInteger &n) {
    return signature.bigPowMod(e, n).number[0];
}

// ========================================
// End of BigInteger cipher
// ========================================


int BigInteger::getBitLength() const {
    return bitLength;
}

bool BigInteger::isZero() const {
    return this->sign == 0;
}
