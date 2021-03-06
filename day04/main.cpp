#include <iostream>
#include <map>

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

bool is_better_password(int pwd){
    std::map<int, int> groups;

    int prev = 10;
    bool found_decrease = false;
    int count = 0;
    while (pwd){
        int digit = pwd % 10;
        if (prev == digit){
            ++groups[digit];
        }
        found_decrease = found_decrease || prev < digit;
        pwd /= 10;
        prev = digit;
        ++count;
    }
    if (found_decrease){
        //std::cout << "found decrease" << std::endl;
        return false;
    }

    if (groups.size() == 0){
        //std::cout << "double not found" << std::endl;
        return false;
    }

    if (count != 6){
        //std::cout << "only 6 digit numbers are enabled" << std::endl;
        return false;
    }

    bool foundSmaller = false;
    for (auto g: groups){
        if (g.second == 1)
            foundSmaller = true;
    }

    if (!foundSmaller){
        //std::cout << "only larger groups of identical numbers found" << std::endl;
        return false;
    }

    return true;
}

void test_sample_part1(int input, bool expected){
    int computed = is_good_password(input);
    if (computed != expected)
        std::cout << "ERROR: password: " << input 
                  << " should be: " << (expected?"OK":"NOT OK")
                  << " but it was: " << (computed ?"OK":"NOT OK")
                  << std::endl;
}


void test_sample_part2(int input, bool expected){
    int computed = is_better_password(input);
    if (computed != expected)
        std::cout << "ERROR: password: " << input 
                  << " should be: " << (expected?"OK":"NOT OK")
                  << " but it was: " << (computed ?"OK":"NOT OK")
                  << std::endl;
}

int main(int argc, char **argv) {
    test_sample_part1(111111, true);
    test_sample_part1(223450, false);
    test_sample_part1(123789, false);
    int part1_counter = 0;
    for (size_t i = rangemin; i < rangemax; i++)
    {
        if (is_good_password(i))
            part1_counter++;
    }
    std::cout << "part 1 solution: " << part1_counter << std::endl;

    test_sample_part2(112233, true);
    test_sample_part2(123444, false);
    test_sample_part2(111122, true);
    int part2_counter = 0;
    for (size_t i = rangemin; i < rangemax; i++)
    {
        if (is_better_password(i))
            part2_counter++;
    }
    std::cout << "part 2 solution: " << part2_counter << std::endl;
    return EXIT_SUCCESS;
}