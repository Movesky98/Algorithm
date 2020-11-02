#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define Max_Vertex 8
#define Max_Edge 11
#define Max_Node 4
#define Max_NEdge 6

typedef struct Vertex {
	char Alphabet;
	int distance;
}Vertex;

typedef struct Edge {
	int cost;
	Vertex* llink;
	Vertex* rlink;
}Edge;

typedef struct Node {
	int number;
	int distance;
}Node;

typedef struct Node_Edge {
	int cost;
	Node* llink;
	Node* rlink;
}Node_Edge;

Vertex* Vertex_list = NULL;
Edge* Edge_list = NULL;
Vertex* Prev_list[Max_Vertex];
bool check;

Node* Node_list = NULL;
Node_Edge* NEdge_list = NULL;
int dist_Matrix[Max_Node + 1][Max_Node + 1];

void Create_Bellman_FordGraph();
void Destroy_Bellman_FordGraph();
void Bellman_Ford_Algorithm();
void update(Edge* edge);
void Print_stage();

void Create_Floyd_WashallGraph();
void Destroy_Floyd_WashallGraph();
void Floyd_Washall_Algorithm();
void update_Floyd(int i, int j, int k);
void Print_Matrix();

int main() {
	int i = 0, command = 0;
	while (command != 3) {
		printf("1. Bellman-Ford Algorithm 실행\n");
		printf("2. Floyd-Warshall Algorithm 실행\n");
		printf("3. 프로그램 종료\n");

		printf("명령어 입력 : ");
		scanf_s("%d", &command);
		switch (command) {
		case 1:
			Create_Bellman_FordGraph();
			Bellman_Ford_Algorithm();
			Destroy_Bellman_FordGraph();
			break;
		case 2:
			Create_Floyd_WashallGraph();
			Floyd_Washall_Algorithm();
			Destroy_Floyd_WashallGraph();
			break;
		case 3:
			printf("프로그램을 종료합니다.\n");
			break;
		default:
			printf("제대로 된 값을 입력하세요.\n");
		}
	}
	return 0;
}

void Create_Bellman_FordGraph() {
	Edge_list = (Edge*)malloc(sizeof(Edge) * 11);		// Bellman-Ford Algorithm 예제에서 나온 Edge들의 숫자만큼 할당.
	Vertex_list = (Vertex*)malloc(sizeof(Vertex) * 8);

	(Vertex_list + 0)->Alphabet = 'S', (Vertex_list + 1)->Alphabet = 'A', (Vertex_list + 2)->Alphabet = 'B', (Vertex_list + 3)->Alphabet = 'C';
	(Vertex_list + 4)->Alphabet = 'D', (Vertex_list + 5)->Alphabet = 'E', (Vertex_list + 6)->Alphabet = 'F', (Vertex_list + 7)->Alphabet = 'G';

	(Edge_list + 0)->cost = 8,	(Edge_list + 0)->llink = Vertex_list + 0,	(Edge_list + 0)->rlink = Vertex_list + 7;			// S -> G
	(Edge_list + 1)->cost = 10,	(Edge_list + 1)->llink = Vertex_list + 0,	(Edge_list + 1)->rlink = Vertex_list + 1;			// S -> A
	(Edge_list + 2)->cost = 1,	(Edge_list + 2)->llink = Vertex_list + 2,	(Edge_list + 2)->rlink = Vertex_list + 1;			// B -> A
	(Edge_list + 3)->cost = 1,	(Edge_list + 3)->llink = Vertex_list + 2,	(Edge_list + 3)->rlink = Vertex_list + 3;			// B -> C
	(Edge_list + 4)->cost = 3,	(Edge_list + 4)->llink = Vertex_list + 3,	(Edge_list + 4)->rlink = Vertex_list + 4;			// C -> D
	(Edge_list + 5)->cost = -1,	(Edge_list + 5)->llink = Vertex_list + 4,	(Edge_list + 5)->rlink = Vertex_list + 5;			// D -> E
	(Edge_list + 6)->cost = -2,	(Edge_list + 6)->llink = Vertex_list + 5,	(Edge_list + 6)->rlink = Vertex_list + 2;			// E -> B
	(Edge_list + 7)->cost = 2,	(Edge_list + 7)->llink = Vertex_list + 1,	(Edge_list + 7)->rlink = Vertex_list + 5;			// A -> E
	(Edge_list + 8)->cost = -1,	(Edge_list + 8)->llink = Vertex_list + 6,	(Edge_list + 8)->rlink = Vertex_list + 5;			// F -> E
	(Edge_list + 9)->cost = 1,	(Edge_list + 9)->llink = Vertex_list + 7,	(Edge_list + 9)->rlink = Vertex_list + 6;			// G -> F
	(Edge_list + 10)->cost = -4, (Edge_list + 10)->llink = Vertex_list + 6, (Edge_list + 10)->rlink = Vertex_list + 1;			// F -> A
}
void Destroy_Bellman_FordGraph() {
	free(Vertex_list);
	free(Edge_list);
}
void Bellman_Ford_Algorithm() {
	int i = 0, j = 0, u = 0, v = 0, weight = 0;
	check = true;

	for (i = 0; i < Max_Vertex; i++) {
		(Vertex_list+ i)->distance = 9999;				// 9999를 무한대로 설정하겠습니다.
		Prev_list[i] = NULL;
	}

	(Vertex_list + 0)->distance = 0;				// dist(S) = 0;

	for (i = 1; i < Max_Vertex; i++) {
		check = false;
		for (j = 0; j < Max_Edge; j++) {
			update(Edge_list + j);
		}
		printf("%d번째 stage\n", i);
		Print_stage();
		
		if (check == false)
			break;
	}
	for (int i = 0; i < Max_Edge; i++)
	{
		u = (Edge_list + i)->llink->distance;
		v = (Edge_list + i)->rlink->distance;
		weight = (Edge_list + i)->cost;
		
		
		if (u != INT_MAX && u + weight < v)
			printf("그래프 내에 음의 사이클이 존재합니다.\n");
	}
}
void update(Edge* edge) {
	int i = 0;

	if (edge->llink->distance != 9999) {
		if (edge->rlink->distance > (edge->llink->distance + edge->cost)) {
			edge->rlink->distance = (edge->llink->distance + edge->cost);
			check = true;
		}
		// dist(v) = ( dist(u) + w(u,v) < dist(v)) ? dist(u) + w(u,v) : dist(v);

		for (i = 0; i < Max_Vertex; i++) {
			if (edge->rlink == (Vertex_list + i)) {
				Prev_list[i] = edge->rlink;			// prev(v) = u로 지정
				break;
			}
		}
	}
}
void Print_stage() {
	int j = 0;
	for (j = 0; j < Max_Vertex; j++) {
		printf("%c\t", (Vertex_list + j)->Alphabet);
	}
	printf("\n");

	for (j = 0; j < Max_Vertex; j++) {
		printf("%d\t", (Vertex_list + j)->distance);
	}
	printf("\n");
}
void Create_Floyd_WashallGraph() {
	int i = 0;

	Node_list = (Node*)malloc(sizeof(Node) * Max_Node);
	NEdge_list = (Node_Edge*)malloc(sizeof(Node_Edge) * Max_NEdge);

	(Node_list + 0)->number = 1, (Node_list + 1)->number = 2; 
	(Node_list + 2)->number = 3, (Node_list + 3)->number = 4;

	(NEdge_list + 0)->cost = 5, (NEdge_list + 0)->llink = Node_list + 0, (NEdge_list + 0)->rlink = Node_list + 3;
	(NEdge_list + 1)->cost = 2, (NEdge_list + 1)->llink = Node_list + 1, (NEdge_list + 1)->rlink = Node_list + 0;
	(NEdge_list + 2)->cost = 3, (NEdge_list + 2)->llink = Node_list + 0, (NEdge_list + 2)->rlink = Node_list + 1;
	(NEdge_list + 3)->cost = 4, (NEdge_list + 3)->llink = Node_list + 1, (NEdge_list + 3)->rlink = Node_list + 3;
	(NEdge_list + 4)->cost = 2, (NEdge_list + 4)->llink = Node_list + 3, (NEdge_list + 4)->rlink = Node_list + 2;
	(NEdge_list + 5)->cost = 1, (NEdge_list + 5)->llink = Node_list + 2, (NEdge_list + 5)->rlink = Node_list + 1;

	dist_Matrix[0][0] = 0;
	for (i = 1; i < Max_Node + 1; i++) {
		dist_Matrix[0][i] = i;
		dist_Matrix[i][0] = i;
	}
}
void Destroy_Floyd_WashallGraph() {
	free(Node_list);
	free(NEdge_list);
}
void Floyd_Washall_Algorithm() {
	int i = 0, j = 0, k = 0;
	int x, y;
	for (i = 1; i < Max_Node + 1; i++) {
		for (j = 1; j < Max_Node + 1; j++) {
			if (i == j)
				dist_Matrix[i][j] = 0;
			else
				dist_Matrix[i][j] = 9999;
		}
	}

	for (i = 0; i < Max_NEdge; i++) {
		x = (NEdge_list + i)->llink->number;
		y = (NEdge_list + i)->rlink->number;
		dist_Matrix[x][y] = (NEdge_list + i)->cost;
	}

	for (k = 1; k <Max_Node + 1; k++) {
		printf("%d번째 stage\n\n", k);
		for (i = 1; i < Max_Node + 1; i++) {
			for (j = 1; j < Max_Node + 1; j++) {
				update_Floyd(i, j, k);
			}
		}
		Print_Matrix();
	}
}
void update_Floyd(int i, int j, int k) {
	if (dist_Matrix[i][j] > dist_Matrix[i][k] + dist_Matrix[k][j]) {
		dist_Matrix[i][j] = dist_Matrix[i][k] + dist_Matrix[k][j];
	}
	else;
}
void Print_Matrix() {
	int i = 0, j = 0;

	for (i = 0; i < Max_Node + 1; i++) {
		for (j = 0; j < Max_Node + 1; j++) {
			printf("%d\t", dist_Matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}
