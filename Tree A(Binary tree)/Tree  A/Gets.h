#define  _CRT_SECURE_NO_WARNINGS


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