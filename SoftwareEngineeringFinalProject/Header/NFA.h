#ifndef NFA_H
#define NFA_H
#include "Header.h"
#include "Error.h"
#include <iostream>
#include <string>
using namespace std;

/*	Non-deterministic Finite Automaton								*/
/*	MoveMatrix: Rows indicates states - a row for each state;
			   Columns indicates a letter from the given Alphabet.	*/
/*	Non-deterministic represent by set of deterministic. i.e. Array
	of matrix for each possibility									*/
class NFA {
private:
	bool extendsFinalStatesFlag;
public:
	int*** moveMatrix;
	char* alphaBet;
	int starterState;
	int* finalStates;
	int currentState;
	int numberOfNonDeterministic;
	int numberOfStates;
	int sizeOfAlphaBet;
	int sizeOfFinalStates;
	// Constructor - Initializes NFA
	NFA() {};
	NFA(int*** givenMoveMatrix, char* givenAlphaBet, int givenStarterState, int* givenFinalState, int givenNumberOfNonDeterministic,
		int givenNumberOfStates, int givenSizeOfAlphaBet, int givenSizeOfFinalStates) {
		numberOfNonDeterministic = givenNumberOfNonDeterministic;
		moveMatrix = givenMoveMatrix;
		starterState = givenStarterState;
		currentState = starterState;
		finalStates = givenFinalState;
		alphaBet = givenAlphaBet;
		numberOfStates = givenNumberOfStates;
		sizeOfAlphaBet = givenSizeOfAlphaBet;
		sizeOfFinalStates = givenSizeOfFinalStates;
		extendsFinalStatesFlag = false;
	}
	// Destructor
	~NFA() {
		for (int possibleIndex = 0; possibleIndex < numberOfNonDeterministic; possibleIndex++)
			for (int stateIndex = 0; stateIndex < numberOfStates; stateIndex++)
				free(moveMatrix[possibleIndex][stateIndex]);
		free(moveMatrix);
	}
	// Switch the current state
	int switchState(char letter, int possiblesIndex) {
		currentState = moveMatrix[possiblesIndex][currentState][letter - 'a'];
		return currentState;
	}
	// Gets a word and return if the word belong to the language of the NFA
	bool isBelong(string word) {
		bool isBelong = false, moveNext = false;
		for (int possiblesIndex = 0; possiblesIndex < numberOfNonDeterministic; possiblesIndex++) {
			for (auto& letter : word) {
				currentState = switchState(letter, possiblesIndex);
				if (currentState == NO_THOROUGHFARE) {
					moveNext = true;
					break;
				}
			}
			if (!moveNext) {
				// Checks if the current state equals to one of the final states
				for (int finalStateIndex = 0; finalStateIndex < sizeOfFinalStates; finalStateIndex++) {
					if (currentState == finalStates[finalStateIndex]) {
						isBelong = true;
					}
				}
			}
			moveNext = false;
			currentState = starterState;
		}
		return isBelong;
	}
	// Print moveMatrix
	void toString() {
		for (int possiblesIndex = 0; possiblesIndex < numberOfNonDeterministic; possiblesIndex++) {
			cout << "Matrix number: " << possiblesIndex + 1 << "\n";
			for (int stateIndex = 0; stateIndex < numberOfStates; stateIndex++) {
				cout << "(";
				for (int charIndex = 0; charIndex < sizeOfAlphaBet - 1; charIndex++) {
					cout << moveMatrix[possiblesIndex][stateIndex][charIndex] << ", ";
				}
				cout << moveMatrix[possiblesIndex][stateIndex][sizeOfAlphaBet - 1];
				cout << ")\n";
			}
			cout << "\n";
		}
	}
	// Extends NFA
	// Adds a new final state
	// All old final states can move to the new final states
	// only if gets padChar (#)
	// Single tone - Call only once
	void extendsFinalStates() {
		if (!extendsFinalStatesFlag) {
			extendsFinalStatesFlag = true;
			/* Alphabet:						*/
			sizeOfAlphaBet++;
			// Allocates new alphabet
			char* newAlphabet = (char*)malloc(sizeof(char) * sizeOfAlphaBet);
			Error::checkMalloc(newAlphabet, sizeOfAlphaBet);
			// Copy old alphabet
			for (int charIndex = 0; charIndex < sizeOfAlphaBet - 1; charIndex++)
				newAlphabet[charIndex] = alphaBet[charIndex];
			newAlphabet[sizeOfAlphaBet - 1] = padChar;
			alphaBet = newAlphabet;
			/* Final states:					*/
			sizeOfFinalStates++;
			numberOfStates++;
			int* newFinalStates = (int*)malloc(sizeof(int) * sizeOfFinalStates);
			Error::checkMalloc(newFinalStates, sizeOfFinalStates);
			// Copy old finalStates
			for (int stateIndex = 0; stateIndex < sizeOfFinalStates - 1; stateIndex++)
				newFinalStates[stateIndex] = finalStates[stateIndex];
			newFinalStates[sizeOfFinalStates - 1] = numberOfStates - 1; // the last state
			finalStates = newFinalStates;
			/* Update moveMatrix				*/
			int*** newMoveMatrix = (int***)malloc(sizeof(int**) * numberOfNonDeterministic);
			Error::checkMalloc(newMoveMatrix, numberOfNonDeterministic);
			for (int possibleIndex = 0; possibleIndex < numberOfNonDeterministic; possibleIndex++) {
				newMoveMatrix[possibleIndex] = (int**)malloc(sizeof(int*) * numberOfStates);
				Error::checkMalloc(newMoveMatrix[possibleIndex], numberOfStates);
				for (int stateIndex = 0; stateIndex < numberOfStates; stateIndex++) {
					newMoveMatrix[possibleIndex][stateIndex] = (int*)malloc(sizeof(int) * sizeOfAlphaBet);
					Error::checkMalloc(newMoveMatrix[possibleIndex][stateIndex], sizeOfAlphaBet);
					// The new final state - NO_NEXT_STATE for all chars
					// exept from the lase char (#) - self loop
					if (stateIndex == numberOfStates - 1) {
						for (int charIndex = 0; charIndex < sizeOfAlphaBet - 1; charIndex++)
							newMoveMatrix[possibleIndex][stateIndex][charIndex] = NO_NEXT_STATE;
						newMoveMatrix[possibleIndex][stateIndex][sizeOfAlphaBet - 1] = stateIndex;
					}
					// Otherwise - copy all old value
					// for the new last state - check if final state
					else {
						for (int charIndex = 0; charIndex < sizeOfAlphaBet - 1; charIndex++)
							newMoveMatrix[possibleIndex][stateIndex][charIndex] = moveMatrix[possibleIndex][stateIndex][charIndex];
						// Check if stateIndex is final state
						// If so - move to the new state, Otherwise - NO_NEXT_STATE
						bool isFinalState = false;
						for (int finalStateIndex = 0; finalStateIndex < sizeOfFinalStates; finalStateIndex++)
							if (finalStates[finalStateIndex] == stateIndex) isFinalState = true;
						if (isFinalState) newMoveMatrix[possibleIndex][stateIndex][sizeOfAlphaBet - 1] = numberOfStates - 1;
						else newMoveMatrix[possibleIndex][stateIndex][sizeOfAlphaBet - 1] = NO_NEXT_STATE;
					}
				}
			}
			moveMatrix = newMoveMatrix;
			return;
		}
		else return;
	}
};

#endif
