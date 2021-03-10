#ifndef ZIP_H
#define ZIP_H
#include "Header.h"
#include "Error.h"

class ZIP{
public:
	// Gets a set S that represent a tuple of finite words over Σ (alphabet)
	// This function "zips" the word in s together
	// We used the sign "#" to indicate the words that have ended
	char** input; // Set of words (hyperWord)
	char* alphaBet;
	int maxSizeWord; // The length of the longest word in S
	int sizeOfInput;
	int* sizeOfWordsInInput;
	char** output;
	// Constructor - Initializes ZIP
	ZIP(char** givenHyperWord, int sizeOfGivenHyperWord, int* sizeOfwords, char* givenAlphaBet) {
		input = givenHyperWord;
		alphaBet = givenAlphaBet;
		sizeOfInput = sizeOfGivenHyperWord;
		sizeOfWordsInInput = sizeOfwords;
		maxSizeWord = 0;
		for (int wordIndex = 0; wordIndex < sizeOfInput; wordIndex++)
			if (maxSizeWord < sizeOfWordsInInput[wordIndex])
				maxSizeWord = sizeOfWordsInInput[wordIndex];
		// Starts to create the new hyperWord - output
		// The size of output (number of words) determined by maxSizeWord
		// The size of each word in output determined by sizeOfGivenHyperWord
		output = (char**)malloc(sizeof(char*) * maxSizeWord);
		Error::checkMalloc(&output, maxSizeWord);
		for (int index_WordInOutput = 0; index_WordInOutput < maxSizeWord; index_WordInOutput++) {
			char* newWord = (char*)malloc(sizeof(char) * sizeOfInput);
			Error::checkMalloc((void*)newWord, sizeOfInput);
			for (int index_wordInS = 0; index_wordInS < sizeOfInput; index_wordInS++) {
				if (sizeOfWordsInInput[index_wordInS] < (index_WordInOutput + 1)) newWord[index_wordInS] = padChar;
				else newWord[index_wordInS] = input[index_wordInS][index_WordInOutput];
			}
			output[index_WordInOutput] = newWord;
		}
	}
	//	Destructor
	~ZIP() { free(output); }
	// Print the output hyperWord
	void toString() {
		for (int index_wordInOutput = 0; index_wordInOutput < maxSizeWord; index_wordInOutput++) {
			std::cout << "(";
			for (int index_CharInWord = 0; index_CharInWord < sizeOfInput; index_CharInWord++)
				std::cout << output[index_wordInOutput][index_CharInWord];
			std::cout << ") ";
		}
		std::cout << "\n";
	}
};


#endif
