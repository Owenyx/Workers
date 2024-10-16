#include <iostream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <regex>
#include <filesystem>
#include <string>

using namespace std;
namespace fs = filesystem;

bool is_all_digits(const string& str) {
    for (char ch : str) {
        if (!isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}

// Takes a regex pattern and a path and deletes all files mathing the pattern
int delete_files(regex pattern, string path) {
    // Iterate over the files in the directory
    for (const auto& entry : fs::directory_iterator(path)) {
        // Get the file name
        string filename = entry.path().filename().string();

        // Check if the file name matches the pattern
        if (regex_match(filename, pattern)) {
            // Attempt to delete the file
            if (remove(entry.path().c_str()) != 0) {
                cerr << ("Error deleting file: " + filename);
                return 1;
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {

    // Delete all previous worker output files
    regex pattern(R"(worker_\d+_\d+)");
    string path = "."; // current directory
    if (delete_files(pattern, path))
        return 1; // File deletion failed


    // get command line input. arg1 == worker count, arg2 == upper bound for prime search
    if (argc < 3) { 
        cerr << "Too few arguments - terminating...\n";
        return 1; // too few arguments
    }
    string tmp1 = argv[1]; 
    string tmp2 = argv[2]; 

    //check argument validity
    if (!is_all_digits(tmp1) || !is_all_digits(tmp2)) { 
        cerr << "Arguments must be positive integers - terminating...\n";
        return 1; // quit if the arguments aren't positive integers
    }
    int workers = stoi(tmp1);
    int bound = stoi(tmp2);
    if (workers == 0) { 
        cerr << "Cannot have zero workers - terminating...\n";
        return 1; // need workers
    }

    // check bound validity
    if (bound < 2) {
        cout << "No primes found\n";
        return 0; // 2 is the lowest prime
    }

    // determine the ranges that the workers should search and assign them accordingly
    int quotient = bound / workers; // quotient is the number of primes that is divided amongst the workers
    
    // If there is less numbers in the range than there are workers, reduce the number of workers so each has something to do
    while (quotient == 0) {
        workers = bound;
        quotient = 1; // Each worker gets one number to check as there is the same number of workers as numbers
    }
    
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

    // Remainder of workload that isn't evenly distributed is given to last worker
    int remainder = bound % workers; 
    ranges[workers-1][1] += remainder; // add remainder to upper bound of last worker

    // create and deploy workers
    for (int w = 0; w < workers; w++) {
        int pid = fork();
        if (pid < 0) {
            // fork failed
            cerr << "Fork failed.\n";
            return 1; 
        }
        if (pid == 0) { // is worker
            int tmp = execl("worker", "./worker", to_string(ranges[w][0]).c_str(), to_string(ranges[w][1]).c_str(), "w", (char*)NULL); 
            // execute worker program with given arguments as bounds. 
            // The "w" arg is used to let the worker know that it was created by a boss and not by the user directly, and should therefore print to a file
            if (tmp == -1) {
                cerr << "Worker failed to execute - terminating...\n";
                return 1;
            }
        }
    }

    return 0;
}