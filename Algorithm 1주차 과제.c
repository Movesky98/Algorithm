#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Print_Menu();
void Algorithm1(int n, const int S[], int x);
int Algorithm2(int n, const int S[]);
void Algorithm3(int n, int S[]);

int main() {
	srand((unsigned)time(NULL));

	int number = 0, i = 0, result = 0;
	int n = 0, *S = NULL, x = 0;

	printf("Algorithms 예제 1, 2, 3을 실행함에 따라 연산횟수를 구하는 프로그램입니다.\n");

	printf("리스트의 크기 n을 입력하세요 : ");
	scanf_s("%d", &n);

	S = (int*)malloc(sizeof(int) * n);
	for (i = 0; i < n; i++) {
		S[i] = rand() % n;
	}
	printf("리스트 S에 나열된 원소들은 다음과 같습니다\n");
	for (i = 0; i < n; i++) {
		printf("[ %d ] : %d\t", i, S[i]);
	}
	printf("\n\n");
	do {
		Print_Menu();

		printf("명령어를 입력하세요 : ");
		scanf_s("%d", &number);
		printf("\n");

		switch (number) {
		case(1):
			printf("찾고자 하는 값 x를 입력하세요 : ");
			scanf_s("%d", &x);
			printf("예제 1번을 실행합니다.\n");
			Algorithm1(n, S, x);
			break;
		case(2):
			printf("예제 2번을 실행합니다.\n");
			Algorithm2(n, S);
			break;
		case(3):
			printf("예제 3번을 실행합니다.\n");
			Algorithm3(n, S);
			break;
		case(4):
			printf("프로그램을 종료합니다.\n");
			free(S);
			break;
		}
	} while (number != 4);

	return 0;
}
void Print_Menu() {
	printf("1. 예제 1번 실행\n");
	printf("2. 예제 2번 실행\n");
	printf("3. 예제 3번 실행\n");
	printf("4. 프로그램 종료\n\n\n");
}
void Algorithm1(int n, const int S[], int x) {	
	int location = 0, counter = 0;

	while (location < n && S[location] != x) {	// 실행될때마다 counter += 2;
		counter += 2;
		location++;
		counter++;
	}

	if (location >= n) {
		counter++;
		location = 0;
		counter++;
	}

	if (location == 0) 
		printf("리스트 S에 x는 존재하지않습니다.\n");
	else 
		printf("x가 리스트의 %d번째 원소로 존재합니다.\n", location);

	printf("n : %d, x : %d 일때, Algorithm 예제 1에서 연산된 횟수는 : %d\n\n\n", n, x, counter);
}
int Algorithm2(int n, const int S[]) {
	int index, result = 0;
	int counter = 0;

	counter++;
	for (index = 0; index < n; index++) {
		counter += 2;
		result += S[index];
		counter += 1;
	}

	counter++;		// return result의 연산 1회
	printf("n : %d 일때, Algorithm 예제 2에서 연산된 횟수는 : %d\n\n\n", n, counter);
	return result;
}
void Algorithm3(int n, int S[]) {		// n 정수로 구성된 리스트 S를 단조감소 순서로 나열하는 예제
	int i, j, counter = 0, temp = 0;

	counter++;
	for (i = 0; i < n; i++) {
		counter += 2;
		counter++;
		for (j = i + 1; j < n; j++) {
			counter += 2;		// 이때 j를 기준으로 하는 반복문 for문은 변수 i로 인해 n-2번 실행됨
			if (S[j] < S[i]) {
				counter++;
				temp = S[j];
				counter++;
				S[j] = S[i];
				counter++;
				S[i] = temp;
				counter++;
			}
		}
	}
	printf("리스트 S의 원소를 나열하면\n");
	for (i = 0; i < n; i++)
		printf("%d\t", S[i]);
	printf("\nn : %d일때, Algorithm 예제 3에서 연산된 횟수는 : %d\n\n\n", n, counter);
}
