// Vertex u와 v가 adjacent한지 체크
// Vertex u와 adjacent한 모든 Vertex 탐색
// deg(u) 답하기

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 20           // 최대 Vertex값
int adj_matrix[MAX_VERTEX][MAX_VERTEX] = { 0 };

typedef struct Vertex {
    int num;                            // Vertex 번호
    struct Vertex* link;               // 인접한 Vertex를 표현하기 위한 link
} Vertex;

typedef struct VertexHead {
    Vertex* head;           
} VertexHead;

typedef struct Graph {
    VertexHead* vlist;
} Graph;


Graph* createGraph(Graph* graph);  
int destroyGraph(Graph* graph); 
int insertVertex(Graph* graph, int v_num);
int insertEdge(Graph* graph, int v1_num, int v2_num);   
int Print_List_Graph(Graph* graph);   
int Check_vlist(Graph* graph, int vertex);      // vlist에 해당하는 Vertex가 존재하는지 확인하기 위한 함수
int Print_Matrix_Graph(Graph* graph);
int Adjacent_Vertex_u(Graph* graph, int v);
int Check_Vertex(Graph* graph, int v1, int v2);
int Degree_Vertex_u(Graph* graph, int v);

int vertex_count = 0;          // 그래프에 삽인된 Vertex들의 개수를 세기 위한 변수 vertex_count

int main() {
    char command;                                   // 명령어를 입력 받기 위한 변수 command
    int v1_num;                                     // 그래프에 Vertex를 입력받기 위한 변수 v1_num
    int v2_num;                                     // 그래프에 Edge를 insert할 때, Vertex 두개를 받기위한 변수 v2_num
    Graph* graph = NULL;                            // 그래프를 나타내기 위한 포인터 변수 graph

    do {
        printf("----------------------------------------------------------------\n");
        printf(" Please Enter the command.\n");
        printf(" Create Graph           = z     Print Adjacent list = p\n");
        printf(" Insert Vertex          = i     Print Adjacent Matix = o\n");
        printf(" Insert Edge            = e     Check Vertex u & v  = c\n");
        printf(" Adjacent with Vertex u = u     Degree ( u ) = d\n");
        printf(" Quit           = q\n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf_s(" %c", &command);             // 명령어 입력

        switch (command) {
        case 'z': case 'Z':                         // z or Z를 입력 받으면
            graph = createGraph(graph);     // 그래프 초기화
            break;
        case 'p': case 'P':                                // p or P를 입력 받으면
            Print_List_Graph(graph);                   // 그래프에 있는 Vertex들을 출력
            break;
        case 'i': case 'I':                             // i or I를 입력 받으면
            printf("Vertex Number : ");
            scanf_s("%d", &v1_num);             // 양의 정수 입력
            insertVertex(graph, v1_num);        // Vertex를 삽입한다.
            break;
        case 'e': case 'E':                                     // e or E를 입력 받으면
            printf("Vertex Number (ex:1 2) : ");
            scanf_s("%d %d", &v1_num, &v2_num);   // 다리를 형성할 Vertex들을 입력
            insertEdge(graph, v1_num, v2_num);       // (v1_num, v2_num) Edge 형성
            break;
        case 'o': case 'O':                                  // o or O를 입력받으면
            Print_Matrix_Graph(graph);                  // Matrix 형태로 그래프를 출력
            break;
        case 'c': case 'C':                                   // c or C를 입력받으면
            printf("Vertex Number (ex:1 2) : ");
            scanf_s("%d %d", &v1_num, &v2_num);  
            Check_Vertex(graph, v1_num, v2_num);    // v1과 v2가 인접한지 비교하는 함수를 실행
            break;
        case 'u': case'U':                          // u or U를 입력받으면
            printf("Vertex Number : ");
            scanf_s("%d", &v1_num);             // 양의 정수 입력
            Adjacent_Vertex_u(graph, v1_num); //v1과 인접한 Vertex들을 출력
            break;
        case 'd': case 'D':                             // d or D를 입력받으면
            printf("Vertex Number : ");
            scanf_s("%d", &v1_num);             // 양의 정수 입력
            Degree_Vertex_u(graph, v1_num);     // v1의 Degree를 출력
            break;
        case 'q': case 'Q':                 // q or Q를 입력 받으면
            destroyGraph(graph);       // 프로그램을 종료하기 전, 동적할당 받은 메모리들을 해제시켜주기 위한 함수 호출
            break;
        default:                             // 그 외 다른 명령어를 입력 받으면
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");      // 오류 메세지 출력
            break;
        }

    } while (command != 'q' && command != 'Q');   // q or Q를 입력 받으면 반복문을 빠져나온다.

    return 1;           // 프로그램 종료.
}

Graph* createGraph(Graph* graph) {      // 그래프의 형태를 만들기 위한 함수
    int i = 0;     

    if (graph != NULL) {        // 그래프가 비어있지 않으면
        destroyGraph(graph);    // 초기화 시키기 위해 동적할당을 받은 메모리들을 해제시켜준다.
    }

    graph = (Graph*)malloc(sizeof(Graph));      
    graph->vlist = (VertexHead*)malloc(sizeof(VertexHead) * MAX_VERTEX);    // Vertex들이 표현될 공간인 vlist를 MAX_VERTEX 크기만큼 동적할당 

    for (i = 0; i < MAX_VERTEX; i++) {          // vlist의 각 head들을 NULL로 초기화
        ((graph->vlist) + i)->head = NULL;
    }

    return graph;               // 형성된, 초기화된 그래프를 반환
}

int destroyGraph(Graph* graph) {   // 그래프를 초기화 시키기 위한 함수
    int i = 0;                      
    Vertex* temp = NULL;              // Vertex와 Edge를 가리킬 포인터 변수 temp
    Vertex* delete_Vertex = NULL;   // 삭제할 Vertex를 가리킬 포인터 변수 delete_Vertex

    if (graph == NULL) {            // 그래프가 이미 비어있으면
        return 1;                       // 정상 종료
    }

    for (i = 0; i < vertex_count; i++) {        // 그래프의 동적할당된 메모리들을 반복문을 이용해 해제
        temp = (graph->vlist + i)->head;     // temp를 각 vlist의 head로 지정

        if ((graph->vlist + i)->head != NULL) {     // 만약 vlist의 head에 Vertex가 존재하면

            while (temp != NULL) {
                delete_Vertex = temp;               // vlist의 head부터 Edge까지 메모리 해제
                temp = temp->link;
                free(delete_Vertex);
            }
            (graph->vlist + i)->head = NULL;        // vlist의 head가 없어졌으므로 NULL로 지정
        }
    }
    free(graph->vlist);         // 동적할당 받았던 vlist의 메모리 해제

    free(graph);                // 동적할당 받았던 graph의 메모리 해제
    graph = NULL;               // 이후 graph를 NULL로 지정
    vertex_count = 0;
    return 1;
}

/* 이미 들어간 Vertex가 있었을 경우도 생각해야함. */
int insertVertex(Graph* graph, int v_num) {      /* vertex insertion */
    int i = 0;                  // 반복문을 운용하기 위한 변수 i

    if (graph == NULL) {                                // 그래프가 형성되어있지 않을 경우
        printf("You didn't form a graph.\n");       // 오류메세지 출력
        return 0;                                           // 함수 종료
    }

    if (Check_vlist(graph, v_num) == 1) {             // 벌텍스가 이미 존재할 경우 Check_vlist()함수에서 1을 반환
        printf("The vertex is already Exist\n");        // 이미 존재한다는 메세지 출력
        return 1;                                              // 함수 종료
    }

    Vertex* temp = (Vertex*)malloc(sizeof(Vertex));  // Vertex를 삽입하기 위해 동적할당을 받음
    i = 0;  // i = 0부터

    while ((graph->vlist + i)->head != NULL) {      // 그래프에 들어있는 Vertex들의 개수가 Max_Vertex를 넘었는지 확인하기 위해
        i++;                                                    // 반복문을 이용
    }

    if (i == MAX_VERTEX) {                          // 중간 점검. 만약 graph에서 Vertex의 최댓값을 넘었을 경우(더이상 못넣을 경우)
        printf("Maximum Vertex value exceeded.\n"); // 오류메세지 출력
        return 0;                                   // 함수 종료
    }

    temp->num = v_num;                  // 삽입할 Vertex에 입력한 정수를 저장
    temp->link = NULL;                  // Vertex를 삽입하므로 Edge가 없기때문에 NULL로 저장
    (graph->vlist + i)->head = temp;    // 해당 Vertex를 vlist에 삽입

    vertex_count++;
    return 1;
}

/* 이미 놓여진 Edge가 있었을 경우도 생각 해야함. */
int insertEdge(Graph* graph, int v1_num, int v2_num) {        /* new edge creation between two vertices */

    if (Check_vlist(graph, v1_num) == 0 || Check_vlist(graph, v2_num) == 0) {     // v2_num, v1_num이 그래프에 있는지 조사
        printf("You have entered Vertex that does not exist in the graph.\n");  // 둘중 하나라도 없으면 오류메시지 출력
        return 0;   // 함수종료
    }

    int i = 0, j = 0;          // 반복문을 운용하기 위한 변수 i
    Vertex* V1 = (Vertex*)malloc(sizeof(Vertex));       // (a ,b)일 경우 a, b 양쪽에 대입해야하므로
    Vertex* V2 = (Vertex*)malloc(sizeof(Vertex));       // 동적할당을 두개 선언해준다.
    Vertex* temp = NULL;                                // 삽입될 위치를 가리키는 temp

    V1->num = v1_num;               // 각 V1,V2에 입력된 값을 대입
    V2->num = v2_num;

    for (i = 0; i < vertex_count; i++) {   // 반복문 시작
        if ((graph->vlist + i)->head != NULL) {             // 만약 Vertex가 존재하면
            if ((graph->vlist + i)->head->num == v1_num) {  // 해당 Vertex가 입력된 값과 같은지 비교

                temp = (graph->vlist + i)->head;        // 만약 값이 같으면 temp를 Vertex로 지정

                while (temp->link != NULL) {              // Vertex의 인접리스트(Edge)의 끝에 도달할때까지
                    temp = temp->link;                  // temp 값 이동
                    if (temp->num == v2_num) {            // 만약 이미 Edge가 놓여있을 경우
                        printf("The Edge is already Placed.\n");    // 오류 메시지 출력
                        return 1;                       // 함수 종료
                    }
                }                                       // 이미 Edge가 놓여있지 않으면
                temp->link = V2;                        // Vertex의 Edge 마지막에 V2를 이어붙이고
                V2->link = NULL;                        // V2의 link에 NULL 지정
                for (j = 0; j < vertex_count; j++) {
                    if ((graph->vlist + j)->head->num == temp->link->num) {
                        adj_matrix[i][j] = 1;
                        adj_matrix[j][i] = 1;
                    }
                }
            }
            else if ((graph->vlist + i)->head->num == v2_num) { // 만약 Vertex가 입력된 값과 같으면

                temp = (graph->vlist + i)->head;             // temp가 해당 Vertex를 가리키도록 지정

                while (temp->link != NULL) {                  // temp가 Vertex의 인접리스트(Edge)의 끝에 도달할때까지
                    temp = temp->link;                      // temp 값 이동
                    if (temp->num == v1_num) {                // 만약 이미 (Edge)값이 존재하면
                        printf("The Edge is already Placed.\n");    // 오류메시지 출력
                        return 1;                           // 함수 종료
                    }
                }

                temp->link = V1;                            // Vertex의 인접리스트(Edge)의 마지막에는 V1을 지정
                V1->link = NULL;                            // V1이 마지막 Edge이므로 link에 NULL지정
                for (j = 0; j < vertex_count; j++) {
                    if ((graph->vlist + j)->head->num == temp->link->num) {
                        adj_matrix[i][j] = 1;
                        adj_matrix[j][i] = 1;
                    }
                }
            }
        }
    }

    return 1;                                               // 함수 종료
}

int Print_List_Graph(Graph* graph) {              /* printing graph with vertices and edges */
    int i = 0;          // 반복문을 운용하기 위한 변수 i

    if (graph == NULL) {  // 그래프가 형성되어 있지 않으면
        printf("You didn't form a graph.\n");   // 오류 메세지 출력
        return 0;       // 함수 종료
    }

    Vertex* temp = NULL;    // Vertex와 Edge를 가리킬 포인터 변수 temp

    printf("Vertex\tEdge\t\n");         // Vertex와 Edge 출력

    for (i = 0; i < vertex_count; i++) {       // 반복문 실행
        if ((graph->vlist + i)->head != NULL) {                 // Vertex가 존재하면

            printf("[ %d ] :\t", (graph->vlist + i)->head->num);  // Vertex부터 출력

            temp = (graph->vlist + i)->head->link;            // temp가 Vertex의 Edge를 가리키도록 지정

            while (temp != NULL) {                            // 해당 Vertex의 모든 Edge 출력
                printf("[ %d ]\t", temp->num);
                temp = temp->link;
            }
            printf("\n");
        }
    }
    printf("\n\n vertex_count : %d\n", vertex_count);        // 마지막으로 delete_count, vertex_count 출력
    return 1;
}

int Check_vlist(Graph* graph, int vertex) {      // vlist에 해당하는 Vertex가 존재하는지 확인하기 위한 함수
    int i = 0;      // 반복문을 돌리기 위한 변수 i

    for (i = 0; i < vertex_count; i++) {   // 반복문 실행
        if ((graph->vlist + i)->head != NULL)              // Vertex가 존재하면
            if ((graph->vlist + i)->head->num == vertex)   // Vertex의 값이 입력된 vertex값과 같은지 비교
                break;                                  // 같으면 함수를 종료한다.
    }

    if (i == vertex_count) {       // 만약 존재하는 Vertex의 끝까지 돌려도 못찾았다면
        return 0;                               // 해당 vertex가 존재하지 않다는 문구 출력 후 종료
    }
    else return 1;                             // 해당 vertex가 존재할 경우 1을 반환 후 종료

}
int Print_Matrix_Graph(Graph* graph) {
    int i = 0, j = 0, k = 0;
    Vertex* temp = NULL;

    if (graph == NULL) {  // 그래프가 형성되어 있지 않으면
        printf("You didn't form a graph.\n");   // 오류 메세지 출력
        return 0;       // 함수 종료
    }

    printf("Vertex\t");
    for (i = 0; i < vertex_count; i++) {
        if ((graph->vlist + i)->head != NULL)
            printf("[ %d ]\t", (graph->vlist + i)->head->num);
        if (i == vertex_count - 1)
            printf("\n");
    }

    for (i = 0; i < vertex_count; i++) {
        if ((graph->vlist + i)->head != NULL)
            printf("[ %d ]\t", (graph->vlist + i)->head->num);
        
        for (j = 0; j < vertex_count; j++) {
            printf("[ %d ]\t", adj_matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
int Adjacent_Vertex_u(Graph* graph, int v) {
    int i = 0;
    Vertex* temp = NULL;

    if (Check_vlist(graph, v) == 0) {     // v1이 그래프에 있는지 조사
        printf("You have entered Vertex that does not exist in the graph.\n");  // 둘중 하나라도 없으면 오류메시지 출력
        return 0;   // 함수종료
    }

    printf("Vertex adjacent to Vertex [ %d ] :", v);

    for (i = 0; i < vertex_count; i++) {
        if ((graph->vlist + i)->head->num == v) {
            temp = (graph->vlist + i)->head->link;

            while (temp != NULL) {
                printf("[ %d ]\t", temp->num);
                temp = temp->link;
            }
            printf("\n\n");
            return 0;
        }
    }
}
int Check_Vertex(Graph* graph, int v1, int v2) {
    int i = 0;
    Vertex* temp = NULL;

    if (Check_vlist(graph, v1) == 0 || Check_vlist(graph, v2) == 0) {     // v2_num, v1_num이 그래프에 있는지 조사
        printf("You have entered Vertex that does not exist in the graph.\n");  // 둘중 하나라도 없으면 오류메시지 출력
        return 0;   // 함수종료
    }

    for (i = 0; i < vertex_count; i++) {
        if ((graph->vlist + i)->head->num == v1) {
            temp = (graph->vlist + i)->head->link;

            while (temp != NULL) {
                if (temp->num == v2) {
                    printf("[ %d ] and [ %d ] are adjacent.\n\n", v1, v2);
                    return 0;
                }
                temp = temp->link;
            }
            printf("[ %d ] and [ %d ] are not adjacent.\n\n", v1, v2);
            return 0;;
        }
    }
}
int Degree_Vertex_u(Graph* graph, int v) {
    int i = 0, count_degree = 0;
    Vertex* temp = NULL;

    if (Check_vlist(graph, v) == 0) {     // v1이 그래프에 있는지 조사
        printf("You have entered Vertex that does not exist in the graph.\n");  // 둘중 하나라도 없으면 오류메시지 출력
        return 0;   // 함수종료
    }

    for (i = 0; i < vertex_count; i++) {
        if ((graph->vlist + i)->head->num == v) {
            temp = (graph->vlist + i)->head->link;

            while (temp != NULL) {
                count_degree++;
                temp = temp->link;
            }
            printf("deg(u) = %d\n\n", count_degree);
            return 0;
        }
    }
}
