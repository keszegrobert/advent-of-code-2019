
class IntCodeComputer {
private:
    std::vector<int> mem;
    std::deque<int> stdinput;
    std::deque<int> stdoutput;
    int pos = 0;
public:
    IntCodeComputer(std::vector<int> _mem){
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