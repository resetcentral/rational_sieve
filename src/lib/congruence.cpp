#include "utils.cpp"
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * List of booleans representing whether each element in a list
 * of integers is odd.
 */
class Parity {
    private:
    std::vector<bool> oddBits; // false = even, true = odd

    public:
    Parity(std::vector<int> values) {
        oddBits.reserve(values.size());
        for (auto value : values) {
            auto odd = (value & 1) == 1;
            oddBits.push_back(odd);
        }
    }

    /**
     * Return true if all elements in this Parity are even.
     * 
     * @param parity Parity to test
     * 
     * @return bool whether all elements in the Parity are even
     */
    bool allEven() {
        for (auto bit : oddBits) {
            if (bit) {
                return false;
            }
        }
        return true;
    }

    /**
     * Perform an element-wise XOR with the given Parity.
     * 
     * @pre `other` must have the same number of elements as this Parity
     * 
     * @param other the Parity to XOR with this Parity
     * 
     */
    void pXor(const Parity& other) {
        for (auto i = 0; i < oddBits.size(); i++) {
            oddBits[i] = oddBits[i] != other.oddBits[i];
        }
    }

    /**
     * Get the index of the odd value in the Parity.
     * 
     * @return the index of the first odd value or
     *          -1 if there are no odd values
     */
    int firstOddIdx() {
        for (int idx = 0; idx < oddBits.size(); idx++) {
            if (oddBits[idx]) {
                return idx;
            }
        }

        return -1;
    }

    const std::vector<bool>& bits() {
        return oddBits;
    }
};

/**
 * A congruence between two values mod some n.
 * 
 * left ≡ right mod n
 * 
 * Each element of the pair represents a list of exponents assigned to the
 * first element.size() prime numbers. Eg. [1, 2, 3] represents 2^1 * 3^2 * 5^3
 */
class Congruence {
    private:
    std::shared_ptr<const std::vector<int>> primes;
    std::vector<int> left, right;

    public:
    const int n;

    Congruence(std::shared_ptr<const std::vector<int>> primes, int n, std::vector<int> left, std::vector<int> right) :
        primes{primes},
        n{n},
        left{left},
        right{right} {
    }

    /**
     * Divide common factors from each side of the congruence.
     * 
     * @pre All elements of this->primes must be coprime with this->n.
     */
    void simplify() {
        for (auto i = 0; i < left.size(); i++) {
            if (left[i] > right[i]) {
                left[i] -= right[i];
            } else {
                right[i] -= left[i];
            }
        }
    }

    /**
     * Square root both sides of the congruence.
     * 
     * @pre All exponents in the Congruence must be even
     */
    void sqrt() {
        for (auto i = 0; i < left.size(); i++) {
            left[i] /= 2;
            right[i] /= 2;
        }
    }

    /**
     * Evaluate the congruence.
     * 
     * @return pair of integers representing the left and right values of the congruence
     */
    std::pair<int, int> evaluate() {
        auto leftVal = 1;
        auto rightVal = 1;

        for (auto i = 0; i < primes->size(); i++) {
            leftVal *= pow(primes->at(i), left[i]);
            rightVal *= pow(primes->at(i), right[i]);
        }

        return { leftVal, rightVal };
    }

    /**
     * Get the parities of each side of the congruence.
     * 
     * @return pair of Parity objects representing the even/oddness of each
     * side of this Congruence
     */
    std::pair<Parity, Parity> parities() {
        Parity leftParity(left);
        Parity rightParity(right);

        return { leftParity, rightParity };
    }

    /**
     * Compute the product of the Congruences in congruenceList.
     * 
     * @pre all Congruences in `congruenceList` must have the same values for
     * `primes` and `n`, and must have the same number of elements in `left`
     *  and `right`.
     * 
     * @param congruenceList a list of Congruences
     * 
     * @return product of the list of Congruences
     */
    static Congruence product(const std::vector<Congruence>& congruenceList) {
        if (congruenceList.size() == 0) {
            throw std::invalid_argument("Congruence list cannot be size 0");
        }

        std::vector<int> productLeft(congruenceList[0].left.size());
        std::vector<int> productRight(congruenceList[0].right.size());
        Congruence product(congruenceList[0].primes, congruenceList[0].n, productLeft, productRight);

        for (auto& cg : congruenceList) {
            for (auto i = 0; i < cg.left.size(); i++) {
                product.left[i] += cg.left[i];
                product.right[i] += cg.right[i];
            }
        }

        return product;
    }
};