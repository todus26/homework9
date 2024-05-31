#include <stdio.h>
#include <stdlib.h>

#define MAX_Vertex 10       // 정점의 최대 개수.

// Grapg를 Adjacency List로 구현하기 위한 구조체.
typedef struct Edge {       // edge
    struct Edge* link;      // 다음 edge를 가리키는 포인터
    int vertex;             // edge가 연결된 정점 (가리키는 vertex 번호)
} Edge;

// Vertex를 인접 리스트로 구현하기 위한 구조체
typedef struct Vertex {     // 정점 구조체
    int data;               // 정점의 데이터
    struct Edge* link;      // 다음 간선을 가리키는 포인터
    int visitflag;          // 방문 여부를 나타내는 플래그
} Vertex;

// 배열로 선언하여 index에 맞게 vertex 번호를 가리키는 구조체
typedef struct Vertexhead { // 정점 헤드 구조체
    struct Vertex* link;    // 다음 정점을 가리키는 포인터
} Vertexhead;

int currentvertex = 0;      // 현재 정점의 개수

// 스택을 구현하기 위한
#define MAX_STACK_SIZE 20   // 스택의 최대 크기

int stack[MAX_STACK_SIZE];  // 스택
int top = -1;               // 스택의 top
int currentstack = 0;       // 현재 스택의 크기

int pop();                  // 스택에서 pop
void push(int aVertex);     // 스택에 push

// 큐를 구현하기 위한
#define MAX_QUEUE_SIZE 20   // 큐의 최대 크기
int queue[MAX_QUEUE_SIZE];  // 큐
int front = -1;             // front
int rear = -1;              // rear
int currentqueue = 0;       // 현재 큐의 크기

int deQueue();              // 큐에서 deQueue
void enQueue(int aVertex);  // 큐에 enQueue

void InitializeGraph(Vertexhead* v);    // 그래프 초기화
void FreeGraph(Vertexhead* v);          // 그래프 해제

void Insertvertex(Vertexhead* v);       // 정점 삽입
void Insertedge(Vertexhead* v);         // 간선 삽입

void DFS_iterative(Vertexhead* v);      // 깊이 우선 탐색
void BFS_itertative(Vertexhead* v);     // 너비 우선 탐색

int find_least_index(Vertexhead* v);                // 방문하지 않은 정점 중 가장 작은 인덱스 찾기
int find_next_index(Vertexhead* head, Vertex* v);   // 다음 방문할 정점 찾기

void print_graph(Vertexhead* head);                 // 그래프 출력

int main()
{
    char command;                                   // 명령어       
    Vertexhead headList[MAX_Vertex];                // 정점 헤드 배열

	printf("----------[정새연] [2023041074]----------\n");

    do {
        printf("\n\n");
        printf("----------------------------------------------------------------\n");
        printf("                        Graph Searches                          \n");
        printf("----------------------------------------------------------------\n");
        printf(" Initialize Graph     = z                                       \n");
        printf(" Insert Vertex        = v      Insert Edge                  = e \n");
        printf(" Depth First Search   = d      Breadth First Search         = b \n");
        printf(" Print Graph          = p      Quit                         = q \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch (command) {                              // command에 따라 다른 함수 호출
        case 'z': case 'Z':                             // z를 입력하면
            InitializeGraph(headList);                  // InitializeGraph 함수 호출
            break;  

        case 'v': case 'V':                             // v를 입력하면
            if (currentvertex >= MAX_Vertex)            // 정점의 개수가 최대 개수보다 (같거나) 크면
            {
                printf("Vertex count is Max(10)\n");    // 정점의 개수가 최대 개수임을 출력
                break;
            }                                           // 정점의 개수가 최대 개수보다 작으면
            Insertvertex(headList);                     // Insertvertex 함수 호출
            break;

        case 'e': case 'E':                             // e를 입력하면
            Insertedge(headList);                       // Insertedge 함수 호출
            break;

        case 'd': case 'D':                             // d를 입력하면
            DFS_iterative(headList);                    // DFS_iterative 함수 호출
            break;

        case 'b': case 'B':                             // b를 입력하면
            BFS_itertative(headList);                   // BFS_itertative 함수 호출
            break;

        case 'p': case 'P':                             // p를 입력하면
            print_graph(headList);                      // print_graph 함수 호출
            break;

        case 'q': case 'Q':                             // q를 입력하면
            break;

        default:                                                     
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    } while (command != 'q' && command != 'Q');         // q를 입력할 때까지 반복
    FreeGraph(headList);                                // FreeGraph 함수 호출을 통해 headList에 할당된 메모리 해제
    return 1;                                           // 프로그램 종료
}

void InitializeGraph(Vertexhead* v)                     // 그래프 초기화
{
    if (currentvertex != 0)                             // 현재 정점의 개수가 0이 아니면
        FreeGraph(v);                                   // FreeGraph 함수 호출을 통해 v에 할당된 메모리 해제

    for (int i = 0; i < MAX_Vertex; i++)                // 정점의 개수만큼 반복
        v[i].link = NULL;                               // 각 정점의 link를 NULL로 초기화
    currentvertex = 0;                                  // 현재 정점의 개수를 0으로 초기화

    top = -1;                                           // top을 -1로 초기화
    currentstack = 0;                                   // 현재 스택의 크기를 0으로 초기화
    front = -1;                                         // front를 -1로 초기화
    rear = -1;                                          // rear를 -1로 초기화                 
    currentqueue = 0;                                   // 현재 큐의 크기를 0으로 초기화
    for (int i = 0; i < MAX_STACK_SIZE; i++)            // 스택의 크기만큼 반복
    {
        stack[i] = 0;                                   // 스택의 각 원소를 0으로 초기화
        queue[i] = 0;                                   // 큐의 각 원소를 0으로 초기화
    }
}

void FreeGraph(Vertexhead* v)                           // 그래프 해제             
{
    Vertexhead* head;                                   // 정점 헤드 포인터
    Vertex* vertex;                                     // 정점 포인터
    Edge* temp, * next;                                 // 간선 포인터
    int i = 0;                                  
    if (currentvertex == 0)                             // 현재 정점의 개수가 0이면
        return;                                         // 함수 종료

    for (head = &v[0]; i < MAX_Vertex; i++, head = &v[i])   // 정점의 개수만큼 반복
    {
        while (1)                                                           
        {
            if (head->link == NULL)                         // head의 link가 NULL이면
                break;
            else if (head->link->link == NULL)                  
            {
                free(head->link);                           // head의 link에 할당된 메모리 해제
                head->link = NULL;                          // head의 link를 NULL로 초기화
            }
            else if (head->link->link->link == NULL)        
            {
                free(head->link->link);                     // head의 link의 link에 할당된 메모리 해제
                free(head->link);                           // head의 link에 할당된 메모리 해제
                head->link = NULL;                          // head의 link를 NULL로 초기화
            }
            else
            {
                temp = head->link->link;                    // temp에 head의 link의 link 저장
                next = temp->link;                          // next에 temp의 link 저장
                while (next != NULL)                        // next가 NULL이 아닐 때까지 반복
                {
                    free(temp);                             // temp에 할당된 메모리 해제
                    temp = next;                            // temp에 next 저장
                    next = next->link;                      // next에 next의 link 저장
                }
                free(temp);                                 // temp에 할당된 메모리 해제
                free(head->link);                           // head의 link에 할당된 메모리 해제
                head->link = NULL;                          // head의 link를 NULL로 초기화
            }
        }
    }
}

void Insertvertex(Vertexhead* v)                        // 정점 삽입                        
{   
    int vertexnumber = 0, data = 0;                     // 정점 번호, 데이터
    printf("Vertex Number (0~9) : ");                   
    scanf("%d", &vertexnumber);                         // 정점 번호 입력

    if (vertexnumber >= 0 && vertexnumber <= 9)         // 정점 번호가 0 이상 9 이하이면
    {
        if (v[vertexnumber].link != NULL)               // 정점이 이미 존재하면
        {
            printf("Already existing vertex\n");        // 메세지 출력
            return;
        }
    }
    else                                                // 정점 번호가 0 이상 9 이하가 아니면
    {
        printf("Wrong input (0~9)\n");                  
        return;
    }

    v[vertexnumber].link = (Vertex*)malloc(sizeof(Vertex));     // 정점에 메모리 할당
    v[vertexnumber].link->visitflag = 0;                        // 방문 여부 플래그를 0으로 초기화
    v[vertexnumber].link->link = NULL;                          // link를 NULL로 초기화
    printf("Data : ");                                          // 데이터 입력
    scanf("%d", &data);
    v[vertexnumber].link->data = data;                          // 데이터 저장            
    currentvertex++;                                            // 현재 정점의 개수 증가
}

void Insertedge(Vertexhead* v)                              // 간선 삽입               
{
    int start, destination;                                 // 시작 정점, 도착 정점
    Edge* check = NULL;                                     // 간선 포인터
    Vertex* check2 = NULL;                                  // 정점 포인터
    printf("Starting vertex number : ");                    
    scanf("%d", &start);

    if (!(start >= 0 && start < 10))                        // 시작 정점이 0 이상 9 이하가 아니면
    {
        printf("Wrong vertex number (0~9)\n");
    }

    if (v[start].link == NULL)                              // 시작 정점이 존재하지 않으면
    {
        printf("Not existing vertex \n");
        return;
    }

    printf("Destination vertex number : ");                 // 도착 정점 입력
    scanf("%d", &destination);

    if (!(destination >= 0 && destination < 10))            // 도착 정점이 0 이상 9 이하가 아니면
    {
        printf("Wrong vertex number (0~9)\n");
    }

    if (v[destination].link == NULL)                        // 도착 정점이 존재하지 않으면
    {
        printf("Not existing vertex \n");
        return;
    }

    check = v[start].link->link;                            // 시작 정점의 link를 check에 저장
    while (check != NULL)                                   // check가 NULL이 아닐 때까지 반복
    {
        if (check->vertex == destination)                   // 이미 존재하는 간선이면
        {
            printf("Already existing Edge \n");
            return;
        }
        check = check->link;                                // check를 다음 간선으로 이동
    }

    check2 = v[start].link;                                 // 시작 정점을 check2에 저장
    while (check2->link != NULL)                            // check2의 link가 NULL이 아닐 때까지 반복
    {
        check2 = check2->link;                              // check2를 다음 정점으로 이동
    }
    check2->link = (Edge*)malloc(sizeof(Edge));             // 간선에 메모리 할당
    check2->link->vertex = destination;                     // 도착 정점 저장
    check2->link->link = NULL;                              // link를 NULL로 초기화
}

void DFS_iterative(Vertexhead* head)                        // 깊이 우선 탐색
{
    int vindex = 0;                                         // 시작 정점
    int num = 0;                                            // 현재 정점
    printf("Starting vertex number : ");                    
    scanf("%d", &vindex);                                   // 시작 정점 입력
    if (!(vindex >= 0 && vindex < 10))                      // 시작 정점이 0 이상 9 이하가 아니면
    {
        printf("Wrong vertex number (0~9)\n");
        return;
    }

    if (head[vindex].link == NULL)                          // 시작 정점이 존재하지 않으면
    {
        printf("Not existing vertex \n");
        return;
    }

    for (int i = 0; i < 10; i++)                            // 정점의 개수만큼 반복
    {
        if (head[i].link != NULL)                           // 정점이 존재하면
        {
            head[i].link->visitflag = 0;                    // 방문 여부 플래그를 0으로 초기화    
        }
    }

    push(vindex);                                           // 시작 정점을 스택에 push
    head[vindex].link->visitflag = 1;                       // 시작 정점의 방문 여부 플래그를 1로 변경
    printf("%5d", head[vindex].link->data);                 // 시작 정점의 데이터 출력

    while (currentstack != 0)                               // 스택이 비어있지 않을 때까지 반복
    {
        num = pop();                                        // 스택에서 pop
        int next;                                           // 다음 정점
        while ((next = find_next_index(head, head[num].link)) != -1)  // 다음 방문할 정점이 있으면
        {
            push(num);                                      // 현재 정점을 스택에 push
            push(next);                                     // 다음 정점을 스택에 push
            printf("%5d", head[next].link->data);           // 다음 정점의 데이터 출력
            head[next].link->visitflag = 1;                 // 다음 정점의 방문 여부 플래그를 1로 변경
            break;
        }
    }
}

void BFS_itertative(Vertexhead* head)                       // 너비 우선 탐색
{
    int vindex = 0;                                         // 시작 정점
    int num = 0;                                            // 현재 정점
    printf("Starting vertex number : ");                    
    scanf("%d", &vindex);                                   // 시작 정점 입력
    if (!(vindex >= 0 && vindex < 10))                      // 시작 정점이 0 이상 9 이하가 아니면
    {
        printf("Wrong vertex number (0~9)\n");
        return;
    }
    if (head[vindex].link == NULL)                          // 시작 정점이 존재하지 않으면
    {
        printf("Not existing vertex \n");
        return;
    }

    for (int i = 0; i < 10; i++)                            // 정점의 개수만큼 반복
    {
        if (head[i].link != NULL)                           // 정점이 존재하면
        {
            head[i].link->visitflag = 0;                    // 방문 여부 플래그를 0으로 초기화
        }
    }
    enQueue(vindex);                                        // 시작 정점을 큐에 enQueue
    head[vindex].link->visitflag = 1;                       // 시작 정점의 방문 여부 플래그를 1로 변경    
    printf("%5d", head[vindex].link->data);                 // 시작 정점의 데이터 출력

    while (currentqueue != 0)                               // 큐가 비어있지 않을 때까지 반복
    {
        num = deQueue();                                    // 큐에서 deQueue
        for (int i = find_next_index(head, head[num].link); i != -1; i = find_next_index(head, head[num].link))
        {
            enQueue(i);                                     // 다음 정점을 큐에 enQueue
            head[i].link->visitflag = 1;                    // 다음 정점의 방문 여부 플래그를 1로 변경
            printf("%5d", head[i].link->data);              // 다음 정점의 데이터 출력    
        }
    }
}

void print_graph(Vertexhead* head)                          // 그래프 출력
{
    Edge* p = NULL;                                         // 간선 포인터    
    for (int i = 0; i < 10; i++)                            // 정점의 개수만큼 반복
    {
        if (head[i].link != NULL)                           // 정점이 존재하면
        {
            printf("[%d] : ", i);                           // 정점 번호 출력    
            printf("%5d  ", head[i].link->data);            // 정점의 데이터 출력
            p = head[i].link->link;                         // 간선을 p에 저장

            while (p != NULL)                               // 간선이 NULL이 아닐 때까지 반복
            {
                printf("%d   ", p->vertex);                 // 간선의 정점 출력
                p = p->link;                                // 다음 간선으로 이동
            }
            printf("\n");
        }
    }
}

int find_next_index(Vertexhead* head, Vertex* v)            // 다음 방문할 정점 찾기
{
    if (v == NULL || head == NULL)                          // 정점이 NULL이거나 head가 NULL이면
        return -1;                                          // -1 반환

    Edge* p = v->link;                                      // 간선 포인터    
    while (p != NULL)                                       // 간선이 NULL이 아닐 때까지 반복    
    {
        if (head[p->vertex].link->visitflag == 0)           // 방문하지 않은 정점이면
        {
            return p->vertex;                               // 정점 반환               
        }
        p = p->link;                                        // 다음 간선으로 이동
    }
    return -1;
}

int find_least_index(Vertexhead* v)                         // 방문하지 않은 정점 중 가장 작은 인덱스 찾기
{
    int i = 0;
    for (i = 0; i < MAX_Vertex; i++)                        // 정점의 개수만큼 반복
    {
        if (v[i].link != NULL && v[i].link->visitflag == 0 && v[i].link->link != NULL) // 방문하지 않은 정점이면
        {
            return i;
        }
    }
    return -1;
}

void push(int aVertex)                                      // 스택에 push
{   
    if (currentstack >= MAX_STACK_SIZE)                     // 스택이 가득 찼으면    
    {
        return;
    }

    stack[++top] = aVertex;                                 // 스택에 push     
    currentstack++;                                         // 현재 스택의 크기 증가
}

int pop()
{
    if (currentstack == 0)                                  // 스택이 비어있으면
    {
        return -1;
    }
    currentstack--;                                         // 현재 스택의 크기 감소
    return stack[top--];                                    // 스택에서 pop
}

void enQueue(int aVertex)                                   // 큐에 enQueue
{
    if (currentqueue >= MAX_QUEUE_SIZE)                     // 큐가 가득 찼으면
    {
        return;
    }
    queue[++rear] = aVertex;                                // 큐에 enQueue
    currentqueue++;                                         // 현재 큐의 크기 증가    
}

int deQueue()                                               // 큐에서 deQueue              
{
    if (currentqueue == 0)                                  // 큐가 비어있으면
    {
        return -1;
    }
    currentqueue--;                                         // 현재 큐의 크기 감소
    return queue[++front];                                  // 큐에서 deQueue
}
