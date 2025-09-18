#ifndef NODE_HPP
#define NODE_HPP
#include <iostream>
#include <vector>

using namespace std;

struct node{
    int state[3][3]; //change for general puzzles like 15 or 25 puzzle
    int edgeCost = 0; 
    int heuristicCost = 0;
    int totalCost = 0;
    // node *ptr = NULL; 

    bool operator==(const node& other) const{   // Compare state of nodes
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(state[i][j] != other.state[i][j])
                    return false;
            }
        }
        return true;
    }
};

struct compareOperator{
    bool operator()(const node& one, const node& two) const{    // Compare total cost of nodes for min heap on frontier
        return (one.edgeCost + one.heuristicCost) > (two.edgeCost + two.heuristicCost);
    }
};

#endif