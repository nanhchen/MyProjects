#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <chrono> 

#include "../header/Instance.hpp"
#include "../header/classifer.hpp"
#include "../header/Validator.hpp"

using namespace std::chrono;
using namespace std;

void normalizeData(vector<dataInstance> &dataset) {
    // for each feature column, we need to take the min and max.
    // Then normalize each feature in the column using
    // Normalize formula: (x - min) / (max - min)

    for (size_t j = 0; j < dataset[0].features.size(); j++) {
        double min = numeric_limits<double>::max();
        double max = numeric_limits<double>::lowest();
        // find min and max for column 'j'
        for (dataInstance instance : dataset) {
            min = std::min(min, instance.features[j]);
            max = std::max(max, instance.features[j]);
        }
        // normalize column 'j'
        double range = max - min;
        if (range == 0) { continue; }
        for  (dataInstance &instance : dataset) {
            instance.features[j] = (instance.features[j] - min) / range;
        }
    }
}

int main() {

    cout << "\n================ EVALUATION FUNCTION & NN-CLASSIFIER ================\n" << endl;
    
    int option;
    do {
        cout << "\nPlease select a dataset to use: (enter '1' or '2')" << endl;
        cout << "1) small dataset" << endl;
        cout << "2) large dataset" << endl;
        cout << "3) titanic dataset" << endl;
        cout << "option: ";
        cin >> option;
        if (option != 1 && option != 2) {
            cout << "\nInvalid input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (option != 1 && option != 2 && option != 3);

    string filename;
    if (option == 1) {
        filename = "../tests/small-test-dataset.txt";
    }
    else if (option == 2) {
        filename = "../tests/large-test-dataset.txt";
    } else if (option == 3) {
        filename = "../tests/titanic-clean.txt";
    }

    // read file with test dataset
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return 1;
    }
    cout << "\nReading File: " << filename << endl;

    auto start_DatasetAnalysis = high_resolution_clock::now();
    
    // vector to store training data.
    vector<dataInstance> dataset;

    string line;
    int i = 0;
    int j = 1;
    int precision = 7;
    while (getline(file, line)) {
        istringstream iss(line);
        double classi;
        double feature;
        dataInstance instance;

        // 1st column is classification, store that.
        if (!(iss >> classi)) {
            cout << "ERROR: invalid data format\n" << endl;
        } else {
            instance.classification = classi;
        }
        // read in and store features of data instance.
        while (iss >> feature) {
            double rounded = round(feature * pow(10, precision) / pow(10, precision));
            instance.features.push_back(feature);
            i++;
        }
        i = 0;
        instance.instanceID = j++;
        dataset.push_back(instance);
    }
    file.close();
    auto stop_DatasetAnalysis = high_resolution_clock::now();
    

    auto start_normalizeData = high_resolution_clock::now();  
    normalizeData(dataset);
    auto stop_normalizeData = high_resolution_clock::now();
    

    cout << "Dataset normalized\n" << endl;

    // // feature set selection
    vector<int> featureSet;
    if (option == 1) {
        option = 0;
        do {
            cout << "Select a feature set to test with: (enter '1' or '2')" << endl;
            cout << "1) entire feature set" << endl;
            cout << "2) only features {3, 5, 7}" << endl;
            cout << "option: ";
            cin >> option;
            if (option != 1 && option != 2) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (option != 1 && option != 2);
        printf("\n");
        // user chose feature subset
        if (option == 2) {
            featureSet.push_back(3);
            featureSet.push_back(5);
            featureSet.push_back(7);
        }
    }
    else if (option == 2) {
        option = 0;
        do {
            cout << "Select a feature set to test with: (enter '1' or '2')" << endl;
            cout << "1) entire feature set" << endl;
            cout << "2) only features {1, 15, 27}" << endl;
            cout << "option: ";
            cin >> option;
            if (option != 1 && option != 2) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (option != 1 && option != 2);
        printf("\n");
        // user chose feature subset
        if (option == 2) {
            featureSet.push_back(1);
            featureSet.push_back(15);
            featureSet.push_back(27);
        }
    }
    // user chose to test with all features
    if (option == 1 || option == 3) {
        for (int i = 1; i < dataset[0].features.size()+1; i++) {
            featureSet.push_back(i);
        }
    }
    
    Classifier classifier;
    classifier.Train(dataset);
     
    for (int i = 0; i<dataset.size(); i++) {
        int predicted = classifier.Test(featureSet, i);
        dataset[i].classifierPrediction = predicted;
        printf("Data Instance #%d\t| ", dataset[i].instanceID);
        printf("Predicted: %d \t| ", dataset[i].classifierPrediction);
        printf("Observed: %d\n", dataset[i].classification);
    }
    
    Validator validator = Validator(featureSet, classifier, dataset);
    double accuracy = validator.computeAccuracy();
    printf("Result: %d/%d\n", (int)accuracy, dataset.size());
    cout << endl;
    double percentAccuracy = accuracy / (double)dataset.size() * 100;
    printf("Accuracy: %.1f%%\n", percentAccuracy);
    cout << "Time taken for data set analysis: "
    << duration_cast<milliseconds>(stop_DatasetAnalysis - start_DatasetAnalysis).count() << "ms" << endl;
    cout << "Time taken for normalization: "
    << duration_cast<milliseconds>(stop_normalizeData - start_normalizeData).count() << "ms" << endl;

    return 0;
}