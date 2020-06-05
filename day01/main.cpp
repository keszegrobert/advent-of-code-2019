#include <iostream>
#include <fstream>

int compute_fuel(int n){
    return n / 3 - 2;
}

void test_sample(int input, int expected){
    int computed = compute_fuel(input);
    if (computed != expected)
        std::cout << "for the mass " << input 
                  << " should be " << expected
                  << " but it was " << computed 
                  << " " << std::endl;
}

int main(int argc, char **argv) {


    test_sample(12, 2);
    test_sample(14, 2);
    test_sample(1969, 654);
    test_sample(100756, 33583);

    if (argc != 2){
        std::cout << "Usage: day01.exe input.txt" << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream infile;
    infile.open(argv[1]);
    std::string line;
    int sum = 0;
    int recursive_sum = 0;
    while (std::getline(infile,line)){
        int mass = std::stoi(line);
        int fuel = compute_fuel(mass);
        sum += fuel;
        while (fuel > 0){
            recursive_sum += fuel;
            fuel = compute_fuel(fuel);
        }
    }
    std::cout << "The parts require " << sum 
              << " litres of fuel." << std::endl;
    std::cout << "The fuel requires " << recursive_sum 
              << " litres of fuel." << std::endl;
    return EXIT_SUCCESS;
}