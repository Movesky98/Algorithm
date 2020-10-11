// Vertex를 이어주는 Edge의 cost값 중, 무한대 값을 -1으로 표현하였습니다.
// Prim Algorithm에서, Vertex들의 Priority를 알파벳으로 표현하였습니다.

#include <stdio.h>
#include <stdlib.h>

#define Max_Vertex 6
#define Heap_FULL(n) (n == Max_Vertex + 1)
#define Heap_EMPTY(n) (!n)

typedef struct Vertex {
	char alphabet;
	int rank;
}Vertex;

typedef struct Edge {
	int cost;
	struct Vertex* llink;			// Vertex를 가리키는 llink
	struct Vertex* rlink;			// Vertex를 가리키는 rlink
}Edge;

typedef struct Heap {				// Priority에서 사용할 구조체 Heap
	struct Vertex* Node;
}Heap;

typedef struct Tree {
	struct Edge* Edge;
}Tree;

Heap* heap[Max_Vertex+1];
Tree* All_Edge[9];
Edge* Cost[Max_Vertex + 1];				// 최소값을 가진 다리들을 표현하기 위한 Cost(v)
Vertex* Pre_Vertex[Max_Vertex + 1];
int count = 0;
int Edge_count = 9;

char Parent_Node[Max_Vertex + 1] = { 0, };			// Union-find를 이용한 Kruskal Algorithm에서 사용할 Parent_Node 최대 6개.
Vertex* UF_Vertex_list[Max_Vertex + 1];				// Union-find를 이용한 Kruskal Algorithm에서 Vertex들을 담을 list

Heap* Create_Graph(Heap* heap);				// 그래프를 형성하기 위한 함수
Heap* DestroyGraph(Heap* heap);				// 종료하기 전, 할당된 메모리를 해제하기 위한 함수

// KruskalAlgorithm Function
void Destroy_KruskalAlgorithm();
void MakeSet();
Vertex* Find(Vertex* x);
void Union(char a, char b);
void Union_Find_Menu();

// Priority Queue Function
Vertex* findmin();										// Priority queue 에서 priority 가 가장 높은 item 을 반환.
Vertex* deletemin(int* count);					// Priority queue 에서 priority 가 가장 높은 item 을 제거 후 반환.
void Insert(Vertex* item, int* count);			// Priority queue 에 새로운 item 을 추가.
Vertex* Delete_Item(char item, int* count);	// item 를 priority queue 에서 제거하고 반환.
void decreasekey(char item, int* count, char k);		// item 의 priority 를 k 로 변경.

// Prim Algorithm Function
void Prim(Vertex* item);						// Prim Algorithm을 실행하는 함수
Edge* Cost_min(Vertex* x, int* count);		// Vertex x에 대해 cost(x) = min w(x, v) (여기서 w는 Edge (x, v)의 cost이다)
void Sort_minCost();

void main() {
	Heap* Heap = NULL;					// Prim 알고리즘에서 사용할 graph
	int command = 0, command_UF = 0, command_Prim = 0;
	char x, y;
	Vertex* Check = NULL;

	do {
		printf("************************************************\n");
		printf("             명령어를 입력하세요                   \n");
		printf("1. Union-find를 이용한 Kruskal Algorithm\n");
		printf("2. Priority Queue를 이용한 Prim Algorithm\n");
		printf("3. 프로그램 종료\n\n");
		printf("명령어 입력 :");
		scanf_s("%d", &command);
		fflush(stdin);
		printf("\n\n");
		switch (command) {
		case 1:
			Union_Find_Menu();
			break;
		case 2:
			do {
				printf("************************************************\n");
				printf("             명령어를 입력하세요                   \n");
				printf("1. Priority Queue를 이용한 Prim Algorithm 실행\n");
				printf("2. Prim Algorithm 종료\n\n");
				printf("명령어 입력 :");
				scanf_s("%d", &command_Prim);
				printf("\n\n");
				switch(command_Prim){
				case 1:
					Heap = Create_Graph(Heap);
					//Check = Delete_Item(heap[1]->Node->alphabet, &count);
					do {
						Prim((heap[1]->Node));			// A를 기준으로 시작
					} while (count != 0);
					printf("\n\n");
					break;
				case 2:
					DestroyGraph(Heap);
					printf("Prim Algorithm을 종료합니다.\n\n");
					break;
				default:
					printf("잘못 입력하셨습니다. 다시 입력해주세요.\n\n");
					break;
				}
			} while (command_Prim != 2);
			break;
		case 3:
			printf("프로그램을 종료합니다.\n");
			break;
		default:
			printf("잘못 입력하셨습니다. 다시 입력해주세요.\n\n");
			break;
		}
	} while (command != 3);
}
Heap* Create_Graph(Heap* heap_) {				// 그래프를 형성하기 위한 함수
	int i = 0;
	if (heap_ != NULL) {
		DestroyGraph(heap_);
	}
	heap_ = (Heap*)malloc(sizeof(Heap));
	heap_->Node = (Vertex*)malloc(sizeof(Vertex) * Max_Vertex);		// A~F까지 6개를 할당받음
	
	for (i = 0; i < Max_Vertex + 1; i++) {
		heap[i] = (Heap*)malloc(sizeof(Heap));
		Pre_Vertex[i] = (Vertex*)malloc(sizeof(Vertex*));
		Cost[i] = (Edge*)malloc(sizeof(Edge*));
	}
	for (i = 0; i < Edge_count; i++) {
		All_Edge[i] = (Tree*)malloc(sizeof(Tree));
		All_Edge[i]->Edge = (Edge*)malloc(sizeof(Edge));
	}

	(heap_->Node + 0)->alphabet = 'A';
	(heap_->Node + 1)->alphabet = 'B';
	(heap_->Node + 2)->alphabet = 'C';
	(heap_->Node + 3)->alphabet = 'D';
	(heap_->Node + 4)->alphabet = 'E';
	(heap_->Node + 5)->alphabet = 'F';

	count = 0;

	for (i = 0; i < Max_Vertex; i++) {
		Insert((heap_ ->Node + i), &count);
	}
	
	All_Edge[0]->Edge->cost = 4, All_Edge[0]->Edge->llink = heap[1]->Node, All_Edge[0]->Edge->rlink = heap[2]->Node;
	All_Edge[1]->Edge->cost = 1, All_Edge[1]->Edge->llink = heap[1]->Node, All_Edge[1]->Edge->rlink = heap[3]->Node;
	All_Edge[2]->Edge->cost = 3, All_Edge[2]->Edge->llink = heap[1]->Node, All_Edge[2]->Edge->rlink = heap[4]->Node;
	All_Edge[3]->Edge->cost = 4, All_Edge[3]->Edge->llink = heap[2]->Node, All_Edge[3]->Edge->rlink = heap[3]->Node;
	All_Edge[4]->Edge->cost = 4, All_Edge[4]->Edge->llink = heap[2]->Node, All_Edge[4]->Edge->rlink = heap[4]->Node;
	All_Edge[5]->Edge->cost = 2, All_Edge[5]->Edge->llink = heap[3]->Node, All_Edge[5]->Edge->rlink = heap[4]->Node;
	All_Edge[6]->Edge->cost = 4, All_Edge[6]->Edge->llink = heap[3]->Node, All_Edge[6]->Edge->rlink = heap[6]->Node;
	All_Edge[7]->Edge->cost = 6, All_Edge[7]->Edge->llink = heap[4]->Node, All_Edge[7]->Edge->rlink = heap[6]->Node;
	All_Edge[8]->Edge->cost = 5, All_Edge[8]->Edge->llink = heap[5]->Node, All_Edge[8]->Edge->rlink = heap[6]->Node;

	return heap_;					// 형성된 그래프를 반환
}
Heap* DestroyGraph(Heap* heap_) {
	int i = 0;
	free(heap_);
	heap_ = NULL;
	return heap_;
}

// Prim Algorithm
void Prim(Vertex* item) {
	int i = 0, j = 0, k = 0, check = 0;
	Vertex* temp = NULL;
	Edge* min_Edge = NULL;
	Edge* Delete_Edge = NULL;

	Cost[1] = 0;
	Pre_Vertex[1]->alphabet = NULL;

	for (i = 2; i < count; i++) {
		Cost[i]->cost = -1;
		Pre_Vertex[i]->alphabet = NULL;
	}

	printf("Vertex\t : \tEdge\n");
	temp = Delete_Item(item->alphabet, &count);
	Sort_minCost();

	do {
		check = 0;
		for (i = 0; i < Edge_count; i++) {								// Heap에서 반환된 Vertex와 연결된 Edge를 찾기 위한 반복문
			if (All_Edge[i]->Edge->llink == temp || All_Edge[i]->Edge->rlink == temp) {		// Edge의 왼쪽 링크필드에 temp가 있으면, [이전 노드]가 temp이면
				for (j = 1; j < count; j++) {					// [ temp ]와 연결된 노드를 찾기위해 반복문 실행
					if (All_Edge[i]->Edge->rlink == heap[j]->Node || All_Edge[i]->Edge->llink == heap[j]->Node) {
						k = i;
						Pre_Vertex[j]->alphabet = temp->alphabet;
						Cost[j] = All_Edge[i]->Edge;			// cost(v)와 pre(v)값을 설정해준다.
						break;
					}
				}
			}
		}

		for (i = 0; i < count; i++) {
			if ((All_Edge[k]->Edge->rlink == heap[i]->Node) || All_Edge[k]->Edge->llink == heap[i]->Node) {
				check = 1;
				break;
			}
		}

		if(check == 1)
			printf("[ %c ]\t - \t", temp->alphabet);
		else {
			temp = deletemin(&count);
			for (i = 0; i < Edge_count; i++) {								// Heap에서 반환된 Vertex와 연결된 Edge를 찾기 위한 반복문
				if (All_Edge[i]->Edge->llink == temp || All_Edge[i]->Edge->rlink == temp) {		// Edge의 왼쪽 링크필드에 temp가 있으면, [이전 노드]가 temp이면
					if (count != 1) {
						for (j = 1; j < count; j++) {					// [ temp ]와 연결된 노드를 찾기위해 반복문 실행
							if (All_Edge[i]->Edge->rlink == heap[j]->Node || All_Edge[i]->Edge->llink == heap[j]->Node) {
								Pre_Vertex[j]->alphabet = temp->alphabet;
								Cost[j] = All_Edge[i]->Edge;			// cost(v)와 pre(v)값을 설정해준다.
								break;
							}
						}
					}
					else{
						if (All_Edge[i]->Edge->llink == findmin() || All_Edge[i]->Edge->rlink == findmin()) {
							Pre_Vertex[1]->alphabet = findmin()->alphabet;
							Cost[1] = All_Edge[i]->Edge;
						}
					}
				}
			}
			printf("[ %c ]\t - \t", temp->alphabet);
		}

		min_Edge = Cost_min(temp, &count);
		if (min_Edge->rlink->alphabet == temp->alphabet) {
			printf("[ %c ]\n", min_Edge->llink->alphabet);
		}
		else printf("[ %c ]\n", min_Edge->rlink->alphabet);

		Delete_Edge = min_Edge;

		if (count != 0) {
			if (min_Edge->rlink->alphabet == temp->alphabet) {
				temp = Delete_Item(min_Edge->llink->alphabet, &count);
			}
			else temp = Delete_Item(min_Edge->rlink->alphabet, &count);
		}

		free(Delete_Edge);
		Delete_Edge = NULL;

		if (count == 0) {
			printf("[ %c ]\t - \t", temp->alphabet);
			k = 1;
			for (i = 0; i < Max_Vertex; i++) {
				Cost[i]->cost = 0;
			}
			for (i = 0; i < Edge_count; i++) {								// Heap에서 반환된 Vertex와 연결된 Edge를 찾기 위한 반복문
				if (All_Edge[i]->Edge->llink == temp) {		// Edge의 왼쪽 링크필드에 temp가 있으면, [이전 노드]가 temp이면
					Pre_Vertex[0]->alphabet = temp->alphabet;
					Cost[k] = All_Edge[i]->Edge;			// cost(v)와 pre(v)값을 설정해준다
					k++;
				}
				else if (All_Edge[i]->Edge->rlink == temp) {
					Pre_Vertex[0]->alphabet = temp->alphabet;
					Cost[k] = All_Edge[i]->Edge;			// cost(v)와 pre(v)값을 설정해준다
					k++;
				}
			}
			j = 1;		// 최소값으로 이용할 j
			for (i = 1; i < Max_Vertex; i++) {
				if (Cost[i]->cost > 0 && Cost[i] < Cost[j])
					j = i;
			}
			min_Edge = Cost[j];
			if (min_Edge->rlink->alphabet == temp->alphabet) {
				printf("[ %c ]\n", min_Edge->llink->alphabet);
			}
			else printf("[ %c ]\n", min_Edge->rlink->alphabet);
		}
		Sort_minCost();
		
	} while (!Heap_EMPTY(count));
	return;
}
// Vertex x에 대해 cost(x) = min w(x, v) (여기서 w는 Edge (x, v)의 cost이다)
// 가장 작은 cost 값을 가진 Edge를 반환
Edge* Cost_min(Vertex* x, int* count) {
	int i = 0, min = 0;
	if (*count == 1) {
		return Cost[1];
	}
	else if (*count == 0) {
		for (i = 1; i < Max_Vertex; i++) {
			if (Cost[i]->cost != 0) {
				min = i;
			}
		}
		for (i = 1; i < Max_Vertex + 1; i++) {
			if (Cost[i]->cost != 0) {
				if (Cost[i]->cost < Cost[min]->cost)
					min = i;
			}
		}
		return Cost[min];
	}
		
	for (i = 1; i < *count; i++) {
		if (Cost[i]->llink == x) {
			min = i;
			break;
		}
	}
	
		if (i == *count)
			for (i = 1; i < *count; i++) {
				if (Cost[i]->rlink == x) {
					min = i;
					break;
				}
			}
		for (i = 1; i < *count; i++) {
			if (Pre_Vertex[i]->alphabet == x->alphabet)
				if (Cost[i]->cost > 0 && Cost[min]->cost > Cost[i]->cost)
					min = i;
		}
	
	return Cost[min];
}
// Vertex x에 대해 Edge (u, x)일때의 이전 Vertex u를 반환 해주는 함수
void Sort_minCost() {
	int min = 0, i = 0, j = 0;
	Edge* temp = NULL;

	for (i = 0; i < Edge_count; i++) {
		min = i;
		for (j = i + 1; j < Edge_count; j++) {
			if (All_Edge[i]->Edge->cost > 0 && All_Edge[j]->Edge->cost > 0) {
				if (All_Edge[i]->Edge->cost > All_Edge[j]->Edge->cost) {
					temp = All_Edge[j];
					All_Edge[j] = All_Edge[i];
					All_Edge[i] = temp;
					min = j;
				}
			}
		}
	}
}


/* Priority Queue */
// Priority queue 에 새로운 item 을 추가.
void Insert(Vertex* item, int* count) {
	int i;
	
	if (Heap_FULL(*count)) {
		printf("The Heap is Full!!\n\n");
		free(item);
		return;
	}
	i = ++(*count);
	while ((i != 1) && item->alphabet < heap[i/2]->Node->alphabet) {			// 'A'~'Z'까지 && 'A'의 우선순위가 가장 크고, 'Z'의 우선순위가 가장 작은 것으로 할 것임.
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i]->Node = item;
	heap[i]->Node->alphabet = item->alphabet;
	heap[i]->Node->rank = 0;
}
// Priority queue 에서 priority 가 가장 높은 item 을 제거 후 반환.
Vertex* deletemin(int* count) {
	int parent, child;
	Vertex* item = NULL; 
	Vertex* temp = NULL;

	if (Heap_EMPTY(*count)) {
		printf("The Heap is Empty!!! \n\n");
		return 0;
	}
	item = heap[1]->Node;
	heap[1]->Node = NULL;
	temp = heap[(*count)--]->Node;
	heap[*count + 1]->Node = NULL;

	parent = 1;
	child = 2;
	while (child <= *count) {
		if (child < *count && (heap[child]->Node->alphabet > heap[child + 1]->Node->alphabet))			// [ child에 위치해있는 alphabet ]이 [ child+1의 alphabet ]보다 우선순위가 낮으면
			child++;					// 위치이동
		if (temp->alphabet <= heap[child]->Node->alphabet) break;
		heap[parent]->Node = heap[child]->Node;
		parent = child;
		child *= 2;
	}
	heap[parent]->Node = temp;
	return item;
}
// Priority queue 에서 priority 가 가장 높은 item 을 반환.
Vertex* findmin() {
	Vertex* c = NULL;
	c = heap[1]->Node;
	return c;
}
// item 를 priority queue 에서 제거하고 반환.
Vertex* Delete_Item(char item, int* count) {	
	// determin과 비슷한 방식으로 해당 heap의 node를 제거해주면 된다.
	int location = 0, child = 0, i;					// 삭제할 노드의 위치를 표시할 location
	Vertex* destroy = NULL; 
	Vertex* temp = NULL;

	if (Heap_EMPTY(*count)) {
		printf("The Heap is Empty!!!\n\n");
		return 0;
	}
	
	for (i = 1; i < *count; i++) {
		if (item == heap[i]->Node->alphabet) {
			location = i;
			break;
		}
	}

	destroy = heap[location]->Node;
	heap[location]->Node = NULL;
	temp = heap[(*count)--]->Node;
	heap[*count + 1]->Node = NULL;
	child = location+1;

	while (child <= *count) {
		if (child < *count && (heap[child]->Node->alphabet > heap[child + 1]->Node->alphabet))			// [ child에 위치해있는 alphabet ]이 [ child+1의 alphabet ]보다 우선순위가 낮으면
			child++;					// 위치이동
		if (temp->alphabet <= heap[child]->Node->alphabet) break;
		heap[location]->Node = heap[child]->Node;
		location = child;
		child *= 2;
	}
	if (location != 0) {
		heap[location]->Node = temp;
	}
	else if (location == 0) {
		return temp;
	}

	return destroy;
}
// item 의 priority 를 k 로 변경. 
void decreasekey(char item, int* count, char k) {	
	// Delete_item과 Insert를 이용해 구현 가능
	// 알파벳으로 표현했기에, k를 알파벳으로 받았습니다.
	Vertex* temp;

	if (Heap_EMPTY(*count)) {
		printf("The Heap is Empty!!!\n\n");
		return;
	}
	temp = Delete_Item(item, count);
	temp->alphabet = k;
	Insert(temp, count);
}

//Kruskal Algorithm
void MakeSet() {
	int i = 0;
	Vertex* x[Max_Vertex + 1];
	for (i = 0; i < Max_Vertex + 1; i++) {
		UF_Vertex_list[i] = (Vertex*)malloc(sizeof(Vertex));
		x[i] = (Vertex*)malloc(sizeof(Vertex));
	}

	x[0]->alphabet = 'A';
	x[1]->alphabet = 'B'; 
	x[2]->alphabet = 'C';
	x[3]->alphabet = 'D';
	x[4]->alphabet = 'E';
	x[5]->alphabet = 'F';
	x[6]->alphabet = 'G';

	for (i = 0; i < Max_Vertex + 1; i++) {
		if (Parent_Node[i] == 0) {
			UF_Vertex_list[i]->alphabet = x[i]->alphabet;
			(UF_Vertex_list[i])->rank = 0;
			Parent_Node[i] = x[i]->alphabet;
		}
	}
	for(i=0;i<Max_Vertex + 1; i++)
		free(x[i]);
}
Vertex* Find(Vertex* x) {	
	int i = 0, j = 0;

	for (i = 0; i < Max_Vertex + 1; i++) {
		if ((UF_Vertex_list[i]) == x) {
			if (Parent_Node[i] == x->alphabet) {
				return x;
			}
			else {
				for (j = 0; j < Max_Vertex + 1; j++) {
					if (Parent_Node[i] == (UF_Vertex_list[j])->alphabet)
						break;
				}
				return Find((UF_Vertex_list[j]));
			}
			break;
		}
	}
}
void Union(char a, char b) {
	int i = 0;
	Vertex* x = NULL; 
	Vertex* y = NULL;

	for (i = 0; i < Max_Vertex + 1; i++) {
		if ((UF_Vertex_list[i])->alphabet == a)
			x = (UF_Vertex_list[i]);
		if ((UF_Vertex_list[i])->alphabet == b)
			y = (UF_Vertex_list[i]);
		
		if (x != NULL && y != NULL)
			break;
	}

	x = Find(x);
	y = Find(y);

	if (x == y) {
		printf("x와 y가 같은 Vertex입니다.\n");
		return;
	}
	
	if (x->rank > y->rank) {
		for (i = 0; i < Max_Vertex + 1; i++) {
			if ((UF_Vertex_list[i]) == y) {
				Parent_Node[i] = x->alphabet;
				return;
			 }
		}
	}
	else {
		for (i = 0; i < Max_Vertex + 1; i++) {
			if((UF_Vertex_list[i]) != NULL)
				if ((UF_Vertex_list[i])->alphabet == x->alphabet) {
					Parent_Node[i] = y->alphabet;
					if (x->rank == y->rank)
						y->rank = x->rank + 1;
					return;
				}
		}
	}
}
void Destroy_KruskalAlgorithm() {
	int i = 0;

	for (i = 0; i < Max_Vertex + 1; i++) {
		free(UF_Vertex_list[i]);
		Parent_Node[i] = 0;
	}
}
void Union_Find_Menu() {
	int command_UF, i = 0;;
	
	do {
		printf("************************************************\n");
		printf("             명령어를 입력하세요                   \n");
		printf("1. Vertex들을 출력\n");
		printf("2. Union()\n");
		printf("3. Union-find를 이용한 Kruskal Algorithm 종료.\n\n");
		printf("명령어 입력 :");
		scanf_s("%d", &command_UF);
		printf("\n\n");

		switch (command_UF) {
		case 1:
			MakeSet();
			for (i = 0; i < Max_Vertex + 1; i++) {
				printf("%c\t", Parent_Node[i]);
			}
			printf("\n");
			for (i = 0; i < Max_Vertex + 1; i++) {
				printf("%d\t", UF_Vertex_list[i]->rank);
			}
			printf("\n\n");
			break;
		case 2:
			Union('A', 'D');
			Union('B', 'E');	
			Union('C', 'F');
			Union('C', 'G');
			Union('E', 'A');
			Union('B', 'G');
			printf("A\tB\tC\tD\tE\tF\tG\n");
			for (i = 0; i < Max_Vertex + 1; i++) {
				printf("%c\t", Parent_Node[i]);
			}
			printf("\n");
			for (i = 0; i < Max_Vertex + 1; i++) {
				printf("%d\t", UF_Vertex_list[i]->rank);
			}
			printf("\n\n");
			break; 

		case 3:
			Destroy_KruskalAlgorithm();
			printf("Kruskal Algorithm을 종료합니다.\n\n");
			break;
		default:
			printf("잘못 입력하셨습니다. 다시 입력해주세요.\n\n");
			break;
		}

	} while (command_UF != 3);
}
