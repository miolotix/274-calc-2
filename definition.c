#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definition.h"

// function definitions


int pop(Stack* stack) { //removes head and frees it

    if (stack -> count == 0) return 0; //empty

    Level* top = stack -> head;
    stack->head = stack -> head -> next;
    free(top);

    stack -> count--;
    return 1;
}

void


void push(Stack* s, int in_type, long in_long, double in_double,
    const char* in_string) {

    //add new item to stack, parameter = new
    //pash rest back one

    Level* newLevel = malloc(sizeof(Level));

    newLevel -> type = in_type; // 0 = long, 1 = double, 2 = string
    newLevel -> longvalue = in_long;
    newLevel -> doublevalue = in_double;
    if (in_string != NULL)
        strcpy(newLevel->stringvalue, in_string);
    else
        newLevel->stringvalue[0] = '\0';

    newLevel -> next = s->head;
    s->head = newLevel;
    s->count++;

    if (s->count > 5) { //drop bottom of stack
        Level* current = s->head;
        while (current -> next != NULL) {
            current = current->next;
        }
        free(current); //free bottom of stack
        current = NULL;
        s->count--; //count back to 5
    }
}

Level* get_level(Stack* s, int n) {

    Level* current = s->head;
    while (--n > 0){ current = current->next; }
    return current;

}

void printStack(Stack* stack) {

    if (stack -> count == 0) {
        printf("Empty Stack\n");
    }

    for(int i = stack->count; i >= 1; i--){
        
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

            default:
                printf("Stack type error\n");

        }//switch end

    }//for end

}

void gatherInput(char* input, int size) {
    char line[33];

    printf("# ");
    fgets(input, size, stdin);

    ssize_t len = strcspn(input, "\n");

    if (input[0] == '\0') {

        printf("Bad Input\n");

    }

}

