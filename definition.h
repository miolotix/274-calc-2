#ifndef DEFIN_H
#define DEFIN_H

#define TYPE_LONG 0
#define TYPE_DOUBLE 1
#define TYPE_STRING 2
#define TYPE_BAD 3
#define MAX_STRING_LEN 32

extern FILE* logFile;

typedef struct Level
{
    int type; // 0 = long, 1 = double, 2 = string
    long longvalue; // long int
    double doublevalue;
    char stringvalue[33];
    struct Level* next;
} Level;

typedef struct Stack
{
    Level* head; //level 1 (top)
    int count; // levels in use
} Stack;

//function headers vvv (def in definition.c)

void push(Stack* stack, int in_type, long in_long, double in_double,
    const char* in_string);
//push(stack*, type, int, double, string);

int pop(Stack* stack);

void outputText(const char* fmt, ...);

Level* get_level(Stack* s, int n);

void printStack(Stack* stack);

int classify_str(const char* str);

int processInput(char* input);

int validateString(char* word);

int isOperation(const char* input);

void roll(Stack* stack);

double asDouble(const Level* level);

void node_to_text(const Level* level, char* out, size_t size);

void add(Stack* stack);

void subtract(Stack* stack);

void multiply(Stack* stack);

void divide(Stack* stack);

void performOperation(Stack* stack, const char* operator);

void gatherInput(Stack* stack);



#endif
