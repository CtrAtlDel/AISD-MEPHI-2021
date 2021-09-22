int delElemKS1(KeySpace1 keyspace)
{
	if (keyspace.info != NULL)
	{
		free(keyspace.info->info->str);
		keyspace.info->info = NULL;
		keyspace.key.str = '\0';
		keyspace.release = 0;
	}

	return 1;
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
