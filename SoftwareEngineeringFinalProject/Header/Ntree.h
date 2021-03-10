#ifndef NTREE_H
#define NTREE_H
#include "Header.h"
#include "Error.h"

// Struct for the node in the N-tree
struct NtreeNode {
public:
    char letterFromAlphabet;
    int numberOfNodes;
    int nextState; // NO_NEXT_STATE (-1) if Node is not a leaf
    NtreeNode** children;            //array of children tnodes
    // Constructor - Creates a tnode with a specified number of children nodes
    NtreeNode(int givenNumberOfNodes, char givenLetterFromAlphabet, int givenNextState) {
        nextState = givenNextState;
        letterFromAlphabet = givenLetterFromAlphabet;
        numberOfNodes = givenNumberOfNodes;
        children = (NtreeNode**)malloc(sizeof(NtreeNode*) * numberOfNodes);
        Error::checkMalloc((void*)children, numberOfNodes);
        for (int nodeIndex = 0; nodeIndex < numberOfNodes; nodeIndex++)
            children[nodeIndex] = NULL;
    }
    // Destructor - Deletes all of the children nodes
    ~NtreeNode() {
        try {
            for (int nodeIndex = 0; nodeIndex < numberOfNodes && children[nodeIndex]; nodeIndex++) {
                delete children[nodeIndex];
            }
            free(children);
        }
        catch (exception e) { return; }
    }
    // Add a child (node) to the Ntree
    NtreeNode* addChild(char givenLetterFromAlphabet, int givenNextState) {
        for (int i = 0; i < numberOfNodes; i++) {
            if (children[i] != NULL) {
                if (children[i]->letterFromAlphabet == givenLetterFromAlphabet && givenNextState == NO_NEXT_STATE) {
                    // If the letter already exists
                    return children[i];
                }
            }
            if (children[i] == NULL) {
                // The first to be empty choose
                children[i] = new NtreeNode(numberOfNodes, givenLetterFromAlphabet, givenNextState);
                return children[i];
            }
        }
    }
};
// Struct for the transition from state to state
struct Transition {
public:
    char* letters;
    int nextState;
    // Constructor - Initializes transition
    Transition(char* givenLetters, int givenNextState) {
        letters = givenLetters;
        nextState = givenNextState;
    }
    // Destructor
    ~Transition() {
        delete[] letters;
    }
};
// Struct for all the transitions that a particular state contains
struct StateTransition {
public:
    Transition** transitions; // The transitions
    int numberOfTransitions; // Number of transitions for each state
    int numberOfVariables; // Variables in the alpha condition
    // Constructor - Initializes stateTransition
    StateTransition(Transition** givenTransitions, int givenNumberOfTransitions, int givenNumberOfVariables) {
        numberOfTransitions = givenNumberOfTransitions;
        numberOfVariables = givenNumberOfVariables;
        transitions = givenTransitions;
    }
    // Destructor
    ~StateTransition() {
        for (int transitionIndex = 0; transitionIndex < numberOfTransitions; transitionIndex++) {
            delete transitions[transitionIndex];
        }
        delete transitions;
    }
};
// Ntree for each state
class Ntree {
public:
    NtreeNode* root;
    // Constructor - Initializes NFH
    Ntree(StateTransition* legitimateSequences) {
        root = new NtreeNode(legitimateSequences->numberOfTransitions, fakeChar, NO_NEXT_STATE); // Creates the root
        NtreeNode* pointerToNode = root;
        // Creates all the nodes inside the Ntree using the transitions
        for (int transitionIndex = 0; transitionIndex < legitimateSequences->numberOfTransitions; transitionIndex++) {
            // Runs on all the transitions
            int variableIndex;
            for (variableIndex = 0; variableIndex < (legitimateSequences->numberOfVariables - 1); variableIndex++) {
                // Runs on all the variables in the specific transition
                // addChild return the Node that added
                // pointerToNode start point to the new Node until no more variables to add
                pointerToNode = pointerToNode->addChild(legitimateSequences->transitions[transitionIndex]->letters[variableIndex], NO_NEXT_STATE);
            }
            // Leaf
            pointerToNode->addChild(legitimateSequences->transitions[transitionIndex]->letters[variableIndex], legitimateSequences->transitions[transitionIndex]->nextState);
            pointerToNode = root; // Go back to the root
        }
    }
    Ntree(NtreeNode* givenRoot) { root = givenRoot; }
    // Destructor
    ~Ntree() { delete root; }
    // Add root's children to NtreeNode** children
    // Merges children
    static NtreeNode** addChildren(NtreeNode** children, NtreeNode* root, int numberOfNodes, int* returnNumberOfNodes) {
        int returnValue = numberOfNodes;
        // if children empty
        if (!children[0]) {
            // Adds children
            for (int nodeIndex = 0; nodeIndex < numberOfNodes; nodeIndex++)
                children[nodeIndex] = root->children[nodeIndex];
            // Looking for same nodeIndex
            for (int nodeIndex = 0; nodeIndex < numberOfNodes; nodeIndex++)
                for (int nodeIndexInner = nodeIndex + 1; nodeIndexInner < numberOfNodes; nodeIndexInner++)
                    if (isEqualBranches(children[nodeIndex], children[nodeIndexInner]))
                        children[nodeIndexInner] = NULL;
            // Return the new number of nodes
            *returnNumberOfNodes = returnValue;
            return children;
        }
        // For each node in children:
        for (int newNodeIndex = 0; newNodeIndex < root->numberOfNodes; newNodeIndex++) {
            if (root->children[newNodeIndex]) {
                bool isExists = false;
                for (int currentNodeIndex = 0; currentNodeIndex < numberOfNodes; currentNodeIndex++)
                    if (children[currentNodeIndex])
                        // if the there are two nodes with the same letterFromAlphabet
                        // (one node from children and one from root)
                        if ((children[currentNodeIndex]->letterFromAlphabet == root->children[newNodeIndex]->letterFromAlphabet) &&
                            (children[currentNodeIndex]->nextState == root->children[newNodeIndex]->nextState)) {
                            isExists = true;
                            children[currentNodeIndex]->children = addChildren(children[currentNodeIndex]->children, root->children[newNodeIndex], numberOfNodes, returnNumberOfNodes);
                            children[currentNodeIndex]->numberOfNodes = *returnNumberOfNodes;
                            currentNodeIndex = numberOfNodes; // break the loop
                        }
                if(!isExists) { // Otherwise - diffrent nodes - add root's specific child
                    bool isInsert = false;
                    for (int nodeIndex = 0; nodeIndex < numberOfNodes; nodeIndex++) {
                        if (!children[nodeIndex]) {
                            children[nodeIndex] = root->children[newNodeIndex];
                            isInsert = true;
                            nodeIndex = numberOfNodes; // Stop the for loop
                        }
                    }
                    if (!isInsert) {
                        // Allocates new, temp, memory
                        NtreeNode** childrenTemp = (NtreeNode**)malloc(sizeof(NtreeNode*) * (returnValue));
                        Error::checkMalloc(childrenTemp, returnValue);
                        // Copy children to childrenTemp
                        for (int nodeIndex = 0; nodeIndex < returnValue; nodeIndex++)
                            childrenTemp[nodeIndex] = new NtreeNode(children[nodeIndex]->numberOfNodes, children[nodeIndex]->letterFromAlphabet,
                                children[nodeIndex]->nextState);
                        // Frees children
                        for (int nodeIndex = 0; nodeIndex < returnValue; nodeIndex++) delete(children[nodeIndex]);
                        free(children);
                        // Allocates bigger memory for children
                        children = (NtreeNode**)malloc(sizeof(NtreeNode*) * (1 + returnValue++));
                        Error::checkMalloc(children, returnValue);
                        // Copy childrenTemp to children
                        for (int nodeIndex = 0; nodeIndex < returnValue - 1; nodeIndex++)
                            children[nodeIndex] = childrenTemp[nodeIndex];
                        children[returnValue - 1] = new NtreeNode(root->children[newNodeIndex]->numberOfNodes,
                            root->children[newNodeIndex]->letterFromAlphabet, root->children[newNodeIndex]->nextState);
                        free(childrenTemp);
                    }
                }
            }
        }
        *returnNumberOfNodes = returnValue;
        return children;
    }
    // Allocates new memory and copies the children
    static NtreeNode* copyRoot(NtreeNode* root, char letterFromAlphabet, int nextState, int numberOfNodes) {
        if (root) {
            // Allocate memory for the new NtreeNode**
            NtreeNode* newRoot = new NtreeNode(numberOfNodes, letterFromAlphabet, nextState);
            // For each child - Copy the data and the children
            for (int nodeIndex = 0; nodeIndex < numberOfNodes; nodeIndex++)
                if (root->children[nodeIndex]) {
                    newRoot->children[nodeIndex] = copyRoot(root->children[nodeIndex], root->children[nodeIndex]->letterFromAlphabet,
                        root->children[nodeIndex]->nextState, root->children[nodeIndex]->numberOfNodes);
                }
            return newRoot;
        }
        else return NULL;
    }
    // Checking if the two branches are equals
    static bool isEqualBranches(NtreeNode* originalRoot_node, NtreeNode* extraRoot_node) {
        // Error condition
        if (!originalRoot_node || !extraRoot_node) return false;
        // Base condition - leaf
        if (!originalRoot_node->children[0] || !extraRoot_node->children[0]) {
            if ((originalRoot_node->letterFromAlphabet == extraRoot_node->letterFromAlphabet) &&
                (originalRoot_node->nextState == extraRoot_node->nextState))
                return true;
            else return false;
        }
        else if (originalRoot_node->letterFromAlphabet != extraRoot_node->letterFromAlphabet ||
            originalRoot_node->nextState != extraRoot_node->nextState)
            return false;
        else {
            bool answer = false;
            for (int nodeIndex = 0; nodeIndex < originalRoot_node->numberOfNodes; nodeIndex++)
                answer *= isEqualBranches(originalRoot_node->children[nodeIndex], extraRoot_node->children[nodeIndex]);
            return answer;
        }
    }
};

#endif
