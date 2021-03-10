#ifndef NFH_H
#define NFH_H
#include "Header.h"
#include "Ntree.h"

// Struct for NFH's alpha condition
struct AlphaCondition {
public:
	Quantifiers* quantifiers;
	int numberOfQuantifiers;
	// Constructor
	AlphaCondition() {};
	AlphaCondition(Quantifiers* givenQuantifiers, int givenNumberOfQuantifiers) {
		quantifiers = givenQuantifiers;
		numberOfQuantifiers = givenNumberOfQuantifiers;
	}
	// Destructor
	~AlphaCondition() {}
};

/*	Non-deterministic Finite Hyper-Automaton							*/
/*	MoveArray: cells indicates states.
			   each cell represents a legitimate sequence of letters.	*/
class NFH {
public:
	Ntree** moveArray;
	AlphaCondition alphaCondition;
	char* alphaBet;
	int starterState;
	int* finalStates;
	int currentState;
	int sizeOfAlphaBet;
	int numberOfStates;
	int sizeOfFinalStates;
	// Constructor - Initializes NFH
	NFH(Ntree** givenMoveArray, char* givenAlphaBet, int givenStarterState, int* givenFinalStates, AlphaCondition givenAlphaCondition,
		int givenNumberOfStates, int givenSizeOfAlphaBet, int givenSizeOfFinalStates) {
		moveArray = givenMoveArray;
		starterState = givenStarterState;
		currentState = starterState;
		finalStates = givenFinalStates;
		alphaBet = givenAlphaBet;
		sizeOfAlphaBet = givenSizeOfAlphaBet;
		numberOfStates = givenNumberOfStates;
		sizeOfFinalStates = givenSizeOfFinalStates;
		alphaCondition = givenAlphaCondition;
	}
	//	Destructor
	~NFH() {
		for (int stateNtreeIndex = 0; stateNtreeIndex < numberOfStates; stateNtreeIndex++)
			delete moveArray[stateNtreeIndex];
	}
	// Switch the current state
	int switchState(char* letters) {
		bool isFound = false;
		NtreeNode* pointerToNode = moveArray[currentState]->root;
		for (int letterIndex = 0; letterIndex < sizeOfAlphaBet; letterIndex++) {
			// Run on all the legitimate sequence
			for (int nodeIndex = 0; nodeIndex < pointerToNode->numberOfNodes; nodeIndex++) {
				// Search for the letter and move on to the next letter
				isFound = false;
				if (pointerToNode->children[nodeIndex]->letterFromAlphabet == letters[letterIndex]) {
					pointerToNode = pointerToNode->children[nodeIndex];
					nodeIndex = pointerToNode->numberOfNodes; // Move to the next letter
					isFound = true;
					if (letterIndex == pointerToNode->numberOfNodes - 1) { // Leaf
						return pointerToNode->nextState;
					}
				}
			}
			if (isFound == false) {
				return NO_THOROUGHFARE;
			}
		}
		return NO_THOROUGHFARE;
	}
	/*	Just for QA!
		(This function does'nt include alpha condition)	*/
	// Gets a word and return if the word belong to the language of the NFA
	bool isBelong(char** hyperWord, int numberOfWords) {
		bool isBelong = false;
		for (int wordIndex = 0; wordIndex < numberOfWords; wordIndex++) {
			currentState = switchState(hyperWord[wordIndex]);
			if (currentState == NO_THOROUGHFARE) {
				currentState = starterState;
				return isBelong; // False
			}
		}
		for (int finalStateIndex = 0; finalStateIndex < sizeOfFinalStates; finalStateIndex++) {
			if (currentState == finalStates[finalStateIndex]) {
				isBelong = true;
			}
		}
		currentState = starterState;
		return isBelong;
	}
	// Go to the leaf and return all the next states values
	static void getNextStateValues(NtreeNode* root, int* nextStateValues, int* index) {
		if (!root) return;
		if (!root->children[0]) { // Leaf
			nextStateValues[*index] = root->nextState;
			*index = *index + 1;
			return;
		}
		for (int nodeIndex = 0; nodeIndex < root->numberOfNodes; nodeIndex++)
			getNextStateValues(root->children[nodeIndex], nextStateValues, index);
	}
	// Removes apsilon moves from root
	// For each first-level nodes (the first nodes after the main root)
	// if the letterFromAlphabet is equal to padChar (#), goes to the leafs's next state
	// values and copies the all the children of the nfh specific state.
	// Notice - if one of the stats is final state - the current state will became
	// final state too.
	void removeEpsilonMoves() {
		// Remove all epsilon moves
		// For each state:
		for (int stateIndex = 0; stateIndex < numberOfStates; stateIndex++) {
			bool removeEpsilonAgain = false;
			// Point to the root and search for padChar in the nodes
			if (moveArray[stateIndex]) {
				NtreeNode* pointerToRoot = moveArray[stateIndex]->root;
				// If exists
				if (pointerToRoot) {
					// Run on all the children and search for padChar (#)
					for (int nodeIndex = 0; nodeIndex < pointerToRoot->numberOfNodes; nodeIndex++) {
						// If exists
						if (pointerToRoot->children[nodeIndex]) {
							if (pointerToRoot->children[nodeIndex]->letterFromAlphabet == padChar) {
								// Gets all the next states moves
								int* nextStatesValues = (int*)malloc(sizeof(int) * pointerToRoot->children[nodeIndex]->numberOfNodes);
								Error::checkMalloc(nextStatesValues, pointerToRoot->children[nodeIndex]->numberOfNodes);
								int nextStatesValuesIndex = 0;
								getNextStateValues(pointerToRoot->children[nodeIndex], nextStatesValues, &nextStatesValuesIndex);
								for (int valueIndex = 0; valueIndex < nextStatesValuesIndex; valueIndex++) {
									// Only if the next state value is diffrent
									if (stateIndex != nextStatesValues[valueIndex]) {
										// if one of the next state value is final state - the current state will also be final state
										NtreeNode* pointerToRootToCopy = moveArray[nextStatesValues[valueIndex]]->root;
										NtreeNode* rootCopy = Ntree::copyRoot(pointerToRootToCopy, pointerToRootToCopy->letterFromAlphabet,
											pointerToRootToCopy->nextState, pointerToRootToCopy->numberOfNodes);
										// Check if we insert a padChar with the same next state value
										for (int newChild = 0; newChild < rootCopy->numberOfNodes; newChild++)
											if (rootCopy->children[newChild])
												if (rootCopy->children[newChild]->letterFromAlphabet == padChar)
													if (Ntree::isEqualBranches(pointerToRoot->children[nodeIndex], rootCopy->children[newChild]))
														rootCopy->children[newChild] = NULL;
										pointerToRoot->children[nodeIndex] = NULL;
										int newNumberOfNodes;
										pointerToRoot->children = Ntree::addChildren(pointerToRoot->children, rootCopy, pointerToRoot->numberOfNodes, &newNumberOfNodes);
										if (newNumberOfNodes > pointerToRoot->numberOfNodes)
											pointerToRoot->numberOfNodes = newNumberOfNodes;
										// Checks if the next state value is final state
										// if so - add the current state to the final states
										bool isFinalState = false;
										for (int finalStateIndex = 0; finalStateIndex < sizeOfFinalStates; finalStateIndex++)
											if (finalStates[finalStateIndex] == nextStatesValues[valueIndex])
												isFinalState = true;
										if (isFinalState) {
											int* newFinalState = (int*)realloc(finalStates, sizeof(int) * (sizeOfFinalStates + 1));
											newFinalState[sizeOfFinalStates] = stateIndex;
											finalStates = newFinalState;
											sizeOfFinalStates = sizeOfFinalStates + 1;
										}
										// In this case we got new children - maybe one of them with padChar
										if (stateIndex < nextStatesValues[valueIndex]) removeEpsilonAgain = true;
									}
								}
								free(nextStatesValues);
							}
						}
					}
				}
				if (removeEpsilonAgain) stateIndex--;
			}
		}
	}
};

#endif
