#include "Header.h"
#include "Error.h"
#include "NFA.h"

/***********  Read me!	*****************/
// *Before each QA test you must change the name of the fucntion to main()
// *Because of the complexity of the project, we cancle the rest of the
//   tests ( //cout << NFH_LanguageBelonging::isBelong(...) )
// *For running the test - remove the '//'
// *You should run only one test each running
/****************************************/
// Change to main() for testing
void NFA_test()
{
	int* final_states = (int*)malloc(sizeof(int));
	Error::checkMalloc((void*)final_states, 1);
	final_states[0] = 1;
	char alphabet_NFA[2] = { 'a', 'b' };
	int moveMatrix1_A[2][2] = { {-1, 0}, {0, 1} };
	int* pointerToMoveMatrix1_A[2] = { moveMatrix1_A[0], moveMatrix1_A[1] }; // pointer to moveMatrix1
	int moveMatrix2_A[2][2] = { {1, 1}, {0, 1} };
	int* pointerToMoveMatrix2_A[2] = { moveMatrix2_A[0], moveMatrix2_A[1] }; // pointer to moveMatrix1
	int** pointerToMoveMatrix_NonDeterministic_A[2] = { pointerToMoveMatrix1_A , pointerToMoveMatrix2_A };
	NFA A(pointerToMoveMatrix_NonDeterministic_A, alphabet_NFA, 0, final_states, 2, 2, 2, 1);
	/****************************************************************************************************************/
	cout << "\n" << "NFA Example:\n";
	std::cout << std::boolalpha; // output 1 to true; output 0 to false
	string wordInLanguage = "ab";
	string wordNotInLanguage = "aa";
	cout << A.isBelong(wordInLanguage) << "\n";
	cout << A.isBelong(wordNotInLanguage) << "\n";
	/*  free all the allocated memory    */
	free(final_states);
}
