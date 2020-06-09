#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

class Map{
private:
    std::map<std::string, std::string> tree;
    std::set<std::string> nodes;
public:
    Map(std::vector<std::string>& lines){
        for (auto line:lines){
            auto pos = line.find(")");
            std::string parent = line.substr(0, pos);
            nodes.insert(parent);
            std::string child  = line.substr(pos+1);
            nodes.insert(child);
            tree[child] = parent;
        }
    }

    int compute(){
        int result = 0;
        for (auto node: nodes){
            result += get_distant_parents(node);
        }
        return result;
    }

    int get_distant_parents(std::string child){
        if (child == "COM")
            return 0;
        else {
            std::string parent = tree[child];
            return 1 + get_distant_parents(parent);
        }
    }

    int get_path_length(std::string start, std::string goal){
        std::set<std::string> parents;
        std::string child = start;
        while (child != "COM"){
            child = tree[child];
            parents.insert(child);
        }
        int result = 0;
        child = goal;
        // std::cout << "Parents of "<< goal << std::endl;
        while(parents.find(child) == parents.end()){
            child = tree[child];
            ++result;
        }
        std::string common = child;
        // std::cout << "Common node: " << common << std::endl;
        child = start;
        // std::cout << "Parents of "<< start << std::endl;
        while(child != common){
            child = tree[child];
            ++result;
        }
        return result - 2;
    }

};

void test_sample(std::vector<std::string> lines, int expected){
    Map m(lines);
    int result = m.compute();
    if (result != expected){
        std::cout << "Error, expecting:" << expected 
            << " but got:" << result << std::endl;
    }
}

void test_path_length(std::vector<std::string> lines, int expected){
    Map m(lines);
    int result = m.get_path_length("YOU", "SAN");
    if (result != expected){
        std::cout << "Error, expecting:" << expected 
            << " but got:" << result << std::endl;
    }
}

int main(int argc, char **argv) {
    test_sample(
        {
            "COM)B","B)C","C)D","D)E","E)F","B)G","G)H",
            "D)I","E)J","J)K","K)L"
        },
        42
    );

    test_path_length(
        { 
            "COM)B","B)C","C)D","D)E","E)F","B)G","G)H",
            "D)I","E)J","J)K","K)L","K)YOU","I)SAN"
        },
        4
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

    Map m(vect);
    std::cout << "part1:" << m.compute() << std::endl;

    std::set<std::string> parents_of_you;
    std::cout << "part2:" << m.get_path_length("YOU", "SAN") << std::endl;
    return EXIT_SUCCESS;
}