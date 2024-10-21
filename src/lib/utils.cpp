#include <set>
#include <algorithm>

/**
 * Integer version of the pow() function.
 * 
 * @param base base
 * @param exp  exponent
 * 
 * @return base^exp
 */
int pow(int base, int exp) {
    auto result = 1;
    for (auto i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

/**
 * Convenience function for std::set_symmetric_difference().
 * 
 * The symmetric difference is the set of elements that are in either of set1
 * or set2 but not in both.
 * 
 * @param set1 first set
 * @param set2 second set
 * 
 * @return the symmetric difference of set1 and set2
 */
template <typename T>
std::set<T> set_symmetric_difference(const std::set<T>& set1, const std::set<T>& set2) {
    std::set<T> result;
    std::set_symmetric_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(result, result.begin()));

    return result;
}