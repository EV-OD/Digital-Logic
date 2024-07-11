#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

// Define a structure for Chip information
struct ChipInfo {
    string type;
    int inputPin;
    int outputPin;
    string name;
};

// Define the Parse class to parse the .chip file
class Parse {
public:
    int globalInputCount;
    int globalOutputCount;
    vector<ChipInfo> chips; // Vector to store chip information
    vector<pair<int, vector<int>>> giToCiBinds;
    vector<pair<vector<int>, vector<int>>> coToCiBinds;
    vector<pair<vector<int>, int>> coToGoBinds;
    vector<pair<int, int>> giToGoBinds; // New vector for GI to GO bindings
    // Function to parse the .chip file
    void parseFile(const string& filename);

private:
    // Helper function to extract a number from a line given a delimiter and offset
    int extractNumber(const string& line, const string& delimiter, int offset);

    // Function to parse chip information
    void parseChips(ifstream& file);

    // Function to parse bind information
    void parseBinds(ifstream& file);

    // Helper function to extract indices from string like GI[0]
    vector<int> extractIndices(const string& str);
};

#endif // PARSE_H
