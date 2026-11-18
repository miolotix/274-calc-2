#ifndef DEFIN_H
#define DEFIN_H

#define TYPE_LONG 0
#define TYPE_DOUBLE 1
#define TYPE_STRING 2

struct Level
{
    int type; // 0 = long, 1 = double, 2 = string
    long longvalue; // long int
    double doublevalue;
    char stringvalue[33];
    struct Level* next;
} typedef Level;

struct Stack
{
    Level* head; //level 1 (top)
    int count; // levels in use
} typedef Stack;

//function headers vvv (def in definition.c)

void push(Stack* stack, int in_type, long in_long, double in_double,
    const char* in_string);
//push(stack*, type, int, double, string);

int pop(Stack* stack);

Level* get_level(Stack* s, int n);

void printStack(Stack* stack);

void gatherInput(char* input, int size);



#endif
