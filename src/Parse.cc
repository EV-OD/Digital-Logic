#include "Parse.h"

void Parse::parseFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.substr(0, 13) == "# Global Input") {
            globalInputCount = extractNumber(line, "->", 2);
        } else if (line.substr(0, 14) == "# Global Output") {
            globalOutputCount = extractNumber(line, "->", 2);
        } else if (line == "# Chips") {
            parseChips(file);
        } else if (line == "# Binds") {
            parseBinds(file);
        }
    }
    file.close();

    // Output parsed data (for testing)
    cout << "Global Inputs: " << globalInputCount << endl;
    cout << "Global Outputs: " << globalOutputCount << endl;

    cout << "\nChips:" << endl;
    for (const auto& chip : chips) {
        cout << "Type: " << chip.type << endl;
        cout << "Input Pin: " << chip.inputPin << endl;
        cout << "Output Pin: " << chip.outputPin << endl;
        cout << "---" << endl;
    }

    cout << "\nGI to CI Binds:" << endl;
    for (const auto& bind : giToCiBinds) {
        cout << "GI[" << bind.first << "] -> CI[" << bind.second << "]" << endl;
    }

    cout << "\nCO to CI Binds:" << endl;
    for (const auto& bind : coToCiBinds) {
        cout << "CO[" << bind.first << "] -> CI[" << bind.second << "]" << endl;
    }

    cout << "\nCO to GO Binds:" << endl;
    for (const auto& bind : coToGoBinds) {
        cout << "CO[" << bind.first << "] -> GO[" << bind.second << "]" << endl;
    }
}

int Parse::extractNumber(const string& line, const string& delimiter, int offset) {
    size_t pos = line.find(delimiter);
    if (pos != string::npos) {
        return stoi(line.substr(pos + delimiter.length() + offset));
    }
    return -1; // Invalid number
}

void Parse::parseChips(ifstream& file) {
    string line;
    while (getline(file, line) && line != "# end") {
        if (line == "start") {
            ChipInfo chip;
            while (getline(file, line) && line != "end") {
                size_t pos = line.find("->");
                if (pos != string::npos) {
                    string key = line.substr(0, pos);
                    string value = line.substr(pos + 3);
                    if (key == "Type") {
                        chip.type = value;
                    } else if (key == "InputPin") {
                        chip.inputPin = stoi(value);
                    } else if (key == "OutputPin") {
                        chip.outputPin = stoi(value);
                    }
                }
            }
            chips.push_back(chip);
        }
    }
}

void Parse::parseBinds(ifstream& file) {
    string line;
    while (getline(file, line) && line != "# end") {
        size_t pos = line.find("->");
        if (pos != string::npos) {
            string from = line.substr(0, pos);
            string to = line.substr(pos + 3);

            if (from.substr(0, 2) == "GI") {
                int giIndex = extractIndex(from);
                int ciIndex = extractIndex(to);
                giToCiBinds.push_back({giIndex, ciIndex});
            } else if (from.substr(0, 2) == "CO") {
                int coIndex = extractIndex(from);
                if (to.substr(0, 2) == "CI") {
                    int ciIndex = extractIndex(to);
                    coToCiBinds.push_back({coIndex, ciIndex});
                } else if (to.substr(0, 2) == "GO") {
                    int goIndex = extractIndex(to);
                    coToGoBinds.push_back({coIndex, goIndex});
                }
            }
        }
    }
}

int Parse::extractIndex(const string& str) {
    size_t startPos = str.find('[') + 1;
    size_t endPos = str.find(']');
    return stoi(str.substr(startPos, endPos - startPos));
}
