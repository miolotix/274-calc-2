#ifndef DEFIN_H
#define DEFIN_H

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
