#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include "stdlib.h"
#include <math.h>
int getInt(int*);
int hextobin(char* str_old, char** str_new, int flag_hex);
int create_str(char** str, char* buf);

int getInt(int* a)
{
	int n;
	do
	{
		n = scanf("%d", a);
		if (n == 0) {
			printf("ERROR\n");
			scanf("%*c", 0);
			scanf("%*[^\n]");
		}
	} while (n == 0);
	return n < 0 ? 0 : 1;
}

char* mystrchr(char* alph, char symbol)
{
	char* ptr;
	ptr = alph;
	for (size_t i = 0; i < strlen(alph); i++)
	{
		if (*ptr == symbol)
		{
			return ptr;
		}
		++ptr;
	}
	return NULL;
}

int task_function(char* str, char* str_bin)
{
	int length = 0;
	int flag_b = 0;
	int flag_hex = 1;
	char elem1 = ' ';
	char elem2 = '\t';
	int step = 0;
	char alpfh[7] = "ABCDEF";
	char numbers[11] = "0123456789";
	char* p_str;
	char* p;
	char** p_str_bin;
	p_str = str;
	p = str_bin;
	p_str_bin = &p;
	for (size_t i = 0; i < strlen(str); i++)
	{
		//printf("pointer %d\n", p_str_bin);
		if ((mystrchr(alpfh, *p_str) == NULL) && (mystrchr(numbers, *p_str) == NULL) && (*p_str != ' ') && (*p_str != '\t'))
		{
			return -1;
		}
		if ((*p_str == ' ') || (*p_str == '\t'))
		{
			elem1 = *(p_str);
			elem2 = *(p_str);
		}
		if ((*p_str != ' ') && (*p_str != '\t'))
		{
			if ((elem1 == ' ') || (elem2 == '\t'))
			{
				flag_hex = 1;
				flag_b = 1;
				step = hextobin(p_str, p_str_bin, flag_hex);
				length += step;
				//p_str_bin += step;
				elem1 = *(p_str);
				elem2 = *(p_str);
			}
			else 
			{
				flag_hex = 0;
				flag_b = 1;
				step = hextobin(p_str, p_str_bin, flag_hex);
				length += step;
				//p_str_bin += step;
				elem1 = *(p_str);
				elem2 = *(p_str);
			}

		}

		if ((((*p_str == ' ') && (flag_b == 1)) || ((*p_str == '\t') && (flag_b == 1))) && (i != (strlen(str) - 1)))
		{
			++length;
			flag_b = 0;
			**p_str_bin = ' ';
			++*p_str_bin;
		}
		++p_str;
		
	}

	return length;
}


int hextobin(char* p_str, char** p_str_bin, int flag_hex)
{
	//printf("pointer %d\n", p_str_bin);
	int step = 0;
	char sim = *p_str;
	int  count = 0;
	char alpfh[7] = "ABCDEF";
	char numbers[11] = "0123456789";
	int t = 0;
	int flag;
	if (flag_hex == 1)
	{
		flag = 0;
	}
	else
	{
		flag = 1;
	}
	if (strchr(alpfh, sim) != NULL) //буква
	{
		count = sim - 'A' + 10;
	}
	if (strchr(numbers, sim) != NULL)//цифра
	{
		count = sim - '0';
	}
	//printf("count is %d\n", count);
	for (int i = 3; i >= 0; i--)
	{
		t = count / (1 << i);
		count &= ((1 << i) - 1);
		flag = flag + t;
		if (i == 0 && flag == 0)
		{

			**p_str_bin = (char)(flag + '0');
			++(*p_str_bin);
			++step;
		}
		if (flag == 0)
		{
			continue;
		}
		**p_str_bin = (char)(t + '0');
		++(*p_str_bin);
		++step;
	}

	return step;
}


int create_str(char** str, char* buf)
{
	*str[0] = '\0';
	int n;
	int m = 1;
	do
	{
		n = scanf("%80[^\n]", buf);
		if (n > 0)
		{
			m += strlen(buf);
			*str = (char*)realloc(*str, m);
			strcat(*str, buf);
		}
		else
		{
			if (n == -1)
			{
				*str = (char*)realloc(*str, 1);
				*str[0] = '\0';
				return -1;
			}
		}

	} while (n > 0);
	scanf("%*c", 0);
	return 1;
}



int main()
{
	
	int k = 1;
	while (k) 
	{
		char buf[81];
		int flag;
		int length = 1;
		char** str = (char*)malloc(1);
		printf("Input your string \n");
		flag = create_str(&str, buf);

		if (flag == -1)
		{
			printf("Empty string\n");
			break;
		}
		char* str_bin = (char*)malloc((4 * (strlen(str))) * sizeof(char) + 1);
		str_bin[4 * strlen(str)] = '\0';
		printf("Old _str %s\n", str);
		length = task_function(str, str_bin);
		if (length == -1)
		{
			printf("Detected bad element from out of alph\n");
			continue;
		}
		if (str_bin[length -1] == ' ')
		{
			str_bin = (char*)realloc(str_bin, length);
			str_bin[length - 1] = '\0';
		}
		else 
		{
			str_bin = (char*)realloc(str_bin, length + 1);
			str_bin[length] = '\0';
		}
		printf("New_str ");
		printf("\"%s\"", str_bin);
		printf("\n");
		free(str);
		free(str_bin);
	}
	
	


	return 0;
}