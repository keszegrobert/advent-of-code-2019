#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
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

    int row = 0;
    int column = 0;
    int layer = 0;
    char pixel = 0;
    int min_zeros = INT32_MAX;
    int min_layer = INT32_MAX;
    int digits0 = 0;
    int digits1 = 0;
    int digits2 = 0;
    int result = 0;
    int image[6][25];
    for (int i = 0; i < 6; ++i){
        for (int j = 0; j < 25; ++j){
            image[i][j] = '2';
        }
    }

    while (infile >> pixel){
        if (pixel == '\r' || pixel == '\n'){
            break;
        }
        ++column;
        //std::cout << pixel;
        if (pixel == '0'){
            ++digits0;
            if (image[row][column] == '2'){
                image[row][column] = '0';
            }
        }
        if (pixel == '1'){
            ++digits1;
            if (image[row][column] == '2'){
                image[row][column] = '1';
            }
        }
        if (pixel == '2'){
            ++digits2;
        }
        if (column >= 25){
            ++row;
            column = 0;
            //std::cout << std::endl;
        }
        if (row >= 6){
            column = 0;
            row = 0;
            //std::cout << "digits 0:" << digits0 << std::endl;
            //std::cout << std::endl;
            if (digits0 < min_zeros){
                min_zeros = digits0;
                min_layer = layer;
                result = digits1*digits2;
            }
            ++layer;
            digits0 = 0;
            digits1 = 0;
            digits2 = 0;
        }
    }
    std::cout << "part1:" << result << std::endl;
    std::cout << "part2:" << std::endl;
    for (int i = 0; i < 6; ++i){
        for (int j = 0; j < 25; ++j){
            std::cout << (image[i][j]=='1'?'0':' ');
        }
        std::cout << std::endl;
    }
    return EXIT_SUCCESS;
}