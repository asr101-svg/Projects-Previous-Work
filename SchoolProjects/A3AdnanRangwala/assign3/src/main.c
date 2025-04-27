#include "VCParser.h"


//--------------------------------------------------REMOVE WHEN SUBMITTING--------------------------------------------------
int main() {
    VCardErrorCode value;
    printf("hello parsing\n");
    Card * theCard = NULL;
    createCard("./bin/cards/testCard-Ann.vcf", &theCard);
    char * theString = cardToString(theCard);
    printf("Here is my card:\n%s\n", theString);
    free(theString);
    writeCard("writefile.vcf", theCard);
    //strcpy(theCard->birthday->time,"212233");
    value = validateCard(theCard);
    if (value != OK) {
        printf("validate card has spotted an error: ");
       char * errCode = errorToString(value);
        printf("%s\n",errCode);
        free(errCode);
    }
    //strcpy(getFromFront(theCard->fn->values),"hitest");
    //printf("FN value is: %s\n",(char*)getFromFront(theCard->fn->values));
    deleteCard(theCard);

    Card * theCard2 = NULL;
    char * fn = malloc(sizeof(char)*100);
    strcpy(fn,"test");
    createCardUI(fn, &theCard2);
    char * theString2 = cardToString(theCard2);
    printf("Here is my card:\n%s\n", theString2);
    return 0;
}
//--------------------------------------------------REMOVE WHEN SUBMITTING--------------------------------------------------