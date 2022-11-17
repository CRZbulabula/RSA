//
// Created by Yongzao Dan on 2022/11/12.
//

#include <fstream>

#include "gtest/gtest.h"

#include "BigInteger.h"
#include "rsa.h"

class PerformanceTests: public::testing::Test {

protected:

    static const int BATCH_SIZE = 100;
    constexpr static const double CLOCKS_PER_MS = 1000;

    void SetUp() override {

    }

    void TearDown() override {

    }
};

static double getSigma(const double *cost, double avg, int length) {
    double sigma = 0;
    for (int i = 0; i < length; i++) {
        double base = cost[i] - avg;
        sigma = sigma + base * base;
    }
    return sigma / (double) length;
}

TEST_F(PerformanceTests, test576) {
    double totalCost = 0;
    double minimum = (1 << 30), maximum = 0;
    auto *cost = new double[BATCH_SIZE];
    for (int i = 0; i < BATCH_SIZE; i++) {
        auto curStart = clock();
        BigInteger n, e, d;
        generateRSANumbers(n, RSA576, e, true, d);
        auto curEnd = clock();

        cost[i] = (double) (curEnd - curStart) / CLOCKS_PER_MS;
        minimum = std::min(minimum, cost[i]);
        maximum = std::max(maximum, cost[i]);
        totalCost += cost[i];
    }

    double avg = totalCost / (double) BATCH_SIZE;
    double sigma = getSigma(cost, avg, BATCH_SIZE);

    std::cout << std::endl << "Generating RSA-576 public/private keys costs: " << std::endl;
    std::cout << "Minimum: " << std::setprecision(3) << minimum << " ms." << std::endl;
    std::cout << "Maximum: " << std::setprecision(3) << maximum << " ms." << std::endl;
    std::cout << "Average: " << std::setprecision(3) << avg << " ms." << std::endl;
    std::cout << "Sigma: " << std::setprecision(3) << sigma << std::endl;
}

TEST_F(PerformanceTests, test640) {
    double totalCost = 0;
    double minimum = (1 << 30), maximum = 0;
    auto *cost = new double[BATCH_SIZE];
    for (int i = 0; i < BATCH_SIZE; i++) {
        auto curStart = clock();
        BigInteger n, e, d;
        generateRSANumbers(n, RSA640, e, true, d);
        auto curEnd = clock();

        cost[i] = (double) (curEnd - curStart) / CLOCKS_PER_MS;
        minimum = std::min(minimum, cost[i]);
        maximum = std::max(maximum, cost[i]);
        totalCost += cost[i];
    }

    double avg = totalCost / (double) BATCH_SIZE;
    double sigma = getSigma(cost, avg, BATCH_SIZE);

    std::cout << std::endl << "Generating RSA-640 public/private keys costs: " << std::endl;
    std::cout << "Minimum: " << std::setprecision(3) << minimum << " ms." << std::endl;
    std::cout << "Maximum: " << std::setprecision(3) << maximum << " ms." << std::endl;
    std::cout << "Average: " << std::setprecision(3) << avg << " ms." << std::endl;
    std::cout << "Sigma: " << std::setprecision(3) << sigma << std::endl;
}

TEST_F(PerformanceTests, test768) {
    double totalCost = 0;
    double minimum = (1 << 30), maximum = 0;
    auto *cost = new double[BATCH_SIZE];
    for (int i = 0; i < BATCH_SIZE; i++) {
        auto curStart = clock();
        BigInteger n, e, d;
        generateRSANumbers(n, RSA768, e, true, d);
        auto curEnd = clock();

        cost[i] = (double) (curEnd - curStart) / CLOCKS_PER_MS;
        minimum = std::min(minimum, cost[i]);
        maximum = std::max(maximum, cost[i]);
        totalCost += cost[i];
    }

    double avg = totalCost / (double) BATCH_SIZE;
    double sigma = getSigma(cost, avg, BATCH_SIZE);

    std::cout << std::endl << "Generating RSA-768 public/private keys costs: " << std::endl;
    std::cout << "Minimum: " << std::setprecision(3) << minimum << " ms." << std::endl;
    std::cout << "Maximum: " << std::setprecision(3) << maximum << " ms." << std::endl;
    std::cout << "Average: " << std::setprecision(3) << avg << " ms." << std::endl;
    std::cout << "Sigma: " << std::setprecision(3) << sigma << std::endl;
}

TEST_F(PerformanceTests, test896) {
    double totalCost = 0;
    double minimum = (1 << 30), maximum = 0;
    auto *cost = new double[BATCH_SIZE];
    for (int i = 0; i < BATCH_SIZE; i++) {
        auto curStart = clock();
        BigInteger n, e, d;
        generateRSANumbers(n, RSA896, e, true, d);
        auto curEnd = clock();

        cost[i] = (double) (curEnd - curStart) / CLOCKS_PER_MS;
        minimum = std::min(minimum, cost[i]);
        maximum = std::max(maximum, cost[i]);
        totalCost += cost[i];
    }

    double avg = totalCost / (double) BATCH_SIZE;
    double sigma = getSigma(cost, avg, BATCH_SIZE);

    std::cout << std::endl << "Generating RSA-896 public/private keys costs: " << std::endl;
    std::cout << "Minimum: " << std::setprecision(3) << minimum << " ms." << std::endl;
    std::cout << "Maximum: " << std::setprecision(3) << maximum << " ms." << std::endl;
    std::cout << "Average: " << std::setprecision(3) << avg << " ms." << std::endl;
    std::cout << "Sigma: " << std::setprecision(3) << sigma << std::endl;
}

TEST_F(PerformanceTests, test1024) {
    double totalCost = 0;
    double minimum = (1 << 30), maximum = 0;
    auto *cost = new double[BATCH_SIZE];
    for (int i = 0; i < BATCH_SIZE; i++) {
        auto curStart = clock();
        BigInteger n, e, d;
        generateRSANumbers(n, RSA1024, e, true, d);
        auto curEnd = clock();

        cost[i] = (double) (curEnd - curStart) / CLOCKS_PER_MS;
        minimum = std::min(minimum, cost[i]);
        maximum = std::max(maximum, cost[i]);
        totalCost += cost[i];
    }

    double avg = totalCost / (double) BATCH_SIZE;
    double sigma = getSigma(cost, avg, BATCH_SIZE);

    std::cout << std::endl << "Generating RSA-1024 public/private keys costs: " << std::endl;
    std::cout << "Minimum: " << std::setprecision(3) << minimum << " ms." << std::endl;
    std::cout << "Maximum: " << std::setprecision(3) << maximum << " ms." << std::endl;
    std::cout << "Average: " << std::setprecision(3) << avg << " ms." << std::endl;
    std::cout << "Sigma: " << std::setprecision(3) << sigma << std::endl;
}



TEST_F(PerformanceTests, test1536) {
    double totalCost = 0;
    double minimum = (1 << 30), maximum = 0;
    auto *cost = new double[BATCH_SIZE];
    for (int i = 0; i < BATCH_SIZE; i++) {
        auto curStart = clock();
        BigInteger n, e, d;
        generateRSANumbers(n, RSA1536, e, true, d);
        auto curEnd = clock();

        cost[i] = (double) (curEnd - curStart) / CLOCKS_PER_MS;
        minimum = std::min(minimum, cost[i]);
        maximum = std::max(maximum, cost[i]);
        totalCost += cost[i];
    }

    double avg = totalCost / (double) BATCH_SIZE;
    double sigma = getSigma(cost, avg, BATCH_SIZE);

    std::cout << std::endl << "Generating RSA-1536 public/private keys costs: " << std::endl;
    std::cout << "Minimum: " << std::setprecision(3) << minimum << " ms." << std::endl;
    std::cout << "Maximum: " << std::setprecision(3) << maximum << " ms." << std::endl;
    std::cout << "Average: " << std::setprecision(3) << avg << " ms." << std::endl;
    std::cout << "Sigma: " << std::setprecision(3) << sigma << std::endl;
}

TEST_F(PerformanceTests, test2048) {
    double totalCost = 0;
    double minimum = (1 << 30), maximum = 0;
    auto *cost = new double[BATCH_SIZE];
    for (int i = 0; i < BATCH_SIZE; i++) {
        auto curStart = clock();
        BigInteger n, e, d;
        generateRSANumbers(n, RSA2048, e, true, d);
        auto curEnd = clock();

        cost[i] = (double) (curEnd - curStart) / CLOCKS_PER_MS;
        minimum = std::min(minimum, cost[i]);
        maximum = std::max(maximum, cost[i]);
        totalCost += cost[i];

        std::cout << i << " " << cost[i] << std::endl;
    }

    double avg = totalCost / (double) BATCH_SIZE;
    double sigma = getSigma(cost, avg, BATCH_SIZE);

    std::cout << std::endl << "Generating RSA-2048 public/private keys costs: " << std::endl;
    std::cout << "Minimum: " << std::setprecision(3) << minimum << " ms." << std::endl;
    std::cout << "Maximum: " << std::setprecision(3) << maximum << " ms." << std::endl;
    std::cout << "Average: " << std::setprecision(3) << avg << " ms." << std::endl;
    std::cout << "Sigma: " << std::setprecision(3) << sigma << std::endl;
}