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

typedef struct KeyType2
{
	unsigned int count;
} KeyType2;

typedef struct KeySpace1
{
	KeyType1 key;		/* ключ элемента*/
	int release;		/* номер версии элемента(если ввелся один ключ)*/
	struct Item* info;			/* указатель на информацию*/
} KeySpace1;

typedef struct KeySpace2 {
	int busy;		
	struct KeyType2 key;		
	struct Item* node;		
} KeySpace2;

typedef struct Node2 {
	int release;	/* номер версии элемента		*/
	InfoType* info;		/* указатель на информацию		*/
	struct Node2* next; 		/* указатель на следующий элемент	*/
} Node2;

typedef struct Table
{
	KeySpace1* ks1;
	KeySpace2* ks2;

	int msize1;
	int msize2;

	int csize1;
	int csize2;
} Table;
