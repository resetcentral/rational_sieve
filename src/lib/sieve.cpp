#include <vector>
#include <numeric>
#include "congruence.cpp"
#include <iostream>

std::vector<Parity> calcuateParities(const std::vector<Congruence>& congruences) {
    std::vector<Parity> parities;

    for (auto cg : congruences) {
        auto results = cg.parities();
        auto &[left, right] = results;
        left.pXor(right);
        parities.push_back(left);
    }

    return parities;
}

// /**
//  * Convert a list of Parities into pseudo row-echelon form and return
//  * the set of Parities XORd together
//  */
// std::vector<std::set<int>> convertRowEchelon(std::vector<Parity>& parities) {
    
// }

std::vector<Congruence> findSquareSolutions(const std::vector<Congruence> congruences) {
    auto parities = calcuateParities(congruences);

    // Tracks which of the original parities need to be XOR'ed together to
    // construct each item in `parities`
    std::vector<std::set<int>> components;
    for (auto i = 0; i < parities.size(); i++) {
        components.push_back({ i });
    }

    for (auto i = 0; i < parities.size(); i++) {
        Parity& refParity = parities[i];

        // Find the first odd parity bit
        int firstOddIdx = refParity.firstOddIdx();
        if (firstOddIdx == -1) {
            continue; // All exponents are even, so skip this one
        }

        // XOR the parity lists, so that refParity is the only
        // one with a 1 at firstOddIdx
        for (auto j = i+1; j < parities.size(); j++) {
            if (parities[j].bits()[firstOddIdx]) {
                // If a Parity p is in both sets, it becomes p^2 which is always all even,
                // so it has no effect in the XOR. Symmetric difference removes these.
                components[j] = set_symmetric_difference(components[j], components[i]);
                
                parities[j].pXor(refParity);
            }
        }
    }

    // Collect solutions
    std::vector<std::set<int>> idxSets;
    for (auto i = 0; i < parities.size(); i++) {
        if (parities[i].allEven()) {
            idxSets.push_back(components[i]);
        }
    }

    std::vector<Congruence> solutions;
    for (auto idxSet : idxSets) {
        std::vector<Congruence> cl;
        for (auto idx : idxSet) {
            cl.push_back(congruences[idx]);
        }
        auto cProduct = Congruence::product(cl);
        solutions.push_back(cProduct);
    }

    return solutions;
}

/**
 * Check if the given Congruence solution gives non-trivial factors of n.
 * 
 * @param solution the Congruence to check
 * 
 * @return a pair containing the two found factors of n and a boolean
 * indicating whether the solution is non-trivial
 */
std::pair<std::pair<int, int>, bool> checkSolution(Congruence& solution) {
    solution.simplify();
    solution.sqrt();
    auto cSqrt = solution.evaluate();
    auto &[left, right] = cSqrt;
    auto n = solution.n;

    int p, q;
    auto nonTrivial = true;
    p = std::gcd(right-left, n);
    if (p != 1 && p != n) {
        q = n / p;
    } else {
        q = std::gcd(right+left, n);
        if (q != 1 && q != n) {
            p = n / q;
        } else {
            nonTrivial = false;
        }
    }

    return {{p, q}, nonTrivial};
}

std::pair<bool, std::vector<int>> primeFactors(int val, const std::vector<int>& primes) {
    std::vector<int> exponents(primes.size());

    for (auto i = 0; i < primes.size(); i++) {
        auto prime = primes[i];
        while (val % prime == 0) {
            val /= prime;
            exponents[i]++;
        }
    }

    auto smooth = val == 1;
    return { smooth, exponents };
}

/**
 * Sieve for values where z and z+n are B-smooth.
 * 
 * B-smooth values are those which have all of their prime factors in `primes`.
 * 
 * @param n number to add to each value tested for B-smoothness
 * @param primes list of primes to check for B-smoothness over
 * 
 * @return list of Congruences z ≡ z+n mod n where z and z+n are B-smooth
 */
std::vector<Congruence> sieve(int n, std::shared_ptr<const std::vector<int>>& primes) {
    std::vector<Congruence> congruences;
    
    for (auto z = 2; z < n && congruences.size() < primes->size()+10; z++) {
        auto zResult = primeFactors(z, *primes.get());
        auto &[zSmooth, zExponents] = zResult;
        if (zSmooth) {
            auto znResult = primeFactors(z+n, *primes.get());
            auto &[znSmooth, znExponents] = znResult;
            if (znSmooth) {
                Congruence cg (primes, n, zExponents, znExponents);
                congruences.push_back(cg);
            }
        }
    }

    return congruences;
}

/**
 * Find factors of the given number.
 * 
 * @param n number to factor
 * 
 * @return pair of factors of n
 */
std::pair<int, int> factorize(int n) {
    const std::vector<int> primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
    auto sharedPrimes = std::make_shared<const std::vector<int>>(primes);
    auto congruences = sieve(n, sharedPrimes);
    
    auto solutions = findSquareSolutions(congruences);
    for (auto solution : solutions) {
        auto result = checkSolution(solution);
        auto &[factors, nonTrivial] = result;
        if (nonTrivial) {
            return factors;
        }
    }

    return {1 , 1};
}