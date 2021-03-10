#include "EmptinessChecking.h"
#include "NFA.h"
#include "Header.h"

/***********  Read me!	*****************/
// *Before each QA test you must change the name of the fucntion to main()
// *Because of the complexity of the project, we cancle the rest of the
//   tests ( //cout << NFH_LanguageBelonging::isBelong(...) )
// *For running the test - remove the '//'
// *You should run only one test each running
/****************************************/
//emptinessChecking_test
// Change to main() for testing
void emptinessChecking_test()
{
    char alphabet_NFA[2] = { 'a', 'b' };
    int* final_states = new int(3);
    /*      NFA nfa_Empty    */
    char alphabet_NFA_4chars[2] = { 'a', 'b' };
    int moveMatrix1_4chars[4][2] = { {1, 2}, {1, 1}, {2, 3}, {3, 3} };
    int* pointerToMoveMatrix1_4chars[4] = { moveMatrix1_4chars[0], moveMatrix1_4chars[1], moveMatrix1_4chars[2], moveMatrix1_4chars[3] };
    int** pointerToMoveMatrix_NonDeterministic_4chars[1] = { pointerToMoveMatrix1_4chars };
    NFA* nfa_Empty = new NFA(pointerToMoveMatrix_NonDeterministic_4chars, alphabet_NFA_4chars, 0, final_states, 1, 4, 2, 1);
    /*      NFA nfa_NotEmpty    */
    int moveMatrix1_A[2][2] = { {1, 0}, {0, 1} };
    int* pointerToMoveMatrix1_A[2] = { moveMatrix1_A[0], moveMatrix1_A[1] }; // pointer to moveMatrix1
    int moveMatrix2_A[2][2] = { {1, 1}, {0, 1} };
    int* pointerToMoveMatrix2_A[2] = { moveMatrix2_A[0], moveMatrix2_A[1] }; // pointer to moveMatrix1
    int** pointerToMoveMatrix_NonDeterministic_A[2] = { pointerToMoveMatrix1_A , pointerToMoveMatrix2_A };
    NFA* nfa_NotEmpty = new NFA(pointerToMoveMatrix_NonDeterministic_A, alphabet_NFA, 0, final_states, 2, 2, 2, 1);
    /****************************************************************************************************************/
    cout << "\n" << "EmptinessChecking Example:\n";
    std::cout << std::boolalpha; // output 1 to true; output 0 to false
    // Create instances of EmptinessChecking
    EmptinessChecking emptinessChecker1(nfa_Empty, nfa_Empty->starterState, nfa_Empty->finalStates);
    EmptinessChecking emptinessChecker2(nfa_NotEmpty, nfa_NotEmpty->starterState, nfa_NotEmpty->finalStates);
    // Initialize a word and a size
    char** word = new char*();
    int* size = new int(0);
    bool answer;
    // Check emptiness with return word
    word = emptinessChecker1.checkEmptiness(nfa_Empty, nfa_Empty->starterState, word, size, &answer);
    cout << answer;
    for (int i = 0; i < *size; i++) cout << *word[i];
    // Check emptiness (regular)
    cout << emptinessChecker2.checkEmptiness(nfa_NotEmpty, nfa_NotEmpty->starterState) << "\n";
    /*  free all the allocated memory    */
    free(final_states);
}
