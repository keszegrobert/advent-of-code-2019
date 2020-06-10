#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include "intcode_computer.h"

class ThrustComputer{
private:
    std::vector<int> opcodes;
    std::vector<int> best_phases;

public:
    int best_thrust = 0;

    ThrustComputer(std::vector<int> _mem){
        opcodes = _mem;
    }

    void run_with_phases(std::vector<int> phases){
        int previous_output = 0;
        for (auto phase: phases){
            std::vector<int> output;
            IntCodeComputer c(opcodes);
            c.compute_output_for_input({phase, previous_output}, output);
            previous_output = output.back();
        }

        if (best_thrust < previous_output){
            best_thrust = previous_output;
            best_phases = phases;
        }
    }

    void permute(std::vector<int> vec,int start, int end){
        if (start == end){
            run_with_phases(vec);
        }
        else{
            for (size_t i = start; i <= end; ++i){
                std::iter_swap(vec.begin()+start, vec.begin()+i);
                permute(vec, start+1, end);
                std::iter_swap(vec.begin()+start, vec.begin()+i);
            }
        }
    }
};

void test_sample(std::vector<int> opcodes, int expected){
    ThrustComputer c(opcodes);
    c.permute({0,1,2,3,4},0,4);
    if (c.best_thrust != expected){
        std::cout << "error in computing the thrust:"
                << ", it should be " << expected
                << ", but it was " << c.best_thrust
                << " " << std::endl;
    }
}

int main(int argc, char **argv) {

    test_sample({3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0}, 43210);
    test_sample({3,23,3,24,1002,24,10,24,1002,23,-1,23,
                101,5,23,23,1,24,23,23,4,23,99,0,0}, 54321);
    test_sample({3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
                1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0}, 65210);


    if (argc != 2){
        std::cout << "Usage: day07.exe input.txt" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream infile;
    infile.open(argv[1]);
    if (infile.fail()){
        std::cerr << "Failed to open file: '" << argv[1] << "' for reading" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::vector<int> vect;
    for (int i; infile >> i;) {
        vect.push_back(i);    
        if (infile.peek() == ',')
            infile.ignore();
    }

    ThrustComputer c(vect);
    c.permute({0,1,2,3,4},0,4);
    std::cout << "part1:" << c.best_thrust << std::endl;
    return EXIT_SUCCESS;
}