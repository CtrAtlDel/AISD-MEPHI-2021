#define size_key  3

Table* CreateKeySpace1(Table* table, int size)
{
	table->msize1 = size;
	
	table->ks1 = (KeySpace1*)malloc(sizeof(KeySpace1) * table->msize1);     // создание столбца размером msize1
	for (size_t i = 0; i < table->msize1; i++)
	{
		table->ks1[i].info = (Item*)malloc(sizeof(Item));
		table->ks1[i].info->info = NULL;
		table->ks1[i].info->next = NULL;
		table->ks1[i].key.str = '\0';
		table->ks1[i].release = 0;
	}
	table->csize1 = 0;
	return table;
}

Table* CreateKeySpace2(Table* table, int size)
{
	table->msize2 = size;

	table->ks2 = (KeySpace2*)malloc(sizeof(KeySpace2) * table->msize2);
	for (size_t i = 0; i < table->msize2; i++)
	{
		table->ks2[i].busy = 0;
		table->ks2[i].key.count = NULL;
		table->ks2[i].node = NULL;
	}
	table->csize2 = 0;
	return table;
}

Table* CreateTable(Table* table)
{
	int size1, size2;
	printf("Input size of the first Keyspace1 \n");
	if (getPositiveInt(&size1) == 0) { return 0; };
	printf("Input size of the second Keyspace2 \n");
	if (getPositiveInt(&size2) == 0) { return 0; };

	table = CreateKeySpace1(table, size1);
	table = CreateKeySpace2(table, size2);

	return table;
}

int delElemKS1(Table* table, int j)
{
	int hesh = -1;
	
	if (table->ks1[j].info != NULL)
	{
		for (size_t i = 0; i < table->msize2; i++)
		{
			if (table->ks2[i].node == table->ks1[j].info)
			{
				table->ks2[i].busy = 0;
				table->ks2[i].key.count = NULL;
				table->ks2[i].node = NULL;
			}
		}
		free(table->ks1[j].info->info->str);
		table->ks1[j].info->info->str = NULL;
		table->ks1[j].info->info = NULL;
		table->ks1[j].key.str = '\0';
		table->ks1[j].release = 0;
	}

	return 1;
}



int delTable(Table* table)
{
	Item* item_temp = NULL;
	Item* item = NULL;
	if (table == NULL)
	{
		return 1;
	}
	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].info->info != NULL)
		{
			item_temp = table->ks1[i].info;
			while (item_temp != NULL)
			{
				item = item_temp;
				item_temp = item_temp->next;
				free(item->info->str);
				free(item->info);
				free(item);
			}
		}
	}
	free(table->ks1);
	free(table->ks2);
	free(table);
}

int addItem(Table* table, char* str_temp1, char* str_temp2, int temp1, int temp2, int key2)
{
	//printf("Key2 %d\n", key2);//
	int some_index = -1;
	int indexKS1 = -1;
	int indexKS2 = -1;
	int success = 0;
	int realese_temp = 0;
	int flag = 0;
	int flag_next = 0;
	Item* item_temp = NULL;
	unsigned int temp_key = 0;

	if (strlen(str_temp1) > (size_key))
	{
		//printf("Bad size of key \n");
		free(str_temp1);
		return -1;
	}

	//Вставка
	
	for (size_t i = 0; i < table->msize1; i++)
	{

		if (success == 0) 
		{
			if ((table->ks1[i].key.str != NULL) && ((strcmp(table->ks1[i].key.str, str_temp1) == 0)) && (strlen(table->ks1[i].key.str) == strlen(str_temp1)))
			{

				for (size_t j = 0; j < table->msize2; j++)
				{
					if (key2 == table->ks2[h(key2, j, table->msize2)].key.count)
					{
						flag_next = 1;
						item_temp = table->ks2[h(key2, j, table->msize2)].node;
						table->ks2[h(key2, j, table->msize2)].busy = 1;
						while (item_temp->next != NULL)
						{
							realese_temp = item_temp->next->release;
							item_temp = item_temp->next; //последний элемент в списке 
						}
						item_temp->next = (Item*)malloc(sizeof(Item));
						item_temp->next->next = NULL;
						item_temp->next->release = realese_temp + 1;

						item_temp->next->info = (InfoType*)malloc(sizeof(InfoType));
						item_temp->next->info->number_1 = temp1;
						item_temp->next->info->number_2 = temp2;
						item_temp->next->info->str = str_temp2;
						success = 1;
						flag = 1;
						break;
					}
				}
				if (flag_next == 0)
				{
					if (table->csize1 == table->msize1 || table->csize2 == table->msize2)
					{
						return -1;
					}
					for (size_t j = 0; j < table->msize1; j++)
					{
						if (table->ks1[j].info->info == NULL)
						{
							indexKS1 = j; 
							break;
						}
					}
					for (size_t l = 0; l < table->msize2; l++)
					{
						if (table->ks2[h(key2, l, table->msize1)].key.count == key2)
						{
							return -1;
						}
					}
					for (size_t j = 0; j < table->msize2; j++)
					{
						if (table->ks2[h(key2, j, table->msize2)].busy == 0)
						{
							 some_index = h(key2, j, table->msize2);
							 table->ks2[h(key2, j, table->msize2)].busy = 1;
							 break;
						}
					}
					table->ks1[indexKS1].key.str = str_temp1;
					table->ks1[indexKS1].info->info = (InfoType*)malloc(sizeof(InfoType));
					table->ks1[indexKS1].info->next = NULL;
					table->ks1[indexKS1].info->info->number_1 = temp1;
					table->ks1[indexKS1].info->info->number_2 = temp2;
					table->ks1[indexKS1].info->info->str = str_temp2;
					table->ks1[indexKS1].info->release = 0;
					for (size_t k = 0; k < table->msize1; k++)
					{
						if ((table->ks1[k].key.str != NULL) && (indexKS1 != k) && ((strcmp(table->ks1[k].key.str, str_temp1) == 0)) && (strlen(table->ks1[k].key.str) == strlen(str_temp1)))
						{
							table->ks1[indexKS1].release = table->ks1[k].release + 1;
							printf("\\\\%d\n", table->ks1[indexKS1].release);
							printf("\\\\%d\n", table->ks1[k].release);
						}
					}
					table->ks2[some_index].node = table->ks1[indexKS1].info;
					table->ks2[some_index].busy = 1;
					table->ks2[some_index].key.count = key2;
					success = 1;
					flag = 1;
					++table->csize1;
					++table->csize2;
				break;
				}
			}
		}
	}
	if (success == 0 && flag == 0)
	{
		if (table->csize1 == table->msize1 || table->csize2 == table->msize2)
		{
			
			return -1;
		}
		for (size_t i = 0; i < table->msize1; i++)
		{
			if (table->ks1[i].info->info == NULL || table->ks1[i].info == NULL)
			{
				indexKS1 = i;//индекс свободной ячейки 
				break;
			}
		}
		
		for (size_t i = 0; i < table->msize2; i++)
		{
			if (table->ks2[h(key2, i, table->msize1)].key.count == key2)
			{
				//printf("Index ks2 3 %d \n", h(key2, i, table->msize2));
				return -1;
			}
		}
		
		table->ks1[indexKS1].key.str = str_temp1;
		table->ks1[indexKS1].info->info = (InfoType*)malloc(sizeof(InfoType));
		table->ks1[indexKS1].info->next = NULL;
		table->ks1[indexKS1].info->info->number_1 = temp1;
		table->ks1[indexKS1].info->info->number_2 = temp2;
		table->ks1[indexKS1].info->info->str = str_temp2;
		table->ks1[indexKS1].info->release = 0;

		for (size_t k = 0; k < table->msize1; k++)
		{
			if ((table->ks1[k].key.str != NULL) && (indexKS1 != k) && ((strcmp(table->ks1[k].key.str, str_temp1) == 0)) && (strlen(table->ks1[k].key.str) == strlen(str_temp1)))
			{
				table->ks1[indexKS1].release = table->ks1[k].release + 1;
			}
		}
		//table->ks1[indexKS1].release = 0;
		++table->csize1;
		for (size_t j = 0; j < table->msize2; j++)
		{
			if (table->ks2[h(key2, j, table->msize2)].busy == 0)
			{
				table->ks2[h(key2, j, table->msize2)].key.count = key2;
				table->ks2[h(key2, j, table->msize2)].node = table->ks1[indexKS1].info;
				table->ks2[h(key2, j, table->msize2)].busy = 1;
				indexKS2 = h(key2, j, table->msize2);
				//printf("Index ks2 3 %d \n", h(key2, j, table->msize2));
				++table->csize2;
				break;
			}
		}
		if (indexKS2 == -1)
		{
			return -1;
		}
		success = 1;
	}
	return 1;
}
