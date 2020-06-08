#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Terminal {
private:
    std::vector<int> mem;
    int pos = 0;
public:
    Terminal(std::vector<int> _mem){
        mem = _mem;
    }

    bool parseHalt(){
        if (mem[pos] == 99)
            return true;
        return false;
    }

    bool parseAdd(){
        if (mem[pos] != 1)
            return false;
        int a = mem[pos+1];
        int b = mem[pos+2];
        int r = mem[pos+3];
        mem[r] = mem[a]+mem[b];
        return true;
    }

    bool parseMul(){
        if (mem[pos] != 2)
            return false;
        int a = mem[pos+1];
        int b = mem[pos+2];
        int r = mem[pos+3];
        mem[r] = mem[a]*mem[b];
        return true;
    }

    bool parseStore(){
        if (mem[pos] != 3)
            return false;
        int a = mem[pos+1];
        int tmp = 0;
        // std::cin >> tmp;
        //mem[a] = tmp;
        return true;
    }

    bool parsePrint(){
        if (mem[pos] != 3)
            return false;
        int a = mem[pos+1];
        int tmp = 0;
        //mem[a] = tmp;
        return true;
    }

    int compute(){
        pos = 0;
        while (true){
            if (parseHalt())
                break;
            else if (parseAdd())
                pos += 4;
            else if (parseMul())
                pos += 4;
            else if (parseStore())
                pos += 2;
            else if (parsePrint())
                pos += 2;
            else
            {
                std::cout << "Something went wrong" << std::endl;
                break;
            }
        }
        return mem[0];
    }

    void print_memory(std::vector<int>& vect){
        vect = mem;
    }

    int compute_with_params(int a, int b){
        mem[1] = a;
        mem[2] = b;
        return compute();
    }

};

void test_sample(std::vector<int> input, std::vector<int> expected){
    Terminal t(input);
    t.compute();
    t.print_memory(input);
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (input[i] != expected[i])
            std::cout << "error on position " << i 
                  << ", it should be " << input[i]
                  << ", but it was " << expected[i] 
                  << " " << std::endl;
    }
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
        std::cout << "Usage: day05.exe input.txt" << std::endl;
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

    Terminal t(vect);
    std::cout << "part 1 solution: " << t.compute_with_params(12, 2) << std::endl;

    for (int noun = 0; noun < 100; ++noun){
        for (int  verb = 0; verb < 100; ++verb){
                Terminal t(vect);
                if (t.compute_with_params(noun, verb) == 19690720){
                std::cout << "part 2 solution: " << 100*noun+verb << std::endl;
                return EXIT_SUCCESS;
            }
        }
    }

    return EXIT_SUCCESS;
}