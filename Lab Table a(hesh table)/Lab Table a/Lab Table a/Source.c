#define  _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "malloc.h"
#include "stdlib.h"
#include "StructuresFunctions.h"
#include "Print_func.h"
//#include "Create_table.h"
#include "Compression.h"
#include "Hesh.h"
#define size_key  3

typedef struct InfoType;
typedef struct KeySpace1;
typedef struct Item;
typedef struct Table;
typedef struct KeyType1;

char* getStr();//получение строки
int getInt(int*);
int getPositiveInt(int*);
int D_addItem(Table* table);
int D_findItem(Table* table);
int addItem(Table* table, char* str_temp1, char* str_temp2, int temp1, int temp2, int key2);
int D_delElem(Table* table);
int addItem(Table* table, char* str_temp1, char* str_temp2, int temp1, int temp2, int key2);
int delElem(Table* table, char* key1, int key2, int rel, int ch);

Table* CreateKeySpace1(Table* table, int size);
Table* CreateKeySpace2(Table* table, int size);
Table* CreateTable(Table* table);

Table* Release_Search(Table* table, KeyType1 key_found,int ver);
Table* Key_Search(Table* table, KeyType1 key);

int Compression(Table* table);

int(*fptr[])(Table*) = { NULL, D_addItem, printTable, D_findItem, Compression, D_delElem};
const char* msgs[] = { " 0. Quit", " 1. Add Item", " 2. Show Table", " 3. Find Item", " 4. Compress table", " 5. Delete elem" };
const char* err_msg[] = { "Ok", "Table overflow", "This element doesn't exist" };
const char* msgs_delete[] = { " 0. Delete elem with 2 keys", " 1. Delete elem with key 1"," 2. Delete elem with key 2" };

int dialog(const char* msgs[], int N);
const int  Nmsgs_delete = sizeof(msgs_delete) / sizeof(msgs_delete[0]);
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int dialog(const char* msgs[], int N) {
	char* errmsg = "";
	int rc;
	int i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeat, please!";
		printf("\t========== MENU ==========\n");
		
		//вывод списка альтернатив
		for (i = 0; i < N; ++i)
			printf("\t%s\n", msgs[i]);
		printf("\t==========================\n");
		puts("Make your choice: -> ");
		n = getInt(&rc); // ввод номера альтернативы 
		if (n == 0) // конец работы
			rc = 0;
	} while (rc < 0 || rc >= N);

	return rc;
}

int main()
{
	Table* table = (Table*)malloc(sizeof(Table));
	table = CreateTable(table);
	int rc = 0, f = 1;
	while ((rc = dialog(msgs, NMsgs)) && f) 
	{
		f = fptr[rc](table);
	}

	delTable(table);
	
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
	} while (n == 0 || (*a < 0));
	scanf("%*c", 0);
	return n < 0 ? 0 : 1;
}

char* getStr()
{
	char buf[81];
	char* str = (char*)malloc(1);
	str[0] = '\0';
	int n;
	int m = 1;
	do
	{
		n = scanf("%80[^\n]", buf);
		if (n > 0)
		{
			m += strlen(buf);
			str = (char*)realloc(str, m);
			strcat(str, buf);
		}
		else
		{
			if (n == -1)
			{
				printf("Bad string");
				free(str);
				return NULL;
			}
		}
	} while (n > 0);
	scanf("%*c", 0);
	return str;
}

int D_addItem(Table* table)
{
	int info_int1 = 0, key2 = 0, info_int2 = 0;
	int add = 0;
	char* key1 = NULL;
	char* info_str = NULL;
	printf("Iput key 1 -> ");
	key1 = getStr();
	printf("\n");
	printf("Iput key 2 -> ");
	if (getPositiveInt(&key2) == 0) { return 0; };
	printf("\n");
	printf("Input first iteger -> ");
	if (getPositiveInt(&info_int1) == 0) { return 0; };
	printf("\n");
	printf("Input second iteger -> ");
	if (getPositiveInt(&info_int2) == 0) { return 0; };
	printf("\n");
	printf("Input str -> ");
	info_str = getStr();
	printf("\n");
	add = addItem(table, key1, info_str, info_int1, info_int2, key2);
	if (add == -1)
	{
		printf("Bad size of key or other problem \n");
	}
}


int D_delElem(Table* table)
{
	int ch = -1, rel = -1, ver = -1, res = -1, key2 = -1, ch2 = -1;
	char* str_temp = NULL;
	printf("Input 2 keys or 1 (1/0) \n");
	if (getInt(&ch) == 0) { return 0; };
	if (ch == 1)// составной ключ
	{
		printf("Whith version (1/0) \n");
		if (getInt(&ver) == 0) { return 0; };
		if (ver == 1)
		{

			printf("Input key 1 -> ");
			str_temp = getStr();
			printf("\n");
			printf("Iput key 2 -> ");
			if (getPositiveInt(&key2) == 0) { return 0; };
			printf("\n");
			printf("Input realese  \n");
			if (getPositiveInt(&rel) == 0) { return 0; };
			printf("\n");
			res = delElem(table, str_temp, key2, rel, ch);
		}
		else
		{
			printf("Input key 1 -> ");
			str_temp = getStr();
			printf("\n");
			printf("Iput key 2 -> ");
			if (getPositiveInt(&key2) == 0) { return 0; };
			printf("\n");
			res = delElem(table, str_temp, key2, rel, ch);
		}
	}
	else
	{
		//ch = 0
		printf("Input KS1 or KS2 (1/0) \n");
		if (getInt(&ch2) == 0) { return 0; };
		if (ch2 == 1)//ks1
		{
			printf("Whith version (1/0) \n");
			if (getInt(&ver) == 0) { return 0; };
			if (ver == 1)
			{
				printf("Input key 1 -> ");
				str_temp = getStr();
				printf("Input realese  \n");
				if (getPositiveInt(&rel) == 0) { return 0; };
				printf("\n");
				res = delElem(table, str_temp, key2, rel, ch);
			}
			else
			{
				printf("Input key 1 -> ");
				if (getInt(&ver) == 0) { return 0; }
				str_temp = getStr();
				res = delElem(table, str_temp, key2, rel, ch);
			}
		}
		else//ks2
		{
			//printf("Whith version (1/0) \n");
			//if (getInt(&ver) == 0) { return 0; };
			if (ver !=1)
			{
				printf("Iput key 2 -> ");
				if (getPositiveInt(&key2) == 0) { return 0; };
				printf("\n");

				res = delElem(table, str_temp, key2, rel, ch);
			}
		}
	}
	if (res == -1)
	{
		printf("ERROR");
	}
}
int delElem(Table* table, char* key1, int key2, int rel, int ch)
{
	int index = -1;
	int hesh = -1;
	if (ch == 1 && key1 != NULL && key2 != NULL && rel != -1)
	{
		for (size_t i = 0; i < table->msize2; i++)
		{
			//hesh = h(key2, i, table->msize2);
			
			if ((table->ks2[i].key.count == key2))
			{
				printf("ks2 chek %d", table->ks2[i].key.count);
				index = i;
			}
		}
		delElem_KS2KS1(table, index, rel);
	}
	if (key2 == -1 && rel != -1)
	{
		for (size_t i = 0; i < table->msize1; i++ )
		{
			if ((table->ks1[i].key.str != NULL) && strcmp(table->ks1[i].key.str, key1) == 0 && (table->ks1[i].release == rel))
			{
				index = i;
				delElemKS1(table, index);
				--table->csize1;
				--table->csize2;
			}
		}
		printf("Ok");
	}
	if (key2 == -1 && rel == -1)
	{
		for (size_t i = 0; i < table->msize1; i++)
		{
			if ((table->ks1[i].key.str != NULL) && strcmp(table->ks1[i].key.str, key1) == 0 )
			{
				index = i;
				delElemKS1(table, index);
				--table->csize1;
				--table->csize2;
			}
		}
		printf("Ok");
	}
	if (key2 != -1 && rel == -1)
	{
		for (size_t i = 0; i < table->msize2; i++)
		{
			hesh = h(key2, i, table->msize2);
			index = hesh;
			if (table->ks2[index].key.count == key2)
			{
				delElemKS2(table, index);
				--table->csize1;
				--table->csize2;
			}
		}
	}
}

int delElemKS2(Table* table, int index) 
{
	int save_index = -1;
	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].info == table->ks2[index].node)
		{
			save_index = i;
		}
	}
	table->ks2[index].busy = 0;
	table->ks2[index].key.count = NULL;
	table->ks2[index].node = NULL;
	free(table->ks1[save_index].info->info->str);

	table->ks1[save_index].info->info->str = NULL;
	table->ks1[save_index].info->info = NULL;
	table->ks1[save_index].key.str = '\0';
	table->ks1[save_index].release = 0;
	return 1;
}

int delElem_KS2KS1(Table* table, int index, int ver) 
{
	Item* item = NULL;
	Item* item_temp = NULL;
	int find = -1;
	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].info == table->ks2[index].node)
		{
			find = i;
		}
	}
	if (find != -1)
	{
		item = table->ks2[index].node;
		if (item->next == NULL)
		{
			--table->csize1;
			--table->csize2;
		}
		table->ks2[index].node = table->ks2[index].node->next;
		table->ks1[find].info = table->ks2[index].node->next;
		free(item->info->str);
		free(item);
	}
	else 
	{
		item = table->ks2[index].node;
		while (item != NULL )
		{
			if (item->release == ver)
			{
				item_temp = item;
			}
			item = item->next;
		}
		item = table->ks2[index].node;
		while (item != NULL) 
		{
			if (item->next == item_temp)
			{
				item->next = item_temp->next;
				free(item_temp->info->str);
				free(item_temp);
			}
			item = item->next;
		}
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
Table* findItem(Table* table, int ch)
{
	KeyType1 key;
	KeyType2 key2;
	Item* item = NULL;
	int realese;
	Table* new_table = NULL;
	if (ch == 1)
	{
		printf("Input key1 -> ");
		key.str = getStr();
		printf("\n");
		printf("Input release -> ");
		if (getInt(&realese) == 0) { return 0; }
		printf("\n");
		new_table = Release_Search(table, key, realese); // 
		if (new_table == NULL || new_table->csize1 == 0)
		{
			delTable(new_table); //printf("I didnt found this realese \n");
			return NULL;
		}
		else
		{
			return new_table;
		}
	}
	if (ch == 0)
	{
		printf("Input key1 -> ");
		key.str = getStr();
		printf("\n");
		new_table = Key_Search(table, key); //key_search
		if (new_table == NULL || new_table->csize1 == 0)
		{
			delTable(new_table); //printf("I didnt found this key \n");
			return NULL;
		}
		else
		{
			printTable(new_table);
			return new_table;
		}
	}
	if (ch == 2)
	{
		printf("Input key2 -> ");
		if (getPositiveInt(&key2.count) == 0) { return 0; };
		printf("\n");
		new_table = Key_search_ks2(table, key2);
		if (new_table == NULL)
		{
			delTable(new_table); //printf("I didnt found this key \n");
			return NULL;
		}
		else
		{
			return new_table;
		}
	}
	if (ch == 3)
	{
		printf("Input key1 -> ");
		key.str = getStr();
		printf("\n");
		printf("Input key2 -> ");
		if (getPositiveInt(&key2.count) == 0) { return 0; };
		printf("\n");
		new_table = Key_search_ks2_ks1(table, key2, key);
		if (new_table == NULL)
		{
			delTable(new_table); //printf("I didnt found this key \n");
			return NULL;
		}
		else
		{
			printTable(new_table);
			return new_table;
		}
	}
	if (ch == 4)
	{
		printf("Input key1 -> ");
		key.str = getStr();
		printf("\n");
		printf("Input key2 -> ");
		if (getPositiveInt(&key2.count) == 0) { return 0; };
		printf("\n");
		printf("Input release -> ");
		if (getInt(&realese) == 0) { return 0; }
		printf("\n");
		item = SearchElem(table, key, key2, realese);
		if (item == NULL)
		{
			return NULL;
		}
		else 
		{
			printf("Number 1 -> %d\n", item->info->number_1);
			printf("Number 2 -> %d\n", item->info->number_2);
			printf("Str %s\n", item->info->str);
			printf("Realese %d \n", item->release);
		}

	}
	if (ch == -1)
	{
		delTable(new_table);
		return NULL;
	}
}

// ch 1 ks1 ключ с релизом 
// с2 0 ks1 ключ
// ch 2 ks2 
// ch 3 составной ключ
// ch 4 составной ключ с релизом

int D_findItem(Table* table)
{
	int ch = -1;
	int realese = -1;
	int find_ks = -1;
	int find = 0;
	KeyType1 key;
	Table* new_table = NULL;
	Item* item = NULL;
	printf("0. ks1 key \n");
	printf("1. ks1 key whith realese \n");
	printf("2. ks2 key \n");
	printf("3. key1 key2 \n");
	printf("4. key1 key2 whith realese \n");
	if (getInt(&ch) == 0) { return 0; }
	new_table = findItem(table, ch);
	if (new_table == 1)
	{
		//
	}
	if (new_table == NULL)
	{
		printf("I didnt found this key or realese, may be Bad input \n");
	}
	else 
	{
		//printTable(new_table);
		//delTable(new_table);	
	}
	return 1;
}

