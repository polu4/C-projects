#include "tokenizer.h"
#include "linkedlist.h"
#include "talloc.h"
#include "stdio.h"
#include <string.h>


// Helper method for tokenize that is called to determine if a token is an int, double, or syntax error,
// and creates the appropriate type of item or returns a syntax error.
Item *createIntegerOrDouble(char charRead){
    char *str = talloc(300);
    int i = 0;
    while(charRead != EOF && (charRead >= '0' && charRead <= '9' || charRead == '.') || charRead == '-'){
        str[i] = charRead;
        i++;
        charRead = (char)fgetc(stdin);
       
    }
    ungetc(charRead, stdin);
    str[i] = '\0';
    
    
    int decimalCounter = 0;
    for(int j = 0; j < i; j++){
        if(str[j] == '.'){
            decimalCounter++;
        }
    }
        
        if(decimalCounter >= 2){  //A double cannot have more than one decimal point in it
            printf("Syntax error\n");
            texit(0);
        }
        

    Item *newItem = makeNull();
    char *dot = ".";
        //if double
        if(strstr(str, dot) != NULL){
            double doub;
            sscanf(str, "%lf", &doub);
            newItem -> type = DOUBLE_TYPE;
            newItem -> d = doub;
        }    

        //if integer{
        else{
            int it = atoi(str); 
            newItem -> type = INT_TYPE;
            newItem -> i = it;
        }

        
        return newItem;

    
}

// Helper method for tokenize that reads in a symbol to a character array (string), creates a SYMBOL_TYPE
// item, and returns it.
Item *readInSymbol(char charRead){
    char *symbol = talloc(300);
    int i = 0;
    while(charRead != ' ' && charRead != EOF && charRead != '(' && charRead != ')' && charRead != '[' && charRead != ']' && charRead != ';' && charRead != '\n'){
            symbol[i] = charRead;
            i++;
            charRead = (char)fgetc(stdin);
            
    }
    ungetc(charRead, stdin);
    symbol[i] = '\0';

    Item *symbolItem = makeNull();
    symbolItem -> type = SYMBOL_TYPE;
    symbolItem -> s = symbol;

    
    return symbolItem;
}



// This function reads all of the input from stdin, and return a linked list consisting of the
// tokens.
Item *tokenize(){

    char charRead;
    Item *list = makeNull();
    charRead = (char)fgetc(stdin);
    
    while (charRead != EOF) {
        
        if(charRead >= '0' && charRead <= '9' || charRead == '.'){  //Int or Double
            Item *newItem = createIntegerOrDouble(charRead);
            list = cons(newItem, list);
        }

        else if(charRead == '-'){   // negative int or double or a symbol starting with a minus sign
            charRead = (char)fgetc(stdin);
            if((charRead >= '0' && charRead <= '9') || charRead == '.'){
                ungetc(charRead, stdin);
                charRead = '-';
                Item *newItem = createIntegerOrDouble(charRead);
                list = cons(newItem, list);
            }
            else{
                ungetc(charRead, stdin);
                charRead = '-';
                Item *symbolItem = readInSymbol(charRead);
                list = cons(symbolItem, list); 
            }
        }
       
        else if(charRead == '"'){    //String
            char *str = talloc(300);
            str[0] = '"';
            charRead = (char)fgetc(stdin);

            
            int i = 1;
            while(charRead != '"'){ 
                
                str[i] = charRead;
                i++;
                charRead = (char)fgetc(stdin);
                
                
            }
            str[i] = charRead;
            i++;
            str[i] = '\0';
            

            Item *stringItem = makeNull();
            stringItem -> type = STR_TYPE;
            stringItem -> s = str; 

            list = cons(stringItem, list);
            
        }

    else if(charRead == '#'){          //boolean
        Item *boolItem = makeNull();
        boolItem -> type = BOOL_TYPE;
    
        charRead = (char)fgetc(stdin);
        if(charRead == 't'){
        boolItem -> i = 1;
        list = cons(boolItem, list);
        
        }

        else if(charRead == 'f'){
            boolItem -> i = 0;
            list = cons(boolItem, list);
        }
        

        else{
            printf("Syntax error\n");
            texit(0);
        }
    }

    

    else if(charRead == '('){             //Open parenthesis
        Item *openItem = makeNull();
        openItem -> type = OPEN_TYPE;
        list = cons(openItem, list);
         
    }
        

    //Symbol
    else if(charRead >= 'A' && charRead <= 'Z' ||  charRead >= 'a' && charRead <= 'z' || charRead == '!'   
            ||charRead == '$'
            || charRead == '%' || charRead == '&' || charRead == '*' || charRead == '/' || charRead == ':'
            || charRead == '<' || charRead == '=' || charRead == '>' || charRead == '?' || charRead == '~'
            || charRead == '_' || charRead == '^' || charRead == '+'){
                
            

        




            Item *symbolItem = readInSymbol(charRead);
            list = cons(symbolItem, list);
                
            }

    // else if(charRead == '+'){
    //     Item *primitiveItem = makeNull();
    //     primitive
    // }
    

    else if(charRead == ')'){                //Closing Parenthesis
        Item *closeItem = makeNull();
        closeItem -> type = CLOSE_TYPE;
        list = cons(closeItem, list);

    }

    else if(charRead == '['){                //Open Bracket
        Item *openBracketItem = makeNull();
        openBracketItem -> type = OPENBRACKET_TYPE;
        list = cons(openBracketItem, list);

    }

    else if(charRead == ']'){                //Closed Bracket
        Item *closeBracketItem = makeNull();
        closeBracketItem -> type = CLOSEBRACKET_TYPE;
        list = cons(closeBracketItem, list);

    }

    else if(charRead == ';'){              //Comments
        while(charRead != (char)EOF && charRead != '\n'){
            
            charRead = (char)fgetc(stdin);  
        }

    }

    else if(charRead == ' '){   //whitespace
         while(charRead == ' '){
            
            charRead = (char)fgetc(stdin); 
        }
        ungetc(charRead, stdin); 
    }


    
    else if(charRead == '\n'){

    }

    else{
        printf("Syntax error\n");
        texit(0);
    }

    
         charRead = (char)fgetc(stdin);
    }


    Item *reverseList = reverse(list);
    return reverseList;
    
}


// Displays the contents of the linked list as tokens, with type information
void displayTokens(Item *list){
    
    while(list -> type == CONS_TYPE){
        Item *carItem = car(list);
        switch ((*carItem).type){

            case (INT_TYPE):
                printf("%i:integer\n", carItem -> i); 
                break;

            case DOUBLE_TYPE:
                printf("%lf:double\n", carItem -> d);
                break;

            case STR_TYPE:
                printf("%s:string\n", carItem -> s);
                //free(carItem -> s); 
                break;

            case BOOL_TYPE:
                if(carItem -> i == 1){
                    printf("#t:boolean\n");
                    break;
                }

                else{
                    printf("#f:boolean\n");
                    break;
                }

            case SYMBOL_TYPE:
                printf("%s:symbol\n", carItem -> s);
                break;

            case OPEN_TYPE:
                printf("(:open\n");
                break;
            
            case CLOSE_TYPE:
                printf("):close\n");
                break;

            case OPENBRACKET_TYPE:
                printf("[:openbracket\n");
                break;

            case CLOSEBRACKET_TYPE:
                printf("]:closebracket\n");
                break;
            
            default:
                break;
        }
        list = cdr(list);
    }
    tfree();
}