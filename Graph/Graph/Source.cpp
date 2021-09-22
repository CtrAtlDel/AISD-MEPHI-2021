#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "Gets.h"
#define inf 10000
#define SIZE 15
struct ListEdge;
struct ListVertex;
struct Vertex;
struct Graph;

typedef struct ListEdge
{
	Vertex* ver;
	int dist;
	struct ListEdge* next;
}ListEdge;	

typedef struct ListVertex 
{
	Vertex* ver;
	struct ListVertex* next;
}ListVertex;

typedef struct Vertex 
{
	int x;
	int y;
	int key;
	ListEdge* adjacency;
}Vertex;

typedef struct Graph
{
	ListVertex* begin;
}Graph;

Graph* Create_graph(Graph* graph);
int Insert_node(Graph* graph, int x, int y, int key);
int Insert_edge(Graph* graph, int key1, int key2);
int Print_edge(ListVertex* vertex);
int Print_node(Graph* graph);
int D_insert_node(Graph* graph);
int Insert_edge(Graph* graph, int key1, int key2);
int D_insert_edge(Graph* graph);
int Delete_vertex(Graph* graph, int key);
int Delete_edge(Graph* graph, int key1, int key2);
int D_Delete_edge(Graph* graph);
int Delete_vertex(Graph* graph, int key);
int Deikstra(Graph* graph);

int(*fptr[])(Graph*) = { NULL, D_insert_node, D_insert_edge, Print_node, D_Delete_edge, Deikstra};
const char* msgs[] = { "0. Quit", "1. Add vertex", "2. Add edge", "3. Print", "4. Delete edge", "4. Delete vertex", "5 Deikstra"};
const char* errors[] = { "Bad size of key \n", "Didnt find this key \n", "Tree is empty \n", "Bad input\n" };
int dialog(const char* msgs[], int N);
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
const int Nerrors = sizeof(errors) / sizeof(errors[0]);

int dialog(const char* msgs[], int N)
{
	const char* errmsg = "";
	int rc;
	int i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeat, please!";
		printf("\t========== MENU ==========\n");

		for (i = 0; i < N; ++i)
			printf("\t%s\n", msgs[i]);
		printf("\t==========================\n");
		puts("Make your choice: -> ");
		n = getInt(&rc);
		if (n == 0)
			rc = 0;
	} while (rc < 0 || rc >= N);

	return rc;
}


Graph* Create_graph(Graph* graph) 
{
	graph = (Graph*)malloc(sizeof(Graph));
	graph->begin = NULL;
	return graph;
}

int D_insert_node(Graph* graph)
{
	int key;
	int x;
	int y;
	int res;
	printf("Input key -> ");
	if (getInt(&key) == 0) { return 0; }
	printf("\n");
	printf("Input x -> ");
	if (getInt(&x) == 0) { return 0; }
	printf("\n");
	printf("Input y -> ");
	if (getInt(&y) == 0) { return 0; }
	printf("\n");
	res = Insert_node(graph, x, y, key);
	if (res == -1)
	{
		printf("ERROR\n");
	}
	else 
	{
		printf("Succses\n");
	}
	return 1;
}

int D_insert_edge(Graph* graph) 
{
	int key1;
	int key2;
	int res;
	printf("Input key1 ->");
	if (getInt(&key1) == 0) { return 0; }
	printf("\n");
	printf("Input key2 ->");
	if (getInt(&key2) == 0) { return 0; }
	printf("\n");
	res = Insert_edge(graph, key1, key2);
	if (res == -1)
	{
		printf("ERROR\n");
	}
	else
	{
		printf("Succses\n");
	}
	return 1;
}

int Insert_node(Graph* graph, int x, int y, int key) 
{
	Vertex* new_vertex = (Vertex*)malloc(sizeof(Vertex));
	new_vertex->x = x;
	new_vertex->y = y;
	new_vertex->key = key;
	new_vertex->adjacency = NULL;
	ListVertex* item = (ListVertex*)malloc(sizeof(ListVertex));
	ListVertex* temp = NULL;
	temp = graph->begin;

	while (temp != NULL) 
	{
		if (temp->ver->key == key)
		{
			return -1;
		}
		temp = temp->next;
	}

	temp = graph->begin;
	if (graph->begin == NULL)
	{
		graph->begin = item;
		graph->begin->next = NULL;
		graph->begin->ver = new_vertex;
	}
	else
	{
		while (temp->next != NULL) 
		{
			temp = temp->next;
		}
		temp->next = item;
		item->next = NULL;
		item->ver = new_vertex;
	}
	return 1;
}

int Insert_edge(Graph* graph, int key1, int key2) 
{
	ListEdge* new_edge = (ListEdge*)malloc(sizeof(ListEdge));
	new_edge->next = NULL;
	new_edge->ver = NULL;
	new_edge->dist = -1;
	int dist = -1;
	ListEdge* temp_edge = NULL;
	ListVertex* find_vertex = NULL;
	ListVertex* find_vertex_key2 = NULL;


	find_vertex_key2 = graph->begin;
	find_vertex = graph->begin;
	
	while (find_vertex != NULL && find_vertex->ver->key != key1) 
	{
		find_vertex = find_vertex->next;
	}
	if (find_vertex == NULL) { return -1; }

	if (find_vertex->ver->adjacency == NULL)
	{
		find_vertex->ver->adjacency = new_edge;

		while (find_vertex_key2 != NULL && find_vertex_key2->ver->key != key2)// поиск по второму ключу
		{
			find_vertex_key2 = find_vertex_key2->next;
		}
		if (find_vertex_key2 == NULL) { return -1; }
		
		find_vertex->ver->adjacency->ver = find_vertex_key2->ver;

		find_vertex->ver->adjacency->dist = sqrt((find_vertex->ver->x) * (find_vertex->ver->x) - 2 * (find_vertex->ver->x) * (find_vertex_key2->ver->x) + (find_vertex_key2->ver->x) * (find_vertex_key2->ver->x) + (find_vertex->ver->y) * (find_vertex->ver->y) - 2 * (find_vertex->ver->y) * (find_vertex_key2->ver->y) + (find_vertex_key2->ver->y) * (find_vertex_key2->ver->y));
	}
	else 
	{
		temp_edge = find_vertex->ver->adjacency;

		while (temp_edge->next != NULL)
		{
			temp_edge = temp_edge->next;
		}

		temp_edge->next = new_edge;

		while (find_vertex_key2 != NULL && find_vertex_key2->ver->key != key2)// поиск по второму ключу
		{
			find_vertex_key2 = find_vertex_key2->next;
		}
		if (find_vertex_key2 == NULL) { return -1; }

		new_edge->ver = find_vertex_key2->ver;

		new_edge->dist = sqrt((find_vertex->ver->x) * (find_vertex->ver->x) - 2 * (find_vertex->ver->x) * (find_vertex_key2->ver->x) + (find_vertex_key2->ver->x) * (find_vertex_key2->ver->x) + (find_vertex->ver->y) * (find_vertex->ver->y) - 2 * (find_vertex->ver->y) * (find_vertex_key2->ver->y) + (find_vertex_key2->ver->y) * (find_vertex_key2->ver->y));
	}

	return 1;
}

int D_Delete_edge(Graph* graph) 
{
	int key1, key2;
	int res;
	printf("Input key1 ->");
	if (getInt(&key1) == 0) { return 0; }
	printf("\n");
	printf("Input key2 ->");
	if (getInt(&key2) == 0) { return 0; }
		printf("\n");
	res = Delete_edge(graph, key1, key2);
	if (res == -1)
	{
		printf("ERROR\n");
	}
	else
	{
		printf("Succses\n");
	}
	return 1;
}

int Delete_edge(Graph* graph, int key1, int key2) 
{
	ListVertex* temp_vertex = NULL;
	ListEdge* temp_edge = NULL;
	ListEdge* tmp = NULL;
	int flag = 0;
	temp_vertex = graph->begin;
	if (temp_vertex == NULL) { return -1; }

	while (temp_vertex->ver != NULL && temp_vertex->ver->key != key1)
	{
		temp_vertex = temp_vertex->next;
	}
	if (temp_vertex->ver == NULL) { return -1; }

	temp_edge = temp_vertex->ver->adjacency;
	if (temp_edge == NULL) { return -1; }
	if (temp_edge->ver->key == key2)
	{
		temp_vertex->ver->adjacency = temp_edge->next;
		free(temp_edge);
		return 1;
	}
	while (temp_edge->next != NULL && temp_edge->next->ver->key != key2) 
	{
		flag = 1;
		temp_edge = temp_edge->next;
	}
	if (temp_edge->next == NULL && flag == 1) { return -1; }
	tmp = temp_edge->next;
	temp_edge->next = temp_edge->next->next; //some problem
	free(tmp);
	return 1;
}

int D_delete_vertex(Graph* graph) 
{
	int res;
	int key;
	printf("Input key -> ");
	if (getInt(&key) == 0) { return 0; }
	printf("\n");
	res = Delete_vertex(graph, key);
	if (res == -1)
	{
		printf("ERROR\n");
	}
	else
	{
		printf("Succses\n");
	}
	return 1;
}


int Delete_vertex(Graph* graph, int key)//some delete
{
	ListVertex* temp_vertex = NULL;
	ListVertex* temp_vertex1 = NULL;
	ListEdge* temp_edge = NULL;
	ListEdge* tmp = NULL;
	int flag = 0;
	temp_vertex = graph->begin;
	if (temp_vertex == NULL) { return -1; }
	while (temp_vertex != NULL)
	{
		if (temp_vertex->ver->key == key)
		{
			flag = 1;
			break;
		}
		temp_vertex = temp_vertex->next;
	}
	if (flag == 0)
	{
		return -1;
	}
	if (temp_vertex->ver->key == key) //delete head
	{
		graph->begin = temp_vertex->next;
		free(temp_vertex);
	}
	while (temp_vertex != NULL && temp_vertex->ver->key != key) // delete body
	{
		temp_vertex = temp_vertex->next;
	}
	temp_vertex1 = graph->begin;
	while (temp_vertex1->next != temp_vertex) 
	{
		temp_vertex1 = temp_vertex1->next;
	}
	return 1;
}

int Deikstra(Graph* graph) 
{
	int matrix[SIZE][SIZE] = { 0 };
	int distant[SIZE];
	int v[SIZE];
	int temp = -1;
	int index = -1;
	int min = -1;



	return 1;
}

int sizeofgraph(Graph* graph) 
{
	int size = 0;
	ListVertex* temp = graph->begin;
	while (temp != NULL)
	{
		++size;
		temp = temp->next;
	}

	return size;
}

int main() 
{
	Graph* graph = NULL;
	graph = Create_graph(graph);
	int rc = 0, f = 1;
	while ((rc = dialog(msgs, NMsgs)) && f)
	{
		f = fptr[rc](graph);
	}
	return 0;
}


int Print_edge(ListVertex* vertex)
{
	ListEdge* temp = NULL;
	temp = vertex->ver->adjacency;
	while (temp != NULL)
	{
		printf("Dist %d \t", temp->dist);
		if (temp->ver == NULL)
		{
			printf("Ver NULL");
		}
		else
		{
			printf("For edge %d\n", temp->ver->key);
		}
		temp = temp->next;
	}
	return 1;
}

int Print_node(Graph* graph)
{
	ListVertex* temp = NULL;
	temp = graph->begin;
	while (temp != NULL)
	{
		printf("Vertex:\n");
		printf("Key -> %d\n", temp->ver->key);
		printf("X -> %d\n", temp->ver->x);
		printf("Y -> %d\n", temp->ver->y);
		Print_edge(temp);
		printf("\n");
		temp = temp->next;
	}
	return 1;
}
