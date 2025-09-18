#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <iostream>
#include <vector>
#include "../header/Instance.hpp"

using namespace std;

class Classifier{
    public:
    vector<dataInstance> trainingDataset;
    // Classifier(/* args */);
    // ~Classifier();
    double euclideanDistance(const vector<int> featuresToTest, int test, int instance);
    void Train(const vector<dataInstance> &trainingInstances);
    int Test(vector<int> featuresToTest, int index);      
};

#endif
