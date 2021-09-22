#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <malloc.h>
#include "Gets.h"
#define size_key 4
#define size_table 11

struct Node;
struct Item;

typedef struct Item
{
	int key;
	char* info_string;
}Item;

typedef struct Node
{
	int n;
	Item* item[size_key - 1];
	struct Node* child[size_key];
}Node;

Node* root = NULL; //global tree

Node* Create_Node();
Item* Get_min_node(Node* root);
Node* Search_Node(int key);
Item* Get_min_node(Node* root);
Item* Get_key_node_max(Node* root);
Item* Getr_key_node_min(Node* root);
Item* Search(Node* node, int key);
Node* Split_node_rigth(Node* node, int position);
Node* Split_node_left(Node* node, int position);
Node* Memory_change_RL(Node* node, int position);
Node* Memory_change_LR(Node* node, int position);
int Delete_item(Node** root, int key);
int Find_position_i(Node* node, int key);
int Split_tree(Node* node, int number);
int InsertNonFull(Node* node, Item* info);
int Insert_item(Node** root, Item* item);
int D_Insert();
int D_Timing();
int D_search();
int D_Print();
int D_Delete();
int D_Key_differ();
int Creep_walk();
int ifleaf(Node* node);
int dialog(const char* msgs[], int N);
void Delete_tree(Node** root);
int Delete_node(Node* node);
void Print_Node(Node* node, int lvl);
int D_Key_differ();


int(*fptr[])() = { NULL, D_Insert, D_Print, D_search, D_Delete,D_Key_differ, Creep_walk ,D_Timing };
const char* msgs[] = { "0. Quit", "1. Add", "2. Print tree", "3. Search","4. Delete","5 Key differ", "6. Walk", "7.Timing" };
const char* errors[] = { "Bad size of key \n", "Didnt find this key \n", "Tree is empty \n", "Bad input\n" };
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

int D_Delete()
{
	int n = -1;
	int key = -1;
	if (root == NULL)
	{
		printf("%s\n\n", errors[2]);
		return 1;
	}
	printf("Input key -> ");
	if (getInt(&key) == 0) { return 0; }

	if (Delete_item(&root, key) == -1)
	{
		printf("%s\n\n", errors[2]);
	}
	else
	{
		printf("Succsess\n");
	}

	return 1;

}

int D_Timing()
{
	Node* node = Create_Node();
	int n = 20, key[10000], k, cnt = 100000, i, m;
	clock_t first, last;
	srand(time(NULL));
	while (--n > 0)
	{
		for (i = 0; i < 10000; i++)
		{
			key[i] = rand() * rand();
		}
		for (i = 0; i < cnt;)
		{
			Item* temp = (Item*)malloc(sizeof(Item));
			temp->key = rand() * rand();
			if (Insert_item(&node, temp) != -1)
			{
				i++;
			}
		}
		m = 0;
		first = clock();
		for (i = 0; i < 10000; i++)
		{
			if (Search(node, key[i]) != NULL)
			{
				m++;
			}
		}
		last = clock();
		printf("Test #%d, number of nodes = %d, time = %d\n", 20 - n, (20 - n) * cnt, last - first);
		//printf("%d\t%d\n", 20 - n, last - first);
	}

	Delete_tree(&node);
	return 1;
}

Item* Get_min_node(Node * root)
{
	while (root->child[0] != NULL)
	{
		root = root->child[0];
	}
	return root->item[0];
}

Item* Get_max_node(Node * root)
{
	while (root->child[root->n] != NULL)
	{
		root = root->child[root->n];
	}

	return root->item[root->n - 1];
}

Item* Get_key_node_max(Node * root)
{
	int index = 0;
	while (root->child[root->n] != NULL)
	{
		root = root->child[root->n];
	}

	while (root->item[index] != NULL)
	{
		index++;
	}

	return root->item[index - 1];
}

Item* Get_key_node_min(Node * root)
{
	int index = -1;

	while (root->child[0] != NULL)
	{
		root = root->child[0];
	}

	return root->item[0];
}

void Print_Node(Node * node, int lvl)
{
	if (node != NULL)
	{
		for (int i = node->n; i; i--)
		{
			Print_Node(node->child[i], lvl + 1);

			for (int j = 0; j < lvl; j++)
			{
				printf("\t");
			}
			printf("key: %d \n", node->item[i - 1]->key);
		}

		Print_Node(node->child[0], lvl + 1);
	}
}

void Creep_node(Node * node)
{
	if (node != NULL)
	{
		for (int i = node->n; i; i--)
		{
			Creep_node(node->child[i]);
			printf("key-> %d \n", node->item[i - 1]->key);
			printf("Info-> %s \n", node->item[i - 1]->info_string);
		}

		Creep_node(node->child[0]);
	}
}

int Creep_walk()
{
	if (root == NULL)
	{
		printf("%s\n\n", errors[2]);
		return 1;
	}
	if (root->n)
	{
		Creep_node(root);
	}
	else { printf("%s\n\n", errors[2]); }
}

int D_Print()
{
	if (root == NULL)
	{
		printf("%s\n\n", errors[2]);
		return 1;
	}

	if (root->n)
	{
		Print_Node(root, 0);
	}
	else { printf("%s\n\n", errors[2]); }

	return 1;
}

int D_Insert()
{
	int n;

	Item* item = (Item*)malloc(sizeof(Item));
	printf("Key -> ");
	if (getInt(&item->key) == 0) { return 0; }
	printf("\n");
	Node* temp = Search_Node(item->key);
	if (temp != NULL)
	{
		printf("%s\n\n", errors[3]);
		free(item);
		return 1;
	}
	printf("Info -> ");
	item->info_string = getStr();
	printf("\n");
	Insert_item(&root, item);
	return 1;
}

Node* Merge(Node * node, int i)
{
	int j;

	Node* left = node->child[i - 1];
	Node* right = node->child[i];

	left->item[left->n] = node->item[i - 1];
	left->n++;

	Split_node_left(node, i);

	for (j = 0; j < right->n; j++, left->n++)
	{
		left->item[left->n] = right->item[j];
		left->child[left->n] = right->child[j];
	}

	left->child[left->n] = right->child[right->n];

	free(right);
	right = NULL;
	return left;
}

Node* Split_node_left(Node * node, int i)
{
	for (int j = i; j < node->n; j++)
	{
		node->item[j - 1] = node->item[j];
		node->child[j] = node->child[j + 1];
	}

	for (int j = node->n; j < 4; j++)
	{
		node->child[j] = NULL;
	}

	for (int j = node->n - 1; j < 3; j++)
	{
		node->item[node->n - 1] = NULL;
	}

	node->n--;

	return node;
}

Node* Split_node_rigth(Node* node, int i)
{
	node->child[node->n + 1] = node->child[node->n];

	for (int j = node->n; j > i; j--)
	{
		node->item[j] = node->item[j - 1];
		node->child[j] = node->child[j - 1];
	}

	node->n++;

	return node;
}

int Delete_item(Node** root, int key)
{
	int index = -1;
	int flag = 0;
	Node* node = NULL;
	Node* merge_left = NULL;
	Node* merge_right = NULL;
	Node* node_child = NULL;
	Item* tmp;

	if ((*root)->child[0] && (*root)->n == 1 && (*root)->child[0]->n == 1 && (*root)->child[1]->n == 1)
	{
		merge_left = Merge(*root, 1);
		free(*root);
		*root = merge_left;
		flag = 1;
	}

	node = *root;

	while (node->child[0] != NULL)
	{
		index = Find_position_i(node, key);

		if (index < node->n && node->item[index]->key == key)
		{
			merge_left = node->child[index];
			merge_right = node->child[index + 1];

			if (merge_left->n >= 2)
			{
				tmp = Get_max_node(merge_left);
				key = tmp->key;
				node->item[index] = tmp;
				node = merge_left;
			}
			else if (merge_right->n >= 2)
			{
				tmp = Get_min_node(merge_right);
				key = tmp->key;
				node->item[index] = tmp;
				node = merge_right;
			}
			else
			{
				node = merge_left = Merge(node, index + 1);
			}
		}
		else
		{
			node_child = node->child[index];
			if (node_child->n < 2)
			{
				merge_left = merge_right = NULL;
				if (index > 0 && node->child[index - 1]->n >= 2)
				{
					merge_left = node->child[index - 1];
					Memory_change_LR(node, index - 1);
				}
				else
				{
					if (index < node->n && node->child[index + 1]->n >= 2)
					{
						merge_right = node->child[index + 1];
						Memory_change_RL(node, index);
					}
					else
					{
						if (node->child[index + 1] != NULL && index + index <= node->n)
						{
							node_child = Merge(node, index + 1);
						}
						else
						{
							node_child = Merge(node, index);
						}
					}
				}
			}
			node = node_child;
		}
	}

	index = Find_position_i(node, key);

	if (index < node->n && node->item[index]->key == key)
	{
		Split_node_left(node, index + 1);
		return 0;
	}

	return -1;
}

Node* Memory_change_RL(Node* node, int i)
{
	Node* node_temp1 = NULL;
	Node* node_temp2 = NULL;

	node_temp1 = node->child[i];
	node_temp2 = node->child[i + 1];

	node_temp1->item[node_temp1->n] = node->item[i];
	++node_temp1->n;
	node_temp1->child[node_temp1->n] = node_temp2->child[0];
	node->item[i] = node_temp2->item[0];
	node_temp2->child[0] = node_temp2->child[1];

	Split_node_left(node_temp2, 1);

	return node;
}

Node* Memory_change_LR(Node * node, int i)
{
	Node* node_temo1 = NULL;
	Node* node_temp2 = NULL;

	node_temo1 = node->child[i];
	node_temp2 = node->child[i + 1];

	Split_node_rigth(node_temp2, 0);
	node_temp2->item[0] = node->item[i];
	node_temp2->child[0] = node_temo1->child[node_temo1->n];
	node->item[i] = node_temo1->item[node_temo1->n - 1];

	for (int j = node_temo1->n; j < 4; j++)
	{
		node_temo1->child[j] = NULL;
	}

	for (int j = node_temo1->n - 1; j < 3; j++)
	{
		node_temo1->item[node_temo1->n - 1] = NULL;
	}

	node_temo1->n--;

	return node;
}

Item* Search(Node * node, int key)
{
	int i = 0;

	while (node != NULL)
	{
		i = Find_position_i(node, key);

		if (node->item[i] && node->item[i]->key == key)
		{
			return node->item[i];
		}
		else
		{
			node = node->child[i];
		}
	}

	return NULL;
}

Node* Search_Node(int key)
{
	Node* node_search = root;
	int i;
	while (node_search != NULL)
	{
		i = Find_position_i(node_search, key);

		if (node_search->item[i] && node_search->item[i]->key == key)
		{
			return node_search;
		}
		else
		{
			node_search = node_search->child[i];
		}
	}

	return NULL;
}

Node* Create_Node()
{
	Node* node = NULL;
	node = (Node*)malloc(sizeof(Node));
	int save_index = 0;

	for (save_index = 0, node->n = 0; save_index < size_key; save_index++)
	{
		*(node->child + save_index) = NULL;
	}

	for (save_index = 0; save_index < (size_key - 1); save_index++)
	{
		node->item[save_index] = NULL;
	}

	return node;
}

int ifleaf(Node * node) //leaf check 
{
	int l = 1;

	for (int i = 0; i < size_key; i++)
	{
		if (node->child[i] != NULL)
		{
			l = 0;
		}
	}
	return l;
}

void Delete_tree(Node * *root)
{
	Delete_node(*root);
}

int Delete_node(Node * node)
{
	Node* ptr = NULL;

	for (int i = 0; i <= node->n; i++)
	{
		ptr = node->child[i];
		if (ptr == NULL) { continue; }
		Delete_node(ptr);
		free(ptr);
		ptr = NULL;
	}

	return 0;
}

int Find_position_i(Node * node, int key)
{
	int i = 0;

	for (; i < node->n && key > node->item[i]->key; i++);
	return i;
}

int Split_tree(Node * node, int number)
{
	int j;
	Node* node_child = node->child[number];
	for (j = node->n - 1; j >= number; j--)
	{
		node->child[j + 2] = node->child[j + 1];
		node->item[j + 1] = node->item[j];
	}

	Node* new_node = Create_Node();

	for (j = node_child->n / 2 + 1, new_node->n = 0; j < node_child->n; j++, new_node->n++)
	{
		new_node->child[new_node->n] = node_child->child[j];
		new_node->item[new_node->n] = node_child->item[j];
	}

	new_node->child[new_node->n] = node_child->child[j];
	node->child[number + 1] = new_node;
	node->item[number] = node_child->item[node_child->n / 2];
	node_child->n = node_child->n / 2;

	for (int i = size_key - 1; i > node_child->n; i--)
	{
		node_child->item[i - 1] = NULL;
	}

	node->n++;

	return 0;
}

int InsertNonFull(Node * node, Item * info)
{
	int index;
	Node* search_child = NULL;
	while (node->child[0])
	{
		index = Find_position_i(node, info->key);
		if (index < node->n && node->item[index]->key == info->key) { return -1; }

		Node* search_chld = node->child[index];
		if (search_chld->n == size_key - 1)
		{
			Split_tree(node, index);
			if (info->key > node->item[index]->key)
			{
				search_chld = node->child[index + 1];
			}
		}
		node = search_chld;
	}
	index = Find_position_i(node, info->key);

	if (index < node->n && info->key == node->item[index]->key) { return -1; }

	for (int j = node->n - 1; j >= index; j--)
	{
		node->item[j + 1] = node->item[j];
	}
	node->item[index] = info;
	node->n++;
	return 1;
}

int Insert_item(Node * *root, Item * item)
{
	Node* search_node = NULL;
	if (*root == NULL)
	{
		*root = Create_Node();
		(*root)->item[0] = item;
		(*root)->n = 1;
		return 1;
	}
	if ((*root)->n == size_key - 1)
	{
		search_node = Create_Node();
		search_node->child[0] = *root;
		Split_tree(search_node, 0);
		*root = search_node;
	}
	InsertNonFull(*root, item);
	return 1;
}



int D_search()
{
	int n = -1;
	int key = -1;
	Item* result = NULL;
	Node* node_search = root;

	if (root == NULL)
	{
		printf("%s\n\n", errors[2]);
		return 1;
	}

	printf("Input key ->");
	if (getInt(&key) == 0) { return 0; }
	printf("\n");
	result = Search(node_search, key);

	if (result != NULL)
	{
		printf("Key -> %d", result->key);
		printf("\n");
		printf("Info -> %s", result->info_string);
		printf("\n");
	}
	else
	{
		printf("I didnt find this item...\n");
	}
}

int D_Key_differ()
{
	Item* node_search_max = NULL;
	Item* node_search_min = NULL;
	int key = -1;
	if (root == NULL)
	{
		printf("%s\n\n", errors[2]);
		return 1;
	}
	printf("Input key ->");
	if (getInt(&key) == 0) { return 0; }
	printf("\n");

	node_search_max = Get_key_node_min(root);
	node_search_min = Get_key_node_max(root);

	if (node_search_max == NULL)
	{
		if (node_search_min == NULL)
		{
			printf("In tree we have one item\n");
		}
		else
		{
			printf("Differens key -> %d", node_search_min->key);
			return 1;
		}
	}

	if (node_search_min == NULL)
	{
		if (node_search_max == NULL)
		{
			printf("In tree we have one item\n");
		}
		else
		{
			printf("Differens key -> %d", node_search_max->key);
			return 1;
		}
	}
	if (node_search_max->key == node_search_min->key)
	{
		printf("Differens key -> %d", node_search_min->key);
		return 1;
	}
	if (abs(node_search_min->key - key) > abs(node_search_max->key - key))
	{
		printf("Differens key -> %d", node_search_min->key);
		return 1;
	}
	else
	{
		if (abs(node_search_max->key - key) > abs(node_search_min->key - key))
		{
			printf("Differens key -> %d", node_search_max->key);
			return 1;
		}
	}
}

int main()
{
	root = Create_Node();

	FILE* file_temp;
	char fname[] = "info.txt";
	file_temp = fopen(fname, "r");
	if (file_temp)
	{
		for (size_t i = 0; i < 16; i++)
		{
			char* info = (char*)malloc(100 * sizeof(char));
			Item* item = (Item*)malloc(sizeof(Item));

			fscanf(file_temp, "%d\n %s\n", &item->key, info);
			item->info_string = info;
			Insert_item(&root, item);
		}
	}
	else
	{
		fprintf(stderr, "can't open this file %s\n", fname);
		return 1;
	}

	int rc = 0, f = 1;
	while ((rc = dialog(msgs, NMsgs)) && f)
	{
		f = fptr[rc]();
	}
	Delete_tree(root);
	return 0;
}