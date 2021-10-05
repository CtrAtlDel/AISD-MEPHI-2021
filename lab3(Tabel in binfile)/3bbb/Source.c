#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#define key_size 4
//просматриваемая таблица

typedef struct KS1 
{
	char* key;
	int release;
	int len_str;
	int offset;
}KS1;

typedef struct Table 
{
	KS1* ks1;
	int csize; // текущий размер таблицы
	int msize; // максимальный размер таблицы
	FILE* fd;
}Table;

char* getStr();
int getInt(int* a);
int dialog(const char* msgs[], int N);
Table* Create_table(Table* table);
int Print_table(Table* table);
int Create_file(Table* table, char* fname, int size);
int Find_key(Table* table, char* key);
int Find_info(Table* table, int iter);
int D_Insert_elem(Table* table);
int Insert_elem(Table* table, char* key, int n1, int n2, char* info);
int Delete_table(Table* table);
int D_loadfile(Table* table);
int D_Find_table(Table* table);
int Find_table(Table* table, char* key);
int D_Delete_elem(Table* table);
int Save_me(Table* table);
char* constr(char* str);
int Compressiv(Table* table);

int(*fptr[])(Table*) = { NULL, D_Insert_elem, Print_table, D_Find_table, D_Delete_elem, Compressiv};
const char* msgs[] = { " 0. Quit", " 1. Add Item", " 2. Print Table", "3. Find Item", "4. Delete Item", "5. Compress"};
const char* err_msg[] = { "Ok", "Table overflow", "This element doesn't exist" };
const char* msgs_delete[] = { " 0. Delete elem with 2 keys", " 1. Delete elem with key 1"," 2. Delete elem with key 2" };
const int  Nmsgs_delete = sizeof(msgs_delete) / sizeof(msgs_delete[0]);
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

Table* Create_table(Table* table) 
{
	int size;
	table = (Table*)malloc(sizeof(Table));
	table->msize = 0;
	table->csize = 0;
	return table;
}

int D_loadfile(Table* table) 
{
	int size;
	int res;
	char* fname = NULL;
	printf("Input filename\n");
	fname = getStr();
	if (fname == NULL) { return -1; }
	res = Load_file(table, fname); 
	if (res == -1) //если нет файла
	{
		printf("Input size of ks1\n");
		if (getInt(&size) == 0) { return 0; };
		Create_file(table, fname, size);
	}
	return 1;
}

int Load_file(Table* table, char* fname) 
{
	fopen_s(&(table->fd), fname, "r+b");
	if (table->fd == NULL) { return -1;	 }
	fread(&(table->msize), sizeof(int), 1, table->fd);
	table->ks1 = (KS1*)calloc(table->msize, sizeof(KS1));
	fread(&(table->csize), sizeof(int), 1, table->fd);
	for (size_t i = 0; i < table->msize; i++)
	{	
		char* str_tmp = calloc(key_size + 1, 1);
		table->ks1[i].key = str_tmp;
		fread(table->ks1[i].key, sizeof(char), key_size + 1, table->fd); //???
		fread(&(table->ks1[i].release), sizeof(int), 1, table->fd);
		fread(&(table->ks1[i].len_str), sizeof(int), 1, table->fd);
		fread(&(table->ks1[i].offset), sizeof(int), 1, table->fd);
	}
	return 1;
}

int Create_file(Table* table,char* fname, int size) 
{
	table->msize = size;
	if (fopen_s(&(table->fd), fname, "w + b") != 0)
	{
		table->ks1 = NULL;
		return -1; // memory error
	}
	table->ks1 = (KS1*)calloc(table->msize, sizeof(KS1));
	fwrite(&(table->msize), sizeof(int), 1, table->fd);
	fwrite(&(table->csize), sizeof(int), 1, table->fd);
	for (size_t i = 0; i < table->msize; i++)
	{
		char* str_tmp = calloc(key_size + 1, 1);
		table->ks1[i].key = str_tmp;
		fwrite(table->ks1[i].key, sizeof(char), key_size + 1, table->fd); //???
		fwrite(&(table->ks1[i].release), sizeof(int), 1, table->fd);
		fwrite(&(table->ks1[i].len_str), sizeof(int),1, table->fd);
		fwrite(&(table->ks1[i].offset), sizeof(int), 1, table->fd);
	}
	return 1;
}

int Find_key(Table* table, char* key) // смотрим есть ли такой ключ уже в списке -1 empty table, -2 no key, else index
{
	if (table->csize == 0)
	{
		return -1; // empty table
	}
	int res = -2;
	for (size_t i = 0; i < table->csize; i++)
	{
		if (strcmp(table->ks1[i].key, key) == 0)
		{
			res = i;
		}
	}
	return res;
}

int Find_info(Table* table, int iter) //ищет по индексу в таблице информацию элемента в файле
{
	int n1 = 0, n2 = 0;
	char* info_str = (char*)malloc(table->ks1[iter].len_str * sizeof(char));
	if (info_str == NULL)
	{
		printf("Memory error\n");
		return -2;
	}
	fseek(table->fd, table->ks1[iter].offset, SEEK_SET);
	fread(&n1, sizeof(int), 1, table->fd);
	fread(&n2, sizeof(int), 1, table->fd);
	fread(info_str, sizeof(char), table->ks1[iter].len_str, table->fd);
	printf("Key -> %s\n rel -> %d\n n1-> %d\n n2->%d\n info -> %s\n\n", table->ks1[iter].key,table->ks1[iter].release, n1, n2, info_str);
	free(info_str);
	return 1;
}

int D_Insert_elem(Table* table) 
{
	int res, n1, n2;
	char* info;
	char* key;
	printf("Input key->");
	key = getStr();
	printf("Input integer 1->");
	if (getInt(&n1) == 0) { return 0; };
	printf("Input integer 2->");
	if (getInt(&n2) == 0) { return 0; };
	printf("Input info->");
	info = getStr();
	res = Insert_elem(table, key, n1, n2, info);
	if (res == -1)
	{
		printf("Eror bad size of key\n");
		return 1;
	}
	if (res == -2)
	{
		printf("Table overflow\n");
	}
	return 1;
}

int Insert_elem(Table* table, char* key, int n1, int n2, char* info) 
{
	int find_index = -1;
	int n11;
	int n22;
	//char* sre = malloc(strlen(info));
	if (strlen(key) > key_size)//check sizeof key
	{
		return -1;
	}
	if (table->csize == table->msize)
	{
		return -2;
	}
	key = constr(key);
	find_index = Find_key(table, key);// индекс элемента, если такой есть
	if (find_index > -1)
	{
		table->ks1[table->csize].release = table->ks1[find_index].release + 1;
	}
	else 
	{
		table->ks1[table->csize].release = 0;
	}
	table->ks1[table->csize].len_str = strlen(info) + 1;
	
	table->ks1[table->csize].key = key;
	fseek(table->fd,0, SEEK_END); //end of file
	table->ks1[table->csize].offset = ftell(table->fd);
	fwrite(&n1, sizeof(int), 1, table->fd);
	fwrite(&n2, sizeof(int), 1, table->fd);
	fwrite(info, sizeof(char) , strlen(info) + 1, table->fd);
	//Debbug
	//int sim = 0;
	//fseek(table->fd, table->ks1[table->csize].offset, SEEK_SET);
	//sim = fread(&n11, sizeof(int), 1, table->fd);
	//fread(&n22, sizeof(int), 1, table->fd);
	//fread(sre, sizeof(char), strlen(sre) + 1, table->fd);
	//printf("n1 = %d n2 = %d sre= %s", n11, n22, sre);
	table->csize++;
	return 1;
}

int D_Find_table(Table* table) 
{
	int res;
	int rel;
	int ch = -1;
	char* key;
	printf("Input key->");
	key = getStr();
	printf("Whith vertsion (1/0) \n");
	if (getInt(&ch) == 0) { return 0; };
	if (ch)
	{
		printf("Input release \n");
		if (getInt(&rel) == 0) { return 0; };
		res = Find_table_rel(table, key, rel);
	}
	else 
	{
		res = Find_table(table, key);
	}
	if (res == -1)
	{
		printf("Bad size of key\n");
	}
	if (res == -2)
	{
		printf("No key in table\n");
	}
	if (res == -3)
	{
		printf("I didnt found this version of key\n");
	}
	return 1;
}

int key_check(char* key) 
{
	if (strlen(key)  > key_size )//check sizeof key
	{
		return -1;
	}
	else { return 1; }
} //-1 if bad

int Find_table(Table* table, char* key) 
{
	if (key_check(key) == -1) { return -1; }
	key = constr(key);
	if (Find_key(table, key) < 0) 
	{
		return -2;
	}
	int index = 0;
	
	char* new_key = NULL;
	Table* new_table = NULL;
	new_key = malloc((strlen(key) + 1) * sizeof(char));
	new_table = Create_table(new_table);
	new_table->ks1 = (KS1*)calloc(table->csize, sizeof(KS1));
	new_table->csize = 0;
	new_table->msize = table->csize;
	new_table->fd = table->fd;
	for (size_t i = 0; i < table->csize; i++)
	{
		if (strcmp(table->ks1[i].key, key) == 0)
		{
			new_key = strcpy(new_key, key);
			new_table->ks1[index].key = new_key;
			new_table->ks1[index].release = table->ks1[i].release;
			new_table->ks1[index].len_str = table->ks1[i].len_str;
			new_table->ks1[index].offset = table->ks1[i].offset;
			++new_table->csize; 
			++index;
		}
	}
	if (new_table->csize == 0)
	{
		Delete_table(new_table);
		return -3;
	}
	Print_table(new_table);
	Delete_table(new_table);
	return 1;
}

int Find_table_rel(Table* table, char* key, int rel) 
{
	if (key_check(key) == -1) { return -1; }
	key = constr(key);
	if (Find_key(table, key) < 0)
	{
		return -2;
	}
	//key = constr(key);
	int index = 0;
	char* new_key = NULL;
	Table* new_table = NULL;
	new_key = malloc((strlen(key) + 1) * sizeof(char));
	new_table = Create_table(new_table);
	new_table->ks1 = (KS1*)calloc(table->csize, sizeof(KS1));
	new_table->csize = 0;
	new_table->msize = table->csize;
	new_table->fd = table->fd;
	for (size_t i = 0; i < table->csize; i++)
	{
		if (strcmp(table->ks1[i].key, key) == 0 && rel == table->ks1[i].release)
		{
			new_key = strcpy(new_key, key);
			new_table->ks1[index].key = new_key;
			new_table->ks1[index].release = table->ks1[i].release;
			new_table->ks1[index].len_str = table->ks1[i].len_str;
			new_table->ks1[index].offset = table->ks1[i].offset;
			++new_table->csize;
			++index;
		}
	}
	if (new_table->csize == 0)
	{
		Delete_table(new_table);
		return -3;
	}
	Print_table(new_table);
	Delete_table(new_table);
	return 1;
}

int D_Delete_elem(Table* table)
{
	int res;
	int rel = 0;
	int ch = -1;
	char* key;
	printf("Input key->");
	key = getStr();
	printf("Whith vertsion (1/0) \n");
	if (getInt(&ch) == 0) { return 0; };
	if (ch)
	{
		printf("Input release \n");
		if (getInt(&rel) == 0) { return 0; };
		res = Delete_elem(table, key, rel, ch);
	}
	else
	{
		res = Delete_elem(table, key, rel, ch);
	}
	//res = Delete_elem(table, key, rel, ch);
	if (res == -1)
	{
		printf("No key in table\n");
	}
	if (res == -2)
	{
		printf("Bad key\n");
	}
	return 1;
}

int Find_key_rel(Table* table, char* key, int rel) 
{
	if (table->csize == 0)
	{
		return -1; // empty table
	}
	int res = -2;
	for (size_t i = 0; i < table->csize; i++)
	{
		if (strcmp(table->ks1[i].key, key) == 0 && table->ks1[i].release == rel)
		{
			res = i;
		}
	}
	return res;
}

int Delete_elem(Table* table, char* key, int rel, int ch) 
{
	int index = -1;
	if (strlen(key) > key_size)//check sizeof key
	{
		return -2;
	}
	key = constr(key);
	if (Find_key(table, key) < 0)
	{
		return -1;
	}
	for (size_t i = 0; i < table->csize; i++)
	{
		if (ch == 1)
		{
			index = Find_key_rel(table, key, rel);
		}
		else 
		{
			index = Find_key(table, key);
		}
		if (index < 0)
		{
			return 1;
		}
		if (index == table->csize - 1) //last elem
		{
			table->ks1[index].release = 0;
			//free(table->ks1[index].key);
			table->ks1[index].key = NULL;
			table->ks1[index].offset = 0;
			table->ks1[index].len_str = 0;
			table->csize--;
		}
		else
		{
			table->ks1[index].release = table->ks1[table->csize - 1].release;
			table->ks1[table->csize - 1].release = 0;
			/*free(table->ks1[index].key);*/
			table->ks1[index].key = table->ks1[table->csize - 1].key;
			table->ks1[table->csize - 1].key = NULL;
			table->ks1[index].offset = table->ks1[table->csize - 1].offset;
			table->ks1[index].len_str = table->ks1[table->csize - 1].len_str;
			table->csize--;
		}
	}
	return 1;
}

int Print_table(Table* table) 
{
	if (table->csize == 0)
	{
		printf("Table is empty");
	}
	for (size_t i = 0; i < table->csize; i++)
	{
		Find_info(table, i);
	}
	return 1;
}

int Delete_table(Table* table) 
{
	free(table->ks1);
	free(table);
	return 1;
}

int Save_me(Table* table) 
{
	for (size_t i = 0; i < table->csize; i++)
	{
		table->ks1[i].key = constr(table->ks1[i].key);
	}
	fseek(table->fd, sizeof(int), SEEK_SET);// msize
	fwrite(&(table->csize), sizeof(int),1, table->fd);
	for (size_t i = 0; i < table->csize; i++)
	{
		fwrite(table->ks1[i].key, sizeof(char), key_size + 1, table->fd); 
		fwrite(&(table->ks1[i].release), sizeof(int), 1, table->fd);
		fwrite(&(table->ks1[i].len_str), sizeof(int), 1, table->fd);
		fwrite(&(table->ks1[i].offset), sizeof(int), 1, table->fd);
	}
	fclose(table->fd);
	table->fd = NULL;
}

char* constr(char* str) 
{
	char* str_tmp = (char*)malloc((key_size + 1 - strlen(str)) * sizeof(char));
	str_tmp[key_size - strlen(str)] = '\0';
	for (size_t i = 0; i < strlen(str_tmp); i++)
	{
		str_tmp[i] = '0';
	}
	str_tmp = strcat(str_tmp, str);
	return str_tmp;
}

int Compressiv(Table* table) 
{
	if (table->csize == 0)
	{
		printf("Empty table\n");
		return 1;
	}
	int max_rel = -1;
	char* str = calloc(key_size + 1, 1);
	for (size_t i = 0; i < table->csize; i++)
	{
		str = table->ks1[i].key;
		max_rel = table->ks1[i].release;
		for (size_t j = 0; j < table->csize; j++)
		{
			if (table->ks1[j].release > max_rel && strcmp(table->ks1[j].key, str) == 0)
			{
				max_rel = table->ks1[j].release;
			}
		}
		for (size_t j = 0; j < table->csize; j++)
		{
			if (strcmp(table->ks1[j].key, str) == 0 && table->ks1[j].release != max_rel)
			{
				Delete_elem(table, str, table->ks1[j].release, 1);
			}
		}
	}
	return 1;
}

int main() 
{
	Table* table = NULL;
	table = Create_table(table);
	D_loadfile(table);
	int rc = 0, f = 1;
	while ((rc = dialog(msgs, NMsgs)) && f)
	{
		f = fptr[rc](table);
	}
	Save_me(table);
	Delete_table(table);
	//FILE* fd = NULL;
	//char* fname = getStr();
	//fopen_s(&fd, fname, "w+b");
	//char* help = getStr();
	//char* hh = calloc(strlen(help) + 1,1);;
	//int offset;
	//fwrite(help, sizeof(char), strlen(help) + 1, fd);
	//hh[strlen(help)] = '\0';
	//offset = ftell(fd);
	//fseek(fd, 0, SEEK_SET);
	//fread(hh, sizeof(char), strlen(help) + 1,fd);
	//printf("asd %s", hh);
	return 0;
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