#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>

class Terminal {
private:
    std::vector<int> mem;
    std::deque<int> stdinput;
    std::deque<int> stdoutput;
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
        int operand1 = a_mode==1?a:mem[a];
        int operand2 = b_mode==1?b:mem[b];
        //std::cout << std::endl << "ADDING " << operand1 << 
        //    " and " << operand2;
        mem[r] = operand1 + operand2;
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
        if (stdinput.size() > 0){
            tmp = stdinput.front();
            stdinput.pop_front();
            //std::cout << "INPUT: " << tmp << std::endl;
        }
        else {
            std::cout << std::endl << "INPUT: ";
            std::cin >> tmp;
        }
        mem[a] = tmp;
        return true;
    }

    bool parsePrint(){
        if (mem[pos]%100 != 4)
            return false;
        int a = mem[pos+1];
        int a_mode = (mem[pos]/100)%10;
        stdoutput.push_back(a_mode==1?a:mem[a]);
        //std::cout << "OUTPUT:" << (a_mode==1?a:mem[a]) << std::endl;
        return true;
    }

    bool parseJumpIfTrue(){
        if (mem[pos]%100 != 5)
            return false;
        int a = mem[pos+1];
        int a_mode = (mem[pos]/100)%10;
        int b = mem[pos+2];
        int b_mode = (mem[pos]/1000)%10;
        //std::cout << std::endl << (a_mode?a:mem[a]) 
        //    << "!=0 -> " << (b_mode?b:mem[b]);
        if ((a_mode==1?a:mem[a]) != 0)
            pos = (b_mode?b:mem[b]);
        else
            pos += 3;
        return true;
    }

    bool parseJumpIfFalse(){
        if (mem[pos]%100 != 6)
            return false;
        int a = mem[pos+1];
        int a_mode = (mem[pos]/100)%10;
        int b = mem[pos+2];
        int b_mode = (mem[pos]/1000)%10;
        //std::cout << std::endl << (a_mode?a:mem[a]) 
        //    << "==0 -> " << (b_mode?b:mem[b]);
        if ((a_mode?a:mem[a]) == 0)
            pos = (b_mode?b:mem[b]);
        else
            pos += 3;
        return true;
    }

    bool parseLessThan(){
        if (mem[pos]%100 != 7)
            return false;
        int a_mode = (mem[pos]/100)%10;
        int b_mode = (mem[pos]/1000)%10;
        int r_mode = (mem[pos]/10000)%10;
        int a = mem[pos+1];
        int b = mem[pos+2];
        int r = mem[pos+3];
        //std::cout << std::endl << (a_mode?a:mem[a])
        //          << "<" << (b_mode?b:mem[b]) << "=>" << r;
        mem[r] = (a_mode?a:mem[a]) < (b_mode?b:mem[b])?1:0;
        return true;
    }

    bool parseEquals(){
        if (mem[pos]%100 != 8)
            return false;
        
        int a_mode = (mem[pos]/100)%10;
        int b_mode = (mem[pos]/1000)%10;
        int r_mode = (mem[pos]/10000)%10;
        int a = mem[pos+1];
        int b = mem[pos+2];
        int r = mem[pos+3];
        //std::cout << std::endl << (a_mode?a:mem[a])
        //          << "==" << (b_mode?b:mem[b]) << "=>" << r;
        mem[r] = ((a_mode?a:mem[a]) == (b_mode?b:mem[b])?1:0);
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
            else if (parseJumpIfTrue())
                pos += 0;
            else if (parseJumpIfFalse())
                pos += 0;
            else if (parseLessThan())
                pos += 4;
            else if (parseEquals())
                pos += 4;
            else
            {
                std::cout << std::endl <<"NOP:"<< mem[pos];
                ++pos;
                if (pos >= mem.size())
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

    int compute_output_for_input(
        std::vector<int> input, 
        std::vector<int>& output
    ){
        for (auto i: input){
            stdinput.push_back(i);
        }
        int result = compute();
        for (auto o: stdoutput){
            output.push_back(o);
        }
        return result;
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

void test_input_output(
    std::vector<int> memory,
    std::vector<int> input,
    std::vector<int> expected
){
    Terminal t(memory);
    std::vector<int> output;
    t.compute_output_for_input(input, output);

    if (output.size() != expected.size()){
        std::cout << "output is not the same as expected" << std::endl;
        return;
    }
    for (size_t i = 0; i < output.size(); ++i)
    {
        if (output[i] != expected[i])
            std::cout << "error in output: index == " << i 
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

    test_sample(
        {1002,4,3,4,33},
        {1002,4,3,4,99}
    );

    test_sample(
        {1101,100,-1,4,0},
        {1101,100,-1,4,99}
    );

    test_input_output(
        {3,9,8,9,10,9,4,9,99,-1,8},
        {8},
        {1}
    );

    test_input_output(
        {3,9,8,9,10,9,4,9,99,-1,8},
        {100},
        {0}
    );

    test_input_output(
        {3,9,7,9,10,9,4,9,99,-1,8},
        {3},
        {1}
    );

    test_input_output(
        {3,9,7,9,10,9,4,9,99,-1,8},
        {8},
        {0}
    );

    test_input_output(
        {3,3,1108,-1,8,3,4,3,99},
        {8},
        {1}
    );

    test_input_output(
        {3,3,1108,-1,8,3,4,3,99},
        {9},
        {0}
    );

    test_input_output(
        {3,3,1107,-1,8,3,4,3,99},
        {3},
        {1}
    );

    test_input_output(
        {3,3,1107,-1,8,3,4,3,99},
        {8},
        {0}
    );

    test_input_output(
        {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9},
        {0},
        {0}
    );

    test_input_output(
        {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9},
        {100},
        {1}
    );

    test_input_output(
        {3,3,1105,-1,9,1101,0,0,12,4,12,99,1},
        {0},
        {0}
    );

    test_input_output(
        {3,3,1105,-1,9,1101,0,0,12,4,12,99,1},
        {100},
        {1}
    );
    
    test_input_output(
        {
            3,21,1008,21,8,20,1005,20,22,107,
            8,21,20,1006,20,31,1106,0,36,98,
            0,0,1002,21,125,20,4,20,1105,1,
            46,104,999,1105,1,46,1101,1000,1,20,
            4,20,1105,1,46,98,99
        },
        {7},
        {999}
    );

    test_input_output(
        {
            3,21,1008,21,8,20,1005,20,22,107,
            8,21,20,1006,20,31,1106,0,36,98,
            0,0,1002,21,125,20,4,20,1105,1,
            46,104,999,1105,1,46,1101,1000,1,20,
            4,20,1105,1,46,98,99
        },
        {8},
        {1000}
    );

    test_input_output(
        {
            3,21,1008,21,8,20,1005,20,22,107,
            8,21,20,1006,20,31,1106,0,36,98,
            0,0,1002,21,125,20,4,20,1105,1,
            46,104,999,1105,1,46,1101,1000,1,20,
            4,20,1105,1,46,98,99
        },
        {9},
        {1001}
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

    // part1
    Terminal t1(vect);
    std::vector<int> output;
    t1.compute_output_for_input({1}, output);
    std::cout << "part1 output:" << std::endl;
    for (auto o: output)
        std::cout << o << std::endl;

    // part2
    Terminal t2(vect);
    std::vector<int> output2;
    t2.compute_output_for_input({5}, output2);
    std::cout << "part2 output:" << std::endl;
    for (auto o: output2)
        std::cout << o << std::endl;

    return EXIT_SUCCESS;
}