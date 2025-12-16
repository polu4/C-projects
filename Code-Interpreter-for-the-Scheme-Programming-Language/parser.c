#include "parser.h"
#include "linkedlist.h"
#include "talloc.h"
#include "stdio.h"
#include <string.h>
#include "tokenizer.h"

// This function prints the parse tree to the screen in a readable fashion that looks just like scheme
// input all on one line.
void printTree(Item *tree){
    
    Item *carItem = tree;

    if(carItem -> type == INT_TYPE){
        printf("%i", carItem -> i);
    }

    else if(carItem -> type == DOUBLE_TYPE){
        printf("%lf", carItem -> d);
    }

    else if(carItem -> type == STR_TYPE){
        printf("%s", carItem -> s);
    }

    else if(carItem -> type == BOOL_TYPE){
        if(carItem -> i == 1){
                    printf("#t");
                    
                }

                else{
                    printf("#f");
                    
                }
    }

    else if(carItem -> type == SYMBOL_TYPE){
        printf("%s ", carItem -> s);
    }

    else if (carItem -> type == NULL_TYPE){
                printf("()");
            }

    else{

   
        if(carItem -> type == CONS_TYPE){ 
            printf("(");
            while(carItem -> type == CONS_TYPE){
            printTree(car(carItem));
             if(carItem-> type == CONS_TYPE){
                     carItem = cdr(carItem);
                     if(carItem -> type == CONS_TYPE){
                         printf(" ");
                    }
                     if(carItem -> type != CONS_TYPE && carItem -> type != NULL_TYPE){
                        printf(" . ");
                        printTree(carItem);
                        
            }
            
             }
            
        }
        printf(")");
        
        
        if(carItem -> type == CONS_TYPE){
            printf(" ");
        }
    }

    }
}


// This function takes a list of tokens from a Scheme program, and returns a pointer to a
// parse tree representing that program.
Item *parse(Item *tokens){
    Item *parseTree = makeNull();
    int openParensToClose = 0;
    while(tokens->type == CONS_TYPE){ 
        Item *carItem = car(tokens);
    
        switch (carItem -> type){
        
        case (OPEN_TYPE):{
            parseTree = cons(carItem, parseTree);
            openParensToClose++;
            break;
        }

        case (OPENBRACKET_TYPE):{
            parseTree = cons(carItem, parseTree);
            openParensToClose++;
            break;
        }

        case (INT_TYPE):{
            parseTree = cons(carItem, parseTree);
            break;
        }

        case (DOUBLE_TYPE):{
            parseTree = cons(carItem, parseTree);
            break;
        }

        case (STR_TYPE):{
            parseTree = cons(carItem, parseTree);
            break;
        }
   
        case (BOOL_TYPE):{
            parseTree = cons(carItem, parseTree);
            break;
        }

        case (SYMBOL_TYPE):{
            parseTree = cons(carItem, parseTree);
            break;
        }

        case (CLOSE_TYPE):{
            Item *list = makeNull();
            openParensToClose--;
            if(openParensToClose < 0){
                printf("Syntax error: too many close parentheses.\n");
                texit(0);
            }
            else{
                Item *carItem2 = car(parseTree);
                
                while(carItem2->type != OPEN_TYPE && carItem2 -> type != OPENBRACKET_TYPE && parseTree -> type != NULL_TYPE){
                    
                    list = cons(carItem2, list);
                    parseTree = cdr(parseTree);
                    carItem2 = car(parseTree);
                                             
                }

            
                if(car(parseTree) -> type == OPENBRACKET_TYPE){
                    printf("Syntax error: mismatched parenthesis with a bracket.\n");
                    texit(0);
                }
                else{
                    parseTree = cdr(parseTree); //To get rid of the opening parenthesis or bracket
                    parseTree = cons(list, parseTree);
                    break;
                }
            }
            }
        case (CLOSEBRACKET_TYPE):{
            Item *list = makeNull();
            openParensToClose--;
            if(openParensToClose < 0){
                printf("Syntax error: too many close parentheses.\n");
                texit(0);
            }
            else{
                Item *carItem2 = car(parseTree);
                while(carItem2 -> type != OPEN_TYPE && carItem2 -> type != OPENBRACKET_TYPE && parseTree -> type != NULL_TYPE){
                    list = cons(carItem2, list);
                    parseTree = cdr(parseTree);
                    carItem2 = car(parseTree); 
                    
                
            }
                if(car(parseTree) -> type == OPEN_TYPE){
                    printf("Syntax error: mismatched bracket with a parenthesis.\n");
                    texit(0);
                }
                else{
                    parseTree = cdr(parseTree); //To get rid of the opening parenthesis or bracket item
                    parseTree = cons(list, parseTree);
                    break;
                }
            }
            }
        default:
            break;

        }
        tokens = cdr(tokens);

    }
    if(openParensToClose > 0){
        printf("Syntax error: not enough close parentheses.\n");
        texit(0);
    }
    Item *reverseParseTree = reverse(parseTree); //Parse Tree in the correct order
    return reverseParseTree;
}


