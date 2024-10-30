#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

int main() {
    cout << "*** Welcome to Li's Data Analyzer ***" << endl;
    
    // Commit 1: Reading inputs from a single file
    int numFiles;
    cout << "Enter the number of files to read: ";
    cin >> numFiles;

    vector<double> combinedData;
    
    // Commit 2: Reading multiple files
    for (int i = 1; i <= numFiles; ++i) {
        string filename;
        cout << "Enter the filename for file " << i << ": ";
        cin >> filename;
        
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error opening file " << filename << endl;
            continue;
        }

        double value;
        while (inFile >> value) {
            combinedData.push_back(value);
        }

        inFile.close();
    }

    // Commit 3: Merging and sorting values
    sort(combinedData.begin(), combinedData.end());
    
    // Commit 4: Calculate relevant statistics
    double mean = accumulate(combinedData.begin(), combinedData.end(), 0.0) / combinedData.size();
    double median = combinedData[combinedData.size() / 2]; // Simplified for demonstration
    double mode = combinedData[0]; // Placeholder, calculate mode properly

    // Commit 5: Output all results to the terminal
    cout << "*** Summarized Statistics ***" << endl;
    cout << "The sorted list of values is:" << endl;
    for (double val : combinedData) {
        cout << val << " ";
    }
    cout << endl;
    cout << "Mean: " << mean << endl;
    cout << "Median: " << median << endl;
    cout << "Mode: " << mode << endl;

    // Commit 6: Writing outputs to a file
    string outputFilename;
    cout << "Enter the output file name to save: ";
    cin >> outputFilename;

    ofstream outFile(outputFilename);
    outFile << "*** Summarized Statistics ***" << endl;
    outFile << "Sorted list of values:" << endl;
    for (double val : combinedData) {
        outFile << val << " ";
    }
    outFile << endl;
    outFile << "Mean: " << mean << endl;
    outFile << "Median: " << median << endl;
    outFile << "Mode: " << mode << endl;
    outFile.close();

    cout << "*** File " << outputFilename << " has been written to disk ***" << endl;
    cout << "*** Goodbye. ***" << endl;

    return 0;
}
