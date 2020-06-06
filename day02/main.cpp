#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

bool parseHalt(std::vector<int>& mem, int pos){
    if (mem[pos] == 99)
        return true;
    return false;
}

bool parseAdd(std::vector<int>& mem, int pos){
    if (mem[pos] != 1)
        return false;
    int a = mem[pos+1];
    int b = mem[pos+2];
    int r = mem[pos+3];
    mem[r] = mem[a]+mem[b];
    return true;
}

bool parseMul(std::vector<int>& mem, int pos){
    if (mem[pos] != 2)
        return false;
    int a = mem[pos+1];
    int b = mem[pos+2];
    int r = mem[pos+3];
    mem[r] = mem[a]*mem[b];
    return true;
}

void compute(std::vector<int>& mem){
    int pc = 0;
    while (true){
        if (parseHalt(mem, pc))
            break;
        else if (parseAdd(mem,pc))
            pc += 4;
        else if (parseMul(mem, pc))
            pc += 4;
        else
        {
            std::cout << "Something went wrong" << std::endl;
            break;
        }
    }
}


void test_sample(std::vector<int> input, std::vector<int> expected){
    compute(input);
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (input[i] != expected[i])
            std::cout << "error on position " << i 
                  << ", it should be " << input[i]
                  << ", but it was " << expected[i] 
                  << " " << std::endl;
    }
}

void print_memory(std::vector<int>& vect){
    for (int i=0; i<vect.size(); ++i){
        std::cout << vect[i] << ',';
    }
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    test_sample(
        {1,9,10,3,2,3,11,0,99,30,40,50},
        {3500,9,10,70,2,3,11,0,99,30,40,50}
    );

    test_sample(
        {1,0,0,0,99},
        {2,0,0,0,99}
    );

    test_sample(
        {2,3,0,3,99},
        {2,3,0,6,99}
    );

    test_sample(
        {2,4,4,5,99,0},
        {2,4,4,5,99,9801}
    );

    test_sample(
        {1,1,1,4,99,5,6,0,99},
        {30,1,1,4,2,5,6,0,99}
    );

    if (argc != 2){
        std::cout << "Usage: day02.exe input.txt" << std::endl;
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
    vect[1] = 12;
    vect[2] = 2;
    compute(vect);
    std::cout << vect[0] << std::endl;
    return EXIT_SUCCESS;
}