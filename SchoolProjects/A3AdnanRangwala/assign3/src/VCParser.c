#include "VCParser.h"
#include <ctype.h>
void updateCardFn(Card * card, char * newVal);
int getOptLength (Card * card);
void createCardUI(char * fn,Card ** obj);

void createCardUI(char * fn,Card ** obj) {
    Card * card = malloc(sizeof(Card));
    //printf("works here\n");
    List * tempOptional = initializeList(&propertyToString,&deleteProperty,&compareProperties); //initialize blank property list;
    //printf("setup tempOptional\n");
    card->optionalProperties = tempOptional;//setup blank optional properties
    card->birthday = NULL;
    card->anniversary = NULL;
    //printf("setup optional Properties, birthday, anniversary\n");
    card->fn = malloc(sizeof(Property));
    char * fnName = malloc(sizeof(char)*100);
    char * fnGroup = malloc(sizeof(char)*100);
    char * fnVal = malloc(sizeof(char)*100);
    strcpy(fnVal,fn);
    strcpy(fnName,"FN");
    strcpy(fnGroup,"");
    //printf("setup fn name and fn group\n");
    card->fn->name = fnName;//setup fn property name
    card->fn->group = fnGroup;//setup fn property group
    List * tempParam = initializeList(&parameterToString,&deleteParameter,&compareParameters);//initialize blank parameter list
    List * tempValue = initializeList(&valueToString,&deleteValue,&compareValues);//initialize blank value list
    card->fn->parameters = tempParam;//setup blank fn parameters
    insertBack(tempValue,fnVal);//insert value from UI into value
    card->fn->values = tempValue;//link values to value list
    *obj = card;
}

void updateCardFn(Card * card, char * newVal) {
    if (card == NULL || newVal == NULL) {
        return;
    }
    strcpy(getFromFront(card->fn->values),newVal);
}

int getOptLength (Card * card) {
    if (card == NULL) {
        return -1;
    }
    else {
        return getLength(card->optionalProperties);
    }
}

VCardErrorCode validateCard(const Card* obj) {
    char * validProps[] = {"SOURCE", "KIND","XML","N","NICKNAME","PHOTO","GENDER","ADR","TEL","EMAIL",
        "IMPP","LANG","TZ","GEO","TITLE","ROLE","LOGO","ORG","MEMBER","RELATED","CATEGORIES","NOTE","PRODID","REV","SOUND","UID","CLIENTPIDMAP","URL",
        "KEY","FBURL","CALADURI","CALURI"};
    char propName[1000];
    void * elem;
    void * elem2;
    int sizeValidProps = sizeof(validProps)/sizeof(validProps[0]);
    int propCards[sizeValidProps];
    for (int i = 0; i < sizeValidProps; i++) {
        propCards[i] = 0;
    }
    ListIterator paramsIter;
    ListIterator optionalPropIter;
    if (obj == NULL) {
        //printf("OBJECT IS NULL\n");
        return INV_CARD;
    }
    Property * fullName = obj->fn;
    if (obj->fn != NULL) {
        strcpy(propName,fullName->name);
    }
    else {
        //printf("INVALID FN, FN IS NULL\n");
        return INV_CARD;
    }
    if (fullName->values == NULL || fullName->name == NULL || fullName->group == NULL || fullName->parameters == NULL) {
        //printf("VALUES WITHIN FN ARE NULL\n");
        return INV_PROP;
    }    
    
    for (int i = 0; propName[i] != '\0'; i++) {
        propName[i] = toupper(propName[i]);
    }

    if (strcmp(propName,"FN") != 0 || strcmp(propName,"") == 0) {
        //printf("INVALID FN, FN IS: %s\n", propName);
        return INV_CARD;
    }

    if (getLength(fullName->parameters) != 0) {
        paramsIter = createIterator(fullName->parameters);
        while ((elem = nextElement(&paramsIter)) != NULL) {//get the parameter each time and add it to a parameter string
            Parameter * curParam = (Parameter*)elem;
            if (curParam->value == NULL || curParam->name == NULL) {
                return INV_PROP;
            }
            if (strcmp(curParam->name,"") == 0) {
                //printf("INVALID PROP NAME, PROP NAME IS: %s\n", curParam->name);
                return INV_PROP;
            }
            if (strcmp(curParam->value,"") == 0) {
                //printf("INVALID PROP VALUE, PROP VAL IS: %s\n", curParam->name);
                return INV_PROP;
            }
        }
    }

    if (getLength(fullName->values) != 1) {//check cardinality
        return INV_PROP;
    }
    if (getLength(fullName->values) == 0) {
        //printf("NO EXISTING VALUES THAT ARE NOT EMPTY\n");
        return INV_PROP;
    }

    if (obj->optionalProperties == NULL) {
        //printf("OBJECT HAS NO EXISTING OPTIONAL PROPERTIES\n");
        return INV_CARD;
    }
    
    if (getLength(obj->optionalProperties) != 0) {
        optionalPropIter = createIterator(obj->optionalProperties);
        while ((elem = nextElement(&optionalPropIter)) != NULL) {
            Property * optionalProp = (Property*)elem;
            if (optionalProp->values == NULL || optionalProp->name == NULL || optionalProp->group == NULL || optionalProp->parameters == NULL) {
                //printf("OPTIONAL PROPERTY HAS A NULL VALUE WITHIN IT\n");
                return INV_PROP;
            }
            strcpy(propName,optionalProp->name);
            if (strcmp(optionalProp->name,"") == 0) {
                return INV_PROP;
            }
            int validPropIndex;
            validPropIndex = -1;
            for (int i = 0; propName[i] != '\0'; i++) {
                propName[i] = toupper(propName[i]);
            }
            if (strcmp(propName,"ANNIVERSARY") == 0 || strcmp(propName,"BDAY") == 0 ) {
                return INV_DT;
            }
            if (strcmp(propName, "VERSION") == 0) {
                return INV_CARD;
            }
            for (int i = 0; i < sizeValidProps; i++) {
                if (strcmp(propName,validProps[i]) == 0) {
                    validPropIndex = i;
                    propCards[i] += 1;
                }
            }
            if (validPropIndex == -1) {
                //printf("PROPERTY %s DOES NOT EXIST IN SPECIFCATIONS\n", propName);
                return INV_PROP;
            }
            if (validPropIndex == 1 || validPropIndex == 3 || validPropIndex == 6 || validPropIndex == 22 || validPropIndex == 23  || validPropIndex == 25) {
                if (propCards[validPropIndex] > 1) {
                    //printf("PROPERTY %s HAS VIOLATED CARDINALITY RULE IN SPECIFCATIONS (value in array = %s)\n", propName, validProps[validPropIndex]);
                    return INV_PROP;
                }
            }
            if (validPropIndex != 3 && validPropIndex != 7 && validPropIndex != 26 &&  validPropIndex != 6 &&  validPropIndex != 0 
                &&  validPropIndex != 4 &&  validPropIndex != 17 &&  validPropIndex != 20 && validPropIndex != 8) {
                if (getLength(optionalProp->values) != 1) {
                    //printf("PROPERTY %s HAS MORE THAN ONE VALUE AND IS NOT CONSIDERED A SPECIAL PROPERTY\n", optionalProp->name);
                    return INV_PROP;//any other values break value number rule
                }

            }
            else {
                if (validPropIndex == 3 && getLength(optionalProp->values) != 5) {
                    //printf("N does not have 5 values\n");
                    return INV_PROP;
                }
                if (validPropIndex == 7 && getLength(optionalProp->values) != 7) {
                    //printf("ADR does not have 7 values\n");
                    return INV_PROP;
                }
                if (validPropIndex == 26 && getLength(optionalProp->values) != 2 ) {
                    //printf("CLIENTPIDMAP does not have 2 values\n");
                    return INV_PROP;
                }
    
                if (validPropIndex == 6 && (getLength(optionalProp->values) != 2 && getLength(optionalProp->values) != 1) ) {
                    //printf("GENDER does not have 1 or 2 values\n");
                    return INV_PROP;
                }
            }
            
            if (getLength(optionalProp->parameters) != 0) {
                paramsIter = createIterator(optionalProp->parameters);
                while ((elem2 = nextElement(&paramsIter)) != NULL) {//get the parameter each time and add it to a parameter string
                    Parameter * curParam = (Parameter*)elem2;
                    if (curParam->name == NULL || curParam->value == NULL) {
                        return INV_PROP;
                    }
                    if (strcmp(curParam->name,"") == 0) {
                        //printf("INVALID PROP NAME, PROP NAME IS: %s\n", curParam->name);
                        return INV_PROP;
                    }
                    if (strcmp(curParam->value,"") == 0) {
                        //printf("INVALID PROP VALUE, PROP VAL IS: %s\n", curParam->name);
                        return INV_PROP;
                    }
                }
                elem2 = NULL;
            }
            if (getLength(optionalProp->values) == 0) {
                //printf("NO EXISTING VALUES THAT ARE NOT EMPTY\n");
                return INV_PROP;
            }
            


        }
    }
    if (obj->birthday != NULL || obj->anniversary != NULL) {
        DateTime * date;
        if (obj->birthday != NULL) {
            date = obj->birthday;
        }
        else {
            date = obj->anniversary;
        }

        if (date->isText == true) {
            if (strcmp(date->time,"") != 0 || strcmp(date->date,"") != 0 ) {
                return INV_DT;
            }
            if (date->UTC == true) {
                return INV_DT;
            }
        }
        else {
            char dateTest[1000];
            if (strcmp(date->text,"")  != 0 ) {
                return INV_DT;
            } 
            strcpy(dateTest,date->time);
            if (strlen(dateTest) == 6) {
                for (int i = 0; dateTest[i] != '\0';i++) {
                    if ((!isdigit(dateTest[i]) && dateTest[i] != '-')) {
                        return INV_DT;
                    }
                }
            }
            else if (strcmp(dateTest,"") != 0){
                return INV_DT;
            }
            
            strcpy(dateTest,date->date);
            if (strlen(dateTest) == 8) {
                for (int i = 0; dateTest[i] != '\0';i++) {
                    if (!isdigit(dateTest[i])) {
                        return INV_DT;
                    }
                }
            }
            else if (strcmp(dateTest,"") != 0){
                return INV_DT;
            }
        }
    }

    return OK;

}

VCardErrorCode writeCard(const char* fileName, const Card* obj) {
    FILE * testPtr;
    void * elem;
    //void * elem2;
    ListIterator paramsIter;
    ListIterator valuesIter;
    ListIterator optionalPropIter;
    if (obj == NULL) {
        return WRITE_ERROR;
    }
    testPtr = fopen(fileName,"w");
    if (testPtr == NULL) {
        //printf("invalid file\n");
        return WRITE_ERROR;
    }
    
    char * lineString = malloc(sizeof(char)*1000); 
    strcpy(lineString,"BEGIN:VCARD\r\n");
    fprintf(testPtr, "%s", lineString);
    strcpy(lineString,"");
    
    strcpy(lineString,"VERSION:4.0\r\n");
    fprintf(testPtr, "%s", lineString);
    strcpy(lineString,"");

    Property * fullName = obj->fn;
    if (strcmp(fullName->group,"") != 0) {//check if fn has a group
        //printf("fn has a group\n");
        strcat(lineString,fullName->group);//add group
        strcat(lineString,".");
    }    
    strcat(lineString,fullName->name); //copy property name
    if (getLength(fullName->parameters)!= 0) {
        //printf("fn has a parameter\n");
        paramsIter = createIterator(fullName->parameters);
        while ((elem = nextElement(&paramsIter)) != NULL) {//get the parameter each time and add it to a parameter string
            Parameter * curParam = (Parameter*)elem;
            strcat(lineString,";");
            strcat(lineString,curParam->name);
            //printf("Currently putting in param value %s\n", curParam->name);
            strcat(lineString,"=");
            strcat(lineString,curParam->value);
        }
        //strcat(lineString,fullName->name);
    }
    elem = NULL;
    strcat(lineString,":");
    if (getLength(fullName->values)!= 0) {
        valuesIter = createIterator(fullName->values);
        elem = nextElement(&valuesIter);
        while (elem != NULL) {//get the parameter each time and add it to a parameter string
            char * curVal = (char*)elem;
            strcat(lineString,curVal);
            if ((elem = nextElement(&valuesIter)) != NULL) {
                strcat(lineString,";");
            }
        }
        elem = NULL;
        
    }
    strcat(lineString,"\r\n");
    //printf("\nFN FOR CARD:\n%s", lineString);
    fprintf(testPtr, "%s", lineString);
    strcpy(lineString,"");
    


    //-----------------------------------------------optional properties--------------------------------
    if (getLength(obj->optionalProperties) != 0) {
        //printf("optional properties exist\n");
        //printf("entering while loop\n");
        optionalPropIter = createIterator(obj->optionalProperties);
        while ((elem = nextElement(&optionalPropIter)) != NULL) {
            Property * optionalProp = (Property*)elem;
            //printf("optional properties group exist\n");
            if (strcmp(optionalProp->group,"") != 0) {//check if fn has a group
                strcat(lineString,optionalProp->group);//add group
                strcat(lineString,".");
            }    
            strcat(lineString,optionalProp->name); //copy property name
            //printf("copied optional properties name %s\n", lineString);
            if (getLength(optionalProp->parameters)!= 0) {
                paramsIter = createIterator(optionalProp->parameters);
                while ((elem = nextElement(&paramsIter)) != NULL) {//get the parameter each time and add it to a parameter string
                    Parameter * curParam = (Parameter*)elem;
                    strcat(lineString,";");
                    strcat(lineString,curParam->name);
                    //printf("Currently putting in param value %s\n", curParam->name);
                    strcat(lineString,"=");
                    strcat(lineString,curParam->value);
                }
                //strcat(lineString,fullName->name);
                elem = NULL;
            }
           
            strcat(lineString,":");
            if (getLength(optionalProp->values)!= 0) {
                valuesIter = createIterator(optionalProp->values);
                elem = nextElement(&valuesIter);
                while (elem != NULL) {//get the parameter each time and add it to a parameter string
                    char * curVal = (char*)elem;
                    strcat(lineString,curVal);
                    if ((elem = nextElement(&valuesIter)) != NULL) {
                        strcat(lineString,";");
                    }
                }
                elem = NULL;
                
            }
            strcat(lineString,"\r\n");
            //printf("\nOPTIONAL PROPERTY FOR CARD:\n%s", lineString);
            fprintf(testPtr, "%s", lineString);
            strcpy(lineString,"");
        }
        
    }
    //-----------------------------------------------date time properties--------------------------------
    if (obj->birthday != NULL) {
        DateTime * birthday = obj->birthday; 
        strcat(lineString,"BDAY"); //copy property name
        if (birthday->isText) {
            strcat(lineString,";VALUE=text");
            strcat(lineString,":");
            strcat(lineString,birthday->text);
        }
        else {
            strcat(lineString,":");
            strcat(lineString,birthday->date);
            strcat(lineString,"T");
            strcat(lineString,birthday->time);
        }
        
        if (birthday->UTC) {
            strcat(lineString,"Z");
        }
        strcat(lineString,"\r\n");
        //printf("\nBIRTHDAY PROPERTY FOR CARD:\n%s", lineString);
        fprintf(testPtr, "%s", lineString);
        strcpy(lineString,"");
        
    }
    if (obj->anniversary != NULL) {
        DateTime * anniversary = obj->anniversary; 
        strcat(lineString,"ANNIVERSARY"); //copy property name
        if (anniversary->isText) {
            strcat(lineString,";VALUE=text");
            strcat(lineString,":");
            strcat(lineString,anniversary->text);
        }
        else {
            strcat(lineString,":");
            strcat(lineString,anniversary->date);
            strcat(lineString,"T");
            strcat(lineString,anniversary->time);
        }
        
        if (anniversary->UTC) {
            strcat(lineString,"Z");
        }
        strcat(lineString,"\r\n");
        //printf("\nANNIVERSARY PROPERTY FOR CARD:\n%s", lineString);
        fprintf(testPtr, "%s", lineString);
        strcpy(lineString,"");
        
    }

    strcpy(lineString,"END:VCARD\r\n");
    fprintf(testPtr, "%s", lineString);
    strcpy(lineString,"");

    free(lineString);
    fclose(testPtr);
    
    
    return OK;
}


VCardErrorCode createCard(char* fileName, Card** obj) {
    FILE * testPtr;
    testPtr = fopen(fileName,"r");
    if (testPtr == NULL) {        
        return INV_FILE;
    }
    fseek(testPtr,0,SEEK_END);
    if (ftell(testPtr) == 0) {
        fclose(testPtr);
        return INV_FILE;
    }
    rewind(testPtr);
    //printf("CREATE CARD IS CALLED, FILE NAME IS %s\n", fileName);
    char * tempString = malloc(sizeof(char)*100);
    char * tempStringNextLine = malloc(sizeof(char)*100);
    char * propertyType = malloc(sizeof(char)*100);
    char * values = malloc(sizeof(char)*100);
    bool createdFN = false;
    bool createdAnni = false;
    bool createdBday = false;
    int versionCount = 0;
    if (tempString == NULL || tempStringNextLine == NULL || propertyType == NULL || values == NULL) {
        return OTHER_ERROR;
    }
    List * tempParam = NULL;
    List * tempValue = NULL;
    List * tempOptional = initializeList(&propertyToString,&deleteProperty,&compareProperties); //initialize blank property list;
    Property * tempProperty = NULL;
    Card * card = malloc(sizeof(Card));
    //setting anniversary and birthday to null by default, and also creating empty lists for card
    card->anniversary = NULL;
    card->birthday = NULL;
    card->optionalProperties = tempOptional;
    card->fn = NULL;

    
    fgets(tempString,sizeof(char)*100,testPtr);
    if (ftell(testPtr) == 0 || strstr(tempString,"BEGIN") == NULL) {
        //printf("test\n");
        free(propertyType);
        free(values);
        free(tempString);
        free(tempStringNextLine);
        deleteCard(card);
        fclose(testPtr);
        return INV_CARD;
    }
    do {

        fgets(tempString,sizeof(char)*100,testPtr);
        
        //printf("LINE IS READ AS %s\n",tempString);
        int val = strlen(tempString);
        if (tempString[val-2] != '\r' || tempString[val-1] != '\n') {
            //printf("ERROR invalid crlf\n");
            free(tempString);
            free(tempStringNextLine);
            free(propertyType);
            free(values);
            deleteCard(card);
            fclose(testPtr);
            return INV_CARD;
        }
        tempString[val-2] = '\0';
        //printf("%stest\n",tempString);
        if (tempString == NULL) {
            return INV_FILE;
        }

        if (strcmp(tempString,"END:VCARD") != 0 && !feof(testPtr)) {
            fgets(tempStringNextLine,sizeof(char)*100,testPtr);
            if (feof(testPtr)) {
                //printf("no end!\n");
                free(tempString);
                free(tempStringNextLine);
                free(propertyType);
                free(values);
                deleteCard(card);
                fclose(testPtr);
                return INV_CARD;
            }
        }
        
        int count = 0;
        do {
            val = strlen(tempStringNextLine);
            tempStringNextLine[val-2] = '\0';
            if (tempStringNextLine[0] == ' ' || tempStringNextLine[0] == '\t') {
                count += 75;
                char *temp = realloc(tempString, sizeof(char)*(100+count));//reallocing with a temp just in case it is null
                if (temp != NULL) {
                    tempString = temp;
                }
                memmove(tempStringNextLine,tempStringNextLine+1,strlen(tempStringNextLine));//copies a string shifted by one to get rid of space
                strcat(tempString,tempStringNextLine);
                fgets(tempStringNextLine,sizeof(char)*100,testPtr);
                //printf("NEW LINE: %s", tempStringNextLine);
            }
            if ((tempStringNextLine[0] != ' ' && tempStringNextLine[0] != '\t')) {
                int currentPos = ftell(testPtr);
                int counter = 0;
                if (currentPos > 0) {
                    do {
                        currentPos--;
                        fseek(testPtr,currentPos,SEEK_SET);
                        counter++;
                        //printf("MOVING BACK\n");
                    }while ((fgetc(testPtr) != '\n' || counter <= 1) && currentPos > 0);
                    fseek(testPtr, currentPos + 1, SEEK_SET);
                }
            }

        } while(tempStringNextLine[0] == ' ' && !feof(testPtr));
        char * firstSemiColon = NULL;
        firstSemiColon = strchr(tempString,':');
        if (firstSemiColon == NULL) {
            //printf("Invalid file format! No semicolon on string %s\n", tempString);
            free(tempString);
            free(tempStringNextLine);
            free(propertyType);
            free(values);
            deleteCard(card);
            fclose(testPtr);
            return INV_PROP;
        }
        if (firstSemiColon == &tempString[0] || firstSemiColon == &tempString[strlen(tempString)-1]) {
            //printf("Invalid file format! Semicolon appears at start/end %s\n", tempString);
            free(tempString);
            free(tempStringNextLine);
            free(propertyType);
            free(values);
            deleteCard(card);
            fclose(testPtr);
            return INV_PROP;
        }
        int semiColonLength = firstSemiColon - tempString;
        //printf("ATTEMPTING TO GET PROPERTY TYPE AND VALUE\n");
        strncpy(propertyType,tempString,semiColonLength);
        //printf("SUCCESSFUL STRING COPY\n");
        propertyType[semiColonLength] = '\0';
        //printf("PROPERTY TYPE: %s\n", propertyType);
        strncpy(values,firstSemiColon+1,sizeof(char)*100);
        values[strlen(values)] = '\0';
        //printf("PROPERTY TYPE: %s\n", propertyType);
        //printf("PROPERTY VALUE: %s\n", values);
        int length = strlen(propertyType);

        char dummyProp[length + 1];

        for (int i = 0; i < length; i++) {
            dummyProp[i] = tolower(propertyType[i]);
        }
        dummyProp[length] = '\0';
        if (strcmp(dummyProp,"version") == 0) {
            versionCount ++;
        }
        if ((strcmp(dummyProp,"begin") == 0 && strcmp(values,"VCARD") != 0) || (strcmp(dummyProp,"version") == 0 && strcmp(values,"4.0") != 0)) {
            //printf("Invalid file format! Begin is incorrect.\n");
            //printf("Invalid file format!\n");
            free(tempString);
            free(tempStringNextLine);
            free(propertyType);
            free(values);
            deleteCard(card);
            fclose(testPtr);
            return INV_CARD;
        }

        if (strcmp(dummyProp,"begin") != 0 && strcmp(dummyProp,"version") != 0 && strcmp(dummyProp,"end") != 0 && (strstr(dummyProp,"anniversary") == NULL 
        || createdAnni == true) && (strstr(dummyProp,"bday") == NULL || createdBday == true)) {
            tempProperty = malloc(sizeof(Property));
            tempProperty->name = malloc(sizeof(char)*100);
            tempProperty->group = malloc(sizeof(char)*100);
            strcpy(tempProperty->group,"");//to initialize group for now
            strcpy(tempProperty->name,"");//to initialize name for now
            tempProperty->parameters = NULL;
            tempProperty->values = NULL;
            tempParam = initializeList(&parameterToString,&deleteParameter,&compareParameters);//initialize blank parameter list
            tempValue = initializeList(&valueToString,&deleteValue,&compareValues);//initialize blank value list
            
            char * mainProperty = malloc(sizeof(char)*100);//main property name 
            strcpy(mainProperty,propertyType);//holds propertyType name for now and will change through further parsing
            //duplicate property so string isnt lost
            char * duplicateProperty = malloc(sizeof(char)*100);
            strcpy(duplicateProperty,propertyType);
            if (strchr(duplicateProperty,';')) {
                //printf("property has parameter!, duplicate property string: %s\n", duplicateProperty);
                char * params = strtok(duplicateProperty,";");
                //printf("property name = %s\n",params);
                Parameter * paramsTemp;
                char * paramString;
                
                strcpy(mainProperty,params);
                params = strtok(NULL,";");//move onto the actual parameter
                //printf("Parameter = %s\n",params);
                do {
                    paramsTemp = malloc(sizeof(Parameter));//main parameter "node"
                    paramString = malloc(sizeof(char)*100); //string to hold parameter string
                    if (paramString != NULL) {
                        strcpy(paramString,params);
                    }
                    char * valParam = malloc(sizeof(char)*100);
                    char * nameParam = malloc(sizeof(char)*100);
                    if (valParam == NULL || nameParam == NULL) {
                        return OTHER_ERROR;
                    }
                    if (paramString[strlen(paramString) -1] == '=') {
                        free(valParam);
                        free(nameParam);
                        free(paramString);
                        free(tempString);
                        free(tempStringNextLine);
                        free(propertyType);
                        free(values);
                        free(mainProperty);
                        free(paramsTemp);
                        free(duplicateProperty);
                        deleteProperty(tempProperty);
                        deleteCard(card);
                        if (tempParam != NULL) {
                            freeList(tempParam);
                        }
                        if (tempValue != NULL) {
                            freeList(tempValue);
                        }
                        fclose(testPtr);
                        return INV_PROP;
                    }
                    char * splitter = NULL;
                    splitter = strchr(params,'=');
                    if (splitter == NULL) {
                        //printf("Invalid parameter format!\n");
                        free(valParam);
                        free(nameParam);
                        free(paramString);
                        free(tempString);
                        free(tempStringNextLine);
                        free(propertyType);
                        free(values);
                        free(mainProperty);
                        free(paramsTemp);
                        free(duplicateProperty);
                        deleteProperty(tempProperty);
                        deleteCard(card);
                        if (tempParam != NULL) {
                            freeList(tempParam);
                        }
                        if (tempValue != NULL) {
                            freeList(tempValue);
                        }
                        fclose(testPtr);
                        return INV_PROP;
                    }
                    //printf("parameter name: %s\n", nameParam);
                    int equalLength = splitter - params;
                    strncpy(nameParam,params,equalLength);
                    //printf("SUCCESSFUL STRING COPY\n");
                    nameParam[equalLength] = '\0';
                    //printf("PROPERTY TYPE: %s\n", propertyType);
                    strncpy(valParam,splitter+1,sizeof(char)*100);
                    values[strlen(values)] = '\0';
                    //printf("parameter value: %s\n", valParam);
                    //printf("new value: %s\n", valTemp);
                    paramsTemp->name = nameParam;
                    //printf("name inserted into temp param\n");
                    paramsTemp->value = valParam;
                    //printf("value inserted into temp param\n");
                    //printf("name = %s, value = %s\n", paramsTemp->name,paramsTemp->value);
                    insertBack(tempParam,paramsTemp);
                    //Parameter * holder = getFromFront(tempProperty->parameters);
                    //printf("successful insert into list name: %s, value: %s\n", holder->name, holder->value);
                    params = strtok(NULL,";");
                    
                    free(paramString);
                }while (params != NULL);
                //strcpy(tempProperty->name,mainProperty);
                //printf("left loop, tempProperty name = %s\n",tempProperty->name);
            }
            strcpy(duplicateProperty,mainProperty);            
            if (strchr(duplicateProperty,'.')) {
                char * group = strtok(duplicateProperty,".");
                //printf("property name = %s\n",params);
                char *  groupTemp = malloc(sizeof(char)*100);
                if (groupTemp == NULL) {
                    return OTHER_ERROR;
                }
                strcpy(groupTemp,group);
                group = strtok(NULL,".");
                if (group == NULL) {
                    free(tempString);
                    free(tempStringNextLine);
                    free(propertyType);
                    free(values);
                    free(mainProperty);
                    free(groupTemp);
                    free(duplicateProperty);
                    deleteProperty(tempProperty);
                    deleteCard(card);
                    if (tempParam != NULL) {
                        freeList(tempParam);
                    }
                    if (tempValue != NULL) {
                        freeList(tempValue);
                    }
                    fclose(testPtr);
                    return INV_PROP;
                }
                strcpy(mainProperty,group);//copies value after group which is the actual property
                strcpy(tempProperty->group,groupTemp);
                free(groupTemp);
            }
            strcpy(tempProperty->name,mainProperty);
            tempProperty->parameters = tempParam;
            free(duplicateProperty);
            free(mainProperty);
            //printf("value = %s\n", values);
            if (strchr(values,';')) {
                //char * valGroup = strtok(values,";");
                char * valTemp;
                char * lastChar;
                char * begin = values;
                do {
                    lastChar = strchr(begin,';');
                    valTemp = malloc(sizeof(char)*100);//so that values doesnt get overwritten each loop
                    if (valTemp == NULL) {
                        return OTHER_ERROR;
                    }
                    if (valTemp != NULL) {
                        if (lastChar != NULL) {
                            strncpy(valTemp,begin,lastChar-begin);
                            valTemp[lastChar-begin] = '\0';
                            begin = lastChar + 1;
                        }
                        else {
                            strcpy(valTemp,begin);
                        }
                        
                    }
                    //printf("%s\n", valTemp);
                    
                    //printf("new value: %s\n", valTemp);
                    insertBack(tempValue,valTemp);
                    tempProperty->values = tempValue;
                    //valGroup = strtok(NULL,";");
                }while (lastChar != NULL);
            }
            else {
                char * valTemp = malloc(sizeof(char)*100);//so that values doesnt get overwritten each loop
                if (valTemp == NULL) {
                    return OTHER_ERROR;
                }
                strcpy(valTemp,values);
                insertBack(tempValue,valTemp);
                tempProperty->values = tempValue;
                
            }
            char * dummyName = malloc(sizeof(char)*100);
            strcpy(dummyName,tempProperty->name);
            for (int i = 0; i < strlen(dummyName); i++) {
                dummyName[i] = tolower(dummyName[i]);
            }
            if (strcmp(dummyName,"fn") == 0 && createdFN == false) {
                card->fn = tempProperty;
                createdFN = true;
                free(dummyName);
                //printf("FN SET");
            }
            else {
                //printf("Inserted into optional properties\n");
                insertBack(tempOptional,tempProperty);
                card->optionalProperties = tempOptional;
                free(dummyName);
            }

        }
        else if ((strstr(dummyProp,"anniversary") != NULL && createdAnni == false) || (strstr(dummyProp,"bday") != NULL && createdBday == false)) {
            //printf("is a date\n");
            if ((createdBday == true && strstr(dummyProp,"bday") != NULL) || (createdAnni == true && strstr(dummyProp,"anniversary") != NULL)) {
                //printf("parameter error\n");
                free(tempString);
                free(tempStringNextLine);
                free(propertyType);
                free(values);
                deleteCard(card);
                fclose(testPtr);
                return INV_CARD;
            }
            DateTime * tempDate = malloc(sizeof(DateTime));
            char * paramName = malloc(sizeof(char)*100);
            char * main = malloc(sizeof(char)*100);//holds property name
            char * value = malloc(sizeof(char)*100);
            char * date = malloc(sizeof(char)*100);
            char * time = malloc(sizeof(char)*100);
            if (paramName == NULL || main == NULL || value == NULL || date == NULL || time == NULL) {
                return OTHER_ERROR;
            }
            char * splitter;
            tempDate->UTC = false;
            tempDate->isText = false;
            strcpy(date,"");    
            strcpy(time,"");
            strcpy(main,propertyType);
            //printf("copied stuffs\n");
            if (strchr(propertyType,';')) {
                //printf("parameter exists\n");
                char * duplicateProperty = malloc(sizeof(char)*100);
                if (duplicateProperty == NULL) {
                    return OTHER_ERROR;
                }
                strcpy(duplicateProperty,propertyType);
                //printf("%s\n", duplicateProperty);
                splitter = strtok(duplicateProperty,";");
                strcpy(main,splitter);
                //printf("main name is %s\n", main);
                splitter = strtok(NULL,";");
                do {
                    strcpy(paramName,splitter);
                    //printf("param name is %s\n", paramName);
                    if (strstr(paramName,"VALUE")) {
                        //printf("is in value\n");
                        char * val = NULL;
                        val = strtok(paramName,"=");
                        val = strtok(NULL,"=");
                        if (val == NULL) {
                            //printf("parameter error in date\n");
                            free(tempString);
                            free(tempStringNextLine);
                            free(propertyType);
                            free(main);
                            free(values);
                            free(value);
                            free(paramName);
                            free(date);
                            free(time);
                            free(duplicateProperty);
                            free(tempDate);
                            deleteCard(card);
                            fclose(testPtr);
                            return INV_DT;
                        }
                        strcpy(value,val);
                        if (strcmp(value,"text")==0) {
                            tempDate->isText = true;
                        }
                        else {
                            tempDate->isText = false;
                        }
                    }
                    //printf("iterating\n");
                    splitter = strtok(NULL,";");

                } while (splitter != NULL);
                //printf("left loop\n");
                
                //printf("anniversary param is %s\n", paramName);
                free(duplicateProperty);
            }
            if (tempDate->isText == true) {
                strcpy(value,values);
                tempDate->text = value;
                
                tempDate->date = date;
                tempDate->time = time;


            }
            else {
                char * empText = malloc(sizeof(char)*100);
                strcpy(empText,"");
                tempDate->text = empText;
                strcpy(value,values);
                if (strchr(value,'T')) {
                    if (value[0] == 'T') {
                        char * dateSplit = strtok(value,"T");
                        strcpy(time,dateSplit);
                        int length = strlen(time);
                        if (time[length-1] == 'Z') {
                            tempDate->UTC = true;
                            time[length-1] = '\0';
                        }
                    }
                    else {
                        char * dateSplit = strtok(value,"T");
                        strcpy(date,dateSplit);
                        dateSplit = strtok(NULL,"T");
                        if (dateSplit != NULL) {
                            strcpy(time,dateSplit);
                            int length = strlen(time);
                            if (time[length-1] == 'Z') {
                                tempDate->UTC = true;
                                time[length-1] = '\0';
                            }
                        }
                    }
                }
                else {
                    strcpy(date,values);
                }
                
                free(value);
                //free(paramName);
                //free(main);
            }
            free(paramName); 
            tempDate->time = time;
            tempDate->date = date;
            for (int i = 0; i < strlen(main); i++) {
                main[i] = tolower(main[i]);
            }
            if (strcmp(main,"anniversary") == 0) {
                card->anniversary = tempDate;
                createdAnni = true;
            }
            else if (strcmp(main,"bday") == 0) {
                card->birthday = tempDate;
                createdBday = true;
            }
            free(main);
        }
        else if (strcmp(dummyProp, "end") == 0) {
            strcpy(propertyType,"END");
        }
        
    }while (strcmp(propertyType,"END") != 0 && !feof(testPtr));
    if (strcmp(propertyType,"END") != 0) {
        //printf("Invalid file format! End is not correct.\n");
        free(tempString);
        free(tempStringNextLine);
        free(propertyType);
        free(values);
        deleteCard(card);
        fclose(testPtr);
        return INV_CARD;
    }
    if (versionCount == 0) {
        free(tempString);
        free(tempStringNextLine);
        free(propertyType);
        free(values);
        deleteCard(card);
        fclose(testPtr);
        return INV_CARD;
    }
    
    //printf("reached end\n");
    *obj = card;
    //printf("successfully set card\n");
    //printf("CARD FN: %s", tempProperty->name);
    free(tempString);
    free(tempStringNextLine);
    free(propertyType);
    free(values);
    
    fclose(testPtr);
    return OK;

}
void deleteCard(Card* obj) {
    if (obj == NULL) {
        return;
    }
    if (obj->fn != NULL) {
        deleteProperty(obj->fn);
        obj->fn = NULL;
    }
    if (obj->optionalProperties != NULL) {
        freeList(obj->optionalProperties);
        obj->optionalProperties = NULL;
    }
    if (obj->anniversary != NULL) {
        //printf("deleting date ani\n");
        deleteDate(obj->anniversary);
        obj->anniversary = NULL;
    }
    if (obj->birthday != NULL) {
        //printf("deleting date bday\n");
        deleteDate(obj->birthday);
        obj->birthday = NULL;
    }
    
    free(obj);
}

char* cardToString(const Card* obj){
    char * string = malloc(sizeof(char)*1000);
    strcpy(string,"");
    if (obj == NULL) {
        return string;
    }
    void * elem;
    char * stringOptProperty = malloc(sizeof(char)*1000);
    char * stringBday = NULL;
    char * stringAnniversary = NULL;
    ListIterator optPropIter;
    char * nameString = propertyToString(obj->fn);
    stringOptProperty[0] = '\0';
    if (getLength(obj->optionalProperties) != 0) { 
        //printf("there are optional properties\n");       
        optPropIter = createIterator(obj->optionalProperties);
        int count = 0;
        while ((elem = nextElement(&optPropIter)) != NULL) {//get the parameter each time and add it to a parameter string
            char * optStr = propertyToString(elem);
            count += 100;
            //printf("count = %d\n",count);
            char * tempReAlloc = realloc(stringOptProperty,sizeof(char)*(1000+count));
            if (tempReAlloc != NULL) {
                stringOptProperty = tempReAlloc;
            }
            strcat(stringOptProperty,optStr);
            free(optStr);
        }
    }
    if (obj->birthday != NULL) {
        stringBday = dateToString(obj->birthday);
    }
    else {
        stringBday = malloc(sizeof(char)*100);
        strcpy(stringBday,"N/A");
    }
    if (obj->anniversary != NULL) {
        stringAnniversary = dateToString(obj->anniversary);
    }
    else {
        stringAnniversary = malloc(sizeof(char)*100);
        strcpy(stringAnniversary,"N/A");
    }
    //printf("attempting to re alloc string\n");
    int stringSize = strlen(nameString) + strlen(stringAnniversary) + strlen(stringBday) + strlen(stringOptProperty);
    char * stringReAlloc = realloc(string,sizeof(char)*stringSize + sizeof(char)*1000);
    if (stringReAlloc != NULL) {
        string = stringReAlloc;
    }
    //printf("attempting to create string\n");
    snprintf(string,stringSize + 1000,"FN:%sOptional properties:%s\nBirthday: %s\nAnniversary: %s\n", nameString, stringOptProperty,stringBday,stringAnniversary);
    free(stringBday);
    free(stringAnniversary);
    free(stringOptProperty);
    free(nameString);
    return string;

}

char* errorToString(VCardErrorCode err){
    char * string = malloc(sizeof(char)*100);

    switch (err) {
        case INV_CARD:
            strcpy(string,"Invalid card format!\n");
            break;
        case INV_PROP:
            strcpy(string,"Invalid property format!\n");
            break;
        case INV_FILE:
            strcpy(string,"Invalid file entered!\n");
            break;
        case OK:
            strcpy(string,"Valid file.\n");
            break;
        case INV_DT: 
            strcpy(string,"Invalid date time\n");
            break;
        default:
            strcpy(string, "Error entered is invalid\n");
            break;
    }
    return string;
}


void deleteProperty(void* toBeDeleted) {
    if (toBeDeleted == NULL) {
        return;
    }
    Property * tempProperty = (Property*)toBeDeleted;
    //printf("FREEING PROPERTY NAMED %s\n", tempProperty->name);
    free(tempProperty->name);
    tempProperty->name = NULL;
    free(tempProperty->group);
    tempProperty->group = NULL;
    freeList(tempProperty->parameters);
    tempProperty->parameters = NULL;
    freeList(tempProperty->values);
    tempProperty->values = NULL;
    free(tempProperty);  
}

int compareProperties(const void* first,const void* second) {
    return 0;
}

char* propertyToString(void* prop) {
    Property * property = (Property*)prop;
    char* string = malloc(sizeof(char)*1000);
    if (prop == NULL) {
        strcpy(string,"");
        return string;
    }
    void * elem;
    void * elem2;
    char * stringParams = malloc(sizeof(char)*1000);
    char * stringValues = malloc(sizeof(char)*1000);
    ListIterator valuesIter = createIterator(property->values);
    ListIterator paramsIter = createIterator(property->parameters);
    //printf("\nvalue = %s\n\n", (char*)getFromFront(property->values));
    stringParams[0] = '\0';
    stringValues[0] = '\0';
    if (getLength(property->parameters) != 0) {
        
        while ((elem = nextElement(&paramsIter)) != NULL) {//get the parameter each time and add it to a parameter string
            char * paramStr = parameterToString((Parameter*)elem);
            strcat(stringParams,paramStr);
            strcat(stringParams, ",");
            free(paramStr);
        }
    }
    elem = NULL;
    
    while ((elem2 = nextElement(&valuesIter)) != NULL) {//get the value each time and add it to a value string
        char * valuesStr = valueToString(elem2);
        strcat(stringValues,valuesStr);
        strcat(stringValues, ",");
        free(valuesStr);
    }
    if (stringValues[strlen(stringValues)-1] == ',') {
        stringValues[strlen(stringValues)-1] = '\0';
    }
    if (strlen(stringParams) > 0 && stringParams[strlen(stringParams)-1] == ',') {
        stringParams[strlen(stringParams)-1] = '\0';
    }
    snprintf(string,1000, "Property Name: %s, Values: %s, Parameters: %s, Group: %s,\n", property->name, stringValues, stringParams,property->group);
    free(stringValues);
    free(stringParams);
    return string;
}

void deleteParameter(void* toBeDeleted){
    Parameter * temp = (Parameter*)toBeDeleted;
    if (toBeDeleted == NULL) {
        return;
    }
    free(temp->name);
    temp->name = NULL;
    free(temp->value);
    temp->value = NULL;
    free(temp);
}
int compareParameters(const void* first,const void* second) {
    return 0;
}
char* parameterToString(void* param) {
    Parameter * parameter = (Parameter*)param;
    char * string = malloc(sizeof(char)*200);
    if (param == NULL) {
        strcpy(string,"");
    }
    else {
        snprintf(string, 200, "%s = %s", parameter->name, parameter->value);
    }
    
    return string;
}

void deleteValue(void* toBeDeleted) {
    if (toBeDeleted == NULL) {
        return;
    }
    free(toBeDeleted);
}

int compareValues(const void* first,const void* second) {
    return 0;
}
char* valueToString(void* val) {
    char * string = malloc(sizeof(char)*200);
    if (val == NULL) {
        strcpy(string,"");
    }
    else {
        snprintf(string, 200, "%s", (char*)val);
    }
    
    return string;
}

void deleteDate(void* toBeDeleted) {
    DateTime * tempDate = (DateTime*)toBeDeleted;
    if (toBeDeleted == NULL) {
        return;
    }
    free(tempDate->date);
    tempDate->date = NULL;
    free(tempDate->time);
    tempDate->time = NULL;
    free(tempDate->text);
    tempDate->text = NULL;
    free(tempDate);
}
int compareDates(const void* first,const void* second) {
    return 0;
}
char* dateToString(void* date) {
    DateTime * obj = (DateTime*)date;
    char * string = malloc(sizeof(char)*1000);
    char * strUTC = malloc(sizeof(char)*20);
    if (date == NULL) {
        strcpy(string,"");
        free(strUTC);
        return string;
    }
    if (obj->UTC == true) {
        strcpy(strUTC,"true");
    }
    else {
        strcpy(strUTC,"false");
    }
    if (obj->isText) {
        snprintf(string,1000, "UTC: %s, Time: %s\n", strUTC, obj->text);
    }
    else {
        snprintf(string,1000, "UTC: %s, Date: %s, Time: %s\n", strUTC, obj->date,obj->time);
    }
    free(strUTC);
    return string;
}