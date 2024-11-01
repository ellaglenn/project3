#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
using namespace std;

// Function to read file and return a vector of doubles
vector<double> readFile(string filename) {
    vector<double> values;
    ifstream inFile(filename);
    string line;
    cout << "Reading file: " << filename << endl; // Debug statement
    while (getline(inFile, line)) {
        // Remove leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

        try {
            if (!line.empty()) { // Check if the line is not empty
                double value = stod(line);
                values.push_back(value);
                cout << "Converted to double: " << value << endl; // Debug statement
            }
        } catch (invalid_argument& e) {
            cerr << "Invalid value (not a number): " << line << endl;
            continue;  // skip lines that cannot be converted to double
        }
    }
    inFile.close();
    return values;
}

// Function to write data to a file
void writeFile(string filename, const vector<double>& values, double mean, double median, double mode) {
    ofstream outFile(filename);
    outFile << "*** Summarized Statistics ***" << endl;
    outFile << "The orderly sorted list of " << values.size() << " values is:" << endl;
    for (size_t i = 0; i < values.size(); ++i) {
        outFile << values[i] << " ";
    }
    outFile << endl;
    outFile << "The mean is " << mean << endl;
    outFile << "The median is " << median << endl;
    outFile << "The mode is " << mode << endl;
    outFile.close();
}

// Function to calculate mean
double calculateMean(const vector<double>& values) {
    double sum = 0;
    for (size_t i = 0; i < values.size(); ++i) {
        sum += values[i];
    }
    return sum / values.size();
}

// Function to calculate median
double calculateMedian(vector<double> values) {
    sort(values.begin(), values.end());
    size_t size = values.size();
    if (size % 2 == 0) {
        return (values[size / 2 - 1] + values[size / 2]) / 2;
    } else {
        return values[size / 2];
    }
}

// Function to calculate mode
double calculateMode(const vector<double>& values) {
    if (values.empty()) {
        return NAN;
    }

    map<double, int> frequency;
    for (size_t i = 0; i < values.size(); ++i) {
        frequency[values[i]]++;
    }

    double mode = values[0];
    int maxCount = 0;

    for (map<double, int>::const_iterator it = frequency.begin(); it != frequency.end(); ++it) {
        if (it->second > maxCount) {
            maxCount = it->second;
            mode = it->first;
        }
    }

    return mode;
}

// Main function
int main() {
    cout << "*** Welcome to Ella's Data Analyzer ***" << endl;
    cout << "Enter the number of files to read: ";
    int numFiles;
    cin >> numFiles;

    vector<double> allValues;
    string filename;
    for (int i = 0; i < numFiles; ++i) {
        cout << "Enter the filename for file " << i + 1 << ": ";
        cin >> filename;
        if (filename == "quit") {
            break;
        }

        vector<double> values = readFile(filename);
        if (values.empty()) {
            cout << "File " << filename << " contained no valid numerical data. Please try again." << endl;
            --i; // retry the current file if it had no valid values
            continue;
        }

        allValues.insert(allValues.end(), values.begin(), values.end());
        cout << "The list of " << values.size() << " values in file " << filename << " is:" << endl;
        for (size_t j = 0; j < values.size(); ++j) {
            cout << values[j] << endl;
        }
        cout << "Merged values so far: ";
        for (size_t k = 0; k < allValues.size(); ++k) {
            cout << allValues[k] << " ";
        }
        cout << endl;
    }

    if (allValues.empty()) {
        cout << "No valid inputs provided. Terminating program." << endl;
        return 0;
    }

    sort(allValues.begin(), allValues.end());

    double mean = calculateMean(allValues);
    double median = calculateMedian(allValues);
    double mode = calculateMode(allValues);

    cout << "*** Summarized Statistics ***" << endl;
    cout << "The orderly sorted list of " << allValues.size() << " values is:" << endl;
    for (size_t i = 0; i < allValues.size(); ++i) {
        cout << allValues[i] << " ";
    }
    cout << endl;
    cout << "The mean is " << mean << endl;
    cout << "The median is " << median << endl;
    cout << "The mode is " << mode << endl;

    cout << "Enter the output filename to save: ";
    cin >> filename;
    writeFile(filename, allValues, mean, median, mode);
    cout << "*** File " << filename << " has been written to disk ***" << endl;
    cout << "*** Goodbye. ***" << endl;

    return 0;
}
