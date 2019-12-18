#include "cal.h"
#include <time.h>

long long getElapsedTime(unsigned int nFlag);

int main(int argc, char *argv[]) {
  getElapsedTime(0);

  printf(" ======= Welcome to infinite calculator! ======= \n");
  printf("Enter the expression in infix notation.\n");

  /*  input  */
  char b = '\0';  // b는 이전 턴의 글자를 기억
  char c = '\0';
  int checkCon = 0; // Check continuous -> 0이면 새로운 숫자
  int leftbucket = 0;
  int rightbucket = 0;
  numList *conNum = NULL;

  MathEx *infix = newMathEx();
  MathEx *postfix = newMathEx();

  printf("Input: ");
  while( ( c = getchar() ) != EOF ) {
    if ('0' <= c && c <= '9') {
      // 새로운 숫자의 시작인지 아닌지 확인
      if (checkCon == 0) {
        conNum = newNumList();
        checkCon = 1;
      }
      lastAppend( conNum, c-'0' );
    }

    else if ( isOperator(c) ) {
      if (b == '\0' && c == '-' || b == '(' && c == '-') {
        conNum = newNumList();
        checkCon = 1;
        conNum->sign = -1;
      }
      if ( conNum->tail->value == -1 ) {
        printf("\nNumber cannot end with floatpoint.\n");
        return 0;
      }
      if ( checkCon == 0 && b != '(' ) {
        printf("\nYou cannot enter two operators in succession.\n");
        return 0;
      }
      // 연산자와 만남 = 숫자의 끝 : stack에 conNum 넣을 필요 있음
      checkCon = 0;
      AppendExNum(infix, conNum);
      if (c == '(') leftbucket++;
      else if (c == ')') rightbucket++;
      if (leftbucket < rightbucket) {
        printf("\nParentheses do not match.\n");
        return 0;
      }
      AppendExOpr(infix, c);
    }

    else if ( c == '.' ) { // value = -1
      // floatpoint 지정
      // 입력하고 있던 숫자가 없으면 오류 떠야함 (.으로 수가 시작할 수 없음)
      if (checkCon == 0) {
        printf("\nNumber cannot start with floatpoint.\n");
        return 0;
      }
      else {
        if(conNum->floatpoint != NULL) {
          printf("\nOne number cannot have two floatpoints.\n");
          return 0;
        }
        lastAppend(conNum, -1);
        conNum->floatpoint = conNum->tail;
      }
    }

    else if ( c == ' ' || c == '\n' ) continue;

    else {
      printf("\nInvalid input format! You cannot enter %c.\n",c);
      return 0;
    }

    printf("%c",c); // 아무 error도 뜨지 않으면 출력
    b = c;  // 직전 입력 기억
  }
  printf("\n");

  if(leftbucket != rightbucket) {
    printf("\nParentheses do not match.\n");
    return 0;
  }

  /*  calculate */
  numList *ans = newNumList();
  ans = calculate(postfix);

  /*  output  */
  printf("Output: ");
  printNum(ans);

  printf("Elapsed Time: %lld\n", getElapsedTime(1));

  return 0;
}


long long getElapsedTime(unsigned int nFlag)
{
	const long long NANOS = 1000000000LL;
	static struct timespec startTS, endTS;
	static long long retDiff = 0;

	if (nFlag == 0) {
		retDiff = 0;
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTS) == -1) {
			printf("Failed to call clock_gettime\n");
		}
	}
	else {
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTS) == -1) {
			printf("Failed to call clock_gettime\n");
		}
		retDiff = NANOS * (endTS.tv_sec - startTS.tv_sec) + (endTS.tv_nsec - startTS.tv_nsec);
 	}

	return retDiff;
}
