#include <iostream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <algorithm>

using namespace std;

bool is_all_digits(const string& str) {
    for (char ch : str) {
        if (!isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    // get command line input. arg1 == worker count, arg2 == upper bound for prime search
    if (argc < 3) { 
        cout << "Too few arguments - terminating...\n";
        return 1; // too few arguments
    }
    string tmp1 = argv[1]; 
    string tmp2 = argv[2]; 

    //check argument validity
    if (!is_all_digits(tmp1) || !is_all_digits(tmp2)) { 
        cout << "Arguments must be positive integers - terminating...\n";
        return 1; // quit if the arguments aren't positive integers
    }
    int workers = stoi(tmp1);
    int bound = stoi(tmp2);
    if (workers == 0) { 
        cout << "Cannot have zero workers - terminating...\n";
        return 1; // need workers
    }

    // check bound validity
    if (bound < 2) {
        cout << "No primes found\n";
        return 0; // 2 is the lowest prime
    }

    // determine the ranges that the workers should search
    int quotient = bound / workers; // quotient is the number of primes that is divided amongst the workers
    int up_bound = quotient; // used in following loop in order to set bounds, changes with each iterations
    vector<vector<int>> ranges(workers, vector<int>(2)); // each row will represent a worker, each column will represent their lower and upper bound
    for (auto &range : ranges) {
        range[0] = up_bound - quotient + 1; //range[0] is the lower bound of that worker
        range[1] = up_bound; //range[1] is the upper bound of that worker
        up_bound += quotient;
        /*
        ranges will end up being
        worker 1: 1  to  quotient
        worker 2: quotient + 1  to  2 * quotient
        ...
        worker n: (n-1) * quotient  to  n * quotient 
        */
    }

    // create/truncate a file for workers to write to
    ofstream file("primes.txt");
    file.close();

    // create and deploy workers
    for (int w = 0; w < workers; w++) {
        int pid = fork();
        if (pid < 0) {
            // fork failed
            cout << "Fork failed.\n";
            return 1; 
        }
        if (pid == 0) { // is worker
            int tmp = execl("worker", "./worker", to_string(ranges[w][0]).c_str(), to_string(ranges[w][1]).c_str(), "w", (char*)NULL); 
            // execute worker program with given arguments as bounds. 
            // The empty string arg is used to let the worker know that it was created by a boss and not by the user directly, and should therefore print to a file
            if (tmp == -1) {
                cout << "Worker failed to execute - terminating...\n";
                return 1;
            }
        }
    }

    // Wait for all workers to get laid off in this economy, then sort and print results
    wait(NULL);

    // Put primes into a vector
    ifstream output_file("primes.txt");
    vector<int> vec;
    int prime;
    while (output_file >> prime) 
        vec.push_back(prime);
    // Sort
    sort(vec.begin(), vec.end());
    // Print
    cout << "Primes found : ";
    for (int prime : vec)
        cout << prime;
    cout << endl;

    return 0;
}