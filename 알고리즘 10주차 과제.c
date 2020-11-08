#include <stdio.h>
#include <stdlib.h>

#define BST_Size 4		// 최적이진트리에서 사용할 노드들의 개수
#define INF 9999

#define Knapsack_Size 5
#define Weight 6

typedef struct BST_Node {
	int key;
	int frequency;
}BST_Node;

typedef struct Knapsack_Node {
	int value;
	int weight;
}Knapsack_Node;

/*
행렬에서의 key값은 C(i, j)의 값을,
frequency는 k를 비교한 후 
가장 최적의 값이 나오는 k 값을 적용할 것입니다. 
*/
BST_Node C_Information[BST_Size][BST_Size];	
BST_Node* BST_Array = NULL;

void Create_BST_Node();
void Optimal_BST();
int C(int i, int j);
int min_C(int i, int j);
void Print_C();
void Destroy_BST_Node();

Knapsack_Node* Knapsack_Array = NULL;
int B[Knapsack_Size + 1][Knapsack_Size + 1] = {
		{0,0,0,0,0,0}, {0,0,0,2,2,2}, {0,0,0,2,3,3},
		{0,0,2,2,3,4}, {0,0,2,2,3,4}, {0,2,2,4,4,5} 
};
int m[Knapsack_Size + 1][Knapsack_Size + 1] = {
	{0,0,0,0,0,0}, {0,0,0,1,1,1}, {0,0,0,0,1,1}, 
	{0,0,1,0,0,1}, {0,0,0,0,0,0}, {0,1,0,1,1,1} 
};

void Create_knapsack();
void Destroy_knapsack();
void Knapsack();
void Print_Knapsack();

void main() {
	int i = 0, command = 0;

	while (command != 3) {
		printf("1. 최적 이진트리 실행\n");
		printf("2. 0/1 배낭문제 실행\n");
		printf("3. 프로그램 종료\n");

		printf("번호를 입력하세요: ");
		scanf_s("%d", &command);

		switch (command) {
		case 1:
			Create_BST_Node();
			Optimal_BST();
			Destroy_BST_Node();
			break;
		case 2:
			Create_knapsack();
			Knapsack();
			Destroy_knapsack();
			break;
		case 3:
			printf("프로그램을 종료합니다.\n");
			break;
		default:
			printf("제대로 된 값을 입력해주세요.\n");
			break;
		}
	}
}
void Create_BST_Node() {
	BST_Array = (BST_Node*)malloc(sizeof(BST_Node) * BST_Size);

	(BST_Array + 0)->key = 3, (BST_Array + 0)->frequency = 3;
	(BST_Array + 1)->key = 7, (BST_Array + 1)->frequency = 2;
	(BST_Array + 2)->key = 9, (BST_Array + 2)->frequency = 4;
	(BST_Array + 3)->key = 12, (BST_Array + 3)->frequency = 1;
}
void Optimal_BST() {
	int i = 0, j = 0;

	for (i = 0; i < BST_Size; i++) {
		for (j = 0; j < BST_Size; j++) {
			C_Information[i][j].key = -1;
			C_Information[i][j].frequency = -1;
		}
	}

	for (i = 0; i < BST_Size; i++) {
		for (j = 0; j <= i; j++) {
			C_Information[i][j].key = C(i, j);
		}
	}
	Print_C(); 
	printf("\n");
	
	for (i = 0; i < BST_Size - 1; i++) {
		for (j = i + 1; j < BST_Size; j++) {
			C_Information[j - (i + 1)][j].key = C(j - (i +1), j);
		}
		Print_C();
		printf("\n");
	}
}
int C(int i, int j) {
	if (i < 0 || j < 0)
		return 0;

	if (C_Information[i][j].key != -1)
		return C_Information[i][j].key;
	if (i > j) {
		C_Information[i][j].frequency = -1;
		return 0;
	}
	else if (i == j) {
		C_Information[i][i].frequency = -1;
		return (BST_Array + i)->frequency;
	}
	else
		return min_C(i, j);
}
int min_C(int i, int j) {
	int k = i, min = INF, fp = 0;
	int min_k = INF;

	for (k = i; k <= j; k++) {
		fp += (BST_Array + k)->frequency;
	}
	for (k = i; k <= j; k++) {
		if (min > C(i, k - 1) + C(k + 1, j) + fp) {
			min = C(i, k - 1) + C(k + 1, j) + fp;
			min_k = k;
		}
	}

	C_Information[i][j].frequency = min_k;

	return min;
}
void Print_C() {
	int i = 0, j = 0;
	printf("i j\t");
	for (i = 0; i < BST_Size; i++) {
		printf("%d\t", i + 1);
	}
	printf("\n");

	for (i = 0; i < BST_Size; i++) {
		printf("%d\t", i + 1);
		for (j = 0; j < BST_Size; j++) {
			if (C_Information[i][j].key != -1) {
				if (C_Information[i][j].frequency != -1)
					printf("%d (%d)\t", C_Information[i][j].key, C_Information[i][j].frequency + 1);
				else
					printf("%d\t", C_Information[i][j].key);
			}
			else
				printf("[  ]\t");
		}
		printf("\n");
	}
}
void Destroy_BST_Node() {
	free(BST_Array);
}
void Create_knapsack() {
	Knapsack_Array = (Knapsack_Node*)malloc(sizeof(Knapsack_Node) * Knapsack_Size);

	(Knapsack_Array + 0)->weight = 3, (Knapsack_Array + 0)->value = 2;
	(Knapsack_Array + 1)->weight = 4, (Knapsack_Array + 1)->value = 3;
	(Knapsack_Array + 2)->weight = 2, (Knapsack_Array + 2)->value = 2;
	(Knapsack_Array + 3)->weight = 5, (Knapsack_Array + 3)->value = 4;
	(Knapsack_Array + 4)->weight = 1, (Knapsack_Array + 4)->value = 2;

	
}
void Knapsack() {
	int i = 0, j = 0;
	for (j = 0; j < Weight; j++) 
		B[0][j] = 0;
	Print_Knapsack();
	printf("\n\n");
	for (i = 1; i < Knapsack_Size; i++) {
		B[i][0] = 0;
		for (j = 1; j < Weight; j++) {
			if ((Knapsack_Array + i)->weight <= j) {
				if ((B[i - 1][Weight - (Knapsack_Array + i)->weight] + (Knapsack_Array + i)->value) > B[i - 1][j]) {
					B[i][j] = B[i - 1][i - (Knapsack_Array + i)->weight] + (Knapsack_Array + i)->value;
					m[i][j] = 1;
				}
				else {
					B[i][j] = B[i - 1][j];
					m[i][j] = 0;
				}
			}
			else {
				B[i][j] = B[i - 1][j];
				m[i][j] = 0;
			}
		}
		Print_Knapsack();
		printf("\n\n");
	}
}
void Print_Knapsack() {
	int i = 0, j = 0;

	printf(" \t");
	for (i = 0; i < Weight; i++) {
		printf("%d\t", i);
	}
	printf("\t\t");
	for (i = 0; i < Weight; i++) {
		printf("%d\t", i);
	}

	printf("\n");
	for (i = 0; i < Weight; i++) {
		printf("%d\t", i);
		for (j = 0; j < Weight; j++) {
			printf("%d\t", B[i][j]);
		}

		printf("\t");
		printf("%d\t", i);
		for (j = 0; j < Weight; j++) {
			printf("%d\t", m[i][j]);
		}
		printf("\n");
	}
}
void Destroy_knapsack() {
	free(Knapsack_Array);
}
