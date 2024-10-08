#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Find all prime numbers in a given range using Eratosthenes's sieve algorithm
int main() {
    // acquire the bounds for the prime search
    // replace with cml args
    int low_bound = 5;
    int up_bound = 25;
    // both ranges are inclusive

    // create a vector with all numbers within the range
    vector<int> vec(up_bound-low_bound+1, 0); // +1 since upper bound is inclusive
    for (int i = 0; i < vec.size(); i++) vec[i] = i + low_bound; // load vector

    /* 
    create a vector of all primes up to and including the square root of up_bound using sieve
    this way we will use these prime numbers to perform the algorithm very efficiently on the given range, 
    as any value that is a multiple of any of the primes <= root is not prime
    */
    int root = (int)sqrt(up_bound);
    vector<int> divisors(root-1, 0); // root-1 as it will include root but not 1 or 0
    for (int i = 0; i < divisors.size(); i++) divisors[i] = i+2; // load vector. i+2 because the first number will be 2

    // now iterate through the vector, deleting every multiple of i
    for (int i = 0; i < divisors.size(); i++) {
        for(auto iter = divisors.begin() + i; iter < divisors.end()-1; iter++) { // iterators needed for deletion, the + i is to start at i
            if (*(iter+1) % divisors[i] == 0) divisors.erase(iter+1);
            // if the NEXT value is divisible by a prime, erase it. Using the NEXT value ensures pointers won't become invalid. 
            // divisors.begin() == 2 so that never has to be checked
        }
    }
    // now divisors should contain all prime numbers <= root

    // Now check which numbers in vec are divisible by the primes in divisors
    for (int i = 0; i < divisors.size(); i++) {

        // since we don't check begin() in the next for loop due to avoiding invalid pointers, we must make sure it is a prime by deleting all prime numbers at the start of vec
        while(vec[0] % divisors[i] == 0) vec.erase(vec.begin()); 

        for(auto iter = vec.begin(); iter < vec.end()-1; iter++) { // iterators needed for deletion
            if (*(iter+1) % divisors[i] == 0) vec.erase(iter+1);
            // if the NEXT value is divisible by a prime, erase it. Using the NEXT value ensures pointers won't become invalid
        }
    }

    cout << "Primes found : ";
    for (int i : vec) cout << i;
}