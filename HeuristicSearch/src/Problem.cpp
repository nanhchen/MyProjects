#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

#include "../header/Problem.hpp"

Problem::Problem(){ // option "1" for default puzzle
    int counter = 0; // position for square
    int squares[9] = {1,0,3,4,2,6,7,5,8}; // 0 is the start square
    for(int i = 0; i < puzzleSize; ++i) {   //row
        for(int j = 0; j < puzzleSize; ++j) {   //column
            puzzle.state[i][j] = squares[counter];    
            counter++;
        }
    }
    initialState = puzzle;
    getGoalState();
}

Problem::Problem(const int* arr, int size){ // option "2" for enter your own puzzle
    int userNum = 0;
    for(int i=0; i < puzzleSize; i++) {         
        for (int j = 0; j < puzzleSize; j++) {
            puzzle.state[i][j] = arr[userNum];
            userNum++;
        }
    }
    initialState = puzzle;  // records problems initial state.
    getGoalState();
}


void Problem::getGoalState() {
    vector<int> nums;

    for(int i = 0; i < puzzleSize; i++) {
        for (int j = 0; j < puzzleSize; j++) {
            nums.push_back(puzzle.state[i][j]);
        }
    }
    sort(nums.begin(), nums.end());
    nums.erase(nums.begin());
    nums.push_back(0);

    int numsIndex = 0;                          // records problem's goalState
    for(int i = 0; i < puzzleSize; i++) {
        for (int j = 0; j < puzzleSize; j++) {
            goalState.state[i][j] = nums.at(numsIndex);
            numsIndex++;
        }
    }
}

void Problem::printState(node currState) {
    for(int i = 0; i < puzzleSize; i++) {       // prints goal state
        for(int j = 0; j < puzzleSize; j++) {
            cout << currState.state[i][j] << " ";
        }
        cout << endl;
    }
}

node Problem::shiftLeft(node puzzleInput){
    int curr_iPos;
    int curr_jPos;

    for(int i = 0; i < puzzleSize; ++i) {   
        for(int j = 1; j < puzzleSize; ++j) { //find the 0    
            if(puzzleInput.state[i][j] == 0){    //save the 0 
                curr_iPos = i;
                curr_jPos = j;
            }
        }
    }
    swap(puzzleInput.state[curr_iPos][curr_jPos],puzzleInput.state[curr_iPos][curr_jPos-1]);
    return puzzleInput;
}

node Problem::shiftRight(node puzzleInput){
    int curr_iPos;
    int curr_jPos;

    for(int i = 0; i < puzzleSize; ++i) {   
        for(int j = 0; j < puzzleSize-1; ++j) { //find the 0    
            if(puzzleInput.state[i][j] == 0){    //save the 0 
                curr_iPos = i;
                curr_jPos = j;
            }
        }
    }
    swap(puzzleInput.state[curr_iPos][curr_jPos],puzzleInput.state[curr_iPos][curr_jPos+1]);
    return puzzleInput;
}

node Problem::shiftUp(node puzzleInput){
    int curr_iPos;
    int curr_jPos;

    for(int i = 1; i < puzzleSize; ++i) {   
        for(int j = 0; j < puzzleSize; ++j) { //find the 0    
            if(puzzleInput.state[i][j] == 0){    //save the 0 
                curr_iPos = i;
                curr_jPos = j;
            }
        }
    }
    swap(puzzleInput.state[curr_iPos][curr_jPos],puzzleInput.state[curr_iPos-1][curr_jPos]);
    return puzzleInput;
}

node Problem::shiftDown(node puzzleInput){
    int curr_iPos;
    int curr_jPos;

    for(int i = 0; i < puzzleSize-1; ++i) {   
        for(int j = 0; j < puzzleSize; ++j) { //find the 0    
            if(puzzleInput.state[i][j] == 0){    //save the 0 
                curr_iPos = i;
                curr_jPos = j;
            }
        }
    }
    swap(puzzleInput.state[curr_iPos][curr_jPos],puzzleInput.state[curr_iPos+1][curr_jPos]);
    return puzzleInput;
}