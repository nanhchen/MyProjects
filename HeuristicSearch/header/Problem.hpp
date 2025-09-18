#ifndef PROBLEM_HPP
#define PROBLEM_HPP
#include <iostream>
#include "../header/Node.hpp"

using namespace std;

class Problem {
    protected:
        int puzzleSize = 3;
        
    public:
        node puzzle = node();
        node initialState = node();
        node goalState;
        Problem();
        Problem(const int* arr, int size);

        void getGoalState();
        void printState(node currState);

        node shiftDown(node puzzleInput); 
        node shiftLeft(node puzzleInput);       //puzzleInput is essentially the previous puzzle being checked to get shifted
        node shiftRight(node puzzleInput);
        node shiftUp(node puzzleInput);
};

#endif
