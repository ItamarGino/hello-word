#ifndef HEADER_H
#define HEADER_H

/* cpp functions		*/
void complement_test();
void convert_test();
void DFA_test();
void emptinessChecking_test();
void main(); // intersection_test
void NFA_test();
void NFH_test();
void ZIP_test();
void DFA_complement_test();

/*	State modes			*/
constexpr auto NO_THOROUGHFARE = -1;
/*	Quantifiers			*/
const enum Quantifiers { EACH, EVERY };
/*	Ntree				*/
constexpr auto fakeChar = '%';
constexpr auto NO_NEXT_STATE = -1;
/*	Emptiness checking	*/
constexpr auto Checked = -1;
/*	ZIP					*/
constexpr auto padChar = '#';
/*  Convert				*/
constexpr auto EMPTY_STATE = 'E';
/* GUI input handler */
constexpr auto BUFFER_SIZE = 1000;
constexpr auto NEW_VALUE_SIZE = 100;
constexpr auto ASCII_ENTER = '\n';
constexpr auto ASCII_COLON = ':';
constexpr auto ASCII_COMA = ',';
constexpr auto ASCII_LEFT_BRACKET = '{';
constexpr auto ASCII_RIGHT_BRACKET = '}';
const char* nfh_file_name = "C:/Users/ASUS/Desktop/FinalinputForAlgorithmNFH.txt";
const char* nfa_file_name = "C:/Users/ASUS/Desktop/FinalinputForAlgorithm.txt";

#endif
