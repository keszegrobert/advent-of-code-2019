#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <memory>
#include "intcode_computer.h"

class ThrustComputer{
private:
    std::vector<int> opcodes;
    std::vector<int> best_phases;

public:
    int best_thrust = 0;

    ThrustComputer(std::vector<int> _opcodes){
        opcodes = _opcodes;
    }

    void run_with_phases(std::vector<int> phases){

        std::deque<IntCodeComputer> machines;
        for (int i=0; i<5; ++i){
            machines.push_back(IntCodeComputer(opcodes));
        }
        
        //std::cout << "outputs:";

        int previous_output = 0;
        for (auto phase: phases){
            std::vector<int> output;
            auto c = machines.front();
            machines.pop_front();
            c.compute_output_for_input({phase, previous_output}, output);
            if (c.paused)
                machines.push_back(c);
            previous_output = output.back();
            //std::cout << previous_output << std::endl;
        }
        if (best_thrust < previous_output){
            best_thrust = previous_output;
            best_phases = phases;
        }

        while (machines.size()){
            for (auto phase: phases){
                std::vector<int> output;
                auto c = machines.front();
                machines.pop_front();
                c.compute_output_for_input({previous_output}, output);
                if (c.paused)
                    machines.push_back(c);
                previous_output = output.back();
                //std::cout << previous_output << std::endl;
            }
            if (best_thrust < previous_output){
                best_thrust = previous_output;
                best_phases = phases;
            }
        }
        /*std::cout << std::endl;

        std::cout << "PHASES:";
        for (auto p: phases){
            std::cout << p << ",";
        }

        std::cout 
            << "thrust:" << previous_output 
            << ", best:" << best_thrust 
            << std::endl;*/
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

void test_run_with_phases(
    std::vector<int> opcodes,
    std::vector<int> phases, 
    int expected
){
    ThrustComputer c(opcodes);
    c.run_with_phases(phases);
    if (c.best_thrust != expected){
        std::cout << "error in computing the best thrust:"
                << "it should be " << expected
                << ", but it was " << c.best_thrust
                << " " << std::endl;
    }
}

void test_sample_feedback_loop(std::vector<int> opcodes, int expected){
    ThrustComputer c(opcodes);
    c.permute({5,6,7,8,9},0,4);
    if (c.best_thrust != expected){
        std::cout << "error in computing the thrust:"
                << ", it should be " << expected
                << ", but it was " << c.best_thrust
                << " " << std::endl;
    }
}

int main(int argc, char **argv) {

    test_run_with_phases(
        {3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0}, 
        {4,3,2,1,0},
        43210
    );
    test_sample({
        3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0}, 43210);
    test_sample({
        3,23,3,24,1002,24,10,24,1002,23,-1,23,
        101,5,23,23,1,24,23,23,4,23,99,0,0}, 54321);
    test_sample({
        3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
        1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0}, 65210);
    
    test_run_with_phases({
        3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,
        27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5
    },
    {9,8,7,6,5},
    139629729);

    test_sample_feedback_loop({
        3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,
        27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5}, 139629729);

    test_run_with_phases({
        3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,
        55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,
        1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,
        0,0,0,0,10},
        {9,7,8,5,6},
        18216);
    test_sample_feedback_loop({
        3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,
        55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,
        1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,
        0,0,0,0,10}, 18216);
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
    std::vector<int> opcodes;
    for (int i; infile >> i;) {
        opcodes.push_back(i);    
        if (infile.peek() == ',')
            infile.ignore();
    }

    ThrustComputer c1(opcodes);
    c1.permute({0,1,2,3,4},0,4);
    std::cout << "part1:" << c1.best_thrust << std::endl;

    ThrustComputer c2(opcodes);
    c2.permute({5,6,7,8,9},0,4);
    std::cout << "part2:" << c2.best_thrust << std::endl;
    return EXIT_SUCCESS;

}