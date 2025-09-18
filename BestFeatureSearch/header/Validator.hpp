#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <vector>
#include <iostream>
#include <iomanip>
#include "../header/Instance.hpp"
#include "../header/classifer.hpp"
#include <chrono>
using namespace std;


class Validator{
    public:
        vector<int> feature_subset;
        Classifier classifier;
        vector<dataInstance> dataset;
        Validator(vector<int> features, Classifier classifier, vector<dataInstance> dataset);
        ~Validator();
        double computeAccuracy();
};

Validator::Validator(vector<int> features, Classifier classifier, vector<dataInstance> dataset)
{
    this->feature_subset = features;
    this->classifier = classifier;
    this->dataset = dataset;
}

Validator::~Validator()
{
    this->feature_subset.clear();
}

double Validator::computeAccuracy(){
    int correctPredictions = 0;
    // for the number of instances in dataset
    for (dataInstance i : dataset){
        // count the instances where classifier correctly predicted the instance label
        if (i.classification == i.classifierPrediction)
            correctPredictions++;
    }
    
    return correctPredictions;
}

#endif /* VALIDATOR_HPP */