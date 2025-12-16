#include "interpreter.h"
#include "parser.h"
#include "linkedlist.h"
#include "talloc.h"
#include "stdio.h"
#include <string.h>
#include "tokenizer.h"
#include "item.h"
#include "stdlib.h"

int a = 0;

//Helper function that takes in a symbol and evaluates it in the current frame.
//If the symbol is not bound in the current frame, then the function is called on the parent frame.
//If the symbol is not bound in the current frame or any of its ancestors, then an evaluation error is returned.
Item *evalSymbol(Item *carItem, Frame *frame){
    Item *result = makeNull();
     Item *bindings = frame -> bindings;
    while(bindings -> type == CONS_TYPE){
            Item *carItemBindings = car(bindings);
            if(carItemBindings -> type == SYMBOL_TYPE){
                if(!strcmp(carItem -> s, carItemBindings -> s)){
                    return car(cdr(bindings));

            }
           
    }
   
        bindings = cdr(bindings);
    
    }

    if(frame -> parent == NULL){
            printf("Evaluation error: symbol is not bound in the current frame or any of its ancestors\n");
            texit(0);
         }
    return evalSymbol(carItem, frame -> parent);
}
    



// This function applies the evaluation of a function: a closure, to the evaluated arguments in 
// a new frame. This allows for functions using lambda to be evaluated.
Item *apply(Item *function, Item *args){
    Item *result = makeNull();
    if(function -> type == PRIMITIVE_TYPE){ //If dealing with a primitive type
        result = function -> pf(args);
        return result;
    }
    if(function -> type == CLOSURE_TYPE){ //If dealing with a function that evaluates to a closure
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame -> parent = (*function).cl.frame;
    newFrame -> bindings = makeNull();
    Item *evaluatedArgs = args;
    Item *argNames = (*function).cl.paramNames;
    Item *functionBody = (*function).cl.functionCode;
    
    if(argNames -> type != CONS_TYPE){
            newFrame -> bindings = cons(evaluatedArgs, newFrame -> bindings);
            newFrame -> bindings = cons(argNames, newFrame -> bindings);
            
        
        

    }
    



else{
    if(length(argNames) != length(evaluatedArgs)){
        printf("Evaluation error: Function application where number of actual parameters does not equal number of formal parameters.\n");
        texit(0);
    }
    while(argNames -> type != NULL_TYPE){
        while(evaluatedArgs -> type != NULL_TYPE){
            newFrame -> bindings = cons(car(evaluatedArgs), newFrame -> bindings);
            newFrame -> bindings = cons(car(argNames), newFrame -> bindings);
            
            
            evaluatedArgs = cdr(evaluatedArgs);
            argNames = cdr(argNames);
    }

    }

}

    while(functionBody -> type != NULL_TYPE){
         result = eval(car(functionBody), newFrame);
         functionBody = cdr(functionBody);
    }
    
    return result;
    }
    
        printf("Evaluation error: function is not a primitive or closure type.\n");
        texit(0);
    
    return result;

}

// This is a helper function that evaluates the formal parameters (the args) of a closure in the
// frame passed in.
Item *evalArgs(Item *args, Frame *frame){
    Item *argsEvaluated = makeNull();
    while(args -> type != NULL_TYPE){
        argsEvaluated = cons(eval(car(args), frame), argsEvaluated);
        if(a != 1){

        
        args = cdr(args);
        }
    }
    argsEvaluated = reverse(argsEvaluated);
    return argsEvaluated;
}


// This function is the C version of the primitive addition function in Scheme.
Item *primitiveAddition(Item *args){
    Item *argsP = args;
    while(argsP -> type != NULL_TYPE){
        if(car(argsP) -> type != INT_TYPE && car(argsP) -> type != DOUBLE_TYPE){
            printf("Evaluation error: + must take all numbers as arguments.\n");
            texit(0);
        }
        argsP = cdr(argsP);
    }
    Item *result = talloc(sizeof(Item));
    int isDouble = 0;
    Item *argsPointer = args;
    while(argsPointer -> type != NULL_TYPE){
        if(car(argsPointer) -> type == DOUBLE_TYPE){
            isDouble++;
        }
        argsPointer = cdr(argsPointer);
    }

    if(isDouble){
        result -> type = DOUBLE_TYPE;
        result -> d = 0.0;
        while(args -> type != NULL_TYPE){
            if(car(args) -> type == DOUBLE_TYPE){
               result -> d = result -> d + car(args) -> d;
            }
            if(car(args) -> type == INT_TYPE){
                result -> d = result -> d + car(args) -> i;
            }
            args = cdr(args);
        }

    }
    else{
        result -> type = INT_TYPE;
        result -> i = 0;
        while(args -> type != NULL_TYPE){
            result -> i = result -> i + car(args) -> i;
            args = cdr(args);
        }
    }

    return result;
}

//This function is the C version of the primitive null? function in Scheme.
Item *primitiveIsNull(Item *args){
    Item *result = talloc(sizeof(Item));
    result -> type = BOOL_TYPE;

    if(args -> type == NULL_TYPE){
        printf("Evaluation error: no arguments are given to null?.\n");
        texit(0);
    }

    if(length(args) > 1){
        printf("Evaluation error: null? is given more than one argument.\n");
        texit(0);
    }

    if(args -> type == CONS_TYPE){

        if(cdr(args) -> type != NULL_TYPE){
        printf("Evaluation error: null? is given more than one argument.\n");
        texit(0);
    }

        if(car(args) -> type == NULL_TYPE){
            result -> i = 1;
            return result;
        }
    }

    

    if(args -> type == NULL_TYPE){
        result -> i = 1;
        return result;
    }
    else{
        result -> i = 0;
        return result;
    }

}

// This function is the C version of the primitive car function in Scheme.
Item *primitiveCar(Item *args){
    Item *result = makeNull();
    if(args -> type == NULL_TYPE){
        printf("Evaluation error: no arguments are given to car.\n");
        texit(0);
    }

    



    if(args -> type == CONS_TYPE){
        

        if(car(args) -> type != CONS_TYPE || cdr(args) -> type != NULL_TYPE){
            printf("Evaluation error: car takes a pair.\n");
            texit(0);
        }

        if(cdr(args) -> type != NULL_TYPE){
            printf("Evaluation error: car is given more than one argument.\n");
            texit(0);
        }
    }


    result = car(car(args));
    return result;

}


//This function is the C version of the primitive cdr function in Scheme.
Item *primitiveCdr(Item *args){
    Item *result = makeNull();
    if(args -> type == NULL_TYPE){
        printf("Evaluation error: no arguments are given to cdr.\n");
        texit(0);
    }

    if(length(args) > 1){
        printf("Evaluation error: cdr is given more than one argument.\n");
        texit(0);
    }

    if(args -> type == CONS_TYPE){

        if(cdr(args) -> type != NULL_TYPE){
        printf("Evaluation error: cdr is given more than one argument.\n");
        texit(0);
        }
    }

    result = cdr(car(args));
    return result;
}

//This function is the C version of the primitive cons function in Scheme;
Item *primitiveCons(Item *args) {
    Item *result = makeNull();

    if (args -> type == NULL_TYPE) {
        printf("Evaluation error: no arguments are given to cons.\n");
        texit(0);
    }

    if (length(args) != 2) {
        printf("Evaluation error: cons requires exactly two arguments.\n");
        texit(0);
    }

    Item *first = car(args);
    Item *second = car(cdr(args));


    result = cons(first, second);
    return result;
}

// This function is the C version of the primitive append version in Scheme that only accepts two arguments.
Item *primitiveAppend(Item *args) {
    Item *result = makeNull();

    if (args == NULL) {
        printf("Evaluation error: no arguments are given to append.\n");
        texit(0);
    }

    if (length(args) != 2) {
        printf("Evaluation error: append requires exactly two arguments.\n");
        texit(0);
    }

    Item *list1 = car(args);
    Item *item2 = car(cdr(args));

    if (list1->type != NULL_TYPE && list1->type != CONS_TYPE) {
        printf("Evaluation error: first argument to append must be a list.\n");
        texit(0);
    }


    if (list1->type == NULL_TYPE) {
        result = item2;
    } 
    else {
        result = cons(car(list1), primitiveAppend(cons(cdr(list1), cons(item2, makeNull()))));
    }

    return result;
}

// This function binds the name of a primitive function to the function itself in the frame passed in.
// This is called on the top level frame in interpret for each primitive function that we have
// written, so that the primitive functions may be accessed when needed.
void bindPrimitiveFunction(char *name, struct Item *(*function)(Item *), Frame *frame) {
   
    Item *nameItem = talloc(sizeof(Item));
    nameItem -> type = SYMBOL_TYPE;  
    nameItem -> s = name;
    Item *primitiveItem = talloc(sizeof(Item));
    primitiveItem -> type = PRIMITIVE_TYPE;
    primitiveItem -> pf = function;
    frame -> bindings = cons(primitiveItem, frame -> bindings);
    frame -> bindings = cons(nameItem, frame -> bindings);
}



// Takes in a parse tree of one s-expression in a frame and returns the value of the s-expression
// Currently the only special forms that can be evaluated are define, lambda, if, let, and quote
Item *eval(Item *tree, Frame *frame){
    int isPrimitive = 0;
    Item *result = makeNull();
    Item *carItem = tree;
    Item *argsEvaluated = talloc(sizeof(Item));
    switch (carItem -> type){ 
     case (INT_TYPE): {                
        return carItem;
        break;
     }
     case (DOUBLE_TYPE): {
        Item *doubleItem = makeNull();
        return carItem;
        break;
     }
     case (STR_TYPE): {
        return carItem;
        break;
     }

     case (BOOL_TYPE): {
        return carItem;
        break;
     } 
     case (SYMBOL_TYPE): {  // We only need to change result when we come across an if, let, or quote, the
     //symbols are not going to be functions in this part
        Item *symbolItem = evalSymbol(carItem, frame);
        result = symbolItem;
        return result;
        break;
    
     }  

     case (CONS_TYPE): {
        Item *first = car(tree);
        
        int open = 1;
        Item *args = cdr(tree);
        
        if (!strcmp(first->s,"if")) {
            if(length(args) < 3){
                printf("Evaluation error: if expression has fewer than three arguments\n");
                texit(0);
            }
            int boolean;
            boolean = eval(car(args), frame) -> i;
            if(eval(car(args), frame) -> type != BOOL_TYPE){
                printf("Evaluation error: condition of an if expression evaluates to a non-boolean\n");
                texit(0);
            }
            if(boolean != 0){
                result = eval(car(cdr(args)), frame);
                return result;
            }
            else{
                result = eval(car(cdr(cdr(args))), frame);
                return result;
            }

        }

        else if (!strcmp(first->s,"let")){ 


            if(length(args) < 2){
                printf("Evaluation error: no args following the bindings in let.\n");
                texit(0);
            }
            Frame *newFrame = talloc(sizeof(Frame));
            Item *letargs = car(args);
            newFrame -> parent = frame;
            newFrame -> bindings = makeNull();
            if(letargs -> type != CONS_TYPE && letargs -> type != NULL_TYPE){
                printf("Evaluation error: list of bindings for let is not a nested list\n");
                texit(0);
    
            }
            else{
                if(letargs -> type != NULL_TYPE && car(letargs) -> type != CONS_TYPE){ 
                    printf("Evaluation error: list of bindings for let is not a nested list\n");
                    texit(0);
                }
            }

            
            while(letargs -> type != NULL_TYPE){
            Item *symbolItem = car(car(letargs));
            
            Item *value = eval(car(cdr(car(letargs))), frame);
            Item *temp  = newFrame -> bindings;
            int i = 0;
            while(temp -> type != NULL_TYPE){
                if(i%2 == 0){
                
                if(!strcmp(symbolItem -> s, car(temp) -> s)){
                    printf("Evaluation error: trying to bind same symbol more than once.\n");
                    texit(0);
                }
                }
                i++;
                temp = cdr(temp);
            }

            newFrame -> bindings = cons(value, newFrame -> bindings);
            if(symbolItem -> type != SYMBOL_TYPE){
                printf("Evaluation error: thing being bound is not a symbol\n");
                texit(0);
            }
            newFrame -> bindings = cons(symbolItem, newFrame -> bindings);
            letargs = cdr(letargs);
            }
            Item *coulder = cdr(args);
            while(cdr(coulder) -> type != NULL_TYPE){
                coulder = cdr(coulder);
                
            }
            result = eval(car(coulder), newFrame);
            return result;
            
        }
        


        else if (!strcmp(first->s,"quote")){

            if(args -> type == NULL_TYPE){ 
                printf("Evaluation error: quote with no arguments\n");
                texit(0);
            }
            if(cdr(args) -> type != NULL_TYPE){
                printf("Evaluation error: quote with multiple arguments\n");
                texit(0);
            }
            result = car(args);
            return result;
        }

        else if(!strcmp(first -> s, "define")){
            
            if(args -> type == NULL_TYPE){
                printf("Evaluation error: no args following define\n");
                texit(0);
            }
            if(cdr(args) -> type == NULL_TYPE){
                printf("Evaluation error: no value following the symbol in define.\n");
                texit(0);
            }
            if(car(args) -> type != SYMBOL_TYPE){
                printf("Evaluation error: define must bind to a symbol.\n");
                texit(0);
            }

        


                Item *expressionValue = eval(car(cdr(args)), frame); 
                frame -> bindings = cons(expressionValue, frame -> bindings);
                Item *var = car(args);
                frame -> bindings = cons(var, frame -> bindings);

                if(frame -> bindings -> type != CONS_TYPE && frame -> bindings -> type != NULL_TYPE){

                }
                
                result -> type = VOID_TYPE;
                return result;
                break;
            

            break;

        }

        else if(!strcmp(first -> s, "lambda")){

            Item *newItem = talloc(sizeof(Item));
            newItem -> type = CLOSURE_TYPE; 
            (*newItem).cl.paramNames = makeNull();


        

            if(args -> type == NULL_TYPE){
                printf("Evaluation error: no args following lambda.\n");
                texit(0);
            }

            if(cdr(args) -> type == NULL_TYPE){
                    printf("Evaluation error: no code in lambda following parameters.\n");
                    texit(0);
                }

                
                if(car(args) -> type == CONS_TYPE){
                    Item *carArgs = car(args);

                if(car(car(args)) -> type != SYMBOL_TYPE){
                    printf("Evaluation error: formal parameters for lambda must be symbols.\n");
                    texit(0);
                }
                
            while(carArgs -> type != NULL_TYPE){
                    while(cdr(carArgs) -> type != NULL_TYPE){
                        if(!strcmp(car(carArgs) -> s, car(cdr(carArgs)) -> s)){
                            printf("Evaluation error: duplicate identifier in lambda.\n");
                            texit(0);
                        }
                        carArgs = cdr(carArgs);
                    }
                break;
            }

                }

            (*newItem).cl.functionCode = cdr(args);
            (*newItem).cl.frame = frame;

        

            result -> type = CLOSURE_TYPE;
            result = newItem;

        

                (*result).cl.paramNames = car(args);
                

                Item *carArgs = car(args);
            
                  
                if(cdr(args) -> type == NULL_TYPE){
                    printf("Evaluation error: no code in lambda following parameters.\n");
                    texit(0);
                }


                (*result).cl.functionCode = (cdr(args));
                (*result).cl.frame = frame;
                
                return result;
                break;
            
        }


        else {
           
        

            Item *argsEvaluated = evalArgs(args, frame);



            Item *function = talloc(sizeof(Item));
            function = eval(first, frame); 
            result = apply(function, argsEvaluated);
           

            return result;
            break;
        
        }
        break;
     
     }

     default:
        break;

    }
      carItem = cdr(carItem);
    return result;
    }
    

// Creates a top level frame, calls eval on each s-expression in a parse tree, and prints out what each s-expression evaluates to.    
void interpret(Item *tree){
    Item *result = makeNull();
    Frame *frame = talloc(sizeof(Frame));
    frame -> parent = NULL;
    frame -> bindings = makeNull();
    
    bindPrimitiveFunction("+", primitiveAddition, frame);
    bindPrimitiveFunction("null?", primitiveIsNull, frame);
    bindPrimitiveFunction("car", primitiveCar, frame);
    bindPrimitiveFunction("cdr", primitiveCdr, frame);
    bindPrimitiveFunction("cons", primitiveCons, frame);
    bindPrimitiveFunction("append", primitiveAppend, frame);
    
    
    while(tree -> type != NULL_TYPE){
        result = eval(car(tree), frame);  //evaluate the first s-expression in the trees
        printTree(result);
        if(result -> type != VOID_TYPE){
        printf("\n");
        
    }
       
        tree = cdr(tree);
        
        }
       
}
