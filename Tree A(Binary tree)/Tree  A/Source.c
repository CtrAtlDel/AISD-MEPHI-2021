#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include"Gets.h"
#include<time.h>
#define SIZE 4
#define size_file_str 100
typedef struct info
{
	char* str;
	struct Node* next_inf;
	int realese;
}info;

typedef struct Node
{
	struct Node* prev;
	struct Node* next;
	struct Node* left;
	struct Node* parent;
	struct Node* right;
	struct info str;
	char* key;
}Node;

typedef struct Tree
{
	Node* root;
}Tree;

Tree* Create_tree(Tree* tree);
Node* Create_node(Node* parent, char* key, char* str);
Node* Create_node_inf(char* key, char* str);
Node* Get_min_node(Node* root);
Node* Get_max_node(Node* root);
Node* Get_key_node(Node* root, char* key);
Node* successor(Node* node);
int Insert_node(Node** node, char* key, char* info);
int Delete_rel(Tree* tree, Node** root, char* key, int release);
int D_Insert_node(Tree* tree);
int D_find(Tree* tree);
int D_find_min(Tree* tree);
int D_walk(Tree* tree);
void Print_table(Tree* root);
void Print_node(Node* node);
void Print_tree(Tree* tree);
int D_delete(Tree* tree);
int d_timing(Node** temp);
int Height(Node* node);
int root_Height(Tree* tree);


int(*fptr[])(Tree*) = { NULL, D_Insert_node, Print_table, D_delete, D_walk, D_find, D_find_min, Print_tree, d_timing, root_Height };
const char* msgs[] = { "0. Quit", "1. Add", "2. Print table", "3. Delete", "4. Walk", "5. Find", "6. Find min", "7. Like tree", "8. Timing", "9. Height" };
const char* errors[] = { "Bad size of key \n", "Didnt find this key \n", "Tree is empty \n", "Bad input\n" };
int dialog(const char* msgs[], int N);
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
const int Nerrors = sizeof(errors) / sizeof(errors[0]);

char* randomstr()
{
	int size = rand() % 100 + 2;
	char* str = NULL;
	str = (char*)malloc(size * sizeof(char));
	int i = 0;
	for  ( ; i < size - 1; ++i)
	{
		*(str + i) = rand() % 26 + 'a';
	}
	*(str + i) = '\0';
	return str;
}

int d_timing(Node** temp)
{
	Tree* tree = NULL;
	tree = Create_tree(tree);
	int n = 10, k, cnt = 100000, i, m;
	char** keybuff = NULL;
	keybuff = (char*)malloc(sizeof(char) * 10000);

	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0)
	{
		for (size_t i = 0; i < 10000; i++) 
		{
			*(keybuff + i) = randomstr();
		}
		for (i = 0; i < cnt; )
		{
			char* key = randomstr();
			char* info = malloc(sizeof(char));
			info[0] = '\0';
			if (Insert_node(&(tree->root),key, info))//вставка в дерево
				++i;
		}
		m = 0;
		first = clock();
		for (i = 0; i < 10000; ++i)
		{
			if (Get_key_node(tree->root, *(keybuff + i)) != NULL)// поиск по ключу
				++m;
		}
		last = clock();
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 - n) * cnt, last - first);
	}
	//deltree(&root);
	return 1;
}

int dialog(const char* msgs[], int N)
{
	char* errmsg = "";
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

void walk(Node * root)
{
	if (root != NULL)
	{
		walk(root->left);
		printf("Key -> %s\n", root->key);
		printf("Info -> %s\n", root->str.str);
		walk(root->right);
	}
}

int D_walk(Tree * tree)
{
	walk(tree->root);
}

void Delete_node(Node * node) //free(node)
{
	Node* temp = NULL;

	while (node != NULL)
	{
		temp = node;
		node = node->str.next_inf;
		if (temp->key != NULL)
		{
			free(temp->key);
		}
		if (temp->str.str != NULL)
		{
			free(temp->str.str);
		}
		free(temp);
	}

}

void Delete_inf(Node * node)
{
	Node* temp = NULL;
	free(node->key);
	free(node->str.str);
	free(node);
}

void Delete_tree(Tree * tree)
{
	Node* temp = NULL;
	Node* root = NULL;
	root = Get_min_node(tree->root);// начало списка
	while (root != NULL)
	{
		temp = root;
		root = root->str.next_inf;
		Delete_node(temp);
	}
	free(tree);
}

int D_delete(Tree * tree)
{
	char* key = NULL;
	int res = 0;
	int realese = -1;
	int ch = -1;
	printf("Input key -> ");
	key = getStr();
	printf("\n");
	printf("Whith realese? (1/0)");
	if (getInt(&ch) == 0) { return 0; }
	if (ch)
	{
		printf("Input realese ->");
		if (getInt(&realese) == 0) { return 0; }
		printf("\n");
		res = Delete_rel(tree, &(tree->root), key, realese);

	}
	else
	{
		if (ch == 0)
		{
			res = Delete(tree, &(tree->root), key, -1);
		}
		else { printf("%s", errors[3]); }
	}
	if (res == 1) { printf("Success\n"); }
	if (res == -2) { printf("Bad size of key \n"); }
	if (res == -1) { printf("I didnt found this key or tree empty\n"); }
	if (res == -3) { printf("I didnt found this realese\n"); }
}

int Delete(Tree * tree, Node * *root, char* key, int release) //еще прошивка
{
	Node* node_search = NULL;
	Node* mig = NULL;
	Node* for_tread = NULL;
	Node* ptr = NULL;
	Node* last_right = NULL;
	Node* parent = NULL;
	Node* temp = NULL;
	if (strlen(key) > SIZE)
	{
		return -2;
	}
	for_tread = *root;
	node_search = NULL;//root
	while (for_tread)
	{
		if (strcmp(key, for_tread->key) > 0)
		{
			last_right = for_tread;//последний правый указатель
			for_tread = for_tread->right;
			continue;
		}
		if (strcmp(key, for_tread->key) < 0)
		{

			for_tread = for_tread->left;
			continue;
		}
		if (strcmp(key, for_tread->key) == 0)
		{
			node_search = for_tread;
			break; //нашли нужный узел
		}
	}
	if (node_search == NULL) { return -1; }
	if (release == -1)
	{
		if ((node_search->left == NULL) && (node_search->right == NULL)) //нет поддеревьев done.
		{
			ptr = node_search->parent;

			if (ptr == NULL)
			{
				Delete_node(node_search);
				tree->root = NULL;
				return 1;
			}
			if (node_search == ptr->right)
			{
				ptr->right = NULL;
				ptr->next = node_search->next;
			}
			if (node_search == ptr->left)
			{
				ptr->left = NULL;
				if (last_right != NULL)
				{
					last_right->next = node_search->next;
				}
			}
			Delete_node(node_search);
			return 1;
		}
		if (node_search->left == NULL && node_search->right != NULL) // поддерево справа 
		{
			ptr = node_search->parent;
			if (ptr == NULL)
			{
				temp = node_search->right;
				//temp->next = node_search->next;
				temp->parent = node_search->parent;
				Delete_node(node_search);
				tree->root = temp;
				return 1;
			}
			if (node_search == ptr->right)
			{
				ptr->right = node_search->right;
				if (node_search->right == Get_min_node(node_search->right))
				{
					ptr->next = node_search->right;
				}
				else
				{
					temp = Get_min_node(node_search);
					temp->next = node_search->next;
					Delete_node(node_search);
					return 1;
				}
			}
			if (node_search == ptr->left)
			{
				ptr->left = node_search->right;
				if (node_search->str.next_inf == NULL)
				{
					if (last_right)
					{
						last_right->next = node_search->next;
						Delete_node(node_search);
						return 1;
					}
				}
			}

		}
		if (node_search->left != NULL && node_search->right == NULL)
		{
			ptr = node_search->parent;

			if (ptr == NULL)
			{
				temp = node_search->left;
				temp->parent = node_search->parent;
				temp->next = node_search->next;
				Delete_node(node_search);
				tree->root = temp;
				return 1;
			}
			if (node_search == ptr->right)
			{
				ptr->right = node_search->left;
				temp = Get_max_node(node_search->left);
				temp->next = node_search->next;
				Delete_node(node_search);
				return 1;

			}
			if (node_search == ptr->left)
			{
				ptr->left = node_search->left;
				temp = Get_max_node(node_search->left);

				temp->next = node_search->next;
				Delete_node(node_search);
				return 1;
			}

		}
		if (node_search->left != NULL && node_search->right != NULL)
		{
			temp = Get_max_node(node_search->left);
			ptr = node_search->parent;
			if (ptr == NULL)
			{
				if (node_search->left == temp)
				{
					temp->next = node_search->next;
					temp->parent = node_search->parent;
					temp->right = node_search->right;
					node_search->right->parent = temp;

					Delete_node(node_search);
					tree->root = temp;
					return 1;
				}

				node_search->right->parent = temp;
				node_search->left->parent = temp;
				temp->next = node_search->next;
				temp->parent->right = temp->left;
				temp->parent = NULL;

				temp->right = node_search->right;
				temp->left = node_search->left;
				Delete_node(node_search);
				tree->root = temp;
				return 1;
			}
			if (node_search == ptr->left)
			{
				if (temp->parent != node_search)
				{
					temp->parent->right = temp->left;
					temp->parent = node_search->parent;
					node_search->parent->left = temp;

					temp->right = node_search->right;
					temp->next = node_search->next;
					Delete_node(node_search);
					return 1;
				}
				else
				{
					node_search->parent->left = temp;

					temp->right = node_search->right;
					temp->parent = node_search->parent;
					temp->next = node_search->next;
					Delete_node(node_search);
					return 1;
				}
			}
			if (node_search == ptr->right)//some problems
			{
				Node* save = NULL;
				//v otr est parent
				if (temp->parent != node_search)
				{
					temp->parent->right = temp->left;//FIX IT
					if (temp->left)
					{
						temp->left->parent = temp->parent;
					}

					temp->parent->right = temp->left;
					temp->next = node_search->next;

					//temp->parent = temp;
					if (temp->left != NULL)
					{
						temp->left->parent = temp;
					}
					node_search->parent->right = temp;

					if (node_search->left != NULL)
					{
						node_search->left->parent = temp;
					}

					temp->left = node_search->left;
					temp->right = node_search->right;
					Delete_node(node_search);
					temp->parent = ptr;
					return 1;
				}
				else
				{
					if (temp->left != NULL)
					{
						temp->left->parent = temp->parent;
					}
					else { temp->parent->right = NULL; }

					temp->next = node_search->next;
					node_search->parent->right = temp;
					if (node_search->right)
					{
						node_search->right->parent = temp;
					}
					temp->right = node_search->right;
					temp->parent = node_search->parent;

					Delete_node(node_search);
					return 1;

				}
			}
			return 1;
		}
	}
	else
	{
		Node* tempp = NULL;
		Node* prev = NULL;
		Node* parent = NULL;
		int flag = 0;
		//Удаление элемента в списке
		//
		prev = Get_min_node(tree->root);//список по элементам дерева
		tempp = node_search;
		while (tempp != NULL)
		{
			if (tempp->str.realese == release)
			{
				flag = 1;
				break;
			}
			tempp = tempp->str.next_inf;
		}
		if (tempp == NULL)
		{
			return -3; //Не нашли версию
		}
		parent = tempp->parent;//указатель на родителя
		if (tempp == node_search)
		{

			return 1;
		}
		else
		{
			prev = node_search;
			while (prev->str.next_inf != tempp)
			{
				prev = prev->str.next_inf;
			}
			prev->str.next_inf = tempp->str.next_inf;
			//удаление temp
			free(tempp->key);
			free(tempp->str.str);
			free(tempp);
			return 1;//well done
		}
	}
	return 1;
}

int Delete_rel(Tree * tree, Node * *root, char* key, int release)
{
	Node* node_search = NULL;
	Node* mig = NULL;
	Node* for_tread = NULL;
	Node* ptr = NULL;
	Node* last_right = NULL;
	Node* parent = NULL;
	Node* temp = NULL;
	if (strlen(key) > SIZE)
	{
		return -2;
	}
	for_tread = *root;
	node_search = NULL;//root
	while (for_tread)
	{
		if (strcmp(key, for_tread->key) > 0)
		{
			last_right = for_tread;
			for_tread = for_tread->right;
			continue;
		}
		if (strcmp(key, for_tread->key) < 0)
		{

			for_tread = for_tread->left;
			continue;
		}
		if (strcmp(key, for_tread->key) == 0)
		{
			node_search = for_tread;
			break;
		}
	}
	if (node_search == NULL) { return -1; }
	if (release == -1)
	{
		if ((node_search->left == NULL) && (node_search->right == NULL))
		{
			ptr = node_search->parent;
			if (node_search->str.next_inf == NULL)
			{
				if (ptr == NULL)
				{
					Delete_node(node_search);
					tree->root = NULL;
					return 1;
				}
				if (node_search == ptr->right)
				{
					ptr->right = NULL;
					ptr->next = node_search->next;
				}
				if (node_search == ptr->left)
				{
					ptr->left = NULL;
					if (last_right != NULL)
					{
						last_right->next = node_search->next;
					}
				}
				Delete_node(node_search);
				return 1;
			}
			else
			{
				if (node_search->left != NULL)
				{
					node_search->left->parent = node_search->str.next_inf;
				}
				if (node_search->right != NULL)
				{
					node_search->right->parent = node_search->str.next_inf;
				}
				ptr = node_search->parent;
				node_search->str.next_inf->left = node_search->left;
				node_search->str.next_inf->right = node_search->right;
				node_search->str.next_inf->parent = node_search->parent;
				node_search->str.next_inf->next = node_search->next;
				mig = Get_min_node(tree->root);
				while (mig->next != node_search)
				{
					mig = mig->next;
				}
				mig->next = node_search->str.next_inf;
				if (ptr != NULL)
				{
					if (node_search == ptr->right)
					{
						ptr->right = node_search->str.next_inf;
					}
					if (node_search == ptr->left)
					{
						ptr->left = node_search->str.next_inf;
					}
				}
				Delete_inf(node_search);
				return 1;
			}
		}
		if (node_search->left == NULL && node_search->right != NULL) // ïîääåðåâî ñïðàâà 
		{
			ptr = node_search->parent;
			if (node_search->str.next_inf == NULL)
			{

				if (ptr == NULL)
				{
					temp = node_search->right;
					//temp->next = node_search->next;
					temp->parent = node_search->parent;
					Delete_node(node_search);
					tree->root = temp;
					return 1;
				}
				if (node_search == ptr->right)
				{
					ptr->right = node_search->right;
					if (node_search->right == Get_min_node(node_search->right))
					{
						ptr->next = node_search->right;
					}
					else
					{
						temp = Get_min_node(node_search);
						temp->next = node_search->next;
						Delete_node(node_search);
						return 1;
					}
				}
				if (node_search == ptr->left)
				{
					ptr->left = node_search->right;
					if (node_search->str.next_inf == NULL)
					{
						if (last_right)
						{
							last_right->next = node_search->next;
							Delete_node(node_search);
							return 1;
						}
					}
				}
			}
			else
			{
				if (node_search->left != NULL)
				{
					node_search->left->parent = node_search->str.next_inf;
				}
				if (node_search->right != NULL)
				{
					node_search->right->parent = node_search->str.next_inf;
				}
				ptr = node_search->parent;
				node_search->str.next_inf->left = node_search->left;
				node_search->str.next_inf->right = node_search->right;
				node_search->str.next_inf->parent = node_search->parent;
				node_search->str.next_inf->next = node_search->next;
				mig = Get_min_node(tree->root);
				while (mig->next != node_search)
				{
					mig = mig->next;
				}
				mig->next = node_search->str.next_inf;
				if (ptr != NULL)
				{
					if (node_search == ptr->right)
					{
						ptr->right = node_search->str.next_inf;
					}
					if (node_search == ptr->left)
					{
						ptr->left = node_search->str.next_inf;
					}
				}
				Delete_inf(node_search);
				return 1;
			}
		}
		if (node_search->left != NULL && node_search->right == NULL)
		{
			ptr = node_search->parent;
			if (node_search->str.next_inf == NULL)
			{
				if (ptr == NULL)
				{
					temp = node_search->left;
					temp->parent = node_search->parent;
					temp->next = node_search->next;
					Delete_node(node_search);
					tree->root = temp;
					return 1;
				}
				if (node_search == ptr->right)
				{
					ptr->right = node_search->left;
					temp = Get_max_node(node_search->left);
					temp->next = node_search->next;
					Delete_node(node_search);
					return 1;

				}
				if (node_search == ptr->left)
				{
					ptr->left = node_search->left;
					temp = Get_max_node(node_search->left);

					temp->next = node_search->next;
					Delete_node(node_search);
					return 1;
				}
			}
			else
			{
				//Добавить родителей
				ptr = node_search->parent;
				if (node_search->left != NULL)
				{
					node_search->left->parent = node_search->str.next_inf;
				}
				if (node_search->right != NULL)
				{
					node_search->right->parent = node_search->str.next_inf;
				}
				node_search->str.next_inf->left = node_search->left;
				node_search->str.next_inf->right = node_search->right;
				node_search->str.next_inf->parent = node_search->parent;
				node_search->str.next_inf->next = node_search->next;
				mig = Get_min_node(tree->root);
				while (mig->next != node_search)
				{
					mig = mig->next;
				}
				mig->next = node_search->str.next_inf;
				if (ptr != NULL)
				{
					if (node_search == ptr->right)
					{
						ptr->right = node_search->str.next_inf;
					}
					if (node_search == ptr->left)
					{
						ptr->left = node_search->str.next_inf;
					}
				}

				Delete_inf(node_search);
				return 1;
			}
		}
		if (node_search->left != NULL && node_search->right != NULL)
		{
			temp = Get_max_node(node_search->left);
			if (node_search->str.next_inf == NULL)
			{
				ptr = node_search->parent;
				if (ptr == NULL)
				{
					if (node_search->left == temp)
					{
						temp->next = node_search->next;
						temp->parent = node_search->parent;
						temp->right = node_search->right;
						node_search->right->parent = temp;

						Delete_node(node_search);
						tree->root = temp;
						return 1;
					}

					node_search->right->parent = temp;
					node_search->left->parent = temp;
					temp->next = node_search->next;
					temp->parent->right = temp->left;
					temp->parent = NULL;

					temp->right = node_search->right;
					temp->left = node_search->left;
					Delete_node(node_search);
					tree->root = temp;
					return 1;
				}
				if (node_search == ptr->left)
				{
					if (temp->parent != node_search)
					{
						temp->parent->right = temp->left;
						temp->parent = node_search->parent;
						node_search->parent->left = temp;

						temp->right = node_search->right;
						temp->next = node_search->next;
						Delete_node(node_search);
						return 1;
					}
					else
					{
						node_search->parent->left = temp;

						temp->right = node_search->right;
						temp->parent = node_search->parent;
						temp->next = node_search->next;
						Delete_node(node_search);
						return 1;
					}
				}
				if (node_search == ptr->right)//some problems
				{
					if (temp->parent != node_search)
					{
						temp->parent->right = temp->left;
						if (temp->left != NULL)
						{
							temp->left->parent = temp->parent;
						}
						else { temp->parent->right = NULL; }
						if (node_search->right != NULL)
						{
							node_search->right->parent = temp;
						}
						temp->next = node_search->next;
						if (node_search->left != NULL)
						{
							node_search->left->parent = temp;
						}
						temp->parent = ptr;
						ptr->right = temp;
						node_search->right->parent = temp;

						temp->left = node_search->left;
						temp->right = node_search->right;
						Delete_node(node_search);
						return 1;
					}
					else
					{
						temp->next = node_search->next;
						node_search->parent->right = temp;
						node_search->right->parent = temp;
						temp->right = node_search->right;
						temp->parent = node_search->parent;

						Delete_node(node_search);
						return 1;

					}
				}
				return 1;
			}
			else
			{
				////а
				ptr = node_search->parent;
				if (node_search->left != NULL)
				{
					node_search->left->parent = node_search->str.next_inf;
				}
				if (node_search->right != NULL)
				{
					node_search->right->parent = node_search->str.next_inf;
				}
				node_search->str.next_inf->left = node_search->left;
				node_search->str.next_inf->right = node_search->right;
				node_search->str.next_inf->parent = node_search->parent;
				node_search->str.next_inf->next = node_search->next;
				mig = Get_min_node(tree->root);
				while (mig->next != node_search)
				{
					mig = mig->next;
				}
				mig->next = node_search->str.next_inf;
				if (ptr != NULL)
				{
					if (node_search == ptr->right)
					{
						ptr->right = node_search->str.next_inf;
					}
					if (node_search == ptr->left)
					{
						ptr->left = node_search->str.next_inf;
					}
				}

				Delete_inf(node_search);
				return 1;
			}
		}
	}
	else
	{
		Node* tempp = NULL;
		Node* prev = NULL;
		int flag = 0;
		tempp = node_search;
		while (tempp != NULL)
		{
			if (tempp->str.realese == release)
			{
				flag = 1;
				break;
			}
			tempp = tempp->str.next_inf;
		}
		if (tempp == NULL)
		{
			return -3; //Íå íàøëè âåðñèþ
		}
		if (tempp == node_search)
		{
			flag = Delete_rel(tree, &(tree->root), key, -1);
			return flag;
		}
		else
		{
			prev = node_search;
			while (prev->str.next_inf != tempp)
			{
				prev = prev->str.next_inf;
			}
			prev->str.next_inf = tempp->str.next_inf;
			//óäàëåíèå temp
			free(tempp->key);
			free(tempp->str.str);
			free(tempp);
			return 1;//well done
		}
	}
	return 1;
}


Node* Get_key_node(Node * root, char* key)
{
	while (root != NULL)
	{
		if (strcmp(key, root->key) > 0)
		{
			root = root->right;
			continue;
		}
		if (strcmp(key, root->key) < 0)
		{
			root = root->left;
			continue;
		}
		if (strcmp(root->key, key) == 0)
		{
			return root;
		}
	}
	return NULL;
}

Node* Get_max_node(Node * root)
{
	while (root->right)
	{
		root = root->right;
	}
	return root;
}

Node* Get_min_node(Node * root)
{
	if (root == NULL)
	{
		return root;
	}
	while (root->left)
	{
		root = root->left;
	}
	return root;
}

Node* successor(Node * node) //ищем следующий за ним узел
{
	Node* ptr = NULL;
	if (node->right != NULL)
	{
		return Get_min_node(node->right);
	}
	ptr = node->parent;
	while ((ptr != NULL) && (node == ptr->right))
	{
		node = ptr;
		ptr = ptr->parent;
	}
	return ptr;
}

int D_find(Tree * tree)
{
	char* key;
	Node* node;
	int ch = -1;
	printf("Input key ->");
	key = getStr();
	printf("\n");
	node = Get_key_node(tree->root, key);
	if (node == NULL)
	{
		printf("%s \n", errors[1]);
	}
	else { Print_node(node); }
}

int D_find_min(Tree * tree)
{
	Node* node = NULL;
	printf("Node with min key: \n");
	node = Get_min_node(tree->root);
	if (node == NULL)
	{
		printf("%s \n", errors[2]);
	}
	else { Print_node(node); }
}

void Print_node(Node * node)
{
	Node* temp = NULL;
	temp = node;
	while (temp)
	{
		printf("Key -> %s\n", temp->key);
		printf("Info -> %s\n", temp->str.str);
		printf("Release -> %d\n", temp->str.realese);
		printf("\n");
		temp = temp->str.next_inf;
	}
}

void Print_table(Tree * tree)
{
	Node* head = NULL;
	Node* temp = NULL;
	head = Get_min_node(tree->root);
	while (head)
	{
		temp = head;
		while (temp)
		{
			printf("Key -> %s\n", temp->key);
			printf("Info -> %s\n", temp->str.str);
			printf("Release -> %d\n", temp->str.realese);
			printf("\n");
			temp = temp->str.next_inf;
		}
		head = head->next;
	}
}

int Height(Node * node)
{
	int left = 0;
	int right = 0;
	int res = 0;
	if (node == NULL)
	{
		return 0;
	}
	if (node->left)
	{
		left = Height(node->left);
	}
	if (node->right)
	{
		right = Height(node->right);
	}
	if (right > left)
	{
		res = right;
	}
	else { res = left; }

	return res + 1;
}

int root_Height(Tree* tree) 
{
	int res = 0;
	res = Height(tree->root);
	printf("res = \n");
	return 1;
}

void Print_node_tree(Node * node, int lvl)
{
	if (node)
	{
		Print_node_tree(node->left, lvl + 1);
		for (size_t i = 0; i < lvl; i++)
		{
			printf("\t\t");
		}
		printf("key : %s ", node->key);
		printf("\n");
		Print_node_tree(node->right, lvl + 1);
	}
}

void Print_tree(Tree * tree)
{
	if (tree->root == NULL)
	{
		printf("%s", errors[2]);
		return 1;
	}
	Print_node_tree(tree->root, 0);
}



int main()
{
	Tree* tree = NULL;
	tree = Create_tree(tree);

	FILE* file_temp;
	char fname[] = "List.txt";
	file_temp = fopen(fname, "r");
	if (file_temp)
	{
		for (size_t i = 0; i < 70; i++)
		{
			char* key = (char*)malloc(10 * sizeof(char));
			char* info = (char*)malloc(100 * sizeof(char));
			fscanf(file_temp, "%s\n %s\n", key, info);
			Insert_node(&(tree->root), key, info);
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
		f = fptr[rc](tree);
	}
	Delete_tree(tree);
	fclose(file_temp);
	return 0;
}

int Insert_node(Node * *root, char* key, char* info) //пусть есть элемент Node
{
	Node* new_node = NULL;
	Node* tread = NULL;
	int release = 0;
	Node* ptr_temp = NULL;
	Node* ptr1 = NULL;
	Node* inf = NULL;
	if ((strlen(key)) > SIZE) { return -1; }
	if (*root == NULL)
	{
		*root = Create_node(NULL, key, info);
		return 1;
	}
	ptr_temp = *root;
	ptr1 = *root;
	tread = *root;
	while (ptr_temp != NULL)
	{
		if (strcmp(key, ptr_temp->key) > 0)
		{
			tread = ptr_temp;
			if (ptr_temp->right != NULL)
			{
				//tread = ptr_temp;//запоминаем последний правый узел;
				ptr_temp = ptr_temp->right;
				continue;
			}
			else
			{
				ptr_temp->right = Create_node(ptr_temp, key, info);
				if (ptr_temp->right == Get_max_node(ptr1))
				{
					tread->next = ptr_temp->right;
					return 1;
				}
				ptr_temp->right->next = tread->next;
				tread->next = ptr_temp->right;
				return 1;
			}
		}
		if (strcmp(key, ptr_temp->key) < 0)
		{
			if (ptr_temp->left != NULL)
			{
				ptr_temp = ptr_temp->left;
				continue;
			}
			else
			{
				ptr_temp->left = Create_node(ptr_temp, key, info);
				if (ptr_temp->left == Get_min_node(ptr1)) //thread == NULL
				{
					ptr_temp->left->next = ptr_temp->left->parent;
					return 1;
				}
				ptr_temp->left->next = tread->next;
				tread->next = ptr_temp->left;
				return 1;
			}
		}
		if (strcmp(key, ptr_temp->key) == 0)
		{
			inf = ptr_temp;
			while (inf->str.next_inf != NULL)
			{
				inf = inf->str.next_inf;
			}
			release = inf->str.realese;
			++release;

			inf->str.next_inf = Create_node_inf(key, info);
			inf->str.next_inf->str.realese = release;
			return 1;
		}
	}
	return 0;
}

int D_Insert_node(Tree * tree)
{
	char* key = NULL;
	char* str = NULL;
	int res = -10;

	printf("Input key -> ");
	key = getStr();
	printf("\n");
	printf("Input info -> ");
	str = getStr();
	printf("\n");

	res = Insert_node(&(tree->root), key, str);

	if (res == -1) { printf("Bad size of key\n"); }
	if (res == 0) { printf("Input fail \n"); }
	if (res == 1) { printf("Success\n"); }

	return 1;
}

Tree* Create_tree(Tree * tree)
{
	tree = (Tree*)malloc(sizeof(Tree));
	tree->root = NULL;
}

Node* Create_node(Node * parent, char* key, char* str)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->str.next_inf = NULL;
	node->left = NULL;
	node->right = NULL;
	node->prev = NULL;
	node->next = NULL;

	node->parent = parent;
	node->key = key;
	node->str.str = str;
	node->str.next_inf = NULL;
	node->str.realese = 0;

	return node;
}

Node* Create_node_inf(char* key, char* str)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->str.next_inf = NULL;
	node->left = NULL;
	node->right = NULL;
	node->prev = NULL;
	node->next = NULL;

	node->parent = NULL;
	node->key = key;
	node->str.str = str;
	node->str.next_inf = NULL;
	node->str.realese = 0;
}
