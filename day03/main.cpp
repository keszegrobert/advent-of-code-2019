#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

bool parseStep(
    std::stringstream& path,
    std::pair<int,int>& direction,
    int& steps
){
    std::map<char, std::pair<int,int>> directions;
    directions['R']= {1,0};
    directions['L']= {-1,0};
    directions['U']= {0,1};
    directions['D']= {0,-1};

    char dir = 0;
    path >> dir;

    if (directions.find(dir) == directions.end()){
        std::cout << ">>> cannot read direction for" << dir <<" !!!" << std::endl;
        return false;
    }
    direction = directions[dir];
    
    path >> steps;

    char comma = 0;
    path >> comma;
    if (comma != ','){ 
        //end of the path
        return false;
    }

    return true;
}

void register_places(
    std::pair<int,int>& current, 
    std::pair<int,int> direction, 
    int steps, 
    std::set<std::pair<int,int>>& places
){
    for (int i = 0; i < steps; ++i){
        current.first += direction.first;
        current.second += direction.second;
        places.insert(current);
    }
}


void find_intersections(
    std::pair<int,int>& current, 
    std::pair<int,int> direction, 
    int steps, 
    std::set<std::pair<int,int>>& places,
    std::set<std::pair<int,int>>& intersections
){
    for (int i = 0; i < steps; ++i){
        current.first += direction.first;
        current.second += direction.second;
         if (places.find(current) != places.end())
            intersections.insert(current);
    }
}

int compute(const std::string input1, const std::string input2){
    std::stringstream path1(input1);
    std::pair<int,int> direction;
    int steps;
    std::pair<int, int> current = {0, 0};

    // registering the places of the first path
    std::set<std::pair<int,int>> places;
    while (parseStep(path1, direction, steps)){
        register_places(current, direction, steps, places);
    }
    register_places(current, direction, steps, places);

    // finding intersections
    std::set<std::pair<int,int>> intersections;
    std::stringstream path2(input2);
    current = {0,0};
    while (parseStep(path2,direction, steps)){
        find_intersections(current, direction, steps, places, intersections);
    }
    find_intersections(current, direction, steps, places, intersections);

    // finding the minimum distance 
    int mindist = INT32_MAX;
    for (auto is: intersections){
        int dist = abs(is.first)+abs(is.second);
        if (dist < mindist)
            mindist = dist;
    }

    return mindist;
}

void test_sample(const std::string input1, const std::string input2, int expected){
    int actual = compute(input1, input2);
    if (actual != expected)
        std::cout << "ERROR: while computing the intersection of " << std::endl
                << input1 << std::endl
                << "and" << std::endl
                << input2 << std::endl
                << ", it should be " << expected
                << ", but it was " << actual
                << " " << std::endl;
}

int main(int argc, char **argv) {
    test_sample(
        "R75,D30,R83,U83,L12,D49,R71,U7,L72",
        "U62,R66,U55,R34,D71,R55,D58,R83",
        159
    );
    test_sample(
        "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51",
        "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7",
        135
    );
    
    if (argc != 2){
        std::cout << "Usage: day03.exe input.txt" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream infile;
    infile.open(argv[1]);
    if (infile.fail()){
        std::cerr << "Failed to open file: '" << argv[1] << "' for reading" << std::endl;
        return EXIT_FAILURE;
    }

    std::string line1;
    std::getline(infile,line1);
    std::string line2;
    std::getline(infile,line2);

    std::cout << "part 1 solution: " << compute(line1, line2) << std::endl;
    return EXIT_SUCCESS;
}