#ifndef TREE_HPP
#define TREE_HPP

#include "../header/Problem.hpp"
#include "../header/Node.hpp"
#include <queue>

class Tree{
    public:
        Problem problem;
        node root;
        node currentNode;
        priority_queue<node, vector<node>, compareOperator> frontier;  // min heap on nodes using node.hpp operator < 
        vector<node> frontierVector;    // vector of nodes in frontier
        vector<node> exploredSet;
        bool foundGoalState = false;

        Tree();
        Tree(Problem& problem);

        int maxNumNodes = 0;
        int goalDepth = 0;
        int totalNodes = 0;

        void expandNode(node currentNode, int inputHeuristicExpandNode);
        void expandNodeHelper(node, int inputHeuristicHelper);
        void graphSearch(int inputHeuristicMenu);
        int calculateMisplacedTiles(node currentNode);
        int calculateEuclideanDistance(node currentNode);
        int getMaxNumNodes();
        int getGoalDepth();
        int getTotalNodes();
};

#endif