#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

struct ChipInfo {
    string type;
    int inputPin;
    int outputPin;
};

class Parse {
private:
    int globalInputCount;
    int globalOutputCount;
    vector<ChipInfo> chips;
    vector<pair<int, int>> giToCiBinds;
    vector<pair<int, int>> coToCiBinds;
    vector<pair<int, int>> coToGoBinds;

public:
    void parseFile(const string& filename);

private:
    // Helper function to extract a number from a line given a delimiter and offset
    int extractNumber(const string& line, const string& delimiter, int offset);

    // Function to parse chip information
    void parseChips(ifstream& file);

    // Function to parse bind information
    void parseBinds(ifstream& file);

    // Helper function to extract index from string like GI[0]
    int extractIndex(const string& str);
};

#endif // PARSE_H
