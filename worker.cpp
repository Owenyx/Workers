#include <vector>

int main() {
    // acquire the bounds for the prime search
    // replace with cml args
    int low_bound = 5;
    int up_bound = 25;
    // both ranges are inclusive

    // create a vector with all numbers within the range
    vec = vector<int> (up_bound-low_bound+1, 0) // +1 since upper bound is inclusive
    for (int i = 0; i < vec.size(); i++) vec[i] = i + low_bound; // load vector

    // Eratosthenes's sieve algorithm
    // All values that are not prime are removed from the vector
    // First need to find the lowest prime
    int lowest;
    if (low_bound <= 2) lowest = 2;
    for(iter = vec.begin(); iter < vec.end(); iter++) {
        
        if (*iter < 
    }
}