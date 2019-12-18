#include "dll.h"


typedef struct numStkNode {
  numList *num;
  struct numStkNode *next;

} numStkNode;

typedef struct numStack {
  numStkNode *top;

} numStack;

typedef struct oprStkNode {
  char opr;
  struct oprStkNode *next;

} oprStkNode;

typedef struct oprStack {
  oprStkNode *top;

} oprStack;


numStkNode *newNumStkNode(numList *Num) {
  numStkNode *tmp = (numStkNode*)malloc(sizeof(numStkNode));
  tmp->num = Num;
  tmp->next = NULL;

  return tmp;
}

numStack *newNumStack(void) {
  numStack *tmp = (numStack*)malloc(sizeof(numStack));
  tmp->top = NULL;

  return tmp;
}

oprStkNode *newOprStkNode(char opr) {
  oprStkNode *tmp = (oprStkNode*)malloc(sizeof(oprStkNode));
  tmp->opr = opr;
  tmp->next = NULL;

  return tmp;
}

oprStack *newOprStack(void) {
  oprStack *tmp = (oprStack*)malloc(sizeof(oprStack));
  tmp->top = NULL;

  return tmp;
}

int isNumStkEmpty(numStack *numStack) {
  if (numStack->top == NULL) return 1; // empty
  return 0; // not empty
}

int isOprStkEmpty(oprStack *oprStack) {
  if (oprStack->top == NULL) return 1; // empty
  return 0; // not empty
}


void numPush(numStack *numStk, numList *Num) {
  numStkNode *tmp = newNumStkNode(Num);
  tmp->next = numStk->top;
  numStk->top = tmp;
}

void oprPush(oprStack *oprStk, char c) {
  oprStkNode *tmp = newOprStkNode(c);
  tmp->next = oprStk->top;
  oprStk->top = tmp;
}

numList *numPop(numStack *numStk) {
  if (isNumStkEmpty(numStk)) return NULL;

  numStkNode *tmp = numStk->top;
  numStk->top = numStk->top->next;
  numList *popped = tmp->num;
  free(tmp);

  return popped;
}

char oprPop(oprStack *oprStk) {
  if (isOprStkEmpty(oprStk)) return '\0';

  oprStkNode *tmp = oprStk->top;
  char popped = tmp->opr;
  oprStk->top = oprStk->top->next;
  free(tmp);

  return popped;
}
