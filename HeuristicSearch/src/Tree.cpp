#include "../header/Tree.hpp"
#include <cmath>
#include <queue>
#include <vector>
using namespace std;

Tree::Tree(){}

Tree::Tree(Problem& problem){
    this->problem = problem;
    root = problem.initialState;
}

void Tree::expandNodeHelper(node leaf, int inputHeuristicHelper){
    bool inExploredSet = false;
    bool inFrontier = false;    

    // Check if in explored set or frontier
    for(int i=0; i<exploredSet.size(); i++){
        if(leaf==exploredSet[i]){
            inExploredSet = true;
            break;
        }
    }
    for(int i=0; i<frontierVector.size(); i++){
        if(leaf==frontierVector[i]){
            inFrontier = true;
            break;
        }
    }

    if(!inExploredSet && !inFrontier){
        leaf.totalCost = leaf.edgeCost++;
        goalDepth = leaf.edgeCost++; //separate int that keeps track of edgecost depth not affect totalcost
        if(inputHeuristicHelper == 2){ 
            leaf.heuristicCost = calculateMisplacedTiles(leaf); //calulate heurisitc based on misplaced tiles func
        }
        else if(inputHeuristicHelper == 3){
            leaf.heuristicCost = calculateEuclideanDistance(leaf);//calulate heurisitc based on euclidean distance func
        }
        frontier.push(leaf);
        frontierVector.push_back(leaf);
        problem.printState(leaf);
        cout << endl;
    }
    else {
        cout << "Found in explored set or frontier\n" << endl;
    }

    if(leaf==problem.goalState){
        cout << "FOUND GOAL STATE" << endl;
        foundGoalState = true;
        currentNode = leaf;
    }
}

void Tree::expandNode(node currentNode, int inputHeuristicExpandNode){
    // find 0 row and column
    int emptyRow, emptyColumn;
    for(int row=0; row<3; row++){
        for(int column=0; column<3; column++){
            if(currentNode.state[row][column]==0){
                cout << "Found empty tile at " << row << " " << column << endl;
                emptyRow = row;
                emptyColumn = column;
                break;
            }
        }
    }

    // Do every move on currentNode and add push valid moves to frontier
    if(emptyRow>0){
        cout << "Move up" << endl;
        // move up
        node leaf = problem.shiftUp(currentNode);
        expandNodeHelper(leaf, inputHeuristicExpandNode);
    }
    if(emptyRow<2){
        cout << "Move down" << endl;
        // move down
        node leaf = problem.shiftDown(currentNode);
        expandNodeHelper(leaf, inputHeuristicExpandNode);
    }
    if(emptyColumn>0){
        cout << "Move left" << endl;
        // move left
        node leaf = problem.shiftLeft(currentNode);
        expandNodeHelper(leaf, inputHeuristicExpandNode);
    }
    if(emptyColumn<2){
        cout << "Move right" << endl;
        // move right
        node leaf = problem.shiftRight(currentNode);
        expandNodeHelper(leaf, inputHeuristicExpandNode); 
    }
}

void Tree::graphSearch(int inputHeuristicMenu){
    // initialize frontier with initial state of problem
    frontier.push(root);
    
    int edgeCounter = currentNode.edgeCost;
    cout << "Right here current node edge cost = " << edgeCounter << endl;

    // loop
    while(!(frontier.empty())){
        // choose top minimum leaf node
        currentNode = frontier.top();

        if (currentNode == problem.goalState) { // handles when initial is goal state.
            foundGoalState = true;
        }

        if(frontier.size() > maxNumNodes) { // update maxNodes if frontiersize is greater than max num of nodes in queue at any one time.
            maxNumNodes = frontier.size();
        }

        if(foundGoalState){
            problem.printState(currentNode);
            cout << endl;
            return;
            // break;
        }

        // remove it from frontier
        frontier.pop();

        // add node to explored set
        exploredSet.push_back(currentNode);
        // node to explored set means adding to total nodes
        totalNodes++;
        
        cout << endl;
        if(inputHeuristicMenu == 2){ 
            currentNode.heuristicCost = calculateMisplacedTiles(currentNode); //calulate heurisitc based on misplaced tiles func
            cout << "The best state to expand with edge cost = " << edgeCounter 
            << " and Misplaced tiles heuristic cost = " << currentNode.heuristicCost << endl;
        }
        else if(inputHeuristicMenu == 3){
            currentNode.heuristicCost = calculateEuclideanDistance(currentNode);//calulate heurisitc based on euclidean distance func
            cout << "The best state to expand with edge cost = " << edgeCounter 
            << " and Euclidean heuristic cost = " << currentNode.heuristicCost << endl;
        }
        problem.printState(currentNode);
        edgeCounter++; //increases edgeCost when actual best state has been declared
        cout << endl;
        expandNode(currentNode, inputHeuristicMenu);
    }
    if(frontier.empty()){
        cout << "\n\nFailed to solve using UCS...\n\n";
        return;
    }
}

int Tree::calculateMisplacedTiles(node currentNode){
    int totalMisplacedTiles = 0;
    int goalStateDirect[3][3] = {{1,2,3},{4,5,6},{7,8,0}}; //initalize directly what is the goalState for 3x3
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            //if (currentNode.state[i][j] != 0 && currentNode.state[i][j] != problem.goalState.state[i][j]){ //tookout the problem goalState.state
            if (currentNode.state[i][j] != 0 && currentNode.state[i][j] != goalStateDirect[i][j]){
                totalMisplacedTiles++;
            }
        }
    }
    return totalMisplacedTiles;
}

int Tree::calculateEuclideanDistance(node currentNode){
    double totalEuclideanDistance = 0.0;
    int goalRow = -1;
    int goalColumn = -1;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(currentNode.state[i][j] != 0 && currentNode.state[i][j] != problem.goalState.state[i][j]){
                for(int row=0; row<3; row++){
                    for(int column=0; column<3; column++){
                        if(problem.goalState.state[row][column] == currentNode.state[i][j]){
                            goalRow = row;
                            goalColumn = column;
                        }
                    }
                }
                totalEuclideanDistance += sqrt((pow(i - goalRow, 2)) + (pow(j - goalColumn, 2)));
            }
        }
    }
    return totalEuclideanDistance; //Euclidean Distance is the heuristic cost 
}

int Tree::getMaxNumNodes(){ //getter function for main
    return maxNumNodes;
}
int Tree::getGoalDepth(){ //getter function for main
    return goalDepth;
}

int Tree::getTotalNodes(){
    return totalNodes;
}