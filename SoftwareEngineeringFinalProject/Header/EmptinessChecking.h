#ifndef EMPTINESSCHECKING_H
#define EMPTINESSCHECKING_H
#include "Header.h"
#include "NFA.h"
#include "Error.h"
#include <stdlib.h>

class EmptinessChecking { // Checking if there is any way to get to the final states
public:
	NFA* nfa;
	int start;
	int* goal;
	int* states;
	EmptinessChecking(NFA* givenNFA, int givenStart, int* givenGoal) {
		nfa = givenNFA;
		start = givenStart;
		goal = givenGoal;
		states = (int*)malloc(nfa->numberOfStates * sizeof(int));
		Error::checkMalloc((void*)states, nfa->numberOfStates);
		for (int stateIndex = 0; stateIndex < nfa->numberOfStates; stateIndex++) states[stateIndex] = stateIndex;
	}
	~EmptinessChecking() { 
		free(states);
	}
	// Returns the neighbors of the given state
	int* getNeighbors(int state) {
		int value;
		int* returnStates = (int*)malloc(sizeof(int) * nfa->numberOfStates);
		Error::checkMalloc(returnStates, nfa->numberOfStates);
		for (int stateIndex = 0; stateIndex < nfa->numberOfStates; stateIndex++)
			returnStates[stateIndex] = Checked; // Not a state
		int returnStatesIndex = 0;
		for (int possiblesIndex = 0; possiblesIndex < nfa->numberOfNonDeterministic; possiblesIndex++) {
			for (int charsIndex = 0; charsIndex < nfa->sizeOfAlphaBet; charsIndex++) {
				value = nfa->moveMatrix[possiblesIndex][state][charsIndex];
				if (value != state && value != Checked) {
					returnStates[returnStatesIndex] = nfa->moveMatrix[possiblesIndex][state][charsIndex];
					returnStatesIndex++;
				}
			}
		}
		if (returnStatesIndex) {
			return returnStates;
		}
		return NULL;
	}
	// Checks if there are possible ways to reach the accepting (finals) states
	// Starts from the given state
	bool checkEmptiness(NFA* nfa, int state) {
		// Stop condition
		for (int stateIndex = 0; stateIndex < nfa->sizeOfFinalStates; stateIndex++)
			if (state == goal[stateIndex]) return true;
		states[state] = Checked; // For not checking again this state
		// Get current state neighbors
		int* neighbors = getNeighbors(state);
		if (neighbors)
			for (int possibleIndex = 0; possibleIndex < nfa->numberOfNonDeterministic; possibleIndex++)
				for (int stateIndex = 0; stateIndex < nfa->numberOfStates; stateIndex++)
					if ((neighbors[stateIndex] != Checked) && checkEmptiness(nfa, neighbors[stateIndex])) {
						free(neighbors);
						return true;
					}
		free(neighbors);
		return false;
	}
	// Checks if there are possible ways to reach the accepting (finals) states
	// Starts from the given state
	char** checkEmptiness(NFA* nfa, int state, char** word, int* size, bool* answer) {
		// Stop condition
		for (int stateIndex = 0; stateIndex < nfa->sizeOfFinalStates; stateIndex++)
			if (state == goal[stateIndex]) {
				*answer = true;
				return word; // Equivalent to epsilon
			}
		states[state] = Checked; // For not checking again this state
		// Get current state neighbors
		int* neighbors = getNeighbors(state);
		if (neighbors)
			for (int possibleIndex = 0; possibleIndex < nfa->numberOfNonDeterministic; possibleIndex++) {
				for (int stateIndex = 0; stateIndex < nfa->numberOfStates; stateIndex++) {
					// Gets the char that move current state to neighbors[stateIndex]
					for (int charIndex = 0; charIndex < nfa->sizeOfAlphaBet; charIndex++)
						if (nfa->moveMatrix[possibleIndex][state][charIndex] == neighbors[stateIndex]) {
							(*size)++;
							if (*size == 1) *word = new char(charIndex + 'a');
							else {
								// Allocates temp memory
								char** newWord = (char**)malloc(sizeof(char*) * (*size - 1));
								Error::checkMalloc(newWord, (*size - 1));
								// Copy memory
								for (int charInWord = 0; charInWord < (*size) - 1; charInWord++)
									newWord[charInWord] = new char(*word[charInWord]);
								// Free word
								for (int charInWord = 0; charInWord < (*size - 1); charInWord++) delete(word[charInWord]);
								free(word);
								// Allocates bigger memory for word
								word = (char**)malloc(sizeof(char*) * (*size));
								Error::checkMalloc(word, *size);
								// Copy newWord to word
								for (int charInWord = 0; charInWord < (*size - 1); charInWord++)
									word[charInWord] = new char(*newWord[charInWord]);
								word[(*size) - 1] = new char(charIndex + 'a');
							}
						}
					if ((neighbors[stateIndex] != Checked)) {
						word = checkEmptiness(nfa, neighbors[stateIndex], word, size, answer);
						if (*answer) {
							// Word accepted
							free(neighbors);
							*answer = true;
							return word;
						}
						else {
							// Word denied
							(*size)--;
							word[(*size)] = NULL;
						}
					}
				}
			}
		free(neighbors);
		*answer = false;
		return word;
	}
};

#endif
