#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<math.h>
#include<time.h>
#define inf 10000
#define MIN 10000
#define SIZE 10
#define max_x 10
#define max_y 10
#define min_x -10
#define min_y -10

typedef struct Vertex //вершина с указателем на список ребер
{
	int key;
	int x;
	int y;
	struct ListEdge* adj; //указатель на список смежных ребер
	struct Vertex* next; // следующая вершина в списке вершин
}Vertex;

typedef struct ListEdge //список смежных ребер
{
	float dist; //вес ребра
	struct ListEdge* next;
	Vertex* vertex;
}ListEdge;

typedef struct Graph
{
	Vertex* begin;
}Graph;

int getInt(int* a);
Vertex* Find_vertex(Graph* graph, int key);
Graph* Create_graph(Graph* graph);
int D_insert_node(Graph* graph);
int Insert_vertex(Graph* graph, int x, int y, int key);
int D_insert_edge(Graph* graph);
int Insert_edge(Graph* graph, int key1, int key2);
void Print_vertex(Graph* graph);
int Print_edge(Vertex* vertex);
int D_delete_vertex(Graph* graph);
int D_Delete_edge(Graph* graph);
int Delete_edge(Graph* graph, int key1, int key2);
Vertex* Find_vertex(Graph* graph, int key);
int Delete_vertex(Graph* graph, int key);
int Vertex_check(Graph* graph, int key);
int dialog(const char* msgs[], int N);
int D_Deiksta(Graph* graph);
int D_BFS(Graph* graph);
int BFS(Graph* graph, int key, int key1);
int Floyd_Warshall(Graph* graph);
int D_Floyd_Warshall(Graph* graph);
int D_Graph_random(Graph* graph);
int Graph_random(Graph* graph, int vertex_size, int edges_size);
int Graph_load(Graph* graph);
int Graph_unload(Graph* graph);
void Print_matrix(float** matrix, int size_graph);
int(*fptr[])(Graph*) = { NULL, D_insert_node, D_insert_edge, Print_vertex, D_Delete_edge, D_delete_vertex, D_Deiksta, D_BFS, D_Floyd_Warshall, D_Graph_random, Graph_load };
const char* msgs[] = { "0. Quit", "1. Add vertex", "2. Add edge", "3. Print", "4. Delete edge", "5. Delete vertex", "6. Deikstra", "7. BFS",
"8. Floyd_Warshall" , "9. Random graph", "10. Load graph from file" };
const char* errors[] = { "Bad size of key \n", "Didnt find this key \n", "Tree is empty \n", "Bad input\n" };
int dialog(const char* msgs[], int N);
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
const int Nerrors = sizeof(errors) / sizeof(errors[0]);

Graph* Create_graph(Graph* graph)
{
	graph = (Graph*)malloc(sizeof(Graph));
	graph->begin = NULL;
	return graph;
}

int D_insert_node(Graph* graph)
{
	int key = 1;
	int x;
	int y;
	int res;
	//printf("Input key -> ");
	//if (getInt(&key) == 0) { return 0; }
	//printf("\n");
	printf("Input x -> ");
	if (getInt(&x) == 0) { return 0; }
	//printf("\n");
	printf("Input y -> ");
	if (getInt(&y) == 0) { return 0; }
	//printf("\n");
	res = Insert_vertex(graph, x, y, key);
	if (res == -1)
	{
		printf("ERROR dublicate \n");
	}
	else
	{
		printf("Succses\n");
	}
	return 1;
}
int Insert_vertex(Graph* graph, int x, int y, int key)
{
	Vertex* new_vertex = (Vertex*)malloc(sizeof(Vertex));
	Vertex* vertex = NULL;
	vertex = Find_vertex(graph, key);
	new_vertex->x = x;
	new_vertex->y = y;
	new_vertex->key = key;
	new_vertex->next = NULL;
	new_vertex->adj = NULL;

	Vertex* temp_vertex = NULL;
	if (graph->begin == NULL)//если граф пустой
	{
		graph->begin = new_vertex;
		return 1;
	}
	temp_vertex = graph->begin; // вставка ребра при совпадении координат 
	while (temp_vertex != NULL) 
	{
		if ((temp_vertex->x == x)  && (temp_vertex->y == y))
		{
			//Insert_edge(graph, temp_vertex->key, temp_vertex->key);
			return -1;
		}
		temp_vertex = temp_vertex->next;
	}
	temp_vertex = graph->begin;
	while (temp_vertex->next != NULL)
	{
		temp_vertex = temp_vertex->next;
	}
	new_vertex->key = temp_vertex->key + 1;
	temp_vertex = graph->begin;
	//if (vertex->key != NULL)
	//{
	//	free(new_vertex);
	//	return -1;
	//}
	while (temp_vertex != NULL) // если проходя по списку вершин нашли с одинаковым ключом
	{
		if (temp_vertex->key == new_vertex->key) 
		{
			free(new_vertex);
			return -1; 
		}
		temp_vertex = temp_vertex->next;
	}
	temp_vertex = graph->begin;
	while (temp_vertex->next != NULL)
	{
		temp_vertex = temp_vertex->next;
	}
	temp_vertex->next = new_vertex;
	
	
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
	if (res == -2)
	{
		printf("ERROR didnt find key2\n");
		return 1;
	}
	if (res == -1)
	{
		printf("ERROR didnt find key1\n");
	}
	else
	{
		printf("Succses\n");
	}
	return 1;
}

int Insert_edge(Graph* graph, int key1, int key2)
{
	ListEdge* new_edge = (ListEdge*)malloc(sizeof(ListEdge));
	new_edge->next = NULL;
	new_edge->dist = -1;
	ListEdge* temp_edge = NULL;
	Vertex* find_key1_vertex = graph->begin; //указатель на вершину в список которой добавить ребро
	Vertex* find_key2_vertex = graph->begin; //указатель на вершину, который необходимо положить в список key1

	while (find_key1_vertex != NULL && find_key1_vertex->key != key1)
	{
		find_key1_vertex = find_key1_vertex->next;
	}
	if (find_key1_vertex == NULL) { return -1; };//нет вершины с key1
	while (find_key2_vertex != NULL && find_key2_vertex->key != key2)
	{
		find_key2_vertex = find_key2_vertex->next;
	}
	if (find_key2_vertex == NULL) { return -2; } //нет вершины с key2

	if (find_key1_vertex->adj == NULL)
	{
		find_key1_vertex->adj = new_edge;
		new_edge->vertex = find_key2_vertex;
		new_edge->dist = sqrt((find_key1_vertex->x) * (find_key1_vertex->x) - 2 * (find_key1_vertex->x) * (find_key2_vertex->x) + (find_key2_vertex->x) * (find_key2_vertex->x) + (find_key1_vertex->y) * (find_key1_vertex->y) - 2 * (find_key1_vertex->y) * (find_key2_vertex->y) + (find_key2_vertex->y) * (find_key2_vertex->y));
	}
	else
	{
		temp_edge = find_key1_vertex->adj;
		while (temp_edge->next != NULL)
		{
			temp_edge = temp_edge->next;
		}
		temp_edge->next = new_edge;
		new_edge->vertex = find_key2_vertex;
		new_edge->dist = sqrt((find_key1_vertex->x) * (find_key1_vertex->x) - 2 * (find_key1_vertex->x) * (find_key2_vertex->x) + (find_key2_vertex->x) * (find_key2_vertex->x) + (find_key1_vertex->y) * (find_key1_vertex->y) - 2 * (find_key1_vertex->y) * (find_key2_vertex->y) + (find_key2_vertex->y) * (find_key2_vertex->y));
	}
	return 1;
}

void Print_vertex(Graph * graph)
{
	Vertex* temp_vertex = NULL;
	temp_vertex = graph->begin;
	if (Size_of_graph(graph) == 0)
	{
		printf("Empty graph\n");
	}
	while (temp_vertex != NULL)
	{
		printf("Vertex:\n");
		printf("Key -> %d\n", temp_vertex->key);
		printf("X -> %d\n", temp_vertex->x);
		printf("Y -> %d\n", temp_vertex->y);
		Print_edge(temp_vertex);
		printf("\n");
		temp_vertex = temp_vertex->next;
	}
}

int Print_edge(Vertex * vertex)
{
	ListEdge* edge = NULL;
	edge = vertex->adj;
	printf("List of edge \n");
	while (edge != NULL)
	{
		printf("Key adj -> %d dist -> %f \n", edge->vertex->key, edge->dist);
		edge = edge->next;
	}
	//printf("\n");
	return 1;
}

int D_delete_vertex(Graph * graph)
{
	int res;
	int key;
	printf("Input key -> ");
	if (getInt(&key) == 0) { return 0; }
	printf("\n");
	res = Delete_vertex(graph, key);
	if (res == 0)
	{
		printf("Empty graph \n");
		return 1;
	}
	if (res == -2)
	{
		printf("ERROR didnt found key2\n");
		return 1;
	}
	if (res == -1)
	{
		printf("ERROR didnt found key1\n");
	}
	else
	{
		printf("Succses\n");
	}
	return 1;
}

int D_Delete_edge(Graph * graph)
{
	int key1, key2;
	int res;
	printf("Input key1 ->");
	if (getInt(&key1) == 0) { return 0; }
	printf("Input key2 ->");
	if (getInt(&key2) == 0) { return 0; }
	res = Delete_edge(graph, key1, key2);
	if (res == -2)
	{
		printf("ERROR I didnt find key2\n");
	}
	if (res == -1)
	{
		printf("ERROR I didnt find key1\n");
	}
	else
	{
		printf("Succses\n");
	}
	return 1;
}

int Delete_edge(Graph * graph, int key1, int key2)
{
	int flag = 0;
	Vertex* temp_vertex = NULL;
	ListEdge* temp_edge = NULL;
	ListEdge* temp_edge_prev = NULL;
	temp_vertex = Find_vertex(graph, key1);
	if (temp_vertex == NULL)
	{
		return -1;
	}
	temp_edge = temp_vertex->adj;
	temp_edge_prev = temp_vertex->adj;
	while (temp_edge != NULL)
	{
		if (temp_edge->vertex->key == key2)
		{
			flag = 1;
			break;
		}
		temp_edge = temp_edge->next;
	}
	if (flag == 0)
	{
		return -2;//такого ребра нет 
	}
	temp_edge = temp_vertex->adj;
	temp_edge_prev = temp_vertex->adj;
	while (temp_edge != NULL) 
	{
		if (temp_edge->vertex->key == key2)
		{
			if (temp_edge == temp_vertex->adj) //head
			{
				temp_vertex->adj = temp_vertex->adj->next;
				free(temp_edge);
				temp_edge = temp_vertex->adj;
				continue;
			}
			else
			{
				temp_edge_prev = temp_vertex->adj;
				while (temp_edge_prev->next != temp_edge)
				{
					temp_edge_prev = temp_edge_prev->next;
				}
				temp_edge_prev->next = temp_edge->next;
				free(temp_edge);
				temp_edge = temp_edge_prev;
				continue;
			}
		}
		temp_edge = temp_edge->next;
	}
	return 1;
}

Vertex* Find_vertex(Graph * graph, int key)
{
	Vertex* find = NULL;
	find = graph->begin;
	while (find != NULL)
	{
		if (find->key == key)
		{
			return find;
		}
		find = find->next;
	}
	return find;
}

int Graph_reconstruct(Graph* graph) 
{
	Vertex* vertex = NULL;
	int i = 1;
	vertex = graph->begin;
	while (vertex != NULL) 
	{
		vertex->key = i;
		++i;
		vertex = vertex->next;
	}
	return 1;
}

int Delete_vertex(Graph * graph, int key)
{
	Vertex* temp_vertex = NULL;
	Vertex* temp_vertex_prev = NULL;
	ListEdge* temp_edge = NULL;
	ListEdge* temp_edge_prev = NULL;
	int check = Vertex_check(graph, key);
	int flag = 0;
	if (Size_of_graph(graph) == 0) 
	{
		return -5;
	}
	//сначала удаляем ребра в виде указателей
	temp_vertex = graph->begin;
	temp_vertex_prev = graph->begin;
	if (graph->begin == NULL)
	{
		return 0;//пустой граф
	}
	if (check == 0)
	{
		return -3;//нет вершины 
	}
	//удаляем все ребра
	while (temp_vertex != NULL)
	{
		temp_edge = temp_vertex->adj;
		temp_edge_prev = temp_vertex->adj;
		if (temp_edge != NULL)
		{
			while (temp_edge != NULL) 
			{
				if (temp_edge->vertex->key == key)
				{
					if (temp_edge == temp_vertex->adj) 
					{
						temp_vertex->adj = temp_vertex->adj->next;
						free(temp_edge);
						temp_edge = temp_vertex->adj;
						continue;
					}
					else
					{
						while (temp_edge_prev->next != temp_edge) 
						{
							//temp_edge_prev->next = temp_edge->next;
							//free(temp_edge);
							temp_edge_prev = temp_edge_prev->next;
						}
						temp_edge_prev->next = temp_edge->next;
						free(temp_edge);
						temp_edge = temp_edge_prev->next;
						continue;
					}
				}
				temp_edge = temp_edge->next;
			}
		}
		temp_vertex = temp_vertex->next;
	}
	//удалены все ребра
	temp_vertex = graph->begin;
	temp_vertex_prev = graph->begin;
	while (temp_vertex != NULL)
	{
		if (temp_vertex->key == key)
		{
			break;
		}
		temp_vertex = temp_vertex->next;
	}
	//ребро первое 
	if (temp_vertex == graph->begin)
	{
		graph->begin = temp_vertex->next;
		free(temp_vertex);
		Graph_reconstruct(graph);
		return 1;
	}
	while (temp_vertex_prev->next != temp_vertex)
	{
		temp_vertex_prev = temp_vertex_prev->next;
	}
	temp_vertex_prev->next = temp_vertex->next;
	free(temp_vertex);
	Graph_reconstruct(graph);
	return 1;
}//no neeed some fix 

int Vertex_check(Graph * graph, int key)
{
	Vertex* temp = graph->begin;
	int flag = 0;

	while (temp != NULL)
	{
		if (temp->key == key)
		{
			flag = 1;
			break;
		}
		temp = temp->next;
	}
	return flag;
}

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

int Size_of_graph(Graph* graph) 
{
	int size = 0;
	Vertex* temp = NULL;
	temp = graph->begin;
	while (temp != NULL)
	{
		++size;
		temp = temp->next;
	}
	return size;
}

int D_Deiksta(Graph* graph) 
{
	int key1, key2;
	int res;
	printf("Input key1 ->");
	if (getInt(&key1) == 0) { return 0; }
	printf("Input key2 ->");
	if (getInt(&key2) == 0) { return 0; }
	res = Deikstra(graph, key1, key2);
	if (res == -3)
	{
		printf("Empty graph\n");
	}
	else 
	{
		printf("Success\n");
	}
}

float* cl_dist(float* arr, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		arr[i] = -1;
	}
	return arr;
}
//ищет минимальный элемент не равный inf и -1
float min_dist(float* arr, int size) 
{
	float min  = 0;
	int index = -1;
	min = arr[0];
	for (size_t i = 0; i < size; i++)
	{
		if (arr[i] != -1)
		{
			min = arr[i];
			break;
		}
	}
	for (size_t i = 0; i < size; i++)
	{
		if (arr[i] < inf && arr[i] != -1 && arr[i] < min)
		{
			min = arr[i];
			index = i;
		}
	}
	if (index == -1 && min == -1)
	{
		return -1;
	}
	arr[index] = -1;
	return min;
}

//key1 откуда key2 куда
int Deikstra(Graph* graph, int key1, int key2)
{
	if (graph->begin == NULL)
	{
		return -3; //empty graph
	}
	int size_graph = Size_of_graph(graph);
	if (size_graph == 0) { return -3; }
	int* visit = (int*)malloc(size_graph * sizeof(int)); //1 - yes 0 - no
	float* dist = (float*)malloc(size_graph * sizeof(float));	//список расстояний
	Vertex* vertex = graph->begin;
	Vertex* temp = NULL;// вершина на которую показывает ребро
	ListEdge* edge = NULL;
	float** matrix = (float**)malloc(size_graph * sizeof(float*)); //матрица связей
	for (size_t i = 0; i < size_graph; i++)
	{
		matrix[i] = (float*)malloc(size_graph * sizeof(float));
	}
	for (size_t i = 0; i < size_graph; i++)
	{
		for (size_t j = 0; j < size_graph; j++) 
		{
			matrix[i][j] = 0;
		}
	}
	for (size_t i = 0; i < size_graph; i++)
	{
		dist[i] = inf;
		visit[i] = 1;
	}
	dist[key1 - 1] = 0;
	// конвертируем в матрицу расстояний
	while (vertex != NULL) 
	{
		edge = vertex->adj;
		while (edge != NULL) 
		{
			temp = edge->vertex;
			matrix[vertex->key - 1][temp->key - 1] = edge->dist;
			edge = edge->next;
		}
		vertex = vertex->next;
	}
	int index = inf;
	float min = inf;
	do
	{
		index = inf;
		min = inf;
		for (size_t i = 0; i < size_graph; i++)
		{
			if (dist[i] < min && visit[i] == 1) 
			{
				min = dist[i];
				index = i;
			}
		}
		if (index != inf)
		{
			for (int i = 0; i < size_graph; i++) 
			{
				if (matrix[index][i] > 0)
				{
					float temp = min + matrix[index][i];
					if (temp < dist[i])
					{
						dist[i] = temp;
					}
				}
			}
			visit[index] = 0;
		}
	} while (index < 10000);
	//for (size_t i = 0; i < size_graph; i++)// массив расстояний
	//{
	//	printf("%f ", dist[i]);
	//}
	printf("\n");
	printf("Min dist -> %f ", dist[key2 - 1]);
	if (dist[key2 - 1] == inf)
	{
		printf("No way\n");
		return 1;
	}

	int* vertex_vis = malloc(size_graph * sizeof(int)); //массив посещенных вершин
	int end = key2 - 1; // index
	vertex_vis[0] = key2;
	int k = 1;
	float weight = dist[key2 - 1];
	//Print_matrix(matrix, size_graph);
	while (end != key1 - 1) 
	{
		for (size_t i = 0; i < size_graph; i++)
		{
			//printf("Elem %f\n", matrix[i][end]);
			if (matrix[i][end] != 0)
			{
				float tmp = weight - matrix[i][end];//вес пути и пред вершины 
				if (tmp == dist[i])
				{
					weight = tmp;
					end = i;
					vertex_vis[k] = i + 1;
					++k;
				}
			}
		}
	}

	printf("\nShortest path %d -> %d: \n", key1, key2);
	for (int i = k - 1; i >= 0; --i)
	{
		printf("%d ", vertex_vis[i]);
	}
	printf("\n");
	free(vertex_vis);
	return 1;
}

typedef struct Queue
{
	int* q;
	int head;
	int tail;
	int size;
}Queue;

Queue* Q_create(Queue* queue, int size) 
{
	queue = (Queue*)malloc(sizeof(Queue));
	queue->q = (int*)malloc(sizeof(int) * size);
	queue->head = 0;
	queue->tail = 0;
	queue->size = size;
	return queue;
}

int Q_isempty(Queue* q) // 1 - пуста 0 - нет
{
	if (q->head == q->tail)
	{
		return 1;
	}
	else { return 0; }
}

int Q_insert(Queue* queue, int item) 
{
	if (queue->tail == queue->size)
	{
		return 0;
	}
	queue->q[queue->tail] = item;
	queue->tail = queue->tail + 1;
	return 1;
}

int Q_delete(Queue* queue)
{
	//queue->head = -1;
	++queue->head;
	return 1;
}

int Q_print(Queue* queue)
{
	int tmphead = NULL;
	int tmptail = NULL;
	tmphead = queue->head;
	tmptail = queue->tail;
	while (tmphead != tmptail)
	{
		printf("item-> %d", queue->q[tmphead]);
		++tmphead;
	}
	printf("\n");
	return 1;
}

Vertex* found_bykey(Graph* graph, int key) 
{
	Vertex* temp = NULL;
	temp = graph->begin;
	while (temp != NULL) 
	{
		if (temp->key == key)
		{
			break;
		}
		temp = temp->next;
	}
	return temp;
}

int D_BFS(Graph* graph) 
{
	int key1, key2;
	int res;
	printf("Input key1 ->");
	if (getInt(&key1) == 0) { return 0; }
	printf("Input key2 ->");
	if (getInt(&key2) == 0) { return 0; }
	res = BFS(graph, key1, key2);
	if (res == -2)
	{
		printf("ERROR I didnt find key2\n");
	}
	if (res == -1)
	{
		printf("ERROR I didnt find key1\n");
	}
	if (res == -3)
	{
		printf("Empty graph\n");
	}
	else
	{
		printf("Succses\n");
	}
	return 1;
}

void Print_path(int* pred, int key1, int key2)
{
	if (key2 == key1) 
	{
		printf(" %d ", key1);
	}
	else 
	{
		if (pred[key2 -1] == -1)
		{
			printf("No way \n");
		}
		else 
		{
			Print_path(pred, key1, pred[key2 - 1]);
			printf(" %d ", key2);
		}
	}
}

int BFS(Graph* graph, int key, int key2)
{
	int temp_key = 0;
	int size_graph = Size_of_graph(graph);
	if (size_graph == 0 )
	{
		return -3;
	}
	Vertex* vertex = NULL;
	ListEdge* edge = NULL;
	vertex = graph->begin;
	Queue* queue = NULL;
	queue = Q_create(queue, size_graph);
	int* dist = (int*)malloc(size_graph * sizeof(int));
	float* dist_edge = (float*)malloc(size_graph * sizeof(float));
	int* color = (int*)malloc(size_graph * sizeof(int)); // 0 - белая 1 - серая 2 - черная
	int* pred = (int*)malloc(size_graph * sizeof(int));
	for (size_t i = 0; i < size_graph; i++)
	{
		color[i] = 0;
		dist[i] = 0;
		dist_edge[i] = 0;
		pred[i] = -1;
	}

	Q_insert(queue, key);
	Q_print(queue);
	while (Q_isempty(queue) == 0) 
	{
		temp_key = queue->q[queue->head];
		vertex = found_bykey(graph, temp_key);
		edge = vertex->adj;

		Q_delete(queue);
		while (edge != NULL) 
		{
			if (color[edge->vertex->key - 1] == 0)
			{
				color[edge->vertex->key - 1] = 1;
				dist[edge->vertex->key - 1] = dist[temp_key - 1] + 1; //edge->dist
				dist_edge[edge->vertex->key - 1] = dist_edge[temp_key - 1] + edge->dist;
				pred[edge->vertex->key - 1] = temp_key;
				Q_insert(queue, edge->vertex->key);
			}
			edge = edge->next;
		}
		color[temp_key - 1] = 2;
	}

	for (size_t i = 0; i < size_graph; i++)
	{
		
		printf("key %d -> %d, num of vertex-> %d dist->%f \n", key, i + 1, dist[i], dist_edge[i]);
	}
	printf("\n");
	printf("Way: \n");
	Print_path(pred, key, key2);
	printf("\n");
	free(color);
	free(dist);
	free(pred);
	return 1;
}

float** Graph_to_matrix(Graph* graph) 
{
	int size_graph = Size_of_graph(graph);
	float** matrix = (float**)malloc(size_graph * sizeof(float*));
	Vertex* vertex = graph->begin;
	Vertex* temp = NULL;
	ListEdge* edge = NULL;
	for (size_t i = 0; i < size_graph; i++)
	{
		matrix[i] = (float*)malloc(size_graph * sizeof(float));
	}
	for (size_t i = 0; i < size_graph; i++)
	{
		for (size_t j = 0; j < size_graph; j++)
		{
			if (i == j)
			{
				matrix[i][j] = 0;
			}
			else { matrix[i][j] = inf; }
		}
	}
	while (vertex != NULL)
	{
		edge = vertex->adj;
		while (edge != NULL)
		{
			temp = edge->vertex;
			matrix[vertex->key - 1][temp->key - 1] = edge->dist;
			edge = edge->next;
		}
		vertex = vertex->next;
	}

	return matrix;
}

void Delete_matrix(float** matrix, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

void Print_matrix(float** matrix, int size_graph) 
{
	printf("Matrix: \n");
	for (size_t i = 0; i < size_graph; i++)
	{
		for (size_t j = 0; j < size_graph; j++)
		{
			printf(" %f ", matrix[i][j]);
		}
		printf("\n");
	}
}

float min(float first, float second) 
{
	if (first <= second) 
	{
		return first;
	}
	else { return second; }
}

int D_Floyd_Warshall(Graph* graph) 
{
	int key1, key2;
	int res;
	res = Floyd_Warshall(graph);
	if (res == -2)
	{
		printf("ERROR I didnt find key2\n");
	}
	if (res == -1)
	{
		printf("Empty graph\n");
	}
	else
	{
		printf("Succses\n");
	}
	return 1;
}

typedef struct Pairs
{
	float dist; //расстояние 
	int i; //откуда
	int j; //куда
}Pairs;

Pairs* mini(float** matrix, int size, Pairs* pairs) 
{
	float min = inf;
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (matrix[i][j] < min && matrix[i][j] != 0) 
			{
				min = matrix[i][j];
				pairs->dist = min;
				pairs->i = i + 1;
				pairs->j = j + 1;
			}
		}
	}
	if (min == inf)
	{
		pairs->dist = min;
		pairs->i = -1;
		pairs->j = -1;
	}
	return pairs;
}

Pairs* min_bigger(float** matrix, int size, Pairs* p_1, Pairs* p_2)
{
	float min_v2 = inf;
	p_1->i = 0;
	p_1->j = 0;
	float min_v1 = p_2->dist;
	if (min_v1 == inf)
	{
		p_1->dist = inf;
		p_1->i = -1;
		p_1->j = -1;
		return p_1;
	}
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (matrix[i][j] <= min_v2 && matrix[i][j] >= min_v1 )
			{
				if (matrix[i][j] == min_v1)
				{
					if (i == (p_2->i - 1) && j == (p_2->j - 1))
					{
						continue;
					}
					else 
					{
						min_v2 = matrix[i][j];
						p_1->dist = min_v2;
						p_1->i = i;
						p_1->j = j;
					}
				}
				else 
				{
					min_v2 = matrix[i][j];
					p_1->dist = min_v2;
					p_1->i = i;
					p_1->j = j;
				}
			}
		}
	}
	++p_1->i;
	++p_1->j;
	if (min_v2 == inf)
	{
		p_1->i = -1;
		p_1->j = -1;
	}
	return p_1;
}

Pairs* min_bigger_s(float** matrix, int size, Pairs* p_1, Pairs* p_2, Pairs* p_3)
{
	float min_v2 = inf;
	p_1->i = 0;
	p_1->j = 0;
	float min_v1 = p_2->dist;
	if (min_v1 == inf)
	{
		p_1->dist = inf;
		p_1->i = -1;
		p_1->j = -1;
		return p_1;
	}
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (matrix[i][j] <= min_v2 && matrix[i][j] >= min_v1)
			{
				if (matrix[i][j] == min_v1)
				{
					if (i == (p_2->i - 1) && j == (p_2->j - 1))
					{
						continue;
					}
					else
					{
						if (i == (p_3->i - 1) && j == (p_3->j - 1))
						{
							continue;
						}
						else 
						{
							min_v2 = matrix[i][j];
							p_1->dist = min_v2;
							p_1->i = i;
							p_1->j = j;
						}
					}
				}
				else
				{
					min_v2 = matrix[i][j];
					p_1->dist = min_v2;
					p_1->i = i;
					p_1->j = j;
				}
			}
		}
	}
	++p_1->i;
	++p_1->j;
	if (min_v2 == inf)
	{
		p_1->i = -1;
		p_1->j = -1;
	}
	return p_1;
}

int Print_path_f(float** dist, int** pred, int key1, int key2)
{
	int k1 = key1 - 1;
	int k2 = key2 - 1;
	if (dist[k1][k2] == inf)
	{
		printf("No way\n");

	}
	int tmp = k1;
	while ( tmp != k2) 
	{
		printf(" %d ", tmp + 1);
		tmp = pred[tmp][k2];
	}
	printf(" %d \n", key2);
	return 1;
}

int Floyd_Warshall(Graph* graph) 
{
	int size_graph = Size_of_graph(graph);
	Vertex* vertex = graph->begin;
	Vertex* temp = NULL;
	ListEdge* edge = NULL;
	float** matrix = NULL;
	int** pred = NULL;
	if (graph->begin == NULL)
	{
		return -1;
	}
	Pairs* pairs_1 = (Pairs*)malloc(sizeof(Pairs));
	Pairs* pairs_2 = (Pairs*)malloc(sizeof(Pairs));
	Pairs* pairs_3 = (Pairs*)malloc(sizeof(Pairs));
	matrix = Graph_to_matrix(graph);// матрица связей
	pred = (int**)malloc(sizeof(int*) * size_graph);
	for (size_t i = 0; i < size_graph; i++)
	{
		pred[i] = (int*)malloc(sizeof(int) * size_graph);
	}
	for (int i = 0; i < size_graph; i++)
	{
		for (int j = 0; j < size_graph; j++) 
		{
			if (matrix[i][j] == 0 || matrix[i][j] == inf)
			{
				pred[i][j] = -1;
			}
			else 
			{
				pred[i][j] = j;
			}
		}
	}

	//Print_matrix(matrix, size_graph);
	//Print_matrix(pred, size_graph);
	for (size_t k = 0; k < size_graph; k++)
	{
		for (size_t i = 0; i < size_graph; ++i)
		{
			for (size_t j = 0; j < size_graph; j++)
			{
				if (matrix[i][k] + matrix[k][j] < matrix[i][j])
				{
					matrix[i][j] = matrix[i][k] + matrix[k][j];
					pred[i][j] = pred[i][k];	
				}
			}
		}
	}

	//Print_matrix(matrix, size_graph);
	//Print_matrix(pred, size_graph);
	//for (size_t i = 0; i < size_graph; i++)
	//{
	//	for (size_t j = 0; j < size_graph; ++j)
	//	{
	//		printf(" %d ", pred[i][j]);
	//	}
	//	printf("\n");
	//}
	pairs_1 = mini(matrix, size_graph, pairs_1);
	pairs_2 = min_bigger(matrix, size_graph, pairs_2, pairs_1);
	pairs_3 = min_bigger_s(matrix, size_graph, pairs_3, pairs_2, pairs_1);
	//Print_matrix(pred, size_graph);
	printf("Dist -> %f key1 -> %d key2 -> %d\n", pairs_1->dist, pairs_1->i, pairs_1->j);
	Print_path_f(matrix, pred, pairs_1->i, pairs_1->j);
	printf("Dist -> %f key1 -> %d key2 -> %d\n", pairs_2->dist, pairs_2->i, pairs_2->j);
	Print_path_f(matrix, pred, pairs_2->i, pairs_2->j);
	printf("Dist -> %f key1 -> %d key2 -> %d\n", pairs_3->dist, pairs_3->i, pairs_3->j);
	Print_path_f(matrix, pred, pairs_3->i, pairs_3->j);
	free(pairs_1);
	free(pairs_2);
	free(pairs_3);
	for (size_t i = 0; i < size_graph; i++)
	{
		free(matrix[i]);
		free(pred[i]);
	}
	free(matrix);
	free(pred)	;
	return 1;
}

int D_Graph_random(Graph* graph) 
{
	int res;
	int vertex_size;
	int edges_size;
	printf("Input size of vertex ->");
	if (getInt(&vertex_size) == 0) { return 0; }
	printf("Input size of edges ->");
	if (getInt(&edges_size) == 0) { return 0; }
	res = Graph_random(graph, vertex_size, edges_size);
	if (res == -1)
	{
		printf("Very danse graph (*2)\n");
		return 1;
	}
	if (vertex_size)
	{
		printf("Succses\n");
	}
	return 1;
}

int Graph_random(Graph* graph, int vertex_size, int edges_size) 
{
	int max_edges = vertex_size * vertex_size;
	if (edges_size > max_edges)
	{
		return -1; //плотный граф
	}
	//Delete_Graph(graph);
	//int* vertex_array = (int*)malloc(sizeof(int)* vertex_size);
	//for (size_t i = 0; i < vertex_size; i++)
	//{
	//	vertex_array[i] = i;
	//}

	for (size_t i = 0; i < vertex_size; i++)
	{
		int r_x = rand() % (max_x - min_x + 1) + min_x;
		int r_y = rand() % (max_y - min_y + 1) + min_y;
		printf("X -> %d, Y -> %d\n", r_x, r_y);
		if (Insert_vertex(graph, r_x, r_y, 1) != 1)
		{
			--i;
		}
	}
	for (int i = 0; i < edges_size; i++)
	{
		int r_key1 = rand() % (vertex_size) + 1;
		int r_key2 = rand() % (vertex_size) + 1;
		printf("key1 -> %d, key2 -> %d\n", r_key1, r_key2);
		if (Insert_edge(graph, r_key1, r_key2) != 1)
		{
			--i;
		}
	}
	return 1; //успешно
}

int Graph_load(Graph* graph) 
{
	FILE* file_tmp = NULL;
	int x;
	int y;
	int size_vertex;
	int size_edge;
	char fname[] = "Info.txt";
	file_tmp = fopen(fname, "r");
	if (file_tmp)
	{
		fscanf(file_tmp, "%d \n %d\n", &size_vertex, &size_edge);
		for (size_t i = 0; i < size_vertex + size_edge; i++)
		{
			if ( i < size_vertex)
			{
				fscanf(file_tmp, "%d %d\n", &x, &y);
				Insert_vertex(graph, x, y, 1);
			}
			else 
			{
				fscanf(file_tmp, "%d %d\n", &x, &y);
				Insert_edge(graph, x, y);
			}			
		}
	}
	else 
	{
		fprintf(stderr, "can't open this file %s\n", fname);
		return 1;
	}
	fclose(file_tmp);
	return 1;
}

int Size_edges(Graph* graph)
{
	int size = 0;
	Vertex* vertex = NULL;
	ListEdge* edge = NULL;
	vertex = graph->begin;
	if (vertex == NULL)
	{
		return 0;
	}
	while (vertex != NULL) 
	{
		edge = vertex->adj;
		while (edge != NULL) 
		{
			++size;
			edge = edge->next;
		}
		vertex = vertex->next;
	}
	return size;
}

int Graph_unload(Graph* graph)
{
	FILE* file_tmp = NULL;
	Vertex* vertex = NULL;
	vertex = graph->begin;
	ListEdge* edge = NULL;
	int x, y;
	int size_edge = Size_edges(graph);
	char fname[] = "Info.txt";
	int size_graph = Size_of_graph(graph);
	file_tmp = fopen(fname, "w");
	if (file_tmp)
	{
		fprintf(file_tmp, "%d\n%d\n", size_graph, size_edge);
		while (vertex != NULL)
		{
			fprintf(file_tmp, "%d %d\n", vertex->x, vertex->y);
			vertex = vertex->next;
		}
		vertex = graph->begin;
		while (vertex != NULL) 
		{
			edge = vertex->adj;
			while (edge != NULL)
			{
				fprintf(file_tmp, "%d %d\n", vertex->key, edge->vertex->key);
				edge = edge->next;
			}
			vertex = vertex->next;
		}
	}
	else
	{
		fprintf(stderr, "can't open this file %s\n", fname);
		return 1;
	}
	fclose(file_tmp);
	return 1;
}

int Delete_Graph(Graph* graph) 
{
	int size = Size_of_graph(graph);
	for (size_t i = 0; i < size; i++)
	{
		Delete_vertex(graph, 1);
	}
	return 1;
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
	Graph_unload(graph);
	Delete_Graph(graph);
	return 0;
}

int getInt(int* a)
{
	int n;
	do {
		n = scanf("%d", a);
		if (n == 0)
		{
			printf("ERROR BAD INPUT\n");
			scanf("%*[^\n]");
			scanf("%*c", 0);
		}
	} while (n == 0);
	scanf("%*c", 0);
	return n < 0 ? 0 : 1;
}

int getPositiveInt(int* a)
{
	int n;
	do {
		n = scanf("%d", a);
		if (n == 0 || (*a <= 0))
		{
			printf("ERROR BAD INPUT\n");
			scanf("%*[^\n]");
			scanf("%*c", 0);
		}
	} while (n == 0 || (*a <= 0));
	scanf("%*c", 0);
	return n < 0 ? 0 : 1;
}