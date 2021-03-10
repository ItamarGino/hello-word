#include "Header.h"
#include "Error.h"
#include "NFH.h"

/***********  Read me!	*****************/
// *Before each QA test you must change the name of the fucntion to main()
// *Because of the complexity of the project, we cancle the rest of the
//   tests ( //cout << NFH_LanguageBelonging::isBelong(...) )
// *For running the test - remove the '//'
// *You should run only one test each running
/****************************************/
// Change to main() for testing
void NFH_test()
{
    /*      NFH B    */
	int* final_states = (int*)malloc(sizeof(int));
	Error::checkMalloc((void*)final_states, 1);
	final_states[0] = 1;
    char alphabet_NFH[2] = { 'a', 'b'};
    char branch1[2] = { 'a', 'a' };
    char* pointerToBranch1 = branch1;
    Transition* state1_transition1 = new Transition(pointerToBranch1, 0);
    char branch2[2] = { '#', 'a' };
    char* pointerToBranch2 = branch2;
    Transition* state1_transition2 = new Transition(pointerToBranch2, 1);
    Transition* state1_givenTransition[2] = { state1_transition1 , state1_transition2 };
    StateTransition* state1 = new StateTransition(state1_givenTransition, 2, 2);
    Transition* state2_transition1 = new Transition(pointerToBranch2, 1);
    Transition* state2_givenTransition[1] = { state2_transition1 };
    StateTransition* state2 = new StateTransition(state2_givenTransition, 1, 2);
    Ntree* moveArray[2] = { new Ntree(state1), new Ntree(state2) };
    Quantifiers quantifiers[2] = { EVERY, EACH };
    AlphaCondition* alphaCondition = new AlphaCondition(quantifiers, 2);
    NFH B(moveArray, alphabet_NFH, 0, final_states, *alphaCondition, 2, 2, 1);
	/****************************************************************************************************************/
    cout << "\n" << "NFH Example:\n";
    std::cout << std::boolalpha; // output 1 to true; output 0 to false
    char word1[2] = { 'a', 'a' };
    char word2[2] = { '#', 'a' };
    char word3[2] = { 'b', 'a' };
    char* hyperWordInLanguage_1[2] = { word1, word2 };
    char* hyperWordInLanguage_2[1] = { word2 };
    char* hyperWordNotInLanguage_1[1] = { word3 };
    char* hyperWordNotInLanguage_2[1] = { word1 };
    cout << B.isBelong(hyperWordInLanguage_1, 2) << "\n";
    cout << B.isBelong(hyperWordInLanguage_2, 1) << "\n";
    cout << B.isBelong(hyperWordNotInLanguage_1, 1) << "\n";
    cout << B.isBelong(hyperWordNotInLanguage_2, 1) << "\n";
    /*  free all the allocated memory    */
    free(final_states);
}
