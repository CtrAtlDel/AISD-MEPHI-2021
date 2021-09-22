#define  _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "malloc.h"
#include "stdlib.h"
#define size_key  3

typedef struct InfoType
{
	int number_1;
	int number_2;
	char** str;
} InfoType;

typedef struct Item
{
	InfoType* info;
	int release;
	/* версия элемента*/
	struct Item* next;
	/* указатель на следующий элемент с данным составным ключем	*/
} Item;

typedef struct KeyType1
{
	char* str;
} KeyType1;

typedef struct KeySpace1
{
	KeyType1 key;		/* ключ элемента*/
	int release;		/* номер версии элемента(если ввелся один ключ)*/
	struct Item* info;			/* указатель на информацию*/
} KeySpace1;

typedef struct Table
{
	KeySpace1* ks1;
	int msize1;
	int csize1;
} Table;

char* getStr();//получение строки
int getInt(int*);
int getPositiveInt(int*);
int D_addItem(Table* table);
int addItem(Table* table, int count1, int count2, char* info, char* key);
int delTable(Table* table);
void printTable(Table* table);
int D_findItem(Table* table);
int D_delete(Table* table);
int Compression(Table* table);
Table* Key_Search(Table* table, KeyType1 key);
Table* CreateTable(Table* table);
Table* CreateKeySpace1(Table* table, int size);
Table* Release_Search(Table* table, KeyType1 key_found, int ver);

int(*fptr[])(Table*) = { NULL, D_addItem, printTable, D_findItem, D_delete, Compression};
const char* msgs[] = { " 0. Quit", " 1. Add Item", " 2. Show Table", " 3. Find Item", "4. Delete item", "5. Compression"};
const char* err_msg[] = { "Ok", "Table overflow", "This element doesn't exist" };
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
int dialog(const char* msgs[], int N);

int dialog(const char* msgs[], int N) 
{
	char* errmsg = "";
	int rc;
	int i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeat, please!";
		printf("\t========== MENU ==========\n");
		//printf("Скорректировать удаление элемент таблицы, и копирование строки");
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
void printTable(Table* table)
{
	if (table->csize1 == 0)
	{
		printf("Empty table\n");
	}
	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].info->info != NULL)
		{
			printf("Key1 -> %s \t", table->ks1[i].key.str);
			printf("Integer 1 -> %d \t", table->ks1[i].info->info->number_1);
			printf("Integer 2 -> %d \t", table->ks1[i].info->info->number_2);
			printf("String  -> %s \t", table->ks1[i].info->info->str);
			printf("Realese ks1 -> %d \n", table->ks1[i].release);
		}
	}
	printf("\n");
}
Table* CreateTable(Table* table)
{
	int size1, size2;
	printf("Input size of the first Keyspace1 \n");
	if (getPositiveInt(&size1) == 0) { return 0; };
	table = CreateKeySpace1(table, size1);
	return table;
}
Table* CreateKeySpace1(Table* table, int size)
{
	table->msize1 = size;
	table->ks1 = (KeySpace1*)malloc(sizeof(KeySpace1) * table->msize1);     // создание столбца размером msize1
	for (size_t i = 0; i < table->msize1; i++)
	{
		table->ks1[i].info = (Item*)malloc(sizeof(Item));
		table->ks1[i].info->info = NULL;
		table->ks1[i].info->next = NULL;
		table->ks1[i].info->release = 0;
		table->ks1[i].key.str = '\0';
		table->ks1[i].release = 0;
	}
	table->csize1 = 0;
	return table;
}
int delTable(Table* table)
{
	if (table == NULL)
	{
		return 1;
	}
	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].info->info != NULL)
		{
			free(table->ks1[i].info->info->str);
			free(table->ks1[i].key.str);
			free(table->ks1[i].info->info);
			free(table->ks1[i].info);
		}
	}

	free(table);
}
int D_addItem(Table* table) 
{
	int res = -5;
	int count1 = 0;
	int count2 = 0;
	int flag = 0;
	int index = 0;
	char* str_temp_key;
	char* str_temp_info;
	printf("Iput key 1 -> ");
	str_temp_key = getStr();
	printf("\n");

	if (strlen(str_temp_key) > (size_key))
	{
		printf("Bad size of key \n");
		free(str_temp_key);
		return 1;
	}
	printf("Input first iteger -> ");
	if (getPositiveInt(&count1) == 0) { return 0; }
	printf("\n");
	printf("Input second iteger -> ");
	if (getPositiveInt(&count2) == 0) { return 0; }
	printf("\n");
	printf("Input str -> ");
	str_temp_info = getStr();
	printf("\n");
	res = addItem(table, count1, count2, str_temp_info, str_temp_key);
	if (res == -1)
	{
		printf("\t %s \n", err_msg[1]);
	}
	else 
	{
		printf("Success\n");
	}
}
int addItem(Table* table, int count1, int count2, char* info, char* key)
{
	int flag = 0;
	int index = 0;
	if (table->csize1 == table->msize1)
	{
		return -1;
	}
	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].info->info == NULL)
		{
			index = i;
			break;
		}
	}
	table->ks1[index].key.str = key;
	table->ks1[index].info->info = (Item*)malloc(sizeof(Item));
	table->ks1[index].info->next = NULL;
	table->ks1[index].info->info->number_1 = count1;
	table->ks1[index].info->info->number_2 = count2;
	table->ks1[index].info->info->str = info;
	table->ks1[index].info->release = 0;
	table->ks1[index].release = 0;
	for (size_t i = 0; i < table->msize1; i++)
	{
		if ((table->ks1[i].key.str != NULL) && ((strcmp(table->ks1[i].key.str, table->ks1[index].key.str) == 0)) && (i != index) && strlen(table->ks1[i].key.str) == strlen(table->ks1[index].key.str))
		{
			table->ks1[index].release = table->ks1[i].release + 1;
		}
	}
	++table->csize1;
	return 1;
}
int D_findItem(Table* table)
{
	int ch = -1;
	int realese = -1;
	KeyType1 key;
	Table* new_table = NULL;
	printf("Search of release or key input (1/0) \n");
	if (getInt(&ch) == 0) { return 0; }
	if (ch == 1)
	{
		printf("Input key -> ");
		key.str = getStr();
		printf("\n");
		printf("Input release -> ");
		if (getInt(&realese) == 0) { return 0; }
		printf("\n");
		new_table = Release_Search(table, key, realese);
		if (new_table == NULL || new_table->csize1 == 0)
		{
			if (new_table == NULL)
			{
				printf("Empty table\n");
				delTable(new_table);
				return 1;
			}
			else
			{
				printf("I didnt found this key \n");
				delTable(new_table);
				return 1;
			}
		}
		else { printTable(new_table); }
	}
	if (ch == 0)
	{
		printf("Input key -> ");
		key.str = getStr();
		printf("\n");
		new_table = Key_Search(table, key);
		if (new_table == NULL || new_table->csize1 == 0)
		{
			if (new_table == NULL)
			{
				printf("Empty table\n");
				delTable(new_table);
				return 1;
			}
			else 
			{
				printf("I didnt found this key \n");
				delTable(new_table);
				return 1;
			}
		}
		else { printTable(new_table); }
	}
	if (ch > 1 && ch < 0)
	{
		printf("Bad input, input (1/0), please \n");
		delTable(new_table);
		return 1;
	}
	delTable(new_table);
	return 1;
}
Table* Release_Search(Table* table, KeyType1 key_found, int ver)
{
	if (table->msize1 == 0)
	{
		return NULL;
	}

	char* str_temp_key = (char*)malloc(sizeof(char) * strlen(key_found.str) + 1);
	str_temp_key[strlen(key_found.str)] = '\0';

	Table * new_table = (Table*)malloc(sizeof(Table));
	size_t index = 0;
	new_table->msize1 = table->csize1;

	new_table->ks1 = (KeySpace1*)malloc(sizeof(KeySpace1) * table->msize1);// создание столбца размером msize1
	for (size_t i = 0; i < new_table->msize1; i++)
	{
		new_table->ks1[i].info = (Item*)malloc(sizeof(Item));
		new_table->ks1[i].info->info = NULL;
		new_table->ks1[i].key.str = '\0';
		new_table->ks1[i].release = 0;
	}
	new_table->csize1 = 0;

	for (size_t i = 0; i < table->msize1; i++)
	{
		//printf("Strcmp %d \n ", strcmp(table->ks1[i].key.str, key_found.str));
		if ((table->ks1[i].key.str != NULL) && table->ks1[i].release == ver && (strcmp(table->ks1[i].key.str, key_found.str) == 0))//(*(table->ks1[i].key.str) == *(key_found.str)))
		{
			new_table->ks1[index].info->info = (Item*)malloc(sizeof(Item));
			new_table->ks1[index].key.str = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].key.str) + 1));//= table->ks1[i].key.str;
			strcpy(new_table->ks1[index].key.str, table->ks1[i].key.str);
			new_table->ks1[index].release = table->ks1[i].release;
			new_table->ks1[index].info->next = NULL;
			new_table->ks1[index].info->release = table->ks1[i].info->release;
			new_table->ks1[index].info->info->number_1 = table->ks1[i].info->info->number_1;
			new_table->ks1[index].info->info->number_2 = table->ks1[i].info->info->number_2;
			new_table->ks1[index].info->info->str = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].info->info->str) + 1));
			strcpy(new_table->ks1[index].info->info->str, table->ks1[i].info->info->str);//table->ks1[i].info->info->str;//
			++new_table->csize1;
			++index;
		}
	}
	return new_table;
}
Table * Key_Search(Table * table, KeyType1 key)
{
	if (table->msize1 == 0)
	{
		return NULL;
	}
	Table* new_table = (Table*)malloc(sizeof(Table));
	size_t index = 0;
	new_table->msize1 = table->csize1;
	new_table->ks1 = (KeySpace1*)malloc(sizeof(KeySpace1) * table->msize1);// создание столбца размером msize1
	for (size_t i = 0; i < new_table->msize1; i++)
	{
		new_table->ks1[i].info = (Item*)malloc(sizeof(Item));
		new_table->ks1[i].info->info = NULL;
		new_table->ks1[i].key.str = '\0';
		new_table->ks1[i].release = 0;
	}
	new_table->csize1 = 0;

	for (size_t i = 0; i < table->msize1; i++)
	{
		if ((table->ks1[i].key.str != NULL) && (strcmp(table->ks1[i].key.str, key.str) == 0)) //*(table->ks1[i].key.str) == *(key.str))
		{
			new_table->ks1[index].info->info = (Item*)malloc(sizeof(Item));
			new_table->ks1[index].key.str = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].key.str) + 1));
			strcpy(new_table->ks1[index].key.str, table->ks1[i].key.str); //table->ks1[i].key.str;//исправить 
			new_table->ks1[index].release = table->ks1[i].release;
			new_table->ks1[index].info->next = NULL;
			new_table->ks1[index].info->release = table->ks1[i].info->release;
			new_table->ks1[index].info->info->number_1 = table->ks1[i].info->info->number_1;
			new_table->ks1[index].info->info->number_2 = table->ks1[i].info->info->number_2;
			new_table->ks1[index].info->info->str = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].info->info->str) + 1));
			strcpy(new_table->ks1[index].info->info->str, table->ks1[i].info->info->str);//table->ks1[i].info->info->str;//исправить 
			++new_table->csize1;
			++index;
		}
	}
	return new_table;
}
int D_delete(Table* table) 
{
	int ch = -1;
	int realese = -1;
	int res = -5;
	KeyType1 key;
	printf("Search of release or key input (1/0) \n");
	if (getInt(&ch) == 0) { return 0; }

	if (ch == 0)
	{
		printf("Input key -> ");
		key.str = getStr();
		printf("\n");
		res = Delete_item_key(table, key, realese);
		if (res == 0)
		{
			printf("I didnt find this item\n");
		}
		return 1;
	}
	else
	{
		if (ch == 1)
		{
			printf("Input key -> ");
			key.str = getStr();
			printf("\n");
			printf("Input release -> ");
			if (getInt(&realese) == 0) { return 0; }
			printf("\n");
			res = Delete_item_rel(table, key, realese);
			if (res == 0)
			{
				printf("I didnt find this item\n");
			}
			return 1;
		}
		else 
		{
			printf("Bad input, input (1/0), please \n");
			return 1;
		}
	}
}
int Delete_item_key(Table* table, KeyType1 key, int rel)
{
	int index = -1;
	int last_index = -1;
	int flag = 0;

	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].key.str != NULL && (strcmp(table->ks1[i].key.str, key.str) == 0))
		{
			flag = 1;
			index = i;
			if (index == table->csize1 - 1)
			{
				free(table->ks1[index].info->info);//delete infostr
				table->ks1[index].info->info = NULL;
				table->ks1[index].key.str = '\0';
				table->ks1[index].release = 0;
				table->csize1--;
				i = -1;
			}
			else
			{
				//index of last elem table->csize - 1
				for (size_t i = 0; i < table->csize1; i++)
				{
					if (table->ks1[i].key.str != NULL)
					{
						last_index = i;
					}
				}
				free(table->ks1[index].info->info);//delete infostr
				table->ks1[index].info->info = NULL;
				table->ks1[index].key.str = '\0';
				table->ks1[index].release = 0;

				table->ks1[index].info->info = table->ks1[last_index].info->info;
				table->ks1[index].key.str = table->ks1[last_index].key.str;
				table->ks1[index].release = table->ks1[last_index].release;

				table->ks1[last_index].info->info = (Item*)malloc(sizeof(Item));//delete infostr
				table->ks1[last_index].info->info = NULL;
				table->ks1[last_index].key.str = '\0';
				table->ks1[last_index].release = 0;

				table->csize1--;
				i = -1;
			}
		}
	}
	return flag;
}
int Delete_item_rel(Table* table, KeyType1 key, int rel)
{
	int index = -1;
	int last_index = -1;
	int flag = 0;

	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].key.str != NULL && (strcmp(table->ks1[i].key.str, key.str) == 0) && (table->ks1[i].release == rel))
		{
			flag = 1;
			index = i;
			if (index == table->csize1 - 1)
			{
				free(table->ks1[index].info->info);//delete infostr
				table->ks1[index].info->info = NULL;
				table->ks1[index].key.str = '\0';
				table->ks1[index].release = 0;
				table->csize1--;
				i = -1;
			}
			else
			{
				//index of last elem table->csize - 1
				for (size_t i = 0; i < table->csize1; i++)
				{
					if (table->ks1[i].key.str != NULL)
					{
						last_index = i;
					}
				}
				free(table->ks1[index].info->info);//delete infostr
				table->ks1[index].info->info = NULL;
				table->ks1[index].key.str = '\0';
				table->ks1[index].release = 0;

				table->ks1[index].info->info = table->ks1[last_index].info->info;
				table->ks1[index].key.str = table->ks1[last_index].key.str;
				table->ks1[index].release = table->ks1[last_index].release;

				table->ks1[last_index].info->info = (Item*)malloc(sizeof(Item));//delete infostr
				table->ks1[last_index].info->info = NULL;
				table->ks1[last_index].key.str = '\0';
				table->ks1[last_index].release = 0;

				table->csize1--;
				i = -1;
			}
		}
	}
	return flag;
}
int Compression(Table* table) 
{
	int max_realese = -1;
	KeyType1 key_temp;
	key_temp.str = NULL;
	for (size_t i = 0; i < table->csize1; i++)
	{
		if (table->ks1[i].info->info != NULL)
		{
			key_temp.str = table->ks1[i].key.str;
			max_realese = table->ks1[i].release;
			for (size_t j = 0; j < table->csize1; j++)
			{
				if (table->ks1[j].key.str != '\0')
				{
					if (table->ks1[j].info->info != NULL && (table->ks1[j].release > max_realese) && (strcmp(table->ks1[j].key.str, key_temp.str) == 0)) //&& *(table->ks1[j].key.str) == *(keyks1.str)
					{
						max_realese = table->ks1[j].release;
					}
				}
			}
			for (size_t j = 0; j < table->csize1; j++)
			{
				if (table->ks1[j].key.str == key_temp.str && table->ks1[j].release < max_realese)
				{
					Delete_item_rel(table, key_temp, table->ks1[j].release);
				}
			}
		}
	}
}

int main() 
{
	Table* table = (Table*)malloc(sizeof(Table));
	table = CreateTable(table);
	/*FILE* file_temp;
	char fname[] = "Info.txt";
	file_temp = fopen(fname, "r");
	if (file_temp)
	{
		for (size_t i = 0; i < 7; i++)
		{
			char* key = (char*)malloc(10 * sizeof(char));
			char* info = (char*)malloc(100 * sizeof(char));
			int count1;
			int count2;
			fscanf(file_temp, "%s\n %d\n %d\n %s\n", key, &count1, &count2, info);
			addItem(table, &count1, &count2,  info, key);
		}
	}
	else
	{
		fprintf(stderr, "can't open this file %s\n", fname);
		return 1;
	}*/
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
	} while (n == 0 || (*a <= 0));
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