#include <iostream>
#include <vector>

void printFactorization(const std::vector<int>& primes, const std::vector<int>& exponents) {
    std::cout << " ";
    for (auto idx = 0; idx < primes.size(); idx++) {
        std::cout << primes[idx] << "^" << exponents[idx] << " ";
    }
}

std::pair<bool, std::vector<int>> factorize(int z, std::vector<int> primes) {
    std::vector<int> exponents(primes.size());

    for (auto i = 0; i < primes.size(); i++) {
        auto prime = primes[i];
        while (z % prime == 0) {
            z /= prime;
            exponents[i]++;
        }
    }
    
    auto smooth = z == 1;

    return { smooth, exponents };
}

using Congruence = std::pair<std::vector<int>, std::vector<int>>;

std::vector<int> sieve(int n, const std::vector<int>& primes) {
    std::vector<Congruence> options;
    for (int z = 2; z < n; z++) {
        auto zResult = factorize(z, primes);
        auto &[zSmooth, zExponents] = zResult;
        if (zSmooth) {
            auto znResult = factorize(z+n, primes);
            auto &[znSmooth, znExponents] = znResult;
            if (znSmooth) {
                std::cout << z << " (";
                printFactorization(primes, zExponents);
                std::cout << ") === " << (z+n) << " (";
                printFactorization(primes, znExponents);
                std::cout << ")" << std::endl;
                options.push_back({zExponents, znExponents});
            }
        }
        // check for even powers
    }
    // gcd

    return primes;
}

int getUserData() {
    int n;
    do {
        std::cout << "Enter a positive integer to factorize: ";
        std::cin.clear();
        std::cin >> n;
    } while (std::cin.fail() || n < 0);

    return n;
}

int main() {
    const std::vector<int> primes = { 2, 3, 5, 7 };
    int n = getUserData();

    std::vector<int> factors = sieve(n, primes);
}