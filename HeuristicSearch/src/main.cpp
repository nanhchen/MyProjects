#include "../header/Problem.hpp"
#include "../header/Tree.hpp"
#include <iostream>
#include <limits>
#include <stdio.h>
#include <time.h>
// personal edits
#include <sstream>
#include <set>
using namespace std;

bool isSolvable(int puzzlebox[9]) {
    int inversions = 0;
    for (int i = 0; i < 9; ++i) {
        if (puzzlebox[i] == 0) continue;
        for (int j = i + 1; j < 9; ++j) {
            if (puzzlebox[j] == 0) continue;
            if (puzzlebox[i] > puzzlebox[j]) inversions++;
        }
    }
    return inversions % 2 == 0;
}

bool getRowInput(int puzzlebox[], int startIndex, set<int>& used) {
    string line;
    int x, y, z;

    while (true) {
        cout << "\nEnter 3 numbers (1-9) seperated by spaces: ";
        getline(cin, line);
        stringstream ss(line);

        if (!(ss >> x >> y >> z)) {
            cout << "\n\tInvalid input - must be 3 integers (1-9).\n";
            continue;
        }
    
        if (x < 0 || x > 9 || y < 0 || y > 9 || z < 0 || z > 9) {
            cout << "\n\tInvalid input - numbers must be between 0 and 9.\n";
            continue;
        }
    
        if (x==y || y==z || x==z || used.count(x) || used.count(y) || used.count(z)) {
            cout << "\n\tInvalid input - numbers must be distinct.\n";
            continue;
        }
    
        string excess;
        if (ss >> excess) {
            cout << "\tInvalid input - too many values.\n";
            continue;
        }

        // valid inputs --> store
        puzzlebox[startIndex] = x;
        puzzlebox[startIndex + 1] = y;
        puzzlebox[startIndex + 2] = z;

        // mark integers used
        used.insert(x);
        used.insert(y);
        used.insert(z);
        return true;
    }
    
}


int main(){
    clock_t start = clock();

    cout << "Welcome to 8-puzzle solver! Enter your puzzle choice from below." << endl;
    cout << "\n\t1. Default Puzzle\n\t2. Custom Puzzle\n\n >";
    int userPuzzleChoice = 0;
    cin >> userPuzzleChoice;       
    // Validate user input for puzzle choice
    while(userPuzzleChoice != 1 && userPuzzleChoice != 2){
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "\nPlease select a valid option.\n\t1. Default Puzzle\n\t2. Custom Puzzle\n\n >";
        cin >> userPuzzleChoice;
    }

    // Initialize puzzle box with 9 pieces
    const int SIZE = 9;
    int puzzleBox[SIZE];
    Problem p = Problem();
    if(userPuzzleChoice==1){
        cout << "\nUSING DEFAULT PUZZLE\n";
        cout << "\n";

        cout << "\nInitial State:\n";
        p.printState(p.initialState);
        cout <<"\nGoal State:\n";
        p.getGoalState();
        p.printState(p.goalState);
    }
    else if(userPuzzleChoice==2){
        set<int> used;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while (true) {
            for (int row = 0; row < 3; row++) {
                cout << "\n\n\t============ ROW " << row+1 << "============\n";
                cout << "\nEnter your puzzle, use a zero to represent the blank.\n";
                getRowInput(puzzleBox, row *3, used);
            }

            // Check solvability
            if (!isSolvable(puzzleBox)) {
                cout << "\n*** This puzzle configuration is unsolvable. Please try again. ***\n";
                used.clear();
            } else {
                break;
            }
        }
        
        p = Problem(puzzleBox, SIZE);
        cout << "\nInitial State:\n";
        p.printState(p.initialState);
        cout <<"\nGoal State:\n";
        p.printState(p.goalState);
    }

    Tree tree = Tree(p);
    // cout << "\nPrint initial state:\n";
    // tree.problem.printState(tree.problem.initialState);

    cout << "\nEnter your choice of algorithm.\n\t1. Uniform Cost Search\n\t2. A* Misplaced Tile Heuristic\n\t3. A* Euclidean Distance Heuristic\n\n >";
    int userAlgorithmChoice = 0;
    cin >> userAlgorithmChoice;
    // Validate user input on algorithm choice
    while(userAlgorithmChoice != 1 && userAlgorithmChoice != 2 && userAlgorithmChoice != 3){
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        cout << "\nPlease select a valid option.\nEnter your choice of algorithm.\n\t1. Uniform Cost Search\n\t2. A* Misplaced Tile Heuristic\n\t3. A* Euclidean Distance Heuristic\n\n >";
        cin >> userAlgorithmChoice;
    }

    if(userAlgorithmChoice==1){
        cout << "\nUSING UNIFORM COST\n";
        tree.graphSearch(1);
    }
    else if(userAlgorithmChoice==2){
        cout << "\nUSING A* MTH\n";
        tree.graphSearch(2);
    }
    else if(userAlgorithmChoice==3){
        cout << "\nUSING A* EDH\n";
        tree.graphSearch(3);
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    cout << "To solve this problem the search algorithm expanded a total of " << tree.getTotalNodes() << " nodes"<< endl;
    cout << "The maximum number of nodes in the queue at any one time: " << tree.getMaxNumNodes() << endl;
    cout << "The depth of the goal node was " << tree.getGoalDepth()<< endl;
    cout << "\nExecution time: " << time_spent << " seconds\n";

    return 0;
}