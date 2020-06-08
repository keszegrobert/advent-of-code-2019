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
        if (mem[pos] % 100 == 99)
            return true;
        return false;
    }

    bool parseAdd(){
        if (mem[pos] % 100 != 1)
            return false;
        int a_mode = (mem[pos]/100)%10;
        int b_mode = (mem[pos]/1000)%10;
        int r_mode = (mem[pos]/10000)%10;
        int a = mem[pos+1];
        int b = mem[pos+2];
        int r = mem[pos+3];
        mem[r] = (a_mode==1?a:mem[a]) + (b_mode==1?b:mem[b]);
        return true;
    }

    bool parseMul(){
        if (mem[pos]%100 != 2)
            return false;
        int a_mode = (mem[pos]/100)%10;
        int b_mode = (mem[pos]/1000)%10;
        int r_mode = (mem[pos]/10000)%10;
        int a = mem[pos+1];
        int b = mem[pos+2];
        int r = mem[pos+3];
        mem[r] = (a_mode==1?a:mem[a]) * (b_mode==1?b:mem[b]);
        return true;
    }

    bool parseStore(){
        if (mem[pos]%100 != 3)
            return false;
        int a = mem[pos+1];
        int tmp = 0;
        std::cout << std::endl << "awaiting input:";
        std::cin >> tmp;
        mem[a] = tmp;
        return true;
    }

    bool parsePrint(){
        if (mem[pos]%100 != 4)
            return false;
        int a = mem[pos+1];
        int a_mode = (mem[pos]/100)%10;
        int tmp = 0;
        std::cout << std::endl<< "STDOUT:" << (a_mode==1?a:mem[a]);
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
                std::cout << "Something went wrong, instruction:"
                          << mem[pos] << "couldn't be processed"<< std::endl;
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
    std::vector<int> output;
    t.print_memory(output);
    for (size_t i = 0; i < output.size(); ++i)
    {
        if (output[i] != expected[i])
            std::cout << "error on position " << i 
                  << ", it should be " << expected[i]
                  << ", but it was " << output[i]
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

    test_sample(
        {1002,4,3,4,33},
        {1002,4,3,4,99}
    );

    test_sample(
        {1101,100,-1,4,0},
        {1101,100,-1,4,99}
    );

    Terminal t(vect);
    t.compute();
    std::cout << std::endl;

    return EXIT_SUCCESS;
}