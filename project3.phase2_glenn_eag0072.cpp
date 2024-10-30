#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>
#include <numeric>

struct Measurement {
    double value;
    int hour, minute, second;

    // Comparator for chronological sorting
    bool operator<(const Measurement &other) const {
        if (hour != other.hour) return hour < other.hour;
        if (minute != other.minute) return minute < other.minute;
        return second < other.second;
    }
};

// Function to read data from a file and handle malformed data
bool readData(const std::string &filename, std::vector<Measurement> &data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        double value;
        std::string timestamp;

        if (!(iss >> value >> timestamp)) {
            std::cerr << "Error: Malformed line in file " << filename << "\n";
            return false; // Malformed line
        }

        Measurement m;
        m.value = value;

        // Split timestamp into hours, minutes, and seconds
        std::replace(timestamp.begin(), timestamp.end(), '.', ' ');
        std::istringstream timeStream(timestamp);
        if (!(timeStream >> m.hour >> m.minute >> m.second) ||
            m.hour < 0 || m.hour > 23 ||
            m.minute < 0 || m.minute > 59 ||
            m.second < 0 || m.second > 59) {
            std::cerr << "Error: Invalid timestamp in file " << filename << "\n";
            return false;
        }

        data.push_back(m);
    }
    return true;
}

// Function to calculate mean
double calculateMean(const std::vector<Measurement> &data) {
    double sum = std::accumulate(data.begin(), data.end(), 0.0, [](double acc, const Measurement &m) {
        return acc + m.value;
    });
    return sum / data.size();
}

// Function to calculate median
double calculateMedian(std::vector<Measurement> data) {
    std::sort(data.begin(), data.end(), [](const Measurement &a, const Measurement &b) {
        return a.value < b.value;
    });
    size_t size = data.size();
    if (size % 2 == 0) {
        return (data[size / 2 - 1].value + data[size / 2].value) / 2;
    } else {
        return data[size / 2].value;
    }
}

// Function to calculate mode
double calculateMode(const std::vector<Measurement> &data) {
    std::map<double, int> frequency;
    for (const auto &m : data) {
        frequency[m.value]++;
    }
    return std::max_element(frequency.begin(), frequency.end(),
                            [](const std::pair<double, int> &a, const std::pair<double, int> &b) {
                                return a.second < b.second;
                            })->first;
}

// Function to display sorted data
void displaySortedData(const std::vector<Measurement> &data) {
    for (const auto &m : data) {
        std::cout << m.value << " ";
    }
    std::cout << "\n";
}

int main() {
    int numFiles;
    std::cout << "*** Welcome to Li's Data Analyzer ***\n";
    std::cout << "Enter the number of files to read: ";
    std::cin >> numFiles;

    std::vector<Measurement> allData;

    // Read each file
    for (int i = 0; i < numFiles; ++i) {
        std::string filename;
        std::cout << "Enter the filename for file " << (i + 1) << ": ";
        std::cin >> filename;

        std::vector<Measurement> fileData;
        if (!readData(filename, fileData)) {
            std::cerr << "Error: Could not read file " << filename << "\n";
            --i; // Prompt for the file again
            continue;
        }
        allData.insert(allData.end(), fileData.begin(), fileData.end());
    }

    // Sort and display data numerically
    std::sort(allData.begin(), allData.end(), [](const Measurement &a, const Measurement &b) {
        return a.value < b.value;
    });

    std::cout << "\n*** Summarized Statistics ***\n";
    std::cout << "The orderly sorted list of values is:\n";
    displaySortedData(allData);

    double mean = calculateMean(allData);
    double median = calculateMedian(allData);
    double mode = calculateMode(allData);

    std::cout << "The mean is " << mean << "\n";
    std::cout << "The median is " << median << "\n";
    std::cout << "The mode is " << mode << "\n";

    // Sort and display data chronologically
    std::sort(allData.begin(), allData.end());

    std::cout << "\nThe chronologically sorted list of values is:\n";
    displaySortedData(allData);

    median = calculateMedian(allData); // Recalculate median for chronological sort

    std::cout << "The mean is " << mean << "\n";
    std::cout << "The median is " << median << "\n";
    std::cout << "The mode is " << mode << "\n";

    // Save results to an output file
    std::string outputFilename;
    std::cout << "Enter the output file name to save: ";
    std::cin >> outputFilename;

    std::ofstream outFile(outputFilename);
    if (outFile) {
        outFile << "*** Summarized Statistics ***\n";
        outFile << "The orderly sorted list of values is:\n";
        for (const auto &m : allData) {
            outFile << m.value << " ";
        }
        outFile << "\nThe mean is " << mean << "\n";
        outFile << "The median is " << median << "\n";
        outFile << "The mode is " << mode << "\n";

        outFile << "\nThe chronologically sorted list of values is:\n";
        for (const auto &m : allData) {
            outFile << m.value << " ";
        }
        outFile << "\nThe mean is " << mean << "\n";
        outFile << "The median is " << median << "\n";
        outFile << "The mode is " << mode << "\n";

        outFile.close();
        std::cout << "*** File " << outputFilename << " has been written to disk ***\n";
    } else {
        std::cerr << "Error: Could not write to file " << outputFilename << "\n";
    }

    std::cout << "*** Goodbye. ***\n";
    return 0;
}
