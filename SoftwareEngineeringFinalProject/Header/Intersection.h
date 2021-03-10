#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "Header.h"
#include "NFA.h"
#include "NFH.h"
#include "Convert.h"
#include "Ntree.h"
#include "Error.h"

class Intersection{
private:
	// Function to generate all new state for intersectionComplementation_NFH_to_NFA
	static void generate(int** newStates, int numberOfStates, int set1Size, int set2Size) {
		int newStates_statesIndex = 0;
		for (int set1Index = 0; set1Index < set1Size; set1Index++)
			for (int set2Index = 0; set2Index < set2Size; set2Index++) {
				newStates[newStates_statesIndex][0] = set1Index;
				newStates[newStates_statesIndex++][1] = set2Index;
			}
	}
	// Creates a new Ntree with the same structure, 
	// Goes to the leafs and changes the next state value
	// Returns the new Ntree
	static NtreeNode* createNewNTree(NtreeNode* root, char letterFromAlphabet, int nextState, int NFAnextState, int numberOfNodes, int** map, int size, int NFA_numberOfStates){
		if (root) {
			// Allocate memory for the new NtreeNode**
			int newNextState = nextState;
			if (!root->children[0]) // Leaf
				newNextState = (map[root->nextState][1]) * NFA_numberOfStates + (NFAnextState);
			NtreeNode* newRoot = new NtreeNode(numberOfNodes, letterFromAlphabet, newNextState);
			// For each child - Copy the data and the children
			for (int nodeIndex = 0; nodeIndex < numberOfNodes; nodeIndex++)
				if (root->children[nodeIndex])
					newRoot->children[nodeIndex] = createNewNTree(root->children[nodeIndex], root->children[nodeIndex]->letterFromAlphabet,
						root->children[nodeIndex]->nextState, NFAnextState, root->children[nodeIndex]->numberOfNodes, map, size, NFA_numberOfStates);
			return newRoot;
		}
		else return NULL;
	}
public:
	// Complementation intersection between two NFAs
	static NFA* intersectionComplementation_NFA_to_NFA(NFA* input1, NFA* input2) {
		int numberOfStatesOutput = input1->numberOfStates * input2->numberOfStates;
		// Starting to build output NFA
		// Calculates the number of new deterministic mattrix (that will be united into one non-deterministic mattrix)
		int output_numberOfNonDeterministic = input1->numberOfNonDeterministic * input2->numberOfNonDeterministic;
		// Allocates the output mattrix
		int*** output_moveMatrix = (int***)malloc(sizeof(int**) * output_numberOfNonDeterministic);
		Error::checkMalloc((void*)output_moveMatrix, output_numberOfNonDeterministic);
		for (int possibleIndex = 0; possibleIndex < output_numberOfNonDeterministic; possibleIndex++) {
			int** newMatrix = (int**)malloc(sizeof(int*) * numberOfStatesOutput);
			Error::checkMalloc((void*)newMatrix, numberOfStatesOutput);
			for (int stateIndex = 0; stateIndex < numberOfStatesOutput; stateIndex++) {
				newMatrix[stateIndex] = (int*)malloc(sizeof(int) * input1->sizeOfAlphaBet);
				Error::checkMalloc((void*)newMatrix[stateIndex], input1->sizeOfAlphaBet);
			}
			output_moveMatrix[possibleIndex] = newMatrix;
		}
		// Fills the output_moveMatrix
		for (int possiblesIndex_input1 = 0; possiblesIndex_input1 < input1->numberOfNonDeterministic; possiblesIndex_input1++)
			for (int possiblesIndex_input2 = 0; possiblesIndex_input2 < input2->numberOfNonDeterministic; possiblesIndex_input2++)
				for (int stateIndex = 0; stateIndex < input1->numberOfStates; stateIndex++)
					for (int charIndex = 0; charIndex < input1->sizeOfAlphaBet; charIndex++)
						for (int stateIndex_input2 = 0; stateIndex_input2 < input2->numberOfStates; stateIndex_input2++) {
							int input1_char = input1->moveMatrix[possiblesIndex_input1][stateIndex][charIndex];
							int input2_char = input2->moveMatrix[possiblesIndex_input2][stateIndex_input2][charIndex];
							if (input1_char != NO_NEXT_STATE && input2_char != NO_NEXT_STATE)
								output_moveMatrix[possiblesIndex_input1 * possiblesIndex_input2][stateIndex * input2->numberOfStates + stateIndex_input2][charIndex] = input1_char * input2->numberOfStates + input2_char;
							else
								output_moveMatrix[possiblesIndex_input1 * possiblesIndex_input2][stateIndex * input2->numberOfStates + stateIndex_input2][charIndex] = NO_NEXT_STATE;
						}
		// Create the NFA output
		int startState = input1->starterState * input2->numberOfStates + input2->starterState;
		int finalStateSize_input1 = input1->sizeOfFinalStates;
		int finalStateSize_Input2 = input2->sizeOfFinalStates;
		int sizeOfFinalStates = finalStateSize_input1 * finalStateSize_Input2;
		int* finalStates = (int*)malloc(sizeof(int) * sizeOfFinalStates);
		Error::checkMalloc((void*)finalStates, sizeOfFinalStates);
		for (int finalStateIndex_input1 = 0; finalStateIndex_input1 < finalStateSize_input1; finalStateIndex_input1++)
			for (int finalStateIndex_input2 = 0; finalStateIndex_input2 < finalStateSize_Input2; finalStateIndex_input2++)
				finalStates[finalStateIndex_input1 * finalStateSize_Input2 + finalStateIndex_input2] =
				input1->finalStates[finalStateIndex_input1] * finalStateSize_Input2 + input2->finalStates[finalStateIndex_input2];
		NFA* output = new NFA(output_moveMatrix, input1->alphaBet, startState, finalStates, output_numberOfNonDeterministic, numberOfStatesOutput, input1->sizeOfAlphaBet, sizeOfFinalStates);
		return output;
	}
	// Complementation intersection between NFA and NFH
	// All NFA's final states extends only for padChar (#)
	static NFH* intersectionComplementation_NFH_to_NFA(NFH* input1, NFA* input2) {
		// Extends NFA, for each final states - we move to a new final state
		// with padChar (#). It allow as to intersection NFH and NFA.
		input2->extendsFinalStates();
		// Calculates and creates new final states
		int numberOfNewStates = input1->numberOfStates * input2->numberOfStates;
		int finalStateSize_input1 = input1->sizeOfFinalStates;
		int finalStateSize_Input2 = input2->sizeOfFinalStates;
		int sizeOfFinalStates = finalStateSize_input1 * finalStateSize_Input2;
		int* finalStates = (int*)malloc(sizeof(int) * sizeOfFinalStates);
		Error::checkMalloc((void*)finalStates, sizeOfFinalStates);
		for (int finalStateIndex_input1 = 0; finalStateIndex_input1 < finalStateSize_input1; finalStateIndex_input1++)
			for (int finalStateIndex_input2 = 0; finalStateIndex_input2 < finalStateSize_Input2; finalStateIndex_input2++)
				finalStates[finalStateIndex_input1 * finalStateSize_Input2 + finalStateIndex_input2] =
					input1->finalStates[finalStateIndex_input1] * finalStateSize_Input2 + input2->finalStates[finalStateIndex_input2];
		// Allocates and initializes the new moveArray
		Ntree** moveArray = (Ntree**)malloc(sizeof(Ntree*) * numberOfNewStates);
		Error::checkMalloc(moveArray, numberOfNewStates);
		for (int moveArrayIndex = 0; moveArrayIndex < numberOfNewStates; moveArrayIndex++) moveArray[moveArrayIndex] = NULL;
		// Auxiliary construction - Storage all the possibles pairs (#stateNFH, #stateNFA)
		// inside an array and uses it for calculates the next state to go for each leaf
		// references to the letters inside the state's children.
		int** newStatesInfo = (int**)malloc(sizeof(int*) * numberOfNewStates);
		Error::checkMalloc(newStatesInfo, numberOfNewStates);
		for (int stateIndex = 0; stateIndex < numberOfNewStates; stateIndex++) {
			newStatesInfo[stateIndex] = (int*)malloc(sizeof(int) * (2));
			Error::checkMalloc((void*)newStatesInfo[stateIndex], 2);
		}
		// Generate newStateInfo
		generate(newStatesInfo, numberOfNewStates, input1->numberOfStates, input2->numberOfStates);
		// For each new state
		for (int moveArrayStateIndex = 0; moveArrayStateIndex < numberOfNewStates; moveArrayStateIndex++) {
			// Point at the specific state in NFH by newStatesInfo
			int NFHcurrentState = newStatesInfo[moveArrayStateIndex][0];
			NtreeNode* pointerToRoot = input1->moveArray[NFHcurrentState]->root;
			if (pointerToRoot) { // if root exists
				int newNumberOfNodes = pointerToRoot->numberOfNodes;
				// Allocates memory for the new children
				NtreeNode** newStateChildren = (NtreeNode**)malloc(sizeof(NtreeNode*) * pointerToRoot->numberOfNodes);
				Error::checkMalloc(newStateChildren, pointerToRoot->numberOfNodes);
				for (int nodeIndex = 0; nodeIndex < pointerToRoot->numberOfNodes; nodeIndex++) newStateChildren[nodeIndex] = NULL;
				// For each node
				for (int nodeIndex = 0; nodeIndex < pointerToRoot->numberOfNodes && pointerToRoot->children[nodeIndex]; nodeIndex++) {
					// Run on all over the children (nodes)
						// For each matrix in the NFA:
						// We create an intersection between pair of states
						// for each leafs next state values we add the next state value for the given NFA
						// (references to the child's letterFromAlphabet)
						// (q[i]_currentState, p[j]_currentState) => (q[i]_nextStateIfGets_letterFromAlphabet, p[j]_nextStateIfGets_letterFromAlphabet)
					int NFAcurrentState = newStatesInfo[moveArrayStateIndex][1];
					for (int possibleIndex = 0; possibleIndex < input2->numberOfNonDeterministic; possibleIndex++) {
						int NFAnextState;
						// If padChar (#) - we allow transition only from NFA's final states
						if (pointerToRoot->children[nodeIndex]->letterFromAlphabet == padChar)
							NFAnextState = input2->moveMatrix[possibleIndex][NFAcurrentState][input2->numberOfStates - 1];
						else NFAnextState = input2->moveMatrix[possibleIndex][NFAcurrentState][pointerToRoot->children[nodeIndex]->letterFromAlphabet - 'a'];
						if (NFAnextState != NO_NEXT_STATE) {
							// Creates new Ntree with diffrent next states values
							NtreeNode* newNtree = createNewNTree(pointerToRoot->children[nodeIndex], pointerToRoot->letterFromAlphabet,
								pointerToRoot->nextState, NFAnextState, pointerToRoot->numberOfNodes, newStatesInfo, numberOfNewStates, input2->numberOfStates);
							// Add the newNtree's children to newStateChildren
							newStateChildren = Ntree::addChildren(newStateChildren, newNtree, pointerToRoot->numberOfNodes, &newNumberOfNodes);
						}
					}
				}
				NtreeNode* newRoot = new NtreeNode(newNumberOfNodes, fakeChar, NO_NEXT_STATE);
				for (int nodeIndex = 0; nodeIndex < newNumberOfNodes; nodeIndex++)
					newRoot->children[nodeIndex] = newStateChildren[nodeIndex];
				moveArray[moveArrayStateIndex] = new Ntree(newRoot);
			}
			else moveArray[moveArrayStateIndex] = NULL;
		}
		// Create the NFH output
		int startState = input1->starterState * input1->numberOfStates + input2->starterState;
		Quantifiers* newQuantifires = (Quantifiers*)malloc(sizeof(Quantifiers) * (input1->alphaCondition.numberOfQuantifiers - 1));
		Error::checkMalloc(newQuantifires, (input1->alphaCondition.numberOfQuantifiers - 1));
		for (int quantifierIndex = 1; quantifierIndex < input1->alphaCondition.numberOfQuantifiers; quantifierIndex++)
			newQuantifires[quantifierIndex - 1] = input1->alphaCondition.quantifiers[quantifierIndex];
		AlphaCondition newAlphaCondition(newQuantifires, (input1->alphaCondition.numberOfQuantifiers - 1));
		NFH* output = new NFH(moveArray, input1->alphaBet, startState, finalStates, newAlphaCondition, numberOfNewStates, input1->sizeOfAlphaBet, sizeOfFinalStates);
		// Free memory
		for (int stateIndex = 0; stateIndex < numberOfNewStates; stateIndex++) free(newStatesInfo[stateIndex]);
		free(newStatesInfo);
		return output;
	}
};

#endif
