#ifndef CONVERT_H
#define CONVERT_H
#include "Header.h"
#include "DFA.h"
#include "NFA.h"
#include "NFH.h"
#include "DFH.h"
#include "Intersection.h"
#include "Error.h"
#include <math.h>

// Convert class - Convert NFA to DFA and vise versa
class Convert{
private:
	// Merge branches with the same sequence of letters but diffrent next state value
	// Notice: Non-deterministic represent by the same sequence of letter with diffrent
	//		   next-state value. Because of the specific data structure of NtreeNode,
	//         mergeNextStateValues merges only the leafs's children by going to the leafs
	//         and looking for the same letterFromAlphabet values.
	//		   The new next state value calculte with newStates (map).
	static void mergeNextStateValues(NtreeNode* root, char** map, int size) {
		// Error case.
		if (!root) return;
		// Error case. Root without children.
		if (!root->children[0]) return;
		// Stop (base) condition. One level before the leafs (leafs father, the main NtreeNode with
		// children that are all leafs).
		if (!root->children[0]->children[0]) {
			// Search for nodes with the same letterFromAlphabet values
			for (int nodeIndex = 0; nodeIndex < root->numberOfNodes && root->children[nodeIndex]; nodeIndex++) {
				// Allocates memory for all next state values with the same letterFromAlphabet
				// Maximum all the nodes + padChar for end of sequence.
				char* goTo = (char*)malloc(sizeof(char) * (root->numberOfNodes + 1));
				Error::checkMalloc(goTo, root->numberOfNodes + 1);
				int goToIndex = 0;
				for (int nodeIndexInner = nodeIndex + 1; nodeIndexInner < root->numberOfNodes; nodeIndexInner++) {
					if (root->children[nodeIndexInner]) { // If exists - then check letterFromAlphabet 
						if (root->children[nodeIndex]->letterFromAlphabet == root->children[nodeIndexInner]->letterFromAlphabet) {
							if (!goToIndex) {
								goTo[goToIndex++] = root->children[nodeIndex]->nextState + '0';
								goTo[goToIndex++] = root->children[nodeIndexInner]->nextState + '0';
								// Delete root->children[nodeIndexInner]
								delete(root->children[nodeIndexInner]);
								root->children[nodeIndexInner] = NULL;
							}
							else {
								// Make sure that the new next state value does'nt exists in goTo
								bool isExists = false;
								for (int charInGoTo = 0; charInGoTo < goToIndex; charInGoTo++)
									if (goTo[charInGoTo] == root->children[nodeIndexInner]->nextState + '0') isExists = true;
								if (!isExists) goTo[goToIndex++] = root->children[nodeIndexInner]->nextState + '0';
							}
						}
					}
				}
				// If no similar sequences were found
				if (!goToIndex) goTo[goToIndex++] = root->children[nodeIndex]->nextState + '0';
				goTo[goToIndex++] = padChar;
				root->children[nodeIndex]->nextState = getIndex(goTo, goToIndex, map, size);
				free(goTo);
			}
		}
		else { // Go down to the leafs
			for (int nodeIndex = 0; nodeIndex < root->numberOfNodes; nodeIndex++)
				mergeNextStateValues(root->children[nodeIndex], map, size);
		}
	}
	// Function to generate all new state
	static void generate(char** states, int numberOfStates, int sizeOfState) {
		// Initializes - Fills the first one-digit states
		for (int i = 0; i < sizeOfState; i++) {
			states[i][0] = i + '0';
			states[i][1] = padChar;
		}
		// Generate the rest
		int currentEmptyIndex = sizeOfState;
		for (int i = 0; i < numberOfStates - 1; i++) {
			bool isMoreGenerates = true;
			int digitIndex = 0, jump = 0;
			while (states[i][digitIndex + 1] != padChar) { digitIndex++; }
			while (isMoreGenerates) {
				if ((states[i][digitIndex] - '0' < sizeOfState - 1) && (states[i][digitIndex] + jump - '0' <= sizeOfState - 2)) {
					for (int k = 0; k <= digitIndex; k++) { states[currentEmptyIndex][k] = states[i][k]; }
					states[currentEmptyIndex][digitIndex + 1] = states[i][digitIndex] + 1 + jump++;
					states[currentEmptyIndex++][digitIndex + 2] = padChar;
				}
				else isMoreGenerates = false;
			}
		}
		states[numberOfStates - 1][0] = EMPTY_STATE;
		states[numberOfStates - 1][1] = padChar;
	}
	// Sort the state
	// Convert to the new state by search for the index
	// of the new state in map
	static int getIndex(char* state, int stateSize, char** map, int mapSize) {
		for (int i = 0; i < stateSize && state[i] != padChar; i++) {
			for (int j = i + 1; j < stateSize && state[j] != padChar; j++) {
				if (state[i] > state[j] or state[j] == padChar) {
					char temp = state[i];
					state[i] = state[j];
					state[j] = temp;
				}
			}
		}
		for (int newState = 0; newState < mapSize; newState++) {
			bool isTheState = false;
			for (int charIndex = 0; charIndex < stateSize; charIndex++) {
				if (state[charIndex] != map[newState][charIndex]) {
					charIndex = stateSize; // Break the loop
					isTheState = false;
				}
				else isTheState = true;
			}
			if (isTheState) return newState;
		}
	}
	// Checks if the given digit is in the final states
	static bool checkFinalStates(int* finalState, int size, char digit) {
		for (int stateIndex = 0; stateIndex < size; stateIndex++) {
			if (finalState[stateIndex] == digit - '0') return true;
		}
		return false;
	}
public:
	// Convert NFA to DFA
	static DFA* convert_NFA_to_DFA(NFA nfa) {
		DFA* returnDFA;
		int** moveMatrix;
		bool includeEmptyState = false;
		if (nfa.numberOfNonDeterministic == 1) {
			// New alphabet
			char* newAlphaBet = (char*)malloc(sizeof(char) * nfa.sizeOfAlphaBet);
			Error::checkMalloc(newAlphaBet, nfa.sizeOfAlphaBet);
			for (int charIndex = 0; charIndex < nfa.sizeOfAlphaBet; charIndex++) newAlphaBet[charIndex] = nfa.alphaBet[charIndex];
			// New final states
			int* newFinalStates = (int*)malloc(sizeof(int) * nfa.sizeOfFinalStates);
			Error::checkMalloc(newFinalStates, nfa.sizeOfFinalStates);
			for (int stateIndex = 0; stateIndex < nfa.sizeOfFinalStates; stateIndex++) newFinalStates[stateIndex] = nfa.finalStates[stateIndex];
			moveMatrix = (int**)malloc(sizeof(int*) * nfa.numberOfStates);
			Error::checkMalloc((void*)moveMatrix, nfa.numberOfStates);
			// For each state in the new moveMatrix:
			for (int stateIndex = 0; stateIndex < nfa.numberOfStates; stateIndex++) {
				// Allocates memory by creating the new state
				moveMatrix[stateIndex] = (int*)malloc(sizeof(int) * nfa.sizeOfAlphaBet);
				Error::checkMalloc(moveMatrix[stateIndex], nfa.sizeOfAlphaBet);
				for (int charIndex = 0; charIndex < nfa.sizeOfAlphaBet; charIndex++)
					moveMatrix[stateIndex][charIndex] = nfa.moveMatrix[0][stateIndex][charIndex];
			}
			returnDFA = new DFA(moveMatrix, newAlphaBet, nfa.starterState, newFinalStates,
									nfa.numberOfStates, nfa.sizeOfAlphaBet, nfa.sizeOfFinalStates);
			return returnDFA;
		}
		/* Gets the new states by checking each row and column pair in
			the same spot in the diffrent matrixs of possibility		*/
		// Allocates memory
		// The maximum size of newState can reach (2^n); n is the number of states
		int newStateSize = pow(2, nfa.numberOfStates), newFinalStatesSize = pow(2, nfa.numberOfStates);
		int* newFinalStates = (int*)malloc(sizeof(int) * newFinalStatesSize);
		Error::checkMalloc((void*)newFinalStates, newFinalStatesSize);
		// Build final states
		int newFinalStateIndex = nfa.sizeOfFinalStates;
		for (int stateIndex = 0; stateIndex < nfa.sizeOfFinalStates; stateIndex++)
			newFinalStates[stateIndex] = nfa.finalStates[stateIndex];
		char** newStates = (char**)malloc(sizeof(char*) * newStateSize);
		Error::checkMalloc((void*)newStates, newStateSize);
		// Initializes
		for (int stateIndex = 0; stateIndex < newStateSize; stateIndex++) {
			newStates[stateIndex] = (char*)malloc(sizeof(char) * (nfa.numberOfStates + 1));
			Error::checkMalloc((void*)newStates[stateIndex], nfa.numberOfStates + 1);
		}
		generate(newStates, newStateSize, nfa.numberOfStates);
		// Allocates memory by creating the new moveMatrix
		moveMatrix = (int**)malloc(sizeof(int*) * newStateSize);
		Error::checkMalloc((void*)moveMatrix, newStateSize);
		// For each state in the new moveMatrix:
		for (int stateIndex = 0; stateIndex < newStateSize - 1; stateIndex++) {
			// Allocates memory by creating the new state
			moveMatrix[stateIndex] = (int*)malloc(sizeof(int) * nfa.sizeOfAlphaBet);
			Error::checkMalloc(moveMatrix[stateIndex], nfa.sizeOfAlphaBet);
			// For each char in the new state:
			for (int charIndex = 0; charIndex < nfa.sizeOfAlphaBet; charIndex++) {
				// Goes to newStates and checks for the new state to move on
				// For each char in the new state - Calculates the new state
				char* goToState = (char*)malloc(sizeof(char) * (nfa.numberOfStates + 1)); // Where to go - Base on all the possibilities
				Error::checkMalloc(goToState, nfa.numberOfStates + 1);
				int goToStateIndex = 0;
				bool isFinalState = false;
				for (int charInNewStates = 0; (charInNewStates < nfa.numberOfStates) && (newStates[stateIndex][charInNewStates] != padChar); charInNewStates++) {
					char currentState = newStates[stateIndex][charInNewStates]; // The current char. Represented a state
					// Gets the new state
					for (int possibleIndex = 0; possibleIndex < nfa.numberOfNonDeterministic; possibleIndex++) {
						char charInMoveMatrix = nfa.moveMatrix[possibleIndex][currentState - '0'][charIndex] + '0';
						if (nfa.moveMatrix[possibleIndex][currentState - '0'][charIndex] != NO_THOROUGHFARE) {
							if (!goToStateIndex) {
								goToState[goToStateIndex++] = charInMoveMatrix;
								isFinalState = checkFinalStates(nfa.finalStates, nfa.sizeOfFinalStates, charInMoveMatrix);
							}
							else {
								bool isInsert = true;
								for (int charInGoToState = 0; charInGoToState < goToStateIndex; charInGoToState++) {
									if (goToState[charInGoToState] == charInMoveMatrix) isInsert = false;
								}
								if (isInsert) {
									goToState[goToStateIndex++] = charInMoveMatrix;
									isFinalState = checkFinalStates(nfa.finalStates, nfa.sizeOfFinalStates, charInMoveMatrix);
								}
							}
						}
					}
				}
				goToState[goToStateIndex] = padChar;
				if (!goToStateIndex) moveMatrix[stateIndex][charIndex] = newStateSize - 1; // The lase index reprsents the empty set
				else {
					int goToStateValue = getIndex(goToState, nfa.numberOfStates + 1, newStates, newStateSize);
					moveMatrix[stateIndex][charIndex] = goToStateValue;
					if (isFinalState) {
						// Checks if the final state is already exists in newFinalStates 
						bool isInsertFinalState = true;
						for (int finalStateIndex = 0; finalStateIndex < newFinalStateIndex; finalStateIndex++)
							if (newFinalStates[finalStateIndex] == goToStateValue) isInsertFinalState = false;
						if(isInsertFinalState) newFinalStates[newFinalStateIndex++] = goToStateValue;
					}
				}
			}
		}
		// Allocates memory by creating the new state
		moveMatrix[newStateSize - 1] = (int*)malloc(sizeof(int) * nfa.sizeOfAlphaBet);
		Error::checkMalloc(moveMatrix[newStateSize - 1], nfa.sizeOfAlphaBet);
		for (int i = 0; i < nfa.sizeOfAlphaBet; i++) moveMatrix[newStateSize - 1][i] = newStateSize - 1;
		returnDFA = new DFA(moveMatrix, nfa.alphaBet, nfa.starterState, newFinalStates, newStateSize, nfa.sizeOfAlphaBet, newFinalStateIndex);
		// Free memory
		free(newStates);
		return returnDFA;
	}
	// Convert DFA to NFA
	// Note: Every DFA is a NFA.
	static NFA* convert_DFA_to_NFA(DFA dfa) {
		// New alphabet
		char* newAlphaBet = (char*)malloc(sizeof(char) * dfa.sizeOfAlphaBet);
		Error::checkMalloc(newAlphaBet, dfa.sizeOfAlphaBet);
		for (int charIndex = 0; charIndex < dfa.sizeOfAlphaBet; charIndex++) newAlphaBet[charIndex] = dfa.alphaBet[charIndex];
		// New final states
		int* newFinalStates = (int*)malloc(sizeof(int) * dfa.sizeOfFinalStates);
		Error::checkMalloc(newFinalStates, dfa.sizeOfFinalStates);
		for (int stateIndex = 0; stateIndex < dfa.sizeOfFinalStates; stateIndex++) newFinalStates[stateIndex] = dfa.finalStates[stateIndex];
		// New move matrix
		int*** newMoveMatrix = (int***)malloc(sizeof(int**) * 1);
		Error::checkMalloc(newMoveMatrix, 1);
		newMoveMatrix[0] = (int**)malloc(sizeof(int*) * dfa.numberOfStates);
		Error::checkMalloc(newMoveMatrix[0], dfa.numberOfStates);
		for (int stateIndex = 0; stateIndex < dfa.numberOfStates; stateIndex++) {
			newMoveMatrix[0][stateIndex] = (int*)malloc(sizeof(int) * dfa.sizeOfAlphaBet);
			Error::checkMalloc(newMoveMatrix[0][stateIndex], dfa.sizeOfAlphaBet);
			for (int charIndex = 0; charIndex < dfa.sizeOfAlphaBet; charIndex++)
				newMoveMatrix[0][stateIndex][charIndex] = dfa.moveMatrix[stateIndex][charIndex];
		}
		NFA* returnNFA = new NFA(newMoveMatrix, newAlphaBet, dfa.starterState, newFinalStates, 1,
									dfa.numberOfStates, dfa.sizeOfAlphaBet, dfa.sizeOfFinalStates);
		return returnNFA;
	}
	// Convert NFH (|alphaCondition| == 1) to NFA
	static NFA* convert_NFH_to_NFA(NFH* nfh) {
		// Remove epsilon moves
		nfh->removeEpsilonMoves();
		// Start from calculates the number of appearance for each char
		// from alpha bet, for each state
		int total_numberOfNonDeterministic = 1;
		// Gets the number of diffrents matrixs
		int** alphaBetCounterMatrix = (int**)malloc(sizeof(int*) * nfh->numberOfStates);
		Error::checkMalloc(alphaBetCounterMatrix, nfh->numberOfStates);
		for (int stateIndex = 0; stateIndex < nfh->numberOfStates; stateIndex++) {
			// Array to calculate the number of matrixs
			int* alphaBetArray = (int*)malloc(sizeof(int) * (nfh->sizeOfAlphaBet)); // Allocates memory
			Error::checkMalloc(alphaBetArray, nfh->sizeOfAlphaBet);
			// Initialize the alphaBetArray
			for (int alphaBetArray_Index = 0; alphaBetArray_Index < nfh->sizeOfAlphaBet; alphaBetArray_Index++)
				alphaBetArray[alphaBetArray_Index] = 0; 
			// For each node: increase the value in the index of the char from alphabet
			if (nfh->moveArray[stateIndex]) {
				NtreeNode* pointerToRoot = nfh->moveArray[stateIndex]->root; // Pointer to root
				if (pointerToRoot) {
					for (int nodeIndex = 0; nodeIndex < pointerToRoot->numberOfNodes; nodeIndex++)
						if (pointerToRoot->children[nodeIndex])
							if (pointerToRoot->children[nodeIndex]->letterFromAlphabet != padChar)
								alphaBetArray[pointerToRoot->children[nodeIndex]->letterFromAlphabet - 'a']++;
					// Multiply the number of times each char from alphabet appears
					int state_numberOfNonDeterministic = 1; // Number of new matrix for the given state
					for (int alphaBetArray_Index = 0; alphaBetArray_Index < nfh->sizeOfAlphaBet; alphaBetArray_Index++)
						if (alphaBetArray[alphaBetArray_Index] > 1) {
							state_numberOfNonDeterministic *= alphaBetArray[alphaBetArray_Index];
						}
					// Multiply the number of nondeterministic of the state by total number
					total_numberOfNonDeterministic *= state_numberOfNonDeterministic;
				}
			}
				alphaBetCounterMatrix[stateIndex] = alphaBetArray;
		}
		// Allocates the output matrix
		int*** moveMatrix = (int***)malloc(sizeof(int**) * total_numberOfNonDeterministic);
		Error::checkMalloc((void*)moveMatrix, total_numberOfNonDeterministic);
		for (int possibleIndex = 0; possibleIndex < total_numberOfNonDeterministic; possibleIndex++) {
			int** newMatrix = (int**)malloc(sizeof(int*) * nfh->numberOfStates);
			Error::checkMalloc((void*)newMatrix, nfh->numberOfStates);
			for (int stateIndex = 0; stateIndex < nfh->numberOfStates; stateIndex++) {
				newMatrix[stateIndex] = (int*)malloc(sizeof(int) * (nfh->sizeOfAlphaBet));
				Error::checkMalloc((void*)newMatrix[stateIndex], nfh->sizeOfAlphaBet);
			}
			moveMatrix[possibleIndex] = newMatrix;
		}
		// Fills the moveMatrix
		int mode, valueCounter, jump = 0;
		for (int treeStateIndex = 0; treeStateIndex < nfh->numberOfStates; treeStateIndex++) { // Run on tree states
			for (int treeCharIndex = 0; treeCharIndex < nfh->sizeOfAlphaBet; treeCharIndex++) { // Run on each char
				int numberOfAppearance = alphaBetCounterMatrix[treeStateIndex][treeCharIndex];
				if (numberOfAppearance == 0)
					/* If the number of appearance is 0 - puts (-1) in all the matrixs */
					for (int possibleIndex = 0; possibleIndex < total_numberOfNonDeterministic; possibleIndex++)
						moveMatrix[possibleIndex][treeStateIndex][treeCharIndex] = NO_NEXT_STATE;
				else if (numberOfAppearance == 1) {
					// Search for next state value
					NtreeNode* pointerToRoot = nfh->moveArray[treeStateIndex]->root; // Pointer to root
					int nextState;
					char currentChar = treeCharIndex + 'a';
					for (int charFromAlphaBet = 0; charFromAlphaBet < nfh->sizeOfAlphaBet; charFromAlphaBet++)
						if (pointerToRoot->children[charFromAlphaBet])
							if (currentChar == pointerToRoot->children[charFromAlphaBet]->letterFromAlphabet) {
								nextState = pointerToRoot->children[charFromAlphaBet]->nextState;
								// puts nextState in all the matrixs
								for (int possibleIndex = 0; possibleIndex < total_numberOfNonDeterministic; possibleIndex++)
									moveMatrix[possibleIndex][treeStateIndex][treeCharIndex] = nextState;
								break;
							}
				}
				else {
					// Search for next state values
					NtreeNode* pointerToRoot = nfh->moveArray[treeStateIndex]->root; // Pointer to root
					int* nextState = (int*)malloc(sizeof(int) * numberOfAppearance);
					Error::checkMalloc((void*)nextState, numberOfAppearance);
					int nextStateIndex = 0;
					char currentChar = treeCharIndex + 'a';
					for (int charFromAlphaBet = 0; charFromAlphaBet < pointerToRoot->numberOfNodes; charFromAlphaBet++)
						if(pointerToRoot->children[charFromAlphaBet])
							if (currentChar == pointerToRoot->children[charFromAlphaBet]->letterFromAlphabet)
								nextState[nextStateIndex++] = pointerToRoot->children[charFromAlphaBet]->nextState;
					mode = nextStateIndex = 0;
					valueCounter = total_numberOfNonDeterministic / pow(2, ++jump);
					// puts nextState in all the matrixs
					for (int possibleIndex = 0; possibleIndex < total_numberOfNonDeterministic; possibleIndex++) {
						if (!mode) {
							moveMatrix[possibleIndex][treeStateIndex][treeCharIndex] = nextState[nextStateIndex];
							valueCounter --;
							if (valueCounter <= 0) {
								mode = 1;
								nextStateIndex++;
								if (nextStateIndex == numberOfAppearance) nextStateIndex = 0;
								valueCounter = total_numberOfNonDeterministic / pow(2, jump);
							}
						}
						else {
							moveMatrix[possibleIndex][treeStateIndex][treeCharIndex] = nextState[nextStateIndex];
							valueCounter--;
							if (!valueCounter) {
								mode = 0;
								nextStateIndex++;
								if (nextStateIndex == numberOfAppearance) nextStateIndex = 0;
								valueCounter = total_numberOfNonDeterministic / pow(2, jump);
							}
						}
					}
				}
			}
		}
		NFA* returnNFA = new NFA(moveMatrix, nfh->alphaBet, nfh->starterState, nfh->finalStates, total_numberOfNonDeterministic,
			nfh->numberOfStates, nfh->sizeOfAlphaBet, nfh->sizeOfFinalStates);
		return returnNFA;
	}
	// Convert NFH to DFH
	// The maximum size of newState can reach (2^n); n is the number of states
	static DFH* convert_NFH_to_DFH(NFH* nfh) {
		// Initializes
		DFH* returnDFH;
		Ntree** moveArray;
		// Allocates memory
		int newStateSize = pow(2, nfh->numberOfStates), newFinalStatesSize = pow(2, nfh->numberOfStates);
		int* newFinalStates = (int*)malloc(sizeof(int) * newFinalStatesSize);
		Error::checkMalloc((void*)newFinalStates, newFinalStatesSize);
		char** newStates = (char**)malloc(sizeof(char*) * newStateSize);
		Error::checkMalloc((void*)newStates, newStateSize);
		for (int stateIndex = 0; stateIndex < newStateSize; stateIndex++) {
			newStates[stateIndex] = (char*)malloc(sizeof(char) * (nfh->numberOfStates + 1));
			Error::checkMalloc((void*)newStates[stateIndex], nfh->numberOfStates + 1);
		}
		// Generates the newStates
		generate(newStates, newStateSize, nfh->numberOfStates);
		// Builds the final states
		// Old final states
		for (int stateIndex = 0; stateIndex < nfh->sizeOfFinalStates; stateIndex++)
				newFinalStates[stateIndex] = nfh->finalStates[stateIndex];
		// New final states
		int newFinalStateIndex = nfh->sizeOfFinalStates;
		for (int newStateIndex = 0; newStateIndex < newStateSize - 1; newStateIndex++){
			// Checks if at least one of the char in newStates is final state
			for (int charInNewStates = 0; charInNewStates < 2; charInNewStates++)
				// For each char in the new state - checks if the char is a final state
				for (int finalStateIndex = 0; finalStateIndex < newFinalStateIndex; finalStateIndex++)
					if (newStates[newStateIndex][charInNewStates] != padChar &&
						newFinalStates[finalStateIndex] == newStates[newStateIndex][charInNewStates] - '0'){
					// if so - insert the new state index to the new final states array
					// (Before - Checks if the new final state is already exists)
						bool isInsertFinalState = true;
						for (int finalStateIndex = 0; finalStateIndex < newFinalStateIndex; finalStateIndex++)
							if (newFinalStates[finalStateIndex] == newStateIndex) isInsertFinalState = false;
						if (isInsertFinalState) newFinalStates[newFinalStateIndex++] = newStateIndex;
					}
		}
		// Allocates memory by creating the new moveMatrix
		moveArray = (Ntree**)malloc(sizeof(Ntree*) * newStateSize);
		Error::checkMalloc((void*)moveArray, newStateSize);
		// For each state in the new moveMatrix: (do not include empty set index)
		for (int newStateIndex = 0; newStateIndex < newStateSize - 1; newStateIndex++) {
			// Allocates memory
			moveArray[newStateIndex] = (Ntree*)malloc(sizeof(Ntree) * nfh->sizeOfAlphaBet);
			Error::checkMalloc(moveArray[newStateIndex], nfh->sizeOfAlphaBet);
			// Copy and merge Ntrees from the original moveArray
			// For each char in the new state:
			for (int charInNewStates = 0; (charInNewStates < nfh->numberOfStates) && (newStates[newStateIndex][charInNewStates] != padChar); charInNewStates++) {
				// Copy the original root in order not to change the input
				char currentState = newStates[newStateIndex][charInNewStates]; // The current char. Represented a state.
				if (nfh->moveArray[currentState - '0']) {
					NtreeNode* pointerToRoot = nfh->moveArray[currentState - '0']->root; // Pointer to the root
					if (charInNewStates) { // Copies and merges the same branches
						NtreeNode* rootBeforeMerge = Ntree::copyRoot(pointerToRoot, pointerToRoot->letterFromAlphabet,
							pointerToRoot->nextState, pointerToRoot->numberOfNodes);
						int newNumberOfNodes;
						moveArray[newStateIndex]->root->children = Ntree::addChildren(moveArray[newStateIndex]->root->children, rootBeforeMerge,
							rootBeforeMerge->numberOfNodes, &newNumberOfNodes);
						moveArray[newStateIndex]->root->numberOfNodes = newNumberOfNodes;
					}
					else // Just copies
						moveArray[newStateIndex]->root = Ntree::copyRoot(pointerToRoot, pointerToRoot->letterFromAlphabet,
							pointerToRoot->nextState, pointerToRoot->numberOfNodes);
				}
			}
			// Merge branches with the same sequence of letters but diffrent next state value
			// Notice: Non-deterministic represent by the same sequence of letter with diffrent
			//		   next-state value. Because of the specific data structure of NtreeNode,
			//         mergeNextStateValues merges only the leafs's children by going to the leafs
			//         and looking for the same letterFromAlphabet values.
			//		   The new next state value calculte with newStates (map).
			mergeNextStateValues(moveArray[newStateIndex]->root, newStates, newStateSize);
		}
		// empty set index:
		// Allocates memory
		moveArray[newStateSize - 1] = (Ntree*)malloc(sizeof(Ntree) * nfh->sizeOfAlphaBet);
		Error::checkMalloc(moveArray[newStateSize - 1], nfh->sizeOfAlphaBet);
		moveArray[newStateSize - 1]->root = NULL;
		// Allocates memory by creating the new state
		returnDFH = new DFH(moveArray, nfh->alphaBet, nfh->starterState, newFinalStates, nfh->alphaCondition, newStateSize, nfh->sizeOfAlphaBet, newFinalStateIndex);
		// Free memory
		free(newStates);
		return returnDFH;
	}
	// Convert NFH to DFH
	static NFH* convert_DFH_to_NFH(DFH* dfh) {
		NFH* returnNFH = new NFH(dfh->nfh->moveArray, dfh->nfh->alphaBet, dfh->nfh->starterState, dfh->nfh->finalStates,
					dfh->nfh->alphaCondition, dfh->nfh->numberOfStates, dfh->nfh->sizeOfAlphaBet, dfh->nfh->sizeOfFinalStates);
		return returnNFH;
	}
};
#endif
