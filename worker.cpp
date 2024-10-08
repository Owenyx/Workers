#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

bool is_all_digits(const string& str) {
    for (char ch : str) {
        if (!isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}

// Find all prime numbers in a given range using Eratosthenes's sieve algorithm, both ranges are inclusive
int main(int argc, char *argv[]) {
    // acquire the bounds from command line input for the prime search
    if (argc < 3) { 
        cout << "Too few arguments - terminating...\n";
        return 1; 
    }
    string tmp1 = argv[1]; // arg1 should be lower bound
    string tmp2 = argv[2]; // arg2 upper bound

    //check argument validity
    if (!is_all_digits(tmp1) || !is_all_digits(tmp2)) { 
        cout << "Arguments must be positive integers - terminating...\n";
        return 1; 
    }
    int low_bound = stoi(tmp1);
    int up_bound = stoi(tmp2);

    if (low_bound < 2) low_bound = 2; // 2 is the lowest prime

    // check bound validity
    if (up_bound < low_bound) { 
        cout << "Upper bound cannot be less than lower bound - terminating...\n";
        return 1;
    }

    // create a vector with all numbers within the range
    vector<int> vec(up_bound-low_bound+1, 0); // +1 since upper bound is inclusive
    for (int i = 0; i < vec.size(); i++) vec[i] = i + low_bound; // load vector

    /* 
    create a vector of all primes up to and including the square root of up_bound using sieve.
    this way we will use these prime numbers to perform the algorithm very efficiently on the given range, 
    as any value that is a multiple of any of the primes <= root is not prime
    */
    int root = (int)sqrt(up_bound);
    vector<int> divisors(root-1, 0); // root-1 as it will include root but not 1 or 0
    for (int i = 0; i < divisors.size(); i++) divisors[i] = i+2; // load vector. i+2 because the first number will be 2

    // now iterate through the vector, deleting every multiple of i
    for (int i = 0; i < divisors.size(); i++) 
        for(int j = i + 1; j < divisors.size(); j++)  // j starts at i + 1 as all values i and lower are prime
            if (divisors[j] % divisors[i] == 0) divisors.erase(divisors.begin() + j);

    // now divisors contains all prime numbers <= root

    // Now check which numbers in vec are divisible by the primes in divisors and erase them
    for (int i : divisors) 
        for(int j = 0; j < vec.size(); j++) 
            if (vec[j] != i && vec[j] % i == 0) vec.erase(vec.begin() + j);

    // write to the output file if the worker was created by a boss, denoted by a 'w' as argv[3]
    if (argc > 3 && argv[3] == "w") {
        fstream file("primes.txt", ios::in | ios::app);
        for (int i : vec) file << i << " ";
    }
    else { // the worker was created directly by a user rather than a boss
        cout << "Primes found : ";
        for (int i : vec) cout << i << " ";
        cout << endl;
    }

    return 0;
}