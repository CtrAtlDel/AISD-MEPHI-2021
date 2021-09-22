#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include "stdlib.h"
#include <math.h>

typedef struct item 
{
	char c;
	struct item* next;
} item;

item* delete_head(item* root);
item* delete_elem(item* lst, item* root);

item* free_list(item* head) 
{
	item* tmp = NULL;
	while (head != NULL) 
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
	return head;
}

int get_list(item** pptr) 
{
	char buf[1001], * str;
	item head = { '*', NULL };
	item* tail = &head;
	int n, rc = 1;
	do {
		n = scanf("%1000[^\n]", buf);
		if (n == -1) {
			free_list(head.next);
			head.next = NULL;
			return -1;
		}
		if (n > 0) {
			for (str = buf; *str != '\0'; ++str) 
			{
				tail->next = (item*)malloc(sizeof(item));
				tail = tail->next;
				tail->c = *str;
			}
			tail->next = NULL;
		}
		else
			scanf("%*c", 0);
	} while (n > 0);
	*pptr = head.next;
	return 1;
}

void print_list(item* ptr) 
{
	printf("\'");
	for (; ptr != NULL; ptr = ptr->next)
		printf("%c", ptr->c);
	printf("\'");
	printf("\n");
}

item* delete_elem(item* lst, item* root)
{
	item* temp;
	item* temp_t;
	temp = root;
	while ((temp->next != lst) && (temp->next != NULL) && (temp != NULL))
	{
		temp = temp->next;
	}
	temp->next = lst->next;
	free(lst);
	return temp;
}


item* delete_head(item* root)
{
	item* temp;
	temp = root->next;
	free(root);
	return temp;
}



item* deletebetweentwoitems(item* ptr1, item* ptr2, item* root)
{
	item* temp = root;
	item* temp2 = root;
	ptr2 = ptr2->next;
	ptr2 = ptr2->next;
	if (temp == ptr1 || temp == ptr2 )
	{
		while (ptr1 != ptr2)
		{
			temp = delete_head(temp);
			ptr1 = temp;
		}
		
		return ptr1;
	}

	while (ptr1 != ptr2)
	{
		ptr1 = delete_elem(ptr1, root);
		ptr1 = ptr1->next;
	}
	//printList(root);
	return root;
}

item* task_function(item* list) 
{
	int flag = 0;
	int flag_P = 0;
	item* head = list;
	char numbers[11] = "0123456789";

	item* ptr1 = NULL;
	item* ptr2 = NULL;

	while(list != NULL && list->next != NULL)
	{
		if ((strchr(numbers, list->c) == NULL) && (list->c != ' ') && (list->c != '\t'))
		{
			return NULL;
		}
		if (list == head  && (list->c == ' ' || list->c == '\t'))
		{
			head = delete_head(list);
			list = head;
		}
		else 
		{
			if (list->c != ' ' && list->c != '\t')
			{
				break;
			}
		}
	}

	ptr1 = list;
	ptr2 = list;
	head = list;

	while (ptr1 != NULL && ptr1->next != NULL && ptr1->c != '\0')
	{
		if (ptr1 == head && (ptr1->c == ' ' || ptr1->c == '\t'))
		{
			head = list;
			head = delete_head(head);
			list = head;
			ptr1 = head;
		}

		if ((ptr1->c == ' ' || ptr1->c == '\t'))
		{
			if (flag_P == 1) 
			{
				head = list;
				ptr1 = delete_elem(ptr1, head);
				//ptr1 = ptr1->next;
				flag_P = 0;
				continue;
			}

			flag_P = 1;
		}
		if ((strchr(numbers, ptr1->c) == NULL) && (ptr1->c != ' ') && (ptr1->c != '\t'))
		{
			return NULL;

		}
		if (ptr1->c != ' ' && ptr1->c != '\t')
		{
			ptr2 = ptr1;
			flag_P = 0;
			flag = 0;
			while (ptr2->c != ' ' && ptr2->c != '\t' && ptr2->next->c != ' ' && ptr2->next->c != '\t') //проверка на последовательность
			{
				if (ptr2->c > ptr2->next->c)
				{
					flag = 1;
				}

				ptr2 = ptr2->next;
			}
			if (flag) //нужно удалить 
			{
				head = list;
				ptr1 = deletebetweentwoitems(ptr1, ptr2, head);
				list = ptr1;
				continue;
			}
		}
		ptr1 = ptr1->next;
	}
	
	return list;
}
item* create_end(char sym, item* root) 
{
	item* temp = (item*)malloc(sizeof(item));
	item* p;
	item* head = root;
	while (head->next != NULL)
	{
		head = head->next;
	}
	head->next = temp;
	temp->c = sym;
	temp->next = NULL;
	return root;
}



item* delete_end(item* root) 
{
	item* temp1 = root;
	item* temp2 = root;
	item* temp3 = root;
	int count = 0;
	while (temp1->next != NULL) 
	{
		if (count >= 1)
		{
			temp2 = temp2->next;
		}
		if (count >= 2)
		{
			temp3 = temp3->next;
		}
		temp1 = temp1->next;
		++count;
	}
	if (temp3->c == ' ')
	{
		return root;
	}
	if (temp1->c == ' ' || temp1->c == '\t')
	{
		
		temp2->next = NULL;
		free(temp1);
		if (temp2->c == ' ' || temp2->c == '\t')
		{
			temp3->next = NULL;
			free(temp2);
		}
	}

	return root;
}

item* skip_tabs(item* root)
{
	item* temp = root;
	while (temp->next != NULL)
	{
		if (temp->c == '\t') 
		{
			temp->c = ' ';
		}
		temp = temp->next;
	}

	return root;
}

item* index(item* root, int index)
{
	int count = 0;
	int flag = 0;
	item* temp = root;

	while (temp != NULL)
	{

		if (count == index)
		{
			return temp;
		}

		if (temp->c == ' ')
		{
			++count;
		}
		temp = temp->next;
	}

}

item* add_elem(item* pointer, char sym) 
{
	item* temp;
	temp = (item*)malloc(sizeof(item));
	temp->next = pointer->next;
	pointer = temp;
	temp->c = sym;

	return temp;
}

item* swap(item* root) 
{
	item* temp = root; // индекс первого слова
	char sym_temp;
	int choise = -1;
	int size = 0;
	int n; // номер
	int iterator = 0;
	item* index_e;
	item* sp_1;
	item* sp_2;
	item* temtem = NULL;
	item* poiter;

	while (temp->next != NULL)
	{
		++size;
		temp = temp->next;
	}
	temp = root;
	
	printf("Input number of number \n");
	n = scanf("%d", &choise);

	index_e = index(root, n); // индекс нужного слова 


	return root;

}

int	SumofElement(item* pointer1) 
{
	int summ = 0;
	int count = 0;
	while (pointer1 != NULL && pointer1->c != ' ')
	{
		count = pointer1->c - '0';
		summ = summ + count;
		pointer1 = pointer1->next;
	}

	return summ;
}

item* MaxSumm(item* root) 
{
	int max_sum = 0;
	int summ = 0;
	item* elem = (item*)malloc(sizeof(item));
	elem->next = NULL;
	elem->c = '*';
	item* temptemp;
	item* end = root;
	item* head = root;
	item* temp = root;
	item* temp1 = root;
	item* temp_of_max_element = NULL;
	while (temp != NULL) 
	{
		if (temp->c != ' ')
		{
			temp1 = temp;
			summ = SumofElement(temp1);

			if (summ >= max_sum)
			{
				max_sum = summ;
				temp_of_max_element = temp1;
			}
		}

		temp = temp->next;
	}
	if (temp_of_max_element->next == NULL)
	{
		temptemp = temp_of_max_element->next;
		temp_of_max_element->next = elem;
		elem->next = temptemp;

		printf("Max summ %d \n", max_sum);

		return head;
	}

	while (elem->next != NULL)
	{
		elem = elem->next;
	}



	while (temp_of_max_element->next != NULL && temp_of_max_element->next->c != ' ' )
	{
		temp_of_max_element = temp_of_max_element->next;
	}

	temptemp = temp_of_max_element->next;
	temp_of_max_element->next = elem;
	elem->next = temptemp;

	printf("Max summ %d \n", max_sum);
	
	return head;
}


int main()
{
	int k = 1;
	item* string;
	int n = 0;
	int length = 0;
	while (k)
	{
		printf("Enter string: ");
		n = get_list(&string);
		if (n == -1)
		{
			printf("Good bye");
			break;
		}
		if ((string == NULL) || (n == 0)) 
		{
			printf("\Empty string or incorrect input\n");
			free_list(string);
			continue;
		}
		string = create_end(' ', string);
		string = skip_tabs(string);
		string = task_function(string);
		
		if (string == NULL)
		{
			printf("\Incorrect input\n");
			free_list(string);
			continue;
		}
		string = delete_end(string);
		print_list(string);
		printf("\n");
		string = MaxSumm(string);
		string = delete_end(string);
		print_list(string);
		printf("\n");
		free_list(string);
	}
	return 0;
}