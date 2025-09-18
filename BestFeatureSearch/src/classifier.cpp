#include  "../header/classifer.hpp"
#include <cfloat>
#include <cmath>

double Classifier::euclideanDistance(const vector<int> featuresToTest, int test, int instance){
    double sum = 0.0;
    dataInstance testInstance = trainingDataset[test];
    dataInstance dataInstance = trainingDataset[instance];
    for (int i = 0; i < featuresToTest.size(); i++){
        sum += pow(testInstance.features[featuresToTest[i] - 1] - dataInstance.features[featuresToTest[i]-1], 2);
    }
    return sqrt(sum);
}
//like a setter for dataset
void Classifier::Train(const vector<dataInstance> &trainingInstances) {
    this->trainingDataset = trainingInstances;
}

int Classifier::Test(vector<int> featuresToTest, int index){
    int nearestNeighborLabel = -1;
    double leastDist = DBL_MAX;
    //for loop that finds the shortest distance
    for (int i = 0; i < trainingDataset.size(); i++) {
        if (trainingDataset[i].instanceID == trainingDataset[index].instanceID){ //skip the one instance
            continue;
        }
        double currdataInstanceDist = euclideanDistance(featuresToTest, index, i);
        if (currdataInstanceDist < leastDist){
            //update nearestNeighbor to be at current index
            nearestNeighborLabel = trainingDataset[i].classification;
            //update whats the least distance from target dataInstance
            leastDist = currdataInstanceDist;
        }
    }
    return nearestNeighborLabel;
}