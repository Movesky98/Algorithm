/*
-Hamiltonian Problem
 작성 과정에서 Node의 수가 5개인 예제와, 8개인 예제가 있었습니다.
 확인해본 결과 Node의 수가 5개인 예제에서는 
 모든 노드를 한번씩만 방문했을 경우, 시작 노드에서 끝날 수가 없어
 노드 수가 8개인 그래프를 가지고 작성하였습니다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define n_GC 4
#define n_HC 8
#define n_KN 4

typedef struct item{
	int id;
	int weight;
	int profit;
}item;

/* Graph Coloring */
int W[4][4] = { {0, 1, 1, 1}, {1, 0, 1, 0}, {1, 1, 0, 1}, {1, 0, 1, 0} };		// 노드의 수 4개
int m = 3;			// 색깔의 수 1: Red, 2: Green, 3: Blue
int vcolor[5];		// i번째 노드에 할당된 색을 표현하기 위한 배열 vcolor[0] = 0
int GC_count = 0;

void m_coloring(int i);
bool promising(int i);

/* Hamiltonian Circuits */
int W_HC[n_HC][n_HC] = { 
	{0, 1, 0, 0, 0, 0, 1, 1}, // V1
	{1, 0, 1, 0, 0, 0, 1, 1}, // V2
	{0, 1, 0, 1, 0, 1, 0, 0}, // V3
	{0, 0, 1, 0, 1, 0, 0, 0}, // V4
	{0, 0, 0, 1, 0, 1, 0, 0}, // V5
	{0, 0, 1, 0, 1, 0, 1, 0}, // V6
	{1, 1, 0, 0, 0, 1, 0, 1}, // V7
	{1, 1, 0, 0, 0, 0, 1, 0} // V8
};
int HC_count = 0;
int vindex[n_HC] = { 0, };

void hamiltonian(int i);
bool promising_HC(int i);

/* 0-1 Knapsack 되추적 알고리즘 */
int W_KN = 16;		// 배낭의 총 부피
item Item[n_KN] = { {1, 2, 40}, {2, 5, 30}, {3, 10, 50}, {4, 5, 10} };	// 이미 오름차순으로 정렬이 되어있음

int maxprofit = 0;	// 지금까지 찾은 최선의 해답이 주는 가치
int numbest = 0;	// 살펴본 item들(않은 item 개수 포함)
int bestset[n_KN] = { 0, };		// 최대 이익일때 선택한 item들
int include[n_KN] = { 0, };		// 현재 선택한 item (선택시 1, 아닐시 0)
int KN_count = 0;

void knapsack(int i, int profit, int weight);
bool promising_KN(int i, int weight, int profit);

int main() {
	int command = 0, i = 0;
	do {
		printf("1. Graph Coloring 실행.\n");
		printf("2. Hamiltonian Problem 실행.\n");
		printf("3. 0-1 Knapsack 문제 실행.\n");
		printf("4. 프로그램 종료.\n");

		printf("번호 입력 : ");
		scanf_s("%d", &command);
		printf("\n\n");

		switch (command) {
		case 1:
			GC_count = 1;	// 루트 노드를 포함하여 1부터 시작
			m_coloring(0);
			printf("상태 공간 트리에서 생성되는 Node의 수 : %d\n\n", GC_count);
			GC_count = 0;	// 프로그램이 끝나고 0으로 초기화.
			break;
		case 2:
			vindex[0] = 1;
			hamiltonian(0);
			printf("상태 공간 트리에서 생성되는 Node의 수 : %d\n\n", HC_count); 
			HC_count = 0;	// 프로그램이 끝나고 0으로 초기화
			break;
		case 3:
			KN_count = 1;		// 루트노드를 포함하여 1부터 시작
			knapsack(-1, 0, 0);
			for (i = 0; i < n_KN; i++) {
				if (bestset[i] == 1) {
					printf("[%d번째 item, profit: %d, weight: %d]\n", Item[i].id, Item[i].profit, Item[i].weight);
				}
			}
			printf("MaxProfit : %d \n상태 공간 트리에서 생성되는 Node의 수 : %d\n\n", maxprofit, KN_count);
			KN_count = 0;		// 프로그램이 끝나고 0으로 초기화
			break;
		case 4:
			printf("프로그램을 종료합니다.\n");
			break;
		default:
			printf("값을 제대로 입력해주세요.\n");
			break;
		}
	} while (command != 4);
	return 0;
}

/*
- Graph Coloring -
인접한 지역에 같은 색이 칠해지지 않도록 해야한다.
입력: n (노드 수) m (색깔의 수) W[i][j] 이음선이 있으면 true, 아니면 false
출력: vcolor[i] - i번째 노드에 할당된 색(1~m)

초기호출 : m_coloring(0)
*/

void m_coloring(int i) {
	int color = 0;
	int k = 0;
	if (promising(i))
		if (i == 4) {	// 4: 노드의 수
			// vcolor 1 ~ n까지 출력 
			for (k = 1; k <= 4; k++) {
				if (vcolor[k] == 1)
					printf("V[%d] = Red, ", k);
				else if (vcolor[k] == 2)
					printf("V[%d] = Green, ", k);
				else if (vcolor[k] == 3)
					printf("V[%d] = Blue, ", k);
			}
			printf("\n");
		}
		else
			for (color = 1; color <= m; color++) {
				vcolor[i + 1] = color;
				m_coloring(i + 1);
				GC_count += 1;
			}
}

bool promising(int i) {
	int j = 1;
	bool check = true;

	while (j < i && check) {
		if (W[i - 1][j - 1] && vcolor[i] == vcolor[j])
			check = false;
		j++;
	}

	return check;
}

/*
- Hamiltonian Circuits -
입력: n (노드 수), W[i][j] 이음선이 있으면 true, 아니면 false
출럭: vindex[i] - 경로상에서 i번째 노드의 인덱스

초기호출 : vindex[0] = 1;
			  hamiltonian(0);
*/

void hamiltonian(int i) {
	int j = 0;
	int k = 0;
	if (promising_HC(i))
		if (i == n_HC - 1) {
			for (k = 0; k < n_HC; k++) {
				if (k == n_HC - 1) {
					printf("V[%d]\n", vindex[k]);
				}
				else
					printf("V[%d] -> ", vindex[k]);
			}
		}
		else
			for (j = 2; j <= n_HC; j++) {
				vindex[i + 1] = j;
				hamiltonian(i + 1);
				HC_count += 1;
			}
}
bool promising_HC(int i) {
	int j = 1;
	bool check = true;

	if (i == n_HC - 1 && !W_HC[vindex[n_HC - 1] - 1][vindex[0] -1])		// 해당 vindex에 숫자 1 ~ 8를 입력했을 경우
		check = false;																	// W_HC[8][8] 과 같은 오류가 발생함.
	else if (i > 0 && !W_HC[vindex[i - 1] - 1][vindex[i] - 1])		// 해당 vindex에 숫자 1 ~ 8를 입력했을 경우
		check = false;														// W_HC[8][8] 과 같은 오류가 발생함.
	else {
		while (j < i && check) {
			if (vindex[i] == vindex[j])
				check = false;
			j++;
		}
	}

	return check;
}
/* 
- 0-1 배낭 되추적 알고리즘 -
W : 배낭의 총 무게, n_KN : item의 개수

w[1], w[2], . . . , w[n] : 각 item의 무게
--> w(i) - i번째 item의 weight(무게)

p[1], p[2], . . . , p[n] : 각 item의 무게
--> p(i) - i번째 item의 profit(가치)

item을 p/w값으로 오름차순으로 정렬 (무게당 가치를 기준)
W하에서 최대 가치가 되어지도록 배낭에 넣을 수 있는 항목을 선택한다.

*/

/*
profit		현재 노드까지 포함된 가치의 합
weight	현재 노드까지 포함된 무게의 합 
*/
void knapsack(int i, int profit, int weight) {
	int k = 0;
	if (weight <= W_KN && profit > maxprofit) {
		maxprofit = profit;
		numbest = i;
		for (k = 0; k < n_KN; k++) {
			bestset[k] = include[k];
		}
	}
	if (promising_KN(i, weight, profit)) {
		include[i + 1] = 1;		// Yes
		knapsack(i + 1, profit + Item[i + 1].profit, weight + Item[i + 1].weight);
		KN_count += 1;
		include[i + 1] = 0;		// No
		knapsack(i + 1, profit, weight);
		KN_count += 1;
	}
}

bool promising_KN(int i, int weight, int profit) {
	int j = 0, k = 0;
	int totweight = 0;		// 해당 노드까지 얻었을 때, 구할 수 있는 무게의 합
	float bound = 0.0;		// 해당 노드까지 얻었을 때, 구할 수 있는 가치의 상한선

	if (weight >= W_KN) return false;
	else {
		j = i + 1;
		bound = profit;
		totweight = weight;
		while ((j <= n_KN) && (totweight + (Item + j)->weight <= W_KN)) {
			totweight = totweight + (Item+ j)->weight;
			bound = bound + (Item + j)->profit;
			j++;
		}
		k = j;
		if (k <= n_KN)
			bound += (W_KN - totweight) * (Item + k)->profit / (Item +k )->weight;
		return bound > maxprofit;
	}
}
