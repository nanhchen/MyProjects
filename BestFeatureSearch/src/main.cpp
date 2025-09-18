#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <variant>

#include "../header/Instance.hpp"
#include "../header/classifer.hpp"
#include "../header/Validator.hpp"
using namespace std;

/*
randomAccuracy double: returns a random double value from 0-100
    random_device rd: non-deterministic random number generator
    mt19937 gen(rd()): Mersenne Twister engine
    uniform_real_distribution<double> urd: distribution range 
*/
double randomAccuracy(){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> urd(0.0, 100.0);
    return urd(gen);
}

/*
node struct: represents each node in search tree
    vector<int> selectFeatures: vector for the features in use at this node
    double accuracy: calls randomAccuracy() to create a random double value from 0-100
    int layer: integer that tracks is layer in the tree
    void printFeatures(): prints the list of features with formatted commas
*/
struct node{
    vector<int> selectFeatures;
    double accuracy;
    int classPredict;   // phase 3
    int layer;
    void printFeatures(){
        for(int i=0; i<selectFeatures.size(); i++){
            if(i==selectFeatures.size()-1)
                cout << selectFeatures[i];
            else
                cout << selectFeatures[i] << ", ";
        }
    }
};

// return true if feature in found in the parameter set.
bool inSet(vector<int> set, int feature) {
    for (int i = 0; i < set.size(); i++) {
        if (set[i] == feature) {
            return true;
        }
    }
    return false;
}

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

vector<dataInstance> parseData(){
    int option;
    do {
        cout << "\nPlease select a dataset to use: (enter '1' or '2' or '3')" << endl;
        cout << "1) small dataset" << endl;
        cout << "2) large dataset" << endl;
        cout << "3) titanic dataset" << endl;
        cout << "option: ";
        cin >> option;
        if (option != 1 && option != 2 && option != 3) {
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
    }
    else if (option == 3) {
        filename = "../tests/titanic-clean.txt";
    }

    // vector to store training data.
    vector<dataInstance> dataset;

    // read file with test dataset
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return dataset;
    }
    cout << "\nReading File: " << filename << endl;

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
    
    normalizeData(dataset);
    cout << "Dataset normalized\n";

    // // feature set selection
    vector<int> featureSet;
    if (option == 1) {
        option = 0;
        do {
            cout << "\nSelect a feature set to test with: (enter '1' or '2')" << endl;
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
            cout << "\nSelect a feature set to test with: (enter '1' or '2')" << endl;
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
        for (int i = 1; i < dataset[0].features.size(); i++) {
            featureSet.push_back(i);
        }
    }
    return dataset;
}

/*
forwardSelection void: takes a node parameter for the root (used for it's complete list of features) and carries out forward selection
    vector<node> tree: vector for nodes in the search tree, without the root node and lastBestNode node
    bool accuracyDecreased: flag to stop search once the accuracy stops increasing
    node lastBestNode: saves the previous best node and is used to determine if accuracy has decreased compared to bestNode
    node bestNode: temporary best node for the each layer of tree and used for creating new nodes
    node leaf: temporary node used for creating new nodes in tree
*/
void forwardSelection(node root, vector<dataInstance> dataset){
    node lastBestNode;
    // ========================================================================================= Phase 3 added coded
    Classifier classifier;
    classifier.Train(dataset);
    for (int i = 0; i<dataset.size(); i++) {
        dataset[i].classifierPrediction = classifier.Test(lastBestNode.selectFeatures, i);
    }
    Validator validator = Validator(lastBestNode.selectFeatures, classifier, dataset);
    lastBestNode.accuracy = validator.computeAccuracy();
    sort(lastBestNode.selectFeatures.begin(), lastBestNode.selectFeatures.end());
    // ========================================================================================= Phase 3 added coded

    bool accuracyDecreased = false;
    printf("Running nearest neighbor with no features (default rate), using \"leaving-one-out\" evaluation, there is an accuracy of %.2f%%", lastBestNode.accuracy / (double)dataset.size() * 100);
    
    vector<node> tree;  
    // initialize root and best accuracy to 0
    node bestNode;
    bestNode.accuracy = root.accuracy = 0;
    bestNode.layer = root.layer = 0;

    cout << "\n\nBeginning search.\n\n";

    // Classifier NN;
    // NN.Train(dataset);
    // create first layer of nodes, each with one of the features
    for(int i=0; i<root.selectFeatures.size(); i++){
        node leaf;
        leaf.selectFeatures.push_back(root.selectFeatures[i]);
        leaf.layer=1;
        // ========================================================================================= Phase 3 added code
        for (int i = 0; i<dataset.size(); i++) {
            dataset[i].classifierPrediction = classifier.Test(leaf.selectFeatures, i);
            // printf("Data Instance #%d\t| ", dataset[i].instanceID);
            // printf("Predicted: %d \t| ", dataset[i].classifierPrediction);
            // printf("Observed: %d\n", dataset[i].classification);
        }
        
        Validator validator = Validator(leaf.selectFeatures, classifier, dataset);
        leaf.accuracy = validator.computeAccuracy();
        sort(leaf.selectFeatures.begin(), leaf.selectFeatures.end());
        // ========================================================================================= Phase 3 added coded

        tree.push_back(leaf);
        printf("\tUsing feature(s) {%d} accuracy is %.2f%%\n", leaf.selectFeatures[0], leaf.accuracy / (double)dataset.size() * 100);
    }

    // carry out forward selection for the each layer of tree, which is the size of the number of features, unless accuracy decreases
    int currentLayer=1;
    while(currentLayer <= root.selectFeatures.size()){
        // compare the accuracy of the leaves in this layer and save the node with best accuracy as bestNode
        for(auto j : tree){
            if(j.layer==currentLayer){
                if(bestNode.accuracy < j.accuracy){
                    bestNode.accuracy = j.accuracy;
                    bestNode.selectFeatures = j.selectFeatures;
                }
            }
            else 
                continue;
        }

        // if the lastBestNode is better than current layer's bestNode, stop greedy search and signal that accuracy has decreased
        if(lastBestNode.accuracy > bestNode.accuracy){
            cout << "\n(Warning, Accuracy has decreased!)";
            accuracyDecreased = true;
            // break;
        }
        // else update the lastBestNode with this new bestNode
        else{
            lastBestNode.accuracy = bestNode.accuracy;
            lastBestNode.selectFeatures = bestNode.selectFeatures;
        }
        
        cout << "\nFeature set {";
        bestNode.printFeatures();
        printf("}, was best, accuracy is %.2f%%\n\n", bestNode.accuracy / (double)dataset.size() * 100);

        // create new nodes by combining a missing feature from best node with the features in best node
        for (int feature : root.selectFeatures) {
            if (find(bestNode.selectFeatures.begin(), bestNode.selectFeatures.end(), feature) == bestNode.selectFeatures.end()) {   // if feature not found in bestNode
                node leaf;
                leaf.selectFeatures = bestNode.selectFeatures;
                leaf.selectFeatures.push_back(feature);
                leaf.layer=currentLayer+1;

                // ========================================================================================= Phase 3 added coded
                for (int i = 0; i<dataset.size(); i++) {
                    dataset[i].classifierPrediction = classifier.Test(leaf.selectFeatures, i);
                }
        
                Validator validator = Validator(leaf.selectFeatures, classifier, dataset);
                leaf.accuracy = validator.computeAccuracy();
                sort(leaf.selectFeatures.begin(), leaf.selectFeatures.end());
                // ========================================================================================= Phase 3 added coded

                tree.push_back(leaf);
                cout << "\tUsing feature(s) {";
                leaf.printFeatures();
                printf("} accuracy is %.2f%%\n", leaf.accuracy / (double)dataset.size() * 100);
            }
        }
        
        // reset best node for the next layer
        bestNode.accuracy=0;
        bestNode.selectFeatures.clear();

        currentLayer++;
    }

    // once all features have been used, stop search and report last best node's features and accuracy
    cout << "\nFinished search! The best feature subset is {";
    lastBestNode.printFeatures();
    printf("}, which has an accuracy of %.2f%%\n\n", lastBestNode.accuracy / (double)dataset.size() * 100);
}

void backwardsElimination(node root, vector<dataInstance> dataset) {
    node lastBestNode = root;
    bool accuracyDecreased = false;
    vector<node> tree;
// ========================================================================================= Phase 3 added coded
    Classifier classifier;
    classifier.Train(dataset);
    for (int i = 0; i<dataset.size(); i++) {
        int predicted = classifier.Test(lastBestNode.selectFeatures, i);
        dataset[i].classifierPrediction = predicted;
    }
    Validator validator = Validator(lastBestNode.selectFeatures, classifier, dataset);
    double accuracy = validator.computeAccuracy();
    double percentAccuracy = accuracy / (double)dataset.size() * 100;
    lastBestNode.accuracy = percentAccuracy;
    sort(lastBestNode.selectFeatures.begin(), lastBestNode.selectFeatures.end());
// =========================================================================================

    printf("Using all %d features and \"leaving-one-out\" evaluation, there is an accuracy of %.2f%%", root.selectFeatures.size(), percentAccuracy);

    // initialize root and best accuracy to 0
    node bestNode;
    bestNode.accuracy = root.accuracy = lastBestNode.accuracy;
    bestNode.layer = root.layer = 0;
    bestNode.selectFeatures = root.selectFeatures;

    cout << "\n\nBeginning search.\n\n";

    // create first layer
    for(int i = 0; i < root.selectFeatures.size(); i++){
        node leaf;
        leaf.selectFeatures = bestNode.selectFeatures;
        leaf.selectFeatures.erase(find(leaf.selectFeatures.begin(), leaf.selectFeatures.end(), root.selectFeatures[i]));
        leaf.layer = 1;     
        // ========================================================================================= Phase 3 added coded
        for (int i = 0; i<dataset.size(); i++) {
            int predicted = classifier.Test(leaf.selectFeatures, i);
            dataset[i].classifierPrediction = predicted;
        }
        Validator validator = Validator(leaf.selectFeatures, classifier, dataset);
        double accuracy = validator.computeAccuracy();
        double percentAccuracy = accuracy / (double)dataset.size() * 100;
        leaf.accuracy = percentAccuracy;
        sort(leaf.selectFeatures.begin(), leaf.selectFeatures.end());
        // ========================================================================================= Phase 3 added coded


        tree.push_back(leaf);
        printf("\tUsing feature(s) {");
        leaf.printFeatures();
        printf("} accuracy is %.2f%%\n", leaf.accuracy);
    }

    int currentLayer = 1;
    while (currentLayer <= root.selectFeatures.size()) {
        // compare accuracies of leaves in the current tree layer, save node w/ best accuracy as bestNode.
        for (auto j : tree) {
            if (j.layer == currentLayer) {
                if (bestNode.accuracy < j.accuracy) {
                    bestNode.accuracy = j.accuracy;
                    bestNode.selectFeatures = j.selectFeatures;
                }
            } else { continue; }
        }
        // if the lastBestNode is better than current layer's bestNode, stop greedy search and signal that accuracy has decreased
        if(lastBestNode.accuracy > bestNode.accuracy){
            cout << "\n(Warning, Accuracy has decreased!)";
            accuracyDecreased = true;
            // break;
        }
        // else update the lastBestNode with this new bestNode
        else{
            lastBestNode.accuracy = bestNode.accuracy;
            lastBestNode.selectFeatures = bestNode.selectFeatures;
        }
        cout << "\nFeature set {";
        bestNode.printFeatures();
        printf("}, was best, accuracy is %.2f%%\n\n", bestNode.accuracy);

        // create new nodes by removing one feature from bestNode set.
        for (int feature : root.selectFeatures) {
            if (inSet(bestNode.selectFeatures, feature)) {
                node leaf;
                leaf.selectFeatures = bestNode.selectFeatures;
                leaf.selectFeatures.erase(find(leaf.selectFeatures.begin(), leaf.selectFeatures.end(), feature));
                leaf.layer = currentLayer + 1;   
            // ========================================================================================= Phase 3 added coded
                for (int i = 0; i<dataset.size(); i++) { 
                    int predicted = classifier.Test(leaf.selectFeatures, i);
                    dataset[i].classifierPrediction = predicted;
                }
                Validator validator = Validator(leaf.selectFeatures, classifier, dataset);
                double accuracy = validator.computeAccuracy();
                double percentAccuracy = accuracy / (double)dataset.size() * 100;
                leaf.accuracy = percentAccuracy;
                sort(leaf.selectFeatures.begin(), leaf.selectFeatures.end());
            // ========================================================================================= Phase 3 added coded
                tree.push_back(leaf);
                printf("\tUsing feature(s) {");
                leaf.printFeatures();
                printf("} accuracy is %.2f%%\n", leaf.accuracy);
            } else {
                continue;
            }
        }

        // reset bestNode for next layer
        bestNode.accuracy = 0;
        bestNode.selectFeatures.clear();
        currentLayer++;
    }

    // once all features have been used, stop search and report last best node's features and accuracy
    cout << "\nFinished search! The best feature subset is {";
    lastBestNode.printFeatures();
    printf("}, which has an accuracy of %.2f%%\n\n", lastBestNode.accuracy);
}

vector<dataInstance> dataSet;

int main(){
    cout << "\nWelcome to our Feature Selection Algorithm\n";
    
    dataSet = parseData();

    if(dataSet.size()<1){
        cout << "Failed to parse file" << endl;
    }
    else{
        cout << "The dataset has " << dataSet[0].features.size() << " features with " << dataSet.size() << " instances." << endl;
    }

    int totalNumFeatures = dataSet[0].features.size();

    // node root;
    // // initialize root node with all features
    // for (int i=1; i<=totalNumFeatures; i++){
    //     root.selectFeatures.push_back(i);
    // }
    
    int userAlgChoice;
    cout << "\nChoose the algorithm you want to run:\n\t1) Forward Selection\n\t2) Backward Selection\n\t";
    cout << "\n > ";
    cin >> userAlgChoice;
    while (cin.fail() || (userAlgChoice != 1 && userAlgChoice != 2)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nPlease input a valid number for the algorithm of choice:\n\t1) Forward Selection\n\t2) Backward Selection\n\t";
        cout << "\n > ";
        cin >> userAlgChoice;
    }

    if (userAlgChoice==1){
        cout << "\nUSING FORWARD SELECTION\n\n";
        node root;
        // initialize root node with all features
        for (int i=1; i<=totalNumFeatures; i++){
            root.selectFeatures.push_back(i);
        }
        forwardSelection(root, dataSet);
    } 
    else if (userAlgChoice==2){
        cout << "\nUSING BACKWARD ELIMINATION\n\n";
        node root;
        // initialize root node with all features
        for (int i=1; i<=totalNumFeatures; i++){
            root.selectFeatures.push_back(i);
        }
        backwardsElimination(root, dataSet);
    } 

    return 0;
}

/*
Group: Joshua Pena (jpena079) Session 1, Nicole Chen (nchen098) Session 1, Truman Fung (tfung012) Session 1
    DatasetID:tittanic-clean
        - Forward: Feature Subset: {2}, Acc: 78.01%
        - Backwards: Feature Subset: {2}, Acc: 78.01%
    Small Dataset Results:
        - Forward: Feature Subset: {3, 5}, Acc: 92%
        - Backwards: Feature Subset: {3, 5}, Acc: 92%
    Large Dataset Results:
        - Forward: Feature Subset: {1, 27}, Acc: 95.5%
        - Backwards: Feature Subset: {27}, Acc: 84.7%   --> chose between two subsets with the same acc but chose the subset w/o feature 1.
*/
