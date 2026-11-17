#include <stdio.c>
#include "definition.h"

// function definitions

Level* push(Level* head, int in_type, long in_long, double in_double,
        char* in_string) {

    Level* newLevel = (Level*)malloc(sizeof(Level));

    newLevel -> type = in_type;
    newLevel -> longvalue = in_long;
    newLevel -> doublevalue = in_double;
    newLevel -> stringvalue = in_string;

    newLevel -> next = head;

    return newLevel;

}

int pop(Stack* stack) { //removes head and frees it

    if (stack -> count == 0) return 0; //empty

    Level* top = stack -> head;
    free(top -> stringvalue);
    free(top);
    stack -> count--;
    return 1;

}

Level* get_level(Stack* s, int n) {

    Level* current = s->head;
    while (--n > 0){ current = current->next; }
    return current;

}

void printStack(Stack* stack) {

    for(i = stack->count; i <= 1; i--){
        
        printf("%d: ", i);

        Level* lev = get_level(stack, i);

        switch(lev -> type){

            case 0: //long
                printf("%ld\n", lev->longvalue);
                break;

            case 1: //double
                printf("%g\n", lev->doublevalue);
                break;

            case 2: //string
                printf("%s\n", lev->stringvalue);
                break;

        }//switch end

    }//for end

}
