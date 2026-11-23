#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "definition.h"

// function definitions

FILE* logFile;

void outputText(const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);                 // to the screen
    va_end(args);

    if (logFile != NULL) {
        va_start(args, fmt);            // restart the arg list for the second use
        vfprintf(logFile, fmt, args);   // to the file
        va_end(args);
        fflush(logFile);                // keep the file current if the program is killed
    }
}

int pop(Stack* stack) { //removes head and frees it

    if (stack -> count == 0) {
        outputText("Insufficient Arguments\n");
        return 0;
    } //empty

    Level* top = stack -> head;
    stack->head = stack -> head -> next;
    free(top);

    stack -> count--;
    return 1;
}

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

    if (s->count > 5) {       // drop the bottom of the stack
        Level* current = s->head;
        while (current->next->next != NULL)     // stop at second-to-last
            current = current->next;

        free(current->next);                    // free the bottom node
        current->next = NULL;                   // new bottom ends the list
        s->count--;
    }
}

Level* get_level(Stack* s, int n) {

    Level* current = s->head;
    while (--n > 0) {
        current = current->next;
    }
    return current;

}

void printStack(Stack* stack) {

    if (stack -> count == 0) {
        outputText("Empty Stack\n");
    }

    for(int i = stack->count; i >= 1; i--){
        
        outputText("%d: ", i);

        Level* lev = get_level(stack, i);

        switch(lev -> type){

            case 0: //long
                outputText("%ld\n", lev->longvalue);
                break;

            case 1: //double
                outputText("%g\n", lev->doublevalue);
                break;

            case 2: //string
                outputText("%s\n", lev->stringvalue);
                break;

            default:
                outputText("Stack type error\n");

        }//switch end

    }//for end

}

int classify_str(const char* str) {
    int i = 0;
    size_t digits = 0;
    size_t dots = 0;

    if (str[i] == '-' && strlen(str) == 1) {
        return TYPE_STRING;
    }
    if (str[i] == '-') { i++; }

    for (; str[i] != '\0'; i++) {
        if (isdigit((unsigned char)str[i])) {digits++;}
        else if (str[i] == '.') {dots++;}
        else { return TYPE_STRING; }
    }

    if (digits == 0) return TYPE_BAD;
    if (dots == 0) return TYPE_LONG;
    if (dots == 1) return TYPE_DOUBLE;
    if (dots > 1 && digits == strlen(str) - dots) return TYPE_BAD;
    return TYPE_STRING;
}

int processInput(char* input) { //return type value

    input[strcspn(input, "\n")] = '\0';

    int kind = classify_str(input);

    if (kind == TYPE_LONG) {
        strtol(input, NULL, 10);
        return TYPE_LONG;
    }
    if (kind == TYPE_DOUBLE) {
        strtod(input, NULL);
        return TYPE_DOUBLE;
    }
    if (kind == TYPE_STRING) {
        return TYPE_STRING;
    }

    return TYPE_BAD;
}

int validateString(char* word) {
    int result = 1;
    for(int i = 0; word[i] != '\0'; i++) {
        if(!islower((unsigned char)word[i])) {
            result = 0;
        }
    }
    return result;
}

int isOperation(const char* input) { // 1 = is operator

    if (strlen(input) != 1) {
        return 0;
    }
    if (strchr("+-*/drq", input[0]) != NULL) { //searches for any valid operators
        return 1;
    }

    return 0;
}

void roll(Stack* stack) {
    if (stack->count == 0) {
        outputText("Insufficient Arguments\n");
    }
    if (stack->count == 1) {
        return;
    }
    Level* current = stack->head;
    while (current->next->next != NULL)
        current = current->next;

    Level* bottom = current->next;
    current->next = NULL;

    bottom->next = stack->head;
    stack->head = bottom;
}

double asDouble(const Level* level) { //returns long value as double
    if (level->type == TYPE_LONG) {
        return (double)level->longvalue;
    }
    return level->doublevalue;

}

void node_to_text(const Level* level, char* out, size_t size) {
    if (level->type == TYPE_LONG) {
        snprintf(out, size, "%ld", level->longvalue);
    }
    else if (level->type == TYPE_DOUBLE) {
        snprintf(out, size, "%g", level->doublevalue);
    }
    else {
        snprintf(out, size, "%s", level->stringvalue);
    }
}

void add(Stack* stack) {

    if (stack->count < 2) {
        outputText("Insufficient Arguments\n");
        return;
    }

    Level* head = stack->head;
    Level* next = head->next;

    if (head->type != TYPE_STRING && next->type != TYPE_STRING) { //both numbers
        if (head->type == TYPE_LONG && next->type == TYPE_LONG) { //both longs
            long result = head->longvalue + next->longvalue;
            pop(stack);
            pop(stack);
            push(stack, TYPE_LONG, result, 0.0, NULL);

        }
        else { //some mix of long and double, or just double
            double result = asDouble(head) + asDouble(next);
            pop(stack);
            pop(stack);
            push(stack, TYPE_DOUBLE, 0, result, NULL);
        }

    }

    else {
        // at least one string: concatenate, level 2 on the left, level 1 on the right
        char left[64], right[64], joined[130];
        node_to_text(next, left, sizeof left);
        node_to_text(head, right, sizeof right);
        snprintf(joined, sizeof joined, "%s%s", left, right);

        pop(stack);
        pop(stack);
        push(stack, TYPE_STRING, 0, 0.0, joined);
    }
}

void subtract(Stack* stack) {

    if (stack->count < 2) {
        outputText("Insufficient Arguments\n");
        return;
    }

    Level* head = stack->head;
    Level* next = head->next;

    if (head->type != TYPE_STRING && next->type != TYPE_STRING) { //both numbers
        if (head->type == TYPE_LONG && next->type == TYPE_LONG) { //both longs
            long result = next->longvalue - head->longvalue;
            pop(stack);
            pop(stack);
            push(stack, TYPE_LONG, result, 0.0, NULL);

        }
        else { //some mix of long and double, or just double
            double result = asDouble(next) - asDouble(head);
            pop(stack);
            pop(stack);
            push(stack, TYPE_DOUBLE, 0, result, NULL);
        }

    }

    else {
        // at least one string
        outputText("Bad Arguments\n");
    }
}

void multiply(Stack* stack) {

    if (stack->count < 2) {
        outputText("Insufficient Arguments\n");
        return;
    }

    Level* head = stack->head;
    Level* next = head->next;

    if (head->type != TYPE_STRING && next->type != TYPE_STRING) { //both numbers
        if (head->type == TYPE_LONG && next->type == TYPE_LONG) { //both longs
            long result = next->longvalue * head->longvalue;
            pop(stack);
            pop(stack);
            push(stack, TYPE_LONG, result, 0.0, NULL);

        }
        else { //some mix of long and double, or just double
            double result = asDouble(next) * asDouble(head);
            pop(stack);
            pop(stack);
            push(stack, TYPE_DOUBLE, 0, result, NULL);
        }
    }
    else {
        // at least one string
        outputText("Bad Arguments\n");
    }
}

void divide(Stack* stack) {

    if (stack->count < 2) {
        outputText("Insufficient Arguments\n");
        return;
    }

    Level* head = stack->head;
    Level* next = head->next;

    if (head->type != TYPE_STRING && next->type != TYPE_STRING) { //both numbers
        if (head->type == TYPE_LONG && next->type == TYPE_LONG) { //both longs
            long result = next->longvalue / head->longvalue;
            pop(stack);
            pop(stack);
            push(stack, TYPE_LONG, result, 0.0, NULL);

        }
        else { //some mix of long and double, or just double
            double result = asDouble(next) / asDouble(head);
            pop(stack);
            pop(stack);
            push(stack, TYPE_DOUBLE, 0, result, NULL);
        }
    }
    else {
        // at least one string
        outputText("Bad Arguments\n");
    }
}

void performOperation(Stack* stack, const char* operator) {

    if (operator[0] == 'q') { //quit
        exit(0);
    }

    if (operator[0] == 'd') { //drop
        pop(stack);
    }

    if (operator[0] == 'r') { //roll
        roll(stack);
    }

    if (operator[0] == '+') {
        add(stack);
    }

    if (operator[0] == '-') {
        subtract(stack);
    }

    if (operator[0] == '*') {
        multiply(stack);
    }
    if (operator[0] == '/') {
        divide(stack);
    }

}

void gatherInput(Stack* stack) {

    char input[256];

    outputText("# ");
    fgets(input, 33, stdin);

    input[strcspn(input, "\r\n")] = '\0';
    fprintf(logFile, "%s\n", input);     // file only, since the terminal already echoed it
    fflush(logFile);

    int type = processInput(input); //processInput strips \n off input

    if (type == TYPE_LONG) {
        push(stack, TYPE_LONG, strtol(input, NULL, 10), 0.0, NULL);
    }

    else if (type == TYPE_DOUBLE) {
        push(stack, TYPE_DOUBLE, 0, strtod(input, NULL), NULL);
    }

    else if (type == TYPE_STRING) {

        int check = validateString(input);
        int isOp = isOperation(input);

        if (isOp == 1) { //is an operator input
            performOperation(stack, input);
        }
        else if (check == 1) { //valid input
            push(stack, TYPE_STRING, 0, 0.0, input);
        }
        else {
            outputText("Bad Input\n");
        }
    }

    else {
        outputText("Bad Input\n");
    }

}

