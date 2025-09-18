#include "Problem.cpp"
#include "Tree.cpp"
#include <iostream>
#include <limits>
#include <stdio.h>
#include <time.h>
using namespace std;


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
        
        // p = Problem();

        cout << "\nInitial State:\n";
        p.printState(p.initialState);
        cout <<"\nGoal State:\n";
        p.getGoalState();
        p.printState(p.goalState);
    }
    else if(userPuzzleChoice==2){
        cout << "\nEnter your puzzle, use a zero to represent the blank.";
        cout << "\nEnter the first row, use space or tabs between numbers: ";
        cin >> puzzleBox[0] >> puzzleBox[1] >> puzzleBox[2];
        // Validate user input on puzzle pieces for each row
        while(puzzleBox[0]<0 || puzzleBox[1]<0 || puzzleBox[2]<0){
            if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "\nPlease select a valid option.\nEnter the first row, use space or tabs between numbers: ";
            cin >> puzzleBox[0] >> puzzleBox[1] >> puzzleBox[2];
        }
        cout << "Enter the second row, use space or tabs between numbers: ";
        cin >> puzzleBox[3] >> puzzleBox[4] >> puzzleBox[5];
        while(puzzleBox[3]<0 || puzzleBox[4]<0 || puzzleBox[5]<0){
            if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "\nPlease select a valid option.\nEnter the second row, use space or tabs between numbers: ";
            cin >> puzzleBox[3] >> puzzleBox[4] >> puzzleBox[5];
        } 
        cout << "Enter the third row, use space or tabs between numbers: ";
        cin >> puzzleBox[6] >> puzzleBox[7] >> puzzleBox[8];
        while(puzzleBox[6]<0 || puzzleBox[7]<0 || puzzleBox[8]<0){
            if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "\nPlease select a valid option.\nEnter the third row, use space or tabs between numbers ";
            cin >> puzzleBox[6] >> puzzleBox[7] >> puzzleBox[8];
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