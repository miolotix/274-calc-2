#include <stdio.h>
#include "definition.h"

int main(){

    // print stack
    // prompt user input "# "
    // store input as head on the stack, push the rest
    // remove end of stack if applicable

    Level* head = NULL;
    Stack s = {NULL, 0};
    Stack* stack = &s;

    printStack(stack);

    push(stack, TYPE_DOUBLE,0, 1.01, NULL);
    push(stack, TYPE_STRING, 0, 0.0, "hello world");
    push(stack, TYPE_LONG, 5, 0.0, NULL);
    push(stack, TYPE_LONG, 72, 0.0, NULL);
    push(stack, TYPE_STRING, 0, 0.0, "butt");

    printStack(stack);
    printf("\n");

    push(stack, TYPE_LONG, 5, 0.0, NULL);

    printStack(stack);


}
;