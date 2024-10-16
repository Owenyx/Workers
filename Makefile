# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g

# Program files
FILE_1 = worker
FILE_2 = boss

# Default target: build both files seperately
all: $(FILE_1) $(FILE_2)

# Rule to build file_1 executable
$(FILE_1): $(FILE_1).cpp
	$(CXX) $(CXXFLAGS) $(FILE_1).cpp -o $(FILE_1)

# Rule to build file_2 executable
$(FILE_2): $(FILE_2).cpp
	$(CXX) $(CXXFLAGS) $(FILE_2).cpp -o $(FILE_2)

# Clean up generated files
clean:
	rm -f $(FILE_1) $(FILE_2) worker_*

# Phony targets to avoid file conflicts
.PHONY: all clean worker boss
