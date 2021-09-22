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
	/* ������ ��������*/
	struct Item* next;
	/* ��������� �� ��������� ������� � ������ ��������� ������	*/
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
	KeyType1 key;		/* ���� ��������*/
	int release;		/* ����� ������ ��������(���� ������ ���� ����)*/
	struct Item* info;			/* ��������� �� ����������*/
} KeySpace1;

typedef struct KeySpace2 {
	int busy;		
	struct KeyType2 key;		
	struct Item* node;		
} KeySpace2;

typedef struct Node2 {
	int release;	/* ����� ������ ��������		*/
	InfoType* info;		/* ��������� �� ����������		*/
	struct Node2* next; 		/* ��������� �� ��������� �������	*/
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
