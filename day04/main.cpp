#include <iostream>
#include <fstream>

const int rangemin = 206938;
const int rangemax = 679128;

bool is_good_password(int pwd){

    int prev = 10;
    bool found_adjacent = false;
    bool found_decrease = false;
    int count = 0;
    while (pwd){
        int digit = pwd % 10;
        found_adjacent = found_adjacent || prev == digit;
        found_decrease = found_decrease || prev < digit;
        pwd /= 10;
        prev = digit;
        ++count;
    }
    if (found_decrease){
        //std::cout << "found decrease" << std::endl;
        return false;
    }

    if (!found_adjacent){
        //std::cout << "double not found" << std::endl;
        return false;
    }

    if (count != 6){
        //std::cout << "only 6 digit numbers are enabled" << std::endl;
        return false;
    }

    return true;
}

void test_sample(int input, bool expected){
    int computed = is_good_password(input);
    if (computed != expected)
        std::cout << "ERROR: password: " << input 
                  << " should be: " << (expected?"OK":"NOT OK")
                  << " but it was: " << (computed ?"OK":"NOT OK")
                  << std::endl;
}

int main(int argc, char **argv) {
    test_sample(111111, true);
    test_sample(223450, false);
    test_sample(123789, false);
    int counter = 0;
    for (size_t i = rangemin; i < rangemax; i++)
    {
        if (is_good_password(i))
            counter++;
    }
    std::cout << "part 1 solution: " << counter << std::endl;
    return EXIT_SUCCESS;
}