#ifndef INSTANCE_HPP
#define INSTANCE_HPP
#include <vector>

// struct for tracking the contents of each data instance.
struct dataInstance {
    int instanceID;
    int classification;
    int classifierPrediction;
    std::vector<double> features;
};

#endif