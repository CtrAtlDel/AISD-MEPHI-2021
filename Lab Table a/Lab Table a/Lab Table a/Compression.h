
int Compression(Table* table)
{
	Item* temp = NULL;
	KeyType1 keyks1;
	int hesh = -1;
	int max_release = 0;
	for (size_t i = 0; i < table->msize1; i++)
	{
		if (table->ks1[i].info->info != NULL)
		{
			keyks1.str = table->ks1[i].key.str;
			max_release = table->ks1[i].release;
			for (size_t j = 0; j < table->msize1; j++)
			{
				if (table->ks1[j].key.str != '\0')
				{
					if (table->ks1[j].info->info != NULL && (table->ks1[j].release > max_release) && (strcmp(table->ks1[j].key.str, keyks1.str) == 0)) //&& *(table->ks1[j].key.str) == *(keyks1.str)
					{
						max_release = table->ks1[j].release;
					}
				}
			}
			for (size_t j = 0; j < table->msize1; j++)
			{
				if (table->ks1[j].key.str == keyks1.str && table->ks1[j].release < max_release)
				{
					delElemKS1(table, j);
					--table->csize2;
					--table->csize1;
				}
			}
		}
	}
	return 1;
}
