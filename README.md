    Complementary joke:
This program simulates the 9-5 experience, where the boss sends his workers out and then proceeds to do nothing.

Note: All bounds/ranges mentioned in this document are inclusive

    Overview:
The program finds all prime numbers from 1 to n by having a boss program create workers to complete the task.
The worker program computes all prime numbers within a given range.
The boss program creates workers and evenly divides the total range among the workers.
Any work that can't be evenly split (the remainder) is given the worker with the highest value range.

    Execution:
The program only works on Linux-based systems.
To run the program, navigate to the 'Workers' folder and enter the following lines:
make
./boss <worker count> <n>
Where worker count is number of workers you wish to divide the taks among
and n is the upper limit to which the program will search for primes.
The workers will each output to a file named after the range they searched.

Example: 
make
./boss 4 302
Worker 1 range: 1 to 75
Worker 2 range: 76 to 150
Worker 3 range: 151 to 225
Worker 4 range: 226 to 302 (Recieves remainder workload)

File names after execution:
worker_1_75
worker_76_150
worker_151_225
worker_226_302

If the number of workers is greater than the size of the range, the number of workers will be reduced to give each worker one number to check

The worker program can also be run standalone by entering.
make
./worker <lower bound> <upper bound>
The program will find the primes within the bounds, which must both be positive.
Running worker this way will NOT print to a file, it will instead print directly to the terminal.

The Makefile can also compile the boss and worker seperately with "make boss" and "make worker" respectively.
Enter "make clean" to clean up all generated files when desired.

All previous worker output files are deleted upon execution of the program.