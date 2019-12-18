#include "stack.h"


typedef struct mathExNode {
  char opr;
  numList *Num;
  struct mathExNode *next;

} MathExNode;

typedef struct mathEx {
  MathExNode *head;

} MathEx;

MathExNode *newMathExNode(void) {
  MathExNode *tmp = (MathExNode*)malloc(sizeof(MathExNode));
  tmp->opr = '\0';
  tmp->Num = NULL;
  tmp->next = NULL;

  return tmp;
}

MathEx *newMathEx(void) {
  MathEx *tmp = (MathEx*)malloc(sizeof(MathEx));
  tmp->head = NULL;
}

void AppendExNum(MathEx *Ex, numList *Num) {
  MathExNode *newNode = newMathExNode();
  newNode->Num = Num;
  if (Ex->head == NULL) {
    Ex->head = newNode;
  }
  else {
    MathExNode *tmp = Ex->head;
    while(tmp->next != NULL) tmp = tmp->next;
    tmp->next = newNode;
  }
}

void AppendExOpr(MathEx *Ex, char opr) {
  MathExNode *newNode = newMathExNode();
  newNode->opr = opr;
  if (Ex->head == NULL) {
    Ex->head = newNode;
  }
  else {
    MathExNode *tmp = Ex->head;
    while (tmp->next != NULL) tmp = tmp->next;
    tmp->next = newNode;
  }
}

int isOperator(char c) {
  /*  우선순위도 한 번에 처리할까 고민 */
  if      (c == '+' || c == '-') return 1;
  else if (c == '*' || c == '/') return 1;
  else if (c == '(' || c == ')') return 1;

  return 0; // not operator
}

int operPri(char c) {
  /*
  괄호에 우선순위를 부여할지 따로 처리할지 고민
  */
  if      (c == '+' || c == '-') return 1;
  else if (c == '*' || c == '/') return 2;
//  else if (c == '(' || c == ')') return 0;
  return 0;
}


MathEx *In2Post(MathEx *infix) {
  oprStack *stack = newOprStack();
  MathEx* postfix = newMathEx();

  MathExNode *tmp = infix->head;

  while(tmp->next != NULL) {
    if (tmp->Num != NULL) AppendExNum(postfix,tmp->Num);

    else if (tmp->opr != '\0')  { // operator
      if ( tmp->opr == '(' )  AppendExOpr(postfix, tmp->opr);
      else if ( tmp->opr == ')' ) {
        while ( stack->top->opr != '(' ) {
          AppendExOpr(postfix, oprPop(stack));
        }
        oprPop(stack); // '(' 버림
      }
      else {
        int now_pri = operPri(tmp->opr);
        if (now_pri == 1) { // + or -
        /*
         스택의 최상위 노드보다 tmp->opr의 OperPri가 높을 때까지 pop
         높아지면 stack에 tmp->opr push
        */
          while (now_pri < operPri(stack->top->opr)) {
            AppendExOpr(postfix, oprPop(stack));
          }
          oprPush(stack, tmp->opr);
        }
        else { // * or /
          oprPush(stack, tmp->opr);
        }
      }
    }
    tmp = tmp->next;
  }

  // stack 에 남아있는 모든 연산자 꺼내기

  while(isOprStkEmpty(stack))  AppendExOpr(postfix, oprPop(stack));

  return postfix;
}
