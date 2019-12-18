#include <stdio.h>
#include <stdlib.h>


typedef struct numNode {
	int value; // -1 means floatpoint
	struct numNode *prev;
	struct numNode *next;
} numNode;


typedef struct numList { // A Real Number
	numNode *head;
	numNode *tail;
	numNode *floatpoint; // this can be NULL
	// if not NULL, floatpoint->value == -1

	int sign; // -1 or +1

} numList;

int isInteger(numList *Num) {
	if (Num->floatpoint == NULL) return 1; //integer
	return 0; //not integer
}


numNode *newNumNode(int value) {

	numNode *tmp = (numNode*)malloc(sizeof(numNode));

	tmp->value = value;
	tmp->prev = NULL;
	tmp->next = NULL;

	return tmp;
}

numList *newNumList(void) {
	numList *tmp = (numList*)malloc(sizeof(numList));

	tmp->head = NULL;
	tmp->tail = NULL;
	tmp->floatpoint = NULL;

	int sign = +1;

	return tmp;
}

void firstAppend(numList *List, int value) {
	numNode *tmp = newNumNode(value);

	if (List->head == NULL)	{
		List->head = tmp;
		List->tail = tmp;
	}
	else	{
		List->head->prev = tmp;
		tmp->next = List->head;
		List->head = tmp;
	}
	if(value == -1) List->floatpoint = tmp;
}

void lastAppend(numList *List, int value) {
	numNode *tmp = newNumNode(value);

	if (List->head == NULL) {
		List->head = tmp;
		List->tail = tmp;
	}
	else {
		List->tail->next = tmp;
		tmp->prev = List->tail;
		List->tail = tmp;
	}
	if(value == -1) List->floatpoint = tmp;
}

void rmZero(numList *List) {
	if (List->floatpoint == NULL) {
		numNode *tmp = List->head;
		while (tmp->value == 0 && tmp->next != NULL) {
			List->head = tmp->next;
			tmp->next->prev = NULL;
			numNode *willFree = tmp;
			free(willFree);
			tmp = tmp->next;
		}
	}
	else {
		numNode *tmp = List->head;
		while (tmp->value == 0 && tmp->next->value != -1) {
			List->head = tmp->next;
			tmp->next->prev = NULL;
			numNode *willFree = tmp;
			free(willFree);
			tmp = tmp->next;
		} // 맨 앞 제거
		//맨 뒤 제거 -> 정수는 234.0 과 같이 끝나게 함
		tmp = List->tail;
		while (tmp->value == 0 && tmp->prev->value != -1) {
			List->tail = tmp->prev;
			tmp->prev->next = NULL;
			numNode *willFree = tmp;
			free(willFree);
			tmp = tmp->prev;
		}
		if (List->tail->value == 0 && List->tail->prev->value == -1)	{
			// 234.0 과 같은 형태 -> 234 로 만듦
			numNode *willFree1 = List->tail->prev;
			numNode *willFree2 = List->tail;
			List->tail = List->tail->prev->prev;
			List->floatpoint = NULL;
			List->tail->next = NULL;
			free(willFree1);
			free(willFree2);
		}
	}
}

int is1Larger(numList *Num1, numList *Num2){ // 길이 아니고 크기 : 뺄셈에서 사용
	/*
	if (Num1 > Num2)  return +1;
	if (Num2 < Num2)  return -1; -> swap Num1 and Num2 , change sign
	if (Num1 == Num2) return  0; -> wow
	*/
	numNode *Node1 = Num1->head;
	numNode *Node2 = Num2->head;

	while (Node1->prev != NULL && Node2->prev != NULL) {
		Node1 = Node1->prev;
		Node2 = Node2->prev;
	}
	if (Node2->prev == NULL) return +1;
	if (Node1->prev == NULL) return -1;

	/*	두 수의 정수 길이가 같음	*/
	while (Node1->next != NULL && Node2->next != NULL) {
		if (Node1->value > Node2->value) return +1;
		if (Node1->value < Node2->value) return -1;
		Node1 = Node1->next;
		Node2 = Node2->next;
	}

	if (Node2->next == NULL) return +1;
	if (Node1->next == NULL) return -1;

	// 끝까지 어느 조건에도 걸리지 않았다면 둘이 같은 값임을 의미
	return 0;
}

void mkFloat(numList *Num) {
	// put floatpoint and one '0'
	// ex) 5523 -> 5523.0
	numNode *fp = newNumNode(-1);

	Num->floatpoint = fp;
	Num->tail->next = fp;
	fp->prev = Num->tail;
	Num->tail = fp;

	lastAppend(Num,0);
}

void sameIntLen(numList *Num1, numList *Num2, numNode *Node1, numNode *Node2) {
	while (Node1->prev != NULL && Node2->prev != NULL) {
		Node1 = Node1->prev;
		Node2 = Node2->prev;
	}

	if (Node1->prev == NULL && Node2->prev == NULL) return;

	else if (Node1->prev == NULL) {
		firstAppend(Num1,0);
		while (Node2->prev != NULL) {
			Node2 = Node2->prev;
			firstAppend(Num1,0);
		}
	}

	else {
		firstAppend(Num2,0);
		while (Node1->prev != NULL) {
			Node1 = Node1->prev;
			firstAppend(Num2,0);
		}
	}
}


void sameDecLen(numList *Num1, numList *Num2, numNode *Node1, numNode *Node2) {
	while (Node1->next != NULL && Node2->next != NULL) {
		Node1 = Node1->next;
		Node2 = Node2->next;
	}

	if (Node1->next == NULL && Node2->next == NULL) return;

	else if (Node1->next == NULL) {
		lastAppend(Num1,0);
		while (Node2->next != NULL) {
			Node2 = Node2->next;
			lastAppend(Num1,0);
		}
	}

	else {
		lastAppend(Num2,0);
		while (Node1->next != NULL) {
			Node1 = Node1->next;
			lastAppend(Num2,0);
		}
	}
}

void sameLen(numList *Num1, numList *Num2) { // 덧셈 이전에 자릿수 맞춰줌
	/*
	길이 차이 혹은 길이 return해주는 함수를 사용하려고 했으나
	이 길이가 int 혹은 unsigned int에 저장될 텐데 이 때 처리할 수 있는 자리수에 제한이 생김
	물론... 죽어도 그 자리수를 넘을 일은 없을 거임.....
	근데 길이 return하는 함수를 구현할 바엔 Node에 int intSize, decSize 변수 넣고말지
	무한 계산기 짠다고 일부러 뺐는데 넣을 순 없지
	*/

	if (isInteger(Num1) == 0 && isInteger(Num2) == 0) {
		//if (Num1->floatpoint != NULL && Num2->floatpoint != NULL) {
		// niether of two are integer

		// 정수 부분부터 처리
		numNode *Node1 = Num1->floatpoint->prev;
		numNode *Node2 = Num2->floatpoint->prev;
		sameIntLen(Num1, Num2, Node1, Node2);

		// 소수 부분 처리
		Node1 = Num1->floatpoint->next;
		Node2 = Num2->floatpoint->next;
		sameDecLen(Num1, Num2, Node1, Node2);
	}

	else if (isInteger(Num1) == 1 && isInteger(Num2) == 1) {
	//else if (Num1->floatpoint == NULL && Num2->floatpoint == NULL) {
		// both integer : start at tail
		numNode *Node1 = Num1->tail;
		numNode *Node2 = Num2->tail;
		sameIntLen(Num1, Num2, Node1, Node2);
	}


	else if (isInteger(Num1) == 1) {
		// Num1 integer, Num2 not
		numNode *Node1 = Num1->tail;
		numNode *Node2 = Num2->floatpoint->prev;
		sameIntLen(Num1, Num2, Node1, Node2);

		mkFloat(Num1);
		Node1 = Num1->floatpoint->next;
		Node2 = Num2->floatpoint->next;
		sameDecLen(Num1, Num2, Node1, Node2);
	}


	else {
		// Num2 integer, Num1 not
		numNode *Node1 = Num1->floatpoint->prev;
		numNode *Node2 = Num2->tail;
		sameIntLen(Num1, Num2, Node1, Node2);

		mkFloat(Num2);
		Node1 = Num1->floatpoint->next;
		Node2 = Num2->floatpoint->next;
		sameDecLen(Num1, Num2, Node1, Node2);
	}
}

void printNum(numList *Num) {
	numNode *digit = Num->head; //자릿수
	do {
		if (digit->value != -1)	printf("%d",digit->value);
		else printf(".");
		digit = digit->next;
	} while (digit->next != NULL);
	printf("\n");
}
