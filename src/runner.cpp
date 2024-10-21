#include <iostream>
#include "lib/sieve.cpp"

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
    
    int n = getUserData();

    auto factors = factorize(n);
    auto &[p, q] = factors;
    std::cout << "The factors of " << n << " are " << p << " and " << q << std::endl;
}