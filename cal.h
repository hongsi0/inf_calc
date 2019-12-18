#include <string.h>
#include "operator.h"


numList *plus     (numList *Num1, numList *Num2);
numList *minus    (numList *Num1, numList *Num2);
numList *multiply (numList *Num1, numList *Num2);


numList *calculate(MathEx *postfix) {
  numStack *stack = newNumStack();
  numList *ans = newNumList();
  MathExNode *tmp = postfix->head;

  /*   calculate postfix  */
  while(tmp->next != NULL) {
    if (tmp->Num != NULL) {
      // Number
      numPush(stack, tmp->Num);
    }
    else if (tmp->opr != '\0') {
      // Operator
      numList *Num1 = numPop(stack);
      numList *Num2 = numPop(stack);
      if (tmp->opr == '+') numPush(stack, plus(Num1,Num2));
      else if (tmp->opr == '-') numPush(stack, minus(Num1,Num2));
      else if (tmp->opr == '*') numPush(stack, multiply(Num1,Num2));
    }

    tmp = tmp->next;
  }
  rmZero(ans);
  return ans;
}


numList *plus     (numList *Num1, numList *Num2)  {
  numList *ans = newNumList();

  if(Num1->sign == -1 && Num2->sign == -1)  {
    Num1->sign = +1;
    Num2->sign = +1;
    ans = plus(Num1,Num2);
    ans->sign = -1;
    rmZero(ans);
    return ans;
  }
  else if(Num1->sign == -1) {
    Num1->sign = +1;
    ans = minus(Num2,Num1);
    rmZero(ans);
    return ans;
  }
  else if(Num2->sign == -1) {
    Num2->sign = +1;
    ans = minus(Num1,Num2);
    rmZero(ans);
    return ans;
  }

  sameLen(Num1,Num2);
  numNode *tmp1 = Num1->tail;
  numNode *tmp2 = Num2->tail;
  int up;

  while (tmp1->prev != NULL) {
    if(tmp1->value == -1) {
      firstAppend(ans,-1);
      tmp1 = tmp1->prev;
      tmp2 = tmp2->prev;
      continue;
    }

    firstAppend(ans, tmp1->value + tmp2->value + up);

    if(ans->head->value >= 10) up = 1;
    else up = 0;
    tmp1 = tmp1->prev;
    tmp2 = tmp2->prev;
  }

  if(up == 1) firstAppend(ans,1);

  rmZero(ans);
  return ans;
}


numList *minus    (numList *Num1, numList *Num2)  {
  numList *ans = newNumList();
  if (Num2->sign == -1) {
    Num2->sign = +1;
    ans = plus(Num1,Num2);
    rmZero(ans);
    return ans;
  }
  else if (Num1->sign == -1)  {
    // 음수 - 양수
    Num2->sign = -1;
    ans = plus(Num1,Num2);
    rmZero(ans);
    return ans;
  }

  //  양수 - 양수

  if (is1Larger(Num1, Num2) == 0) {
    firstAppend(ans,0);
    return ans;
  }
  else if (is1Larger(Num1, Num2) == -1) {
    numList *tmp = newNumList();
    tmp = Num1;
    Num1 = Num2;
    Num2 = tmp;
    ans->sign = -1;
  }
  // now Num1 > Num2
  sameLen(Num1,Num2);

  int down = 0;
  numNode *tmp1 = Num1->tail;
  numNode *tmp2 = Num2->tail;

  while(tmp1->prev != NULL) {
    if (tmp1->value == -1) {
      firstAppend(ans,-1);
      tmp1 = tmp1->prev;
      tmp2 = tmp2->prev;
      continue;
    }

    firstAppend(ans, tmp1->value - tmp2->value + down);
    if(ans->head->value < 0)  {
      down = -1;
      ans->head->value += 10;
    }
    else  down = 0;
  }

  rmZero(ans);
  return ans;

}

numList *multiply (numList *Num1, numList *Num2)  {
  numList *ans = newNumList();

  rmZero(ans);
  return ans;
}
