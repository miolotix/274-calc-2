#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "definition.h"

int main(){

    Stack s = {NULL, 0};
    Stack* stack = &s;

    logFile = fopen("prog5_log.txt", "w");
    if (logFile == NULL) {
        printf("Could not open log file: %s\n", strerror(errno));
        return 1;
    }

    printStack(stack);
    while (1) { // loops forever
        gatherInput(stack);
        printStack(stack);
    }

}
