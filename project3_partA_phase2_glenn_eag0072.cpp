#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
using namespace std;

struct Measurement {
    double value;
    int hour;
    int minute;
    int second;
    Measurement(double v, int h, int m, int s) : value(v), hour(h), minute(m), second(s) {}
};

// Function to read data from the input file
bool readFile(const string& filename, vector<Measurement>& measurements) {
    ifstream infile(filename);
    if (!infile) {
        cout << "File not found. Please try again." << endl;
        return false;
    }
    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue; // Skip empty lines
        stringstream ss(line);
        double value;
        int hour, minute, second;
        char sep1, sep2;
        if (!(ss >> value >> hour >> sep1 >> minute >> sep2 >> second) || sep1 != '.' || sep2 != '.') {
            cout << "Not an input file. Illegal content/structure detected. Please try again." << endl;
            return false;
        }
        measurements.push_back(Measurement(value, hour, minute, second));
    }
    return true;
}

// Sorting function based on the Measurement value
bool compareByValue(const Measurement& a, const Measurement& b) {
    return a.value < b.value;
}

// Sorting function based on the timestamp
bool compareByTime(const Measurement& a, const Measurement& b) {
    if (a.hour != b.hour) return a.hour < b.hour;
    if (a.minute != b.minute) return a.minute < b.minute;
    return a.second < b.second;
}

// Function to calculate statistics
void calculateStatistics(const vector<Measurement>& measurements, double& mean, double& median, double& mode) {
    if (measurements.empty()) return;
    double sum = 0;
    map<double, int> frequency;
    for (size_t i = 0; i < measurements.size(); ++i) {
        sum += measurements[i].value;
        frequency[measurements[i].value]++;
    }
    mean = sum / measurements.size();
    // Find median
    vector<double> values;
    for (size_t i = 0; i < measurements.size(); ++i) {
        values.push_back(measurements[i].value);
    }
    sort(values.begin(), values.end());
    if (values.size() % 2 == 0) {
        median = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;
    } else {
        median = values[values.size() / 2];
    }
    // Find mode
    int max_freq = 0;
    for (map<double, int>::iterator entry = frequency.begin(); entry != frequency.end(); ++entry) {
        if (entry->second > max_freq) {
            max_freq = entry->second;
            mode = entry->first;
        }
    }
}

int main() {
    cout << "*** Welcome to Ella's Data Analyzer ***" << endl;
    int numFiles;
    cout << "Enter the number of files to read: ";
    cin >> numFiles;
    cin.ignore(); // Ignore the newline after the number input
    vector<Measurement> combinedData;
    for (int i = 0; i < numFiles; i++) {
        string filename;
        cout << "Enter the filename for file " << (i + 1) << ": ";
        getline(cin, filename);
        if (filename == "quit") {
            cout << "Input cancelled. Proceeding to calculation..." << endl;
            break;
        }
        vector<Measurement> measurements;
        if (readFile(filename, measurements)) {
            combinedData.insert(combinedData.end(), measurements.begin(), measurements.end());
            cout << "The list of " << measurements.size() << " values in file " << filename << " is:" << endl;
            for (size_t j = 0; j < measurements.size(); ++j) {
                cout << measurements[j].value << " " << measurements[j].hour << "." << measurements[j].minute << "." << measurements[j].second << endl;
            }
        } else {
            i--; // Retry the same file input
        }
    }
    // Statistics for ordered list
    sort(combinedData.begin(), combinedData.end(), compareByValue);
    double mean, median, mode;
    calculateStatistics(combinedData, mean, median, mode);
    cout << "*** Summarized Statistics ***" << endl;
    cout << "The orderly sorted list of " << combinedData.size() << " values is:" << endl;
    for (size_t i = 0; i < combinedData.size(); ++i) {
        cout << combinedData[i].value << " ";
    }
    cout << endl;
    cout << "The mean is " << fixed << setprecision(4) << mean << endl;
    cout << "The median is " << median << endl;
    cout << "The mode is " << mode << endl;
    // Statistics for chronological list
    sort(combinedData.begin(), combinedData.end(), compareByTime);
    cout << "The chronologically sorted list of " << combinedData.size() << " values is:" << endl;
    for (size_t i = 0; i < combinedData.size(); ++i) {
        cout << combinedData[i].value << " ";
    }
    cout << endl;
    cout << "The mean is " << mean << endl; // The mean remains the same
    cout << "The median is " << median << endl;
    cout << "The mode is " << mode << endl;
    string outputFilename;
    cout << "Enter the output filename to save: ";
    cin >> outputFilename;
    ofstream outfile(outputFilename);
    outfile << "*** Summarized Statistics ***" << endl;
    outfile << "The orderly sorted list of " << combinedData.size() << " values is:" << endl;
    for (size_t i = 0; i < combinedData.size(); ++i) {
        outfile << combinedData[i].value << " ";
    }
    outfile << endl;
    outfile << "The mean is " << fixed << setprecision(4) << mean << endl;
    outfile << "The median is " << median << endl;
    outfile << "The mode is " << mode << endl;
    outfile << "The chronologically sorted list of " << combinedData.size() << " values is:" << endl;
    for (size_t i = 0; i < combinedData.size(); ++i) {
        outfile << combinedData[i].value << " ";
    }
    outfile << endl;
    outfile << "The mean is " << mean << endl;
    outfile << "The median is " << median << endl;
    outfile << "The mode is " << mode << endl;
    outfile.close();
    cout << "*** File " << outputFilename << " has been written to disk ***" << endl;
    cout << "*** Goodbye. ***" << endl;
    return 0;
}

