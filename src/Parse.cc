#include "Parse.h"
#include <stdexcept>
#include <algorithm>

void Parse::parseFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {

        if (line.substr(0, 14) == "# Global Input") {
            globalInputCount = extractNumber(line, "->", 1);
        } else if (line.substr(0, 15) == "# Global Output") {
            globalOutputCount = extractNumber(line, "->", 1);
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
        cout << "Name" << chip.name << endl;
        cout << "Type: " << chip.type << endl;
        cout << "Input Pin: " << chip.inputPin << endl;
        cout << "Output Pin: " << chip.outputPin << endl;
        cout << "---" << endl;
    }

    cout << "\nGI to CI Binds:" << endl;
    for (const auto& bind : giToCiBinds) {
        cout << "GI[" << bind.first << "] -> CI[";
        for (size_t i = 0; i < bind.second.size(); ++i) {
            if (i > 0) cout << "][";
            cout << bind.second[i];
        }
        cout << "]" << endl;
    }

    cout << "\nCO to CI Binds:" << endl;
    for (const auto& bind : coToCiBinds) {
        cout << "CO[" << bind.first[0] << "][" << bind.first[1] <<"] -> CI[";
        for (size_t i = 0; i < bind.second.size(); ++i) {
            if (i > 0) cout << "][";
            cout << bind.second[i];
        }
        cout << "]" << endl;
    }

    cout << "\nCO to GO Binds:" << endl;
    for (const auto& bind : coToGoBinds) {
        cout << "CO[" << bind.first[0] << "][" << bind.first[1] << "] -> GO[" << bind.second << "]" << endl;
    }

    cout << "\nGI to GO Binds:" << endl;
    for (const auto& bind : giToGoBinds) {
        cout << "GI[" << bind.first << "] -> GO[" << bind.second << "]" << endl;
    }
}

int Parse::extractNumber(const string& line, const string& delimiter, int offset) {
    size_t pos = line.find(delimiter);
    if (pos != string::npos) {
        try {
            std::cout << "Line: " << line << std::endl;
            std::cout << "Pos: " << pos << std::endl;
            std::cout << "Substring: " << line.substr(pos + delimiter.length() + offset) << std::endl;
            return std::stoi(line.substr(pos + delimiter.length() + offset));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument error: extractNumber" << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range error: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error occurred during conversion." << std::endl;
        }
    }
    return -1; // Invalid number
}

void Parse::parseChips(ifstream& file) {
    string line;
    while (getline(file, line) && line != "# end") {
        // name
        if(line == "# start"){
            getline(file, line);
        }
        std::string name = line.substr(line.find(".") + 1);

        // inputpin
        getline(file, line);
        int delPos = line.find("->");
        int inputPin = stoi(line.substr(delPos + 3));


        // outputpin
        getline(file, line);
        delPos = line.find("->");
        int outputPin = stoi(line.substr(delPos + 3));


        // Type
        getline(file, line);
        delPos = line.find("->");
        std::string type = line.substr(delPos + 3);
        ChipInfo *chip = new ChipInfo{type, inputPin, outputPin, name};
        chips.push_back(*chip);
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
                int giIndex = extractIndices(from)[0];
                if (to.substr(0, 2) == "CI") {
                    vector<int> ciIndices = extractIndices(to);
                    giToCiBinds.push_back({giIndex, ciIndices});
                } else if (to.substr(0, 2) == "GO") {
                    int goIndex = extractIndices(to)[0];
                    giToGoBinds.push_back({giIndex, goIndex});
                }
            } else if (from.substr(0, 2) == "CO") {
                std::vector<int> coIndex = extractIndices(from);
                if (to.substr(0, 2) == "CI") {
                    vector<int> ciIndices = extractIndices(to);
                    coToCiBinds.push_back({coIndex, ciIndices});
                } else if (to.substr(0, 2) == "GO") {
                    vector<int> goIndex = extractIndices(to);
                    coToGoBinds.push_back({coIndex, goIndex[0]});
                }
            }
        }
    }
}

vector<int> Parse::extractIndices(const string& str) {
    vector<int> indices;
    size_t startPos = str.find('[') + 1;
    size_t endPos = str.find(']');

    while (startPos != std::string::npos && endPos != std::string::npos && startPos < endPos) {
        try {

            // Extract substring containing the index and convert to integer
            std::string indexStr = str.substr(startPos, endPos - startPos);
            // Trim whitespace characters (if needed)
            indexStr.erase(std::remove_if(indexStr.begin(), indexStr.end(), ::isspace), indexStr.end());
            indices.push_back(std::stoi(indexStr));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument error in extractIndices: " << e.what() << std::endl;
            // Handle the error (e.g., continue parsing or break out)
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range error in extractIndices: " << e.what() << std::endl;
            // Handle the error (e.g., continue parsing or break out)
        } catch (...) {
            std::cerr << "Unknown error occurred during conversion in extractIndices." << std::endl;
            // Handle other unknown errors
        }

        // Move to the next occurrence of '[' and ']'
        startPos = str.find('[', endPos);
        if (startPos != std::string::npos) {
            startPos += 1; // Move past '['
        }
        endPos = str.find(']', startPos);
    }

    return indices;
}