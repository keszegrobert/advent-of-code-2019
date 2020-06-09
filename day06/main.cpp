#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>


int get_distant_parents(std::string child, std::map<std::string, std::string>& tree){
    if (child == "COM")
        return 0;
    else {
        std::string parent = tree[child];
        return 1 + get_distant_parents(parent, tree);
    }
}

int compute(std::vector<std::string>& lines){
    std::map<std::string, std::string> tree;
    std::set<std::string> nodes;
    for (auto line:lines){
        auto pos = line.find(")");
        std::string parent = line.substr(0, pos);
        nodes.insert(parent);
        std::string child  = line.substr(pos+1);
        nodes.insert(child);
        tree[child] = parent;
    }
    int result = 0;
    for (auto node: nodes){
        result += get_distant_parents(node, tree);
    }
    return result;
}

void test_sample(std::vector<std::string> lines, int expected){
    int result = compute(lines);
    if (result != expected){
        std::cout << "Error, expecting:" << expected 
            << " but got:" << result << std::endl;
    }
}

int main(int argc, char **argv) {
    test_sample(
        {"COM)B","B)C","C)D","D)E","E)F","B)G","G)H","D)I","E)J","J)K","K)L"},
        42
    );

    if (argc != 2){
        std::cout << "Usage: day06.exe input.txt" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream infile;
    infile.open(argv[1]);
    if (infile.fail()){
        std::cerr << "Failed to open file: '" << argv[1] << "' for reading" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    std::vector<std::string> vect;
    while (std::getline(infile,line)){
        vect.push_back(line);
    }

    std::cout << "part1:" << compute(vect) << std::endl;
    return EXIT_SUCCESS;
}