#include "ZIP.h"

/***********  Read me!	*****************/
// *Before each QA test you must change the name of the fucntion to main()
// *Because of the complexity of the project, we cancle the rest of the
//   tests ( //cout << NFH_LanguageBelonging::isBelong(...) )
// *For running the test - remove the '//'
// *You should run only one test each running
/****************************************/
// Change to main() for testing
void ZIP_test()
{
    cout << "\n" << "ZIP Example:\n";
    char zip_alphabet[4] = { 'a', 'b', 'c', 'd' };
    char zip_word1[3] = { 'a', 'b', 'c' };
    char zip_word2[4] = { 'a', 'b', 'c', 'd' };
    char zip_word3[3] = { 'a', 'a', 'b' };
    int sizeOfWords[3] = { 3, 4, 3 };
    char* S[3] = { zip_word1, zip_word2, zip_word3 };
    ZIP zipExample(S, 3, sizeOfWords, zip_alphabet);
    zipExample.toString();
}
