#include "Structures.h"
#include "Create_table.h" 
//ks1
void printTable(Table* table);
Table* Release_Search(Table* table, KeyType1 key_found, int ver)
{
	int indexks2 = -1;
	int indexks2_ks2 = 0;
	int size_of_rel = 0;
	Item* item_temp = NULL;
	Item* item_temp_ks2 = NULL;
	if (table->msize1 == 0)
	{
		//printf("Table is empty\n");
		return NULL;
	}

	char* str_temp_key = (char*)malloc(sizeof(char) * strlen(key_found.str) + 1);
	str_temp_key[strlen(key_found.str)] = '\0';

	Table * new_table = (Table*)malloc(sizeof(Table));
	size_t index = 0;
	new_table->msize1 = table->msize1;
	new_table->msize2 = table->msize2;
	new_table->ks1 = (KeySpace1*)malloc(sizeof(KeySpace1) * table->msize1);
	new_table->ks2 = (KeySpace2*)malloc(sizeof(KeySpace2) * table->msize2);
	for (size_t i = 0; i < new_table->msize1; i++)
	{
		new_table->ks1[i].info = (Item*)malloc(sizeof(Item));
		new_table->ks1[i].info->info = NULL;
		new_table->ks1[i].key.str = '\0';
		new_table->ks1[i].release = 0;
	}
	for (size_t i = 0; i < new_table->msize2; i++)
	{
		new_table->ks2[i].node = NULL;
		new_table->ks2[i].busy = 0;
		new_table->ks2[i].key.count = NULL;
	}
	new_table->csize1 = 0;
	for (size_t i = 0; i < table->msize1; i++)
	{
		if ((table->ks1[i].key.str != NULL) && (strcmp(table->ks1[i].key.str, key_found.str) == 0) &&(table->ks1[i].release == ver))//(*(table->ks1[i].key.str) == *(key_found.str)))
		{
			new_table->ks1[index].info->info = (Item*)malloc(sizeof(Item));
			new_table->ks1[index].key.str = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].key.str) + 1));
			strcpy(new_table->ks1[index].key.str, table->ks1[i].key.str);
			new_table->ks1[index].release = table->ks1[i].release;
			new_table->ks1[index].info->next = NULL;
			new_table->ks1[index].info->release = table->ks1[i].info->release;
			new_table->ks1[index].info->info->number_1 = table->ks1[i].info->info->number_1;
			new_table->ks1[index].info->info->number_2 = table->ks1[i].info->info->number_2;
			new_table->ks1[index].info->info->str = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].info->info->str) + 1));
			strcpy(new_table->ks1[index].info->info->str, table->ks1[i].info->info->str);
			for (size_t j = 0; j < table->msize2; j++)
			{
				if (table->ks2[j].node != NULL)
				{
					if (table->ks1[i].info == table->ks2[j].node)
					{
						indexks2 = j;
						break;
					}
				}
			}
			new_table->ks2[indexks2].key.count = new_table->ks2[indexks2].key.count;
			new_table->ks2[indexks2].busy = 1;
			new_table->ks2[indexks2].key.count = table->ks2[indexks2].key.count;
			new_table->ks2[indexks2].node = new_table->ks1[index].info;
			item_temp = table->ks2[indexks2].node->next;
			item_temp_ks2 = new_table->ks2[indexks2].node;
			while (item_temp != NULL)
			{
				item_temp_ks2->next = (Item*)malloc(sizeof(Item));
				item_temp_ks2->next->info = (InfoType*)malloc(sizeof(InfoType));
				item_temp_ks2->next->next = NULL;
				item_temp_ks2->next->release = item_temp->release;
				item_temp_ks2->next->info->number_1 = item_temp->info->number_1;
				item_temp_ks2->next->info->number_2 = item_temp->info->number_2;
				item_temp_ks2->next->info->str = (char*)malloc(sizeof(char) * (strlen(item_temp->info->str) + 1));
				strcpy(item_temp_ks2->next->info->str, item_temp->info->str);
				item_temp = item_temp->next;
				item_temp_ks2->next->next = item_temp;
			}
			++new_table->csize2;
			++new_table->csize1;
			++index;
		}
	}
	return new_table;
}
//&& (strcmp(table->ks1[i].key.str, key_found.str) == 0))//(*(table->ks1[i].key.str) == *(key_found.str)))
Table* Key_Search(Table* table, KeyType1 key_found)
{
	int indexks2 = -1;
	int indexks2_ks2 = 0;
	int size_of_rel = 0;
	Item* item_temp = NULL;
	Item* item_temp_ks2 = NULL;
	if (table->msize1 == 0)
	{
		//printf("Table is empty\n");
		return NULL;
	}

	char* str_temp_key = (char*)malloc(sizeof(char) * strlen(key_found.str) + 1);
	str_temp_key[strlen(key_found.str)] = '\0';

	Table * new_table = (Table*)malloc(sizeof(Table));
	size_t index = 0;
	new_table->msize1 = table->msize1;
	new_table->msize2 = table->msize2;
	new_table->ks1 = (KeySpace1*)malloc(sizeof(KeySpace1) * table->msize1);
	new_table->ks2 = (KeySpace2*)malloc(sizeof(KeySpace2) * table->msize2);
	for (size_t i = 0; i < new_table->msize1; i++)
	{
		new_table->ks1[i].info = (Item*)malloc(sizeof(Item));
		new_table->ks1[i].info->info = NULL;
		new_table->ks1[i].key.str = '\0';
		new_table->ks1[i].release = 0;
	}
	for (size_t i = 0; i < new_table->msize2; i++)
	{
		new_table->ks2[i].node = NULL;
		new_table->ks2[i].busy = 0;
		new_table->ks2[i].key.count = NULL;
	}
	new_table->csize1 = 0;
	for (size_t i = 0; i < table->msize1; i++)
	{
		if ((table->ks1[i].key.str != NULL) && (strcmp(table->ks1[i].key.str, key_found.str) == 0))//(*(table->ks1[i].key.str) == *(key_found.str)))
		{
			new_table->ks1[index].info->info = (Item*)malloc(sizeof(Item));
			new_table->ks1[index].key.str = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].key.str) + 1));
			strcpy(new_table->ks1[index].key.str, table->ks1[i].key.str);
			new_table->ks1[index].release = table->ks1[i].release;
			new_table->ks1[index].info->next = NULL;
			new_table->ks1[index].info->release = table->ks1[i].info->release;
			new_table->ks1[index].info->info->number_1 = table->ks1[i].info->info->number_1;
			new_table->ks1[index].info->info->number_2 = table->ks1[i].info->info->number_2;
			new_table->ks1[index].info->info->str = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].info->info->str) + 1));
			strcpy(new_table->ks1[index].info->info->str, table->ks1[i].info->info->str);
			for (size_t j = 0; j < table->msize2; j++)
			{
				if (table->ks2[j].node != NULL)
				{
					if (table->ks1[i].info == table->ks2[j].node)
					{
						indexks2 = j;
						break;
					}
				}
			}
			new_table->ks2[indexks2].key.count = new_table->ks2[indexks2].key.count;
			new_table->ks2[indexks2].busy = 1;
			new_table->ks2[indexks2].key.count = table->ks2[indexks2].key.count;
			new_table->ks2[indexks2].node = new_table->ks1[index].info;
			item_temp = table->ks2[indexks2].node->next;
			item_temp_ks2 = new_table->ks2[indexks2].node;
			while (item_temp != NULL)
			{
				item_temp_ks2->next = (Item*)malloc(sizeof(Item));
				item_temp_ks2->next->info = (InfoType*)malloc(sizeof(InfoType));
				item_temp_ks2->next->next = NULL;
				item_temp_ks2->next->release = item_temp->release;
				item_temp_ks2->next->info->number_1 = item_temp->info->number_1;
				item_temp_ks2->next->info->number_2 = item_temp->info->number_2;
				item_temp_ks2->next->info->str = (char*)malloc(sizeof(char) * (strlen(item_temp->info->str) + 1));
				strcpy(item_temp_ks2->next->info->str, item_temp->info->str);
				item_temp = item_temp->next;
				item_temp_ks2->next->next = item_temp;
			}
			++new_table->csize2;
			++new_table->csize1;
			++index;
		}
	}
	return new_table;
}
// поиск по ключу (помнить про то что каждый ключ в ks2 - уникальный)
//ks2
Table* Key_search_ks2(Table* table, KeyType2 key2)
{
	Item* item_temp_old = NULL;
	Item* item_temp_new = NULL;
	Item* item_temp_save = NULL;
	//char* key2_n = NULL;
	char* key11 = NULL;
	int indexks2 = -1;
	int indexks1 = -1;
	if (table->msize2 == 0) { return NULL; }
	Table* new_table = (Table*)malloc(sizeof(Table));
	size_t index = 0;

	new_table = CreateKeySpace1(new_table, 1);
	new_table = CreateKeySpace2(new_table, table->msize2);

	for (size_t i = 0; i < table->msize2; i++)
	{
		if (table->ks2[h(key2.count, i, table->msize2)].key.count == key2.count)
		{
			indexks2 = h(key2.count, i, table->msize2); //index ks2 input
			break;
		}
	}

	if (indexks2 == -1)
	{
		free(new_table->ks2);
		free(new_table);
		return NULL;
	}

	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks2[indexks2].node == table->ks1[i].info)
		{
			item_temp_save = table->ks1[i].info;
		}
	}
	while (item_temp_save!= NULL) 
	{
		for (size_t i = 0; i < table->msize1; i++)
		{
			if (table->ks2[indexks2].node == table->ks1[i].info)
			{
				char* key2_n = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].key.str) + 1));
				key11 = key2_n;
				strcpy(key2_n, table->ks1[i].key.str);
			}
		}
		char* str_info = (char*)malloc(sizeof(char) * ((strlen(item_temp_save->info->str)) + 1));
		strcpy(str_info, item_temp_save->info->str);
		addItem(new_table , key11, str_info, item_temp_save->info->number_1, item_temp_save->info->number_2, key2.count);
		item_temp_save = item_temp_save->next;
	}

	return new_table;
}

Table* Key_search_ks2_ks1(Table* table, KeyType2 key2, KeyType1 key1)
{
	Item* item_temp_old = NULL;
	Item* item_temp_new = NULL;
	Item* item_temp_save = NULL;
	//char* key2_n = NULL;
	char* key11 = NULL;
	int indexks2 = -1;
	int indexks1 = -1;
	if (table->msize2 == 0) { return NULL; }
	Table* new_table = (Table*)malloc(sizeof(Table));
	size_t index = 0;

	new_table = CreateKeySpace1(new_table, 1);
	new_table = CreateKeySpace2(new_table, table->msize2);

	for (size_t i = 0; i < table->msize2; i++)
	{
		if (table->ks2[h(key2.count, i, table->msize2)].key.count == key2.count)
		{
			indexks2 = h(key2.count, i, table->msize2); //index ks2 input
			break;
		}
	}
	for (size_t i = 0; i < table->msize1; i++)
	{
		if ((table->ks1[i].key.str != NULL) && (strcmp(table->ks1[i].key.str, key1.str) == 0))
		{
			indexks1 = i;
		}
	}

	if (indexks2 == -1 || indexks1 == -1)
	{
		free(new_table->ks2);
		free(new_table->ks1);
		free(new_table);
		return NULL;
	}

	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks2[indexks2].node == table->ks1[i].info)
		{
			item_temp_save = table->ks1[i].info;
		}
	}
	while (item_temp_save != NULL)
	{
		for (size_t i = 0; i < table->msize1; i++)
		{
			if (table->ks2[indexks2].node == table->ks1[i].info)
			{
				char* key2_n = (char*)malloc(sizeof(char) * (strlen(table->ks1[i].key.str) + 1));
				key11 = key2_n;
				strcpy(key2_n, table->ks1[i].key.str);
			}
		}
		char* str_info = (char*)malloc(sizeof(char) * ((strlen(item_temp_save->info->str)) + 1));
		strcpy(str_info, item_temp_save->info->str);
		addItem(new_table, key11, str_info, item_temp_save->info->number_1, item_temp_save->info->number_2, key2.count);
		item_temp_save = item_temp_save->next;
	}

	return new_table;
}


Item* SearchElem(Table* table, KeyType1 key1, KeyType2 key2, int ver)
{
	Item* item_temp_old = NULL;
	Item* item_temp_new = NULL;
	Item* item_temp_save = NULL;
	char* key11 = NULL;
	Item* node_search = NULL;
	int indexks2 = -1;
	int indexks1 = -1;
	if (table->msize2 == 0) { return NULL; }
	Table* new_table = (Table*)malloc(sizeof(Table));
	size_t index = 0;

	new_table = CreateKeySpace1(new_table, 1);
	new_table = CreateKeySpace2(new_table, table->msize2);
	///////////////////////////
	for (size_t i = 0; i < table->msize2; i++)
	{
		if (table->ks2[h(key2.count, i, table->msize2)].key.count == key2.count)
		{
			indexks2 = h(key2.count, i, table->msize2); //index ks2 input
			break;
		}
	}
	for (size_t i = 0; i < table->msize1; i++)
	{
		if ((table->ks1[i].key.str != NULL) && (strcmp(table->ks1[i].key.str, key1.str) == 0))
		{
			indexks1 = i;
		}
	}

	if (indexks2 == -1 || indexks1 == -1)
	{
		free(new_table->ks2);
		free(new_table->ks1);
		free(new_table);
		return NULL;
	}/////////////////////////

	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks2[indexks2].node == table->ks1[i].info)
		{
			item_temp_save = table->ks1[i].info;// откуда начинаем искать
		}
	}
	while (item_temp_save != NULL)
	{
		if (item_temp_save->release == ver)
		{
			node_search = (Item*)malloc(sizeof(Item));
			char* str_info = (char*)malloc(sizeof(char) * ((strlen(item_temp_save->info->str)) + 1));
			strcpy(str_info, item_temp_save->info->str);
			node_search->info = (InfoType*)malloc(sizeof(InfoType));
			node_search->info->str = str_info;
			node_search->release = item_temp_save->release;
			node_search->next = NULL;
			node_search->info->number_1 = item_temp_save->info->number_1;
			node_search->info->number_2 = item_temp_save->info->number_2;
		}

		item_temp_save = item_temp_save->next;
	}


	return node_search;


}
/////////////////////////////////////////////////////////////////////////////////////
void printTable(Table* table)
{
	Item* temp = NULL;
	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].info != NULL && table->ks1[i].info->info != NULL) //&& (table->ks2[i].busy != 0))
		{
			printf("Key1 -> %s \t", table->ks1[i].key.str);
			for (size_t j = 0; j < table->msize2; j++)
			{
				if (table->ks2[j].node != NULL)
				{
					if (table->ks1[i].info == table->ks2[j].node)
					{
						printf("Key2 -> %d \t", table->ks2[j].key.count);
					}
				}
			}
			printf("Realese KS1 -> %d \n", table->ks1[i].release);
			temp = table->ks1[i].info;
			while (temp != NULL)
			{
				printf("Integer 1 -> %d \t", temp->info->number_1);
				printf("Integer 2 -> %d \t", temp->info->number_2);
				printf("String  -> %s \t", temp->info->str);//table->ks1[i].info->info->str);
				printf("Realese -> %d \t", temp->release);
				temp = temp->next;
				printf("\n");
			}
		}
	}
	printf("\n");
}

