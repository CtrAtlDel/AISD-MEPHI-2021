#define  _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "malloc.h"
#include "stdlib.h"
struct collums;
struct matrix;
int getInt(int*);
void create_col(struct collums*, int);
void create_matrix(struct matrix*);
void print_matrix(struct matrix*);
void clear_memory(struct matrix*);
void task_function(struct matrix*, struct matrix*);

struct collums
{
	int size_of_collums;
	int* elem;
};

struct matrix
{
	int rows;
	struct collums* rows_of_matr;
};

void create_matrix(struct matrix* matr)
{
	int rows;
	do {
		printf("Input size of rows\n");
		if (getInt(&rows) == 0) { return 0; }
	} while (rows <= 0);

	matr->rows = rows;
	matr->rows_of_matr = (struct collums*)malloc(sizeof(struct collums) * rows);

	for (int i = 0; i < rows; ++i) {
		create_col(matr->rows_of_matr + i, i);
	}
}

void create_col(struct collums* col, int index)
{
	int collums;

	do {
		printf("Input size of collums in %d rows\n", index);
		if (getInt(&collums) == 0) { return 0; }
	} while (collums <= 0);

	col->size_of_collums = collums;
	col->elem = (int*)malloc(sizeof(int) * collums);
	for (size_t i = 0; i < collums; ++i) {
		printf("Input %d elem of %d rows[%d][%d] ->", i, index,index, i);
		if (getInt(col->elem + i) == 0) { return 0; }
		printf("\n");
	}
}

void print_matrix(struct matrix* matr) 
{
	for (size_t i = 0; i < matr->rows; i++){
		for (size_t j = 0; j < matr->rows_of_matr[i].size_of_collums; j++){
			if (matr->rows_of_matr[i].elem[j] > -1000000 && matr->rows_of_matr[i].elem[j] < 1000000)
			{
				printf(" %d ", matr->rows_of_matr[i].elem[j]);
			}
		}
		printf("\n");
	}
}

void clear_memory(struct matrix* matr)
{
	for (size_t i = 0; i < matr->rows; i++){
		if (matr->rows_of_matr[i].size_of_collums != 0)
		{
			free(matr->rows_of_matr[i].elem);
		}
	}
	free(matr->rows_of_matr);
}

void task_function(struct matrix* old_matr, struct matrix* new_matr) 
{	
	int flag_plus = -1;
	int flag_minus = -1;
	int *p = NULL;
	int *p_new = NULL;
	int count_of_rows = 0;//индекс строки новой матрицы
	int begin = 0;
	int size_of_delete;
	new_matr->rows = old_matr->rows;
	new_matr->rows_of_matr = (struct collums*)malloc(sizeof(struct collums) * (new_matr->rows));
	for (size_t i = 0; i < new_matr->rows; i++)
	{
		new_matr->rows_of_matr[i].size_of_collums = 0;
	}
	 
	for (size_t i = 0; i < old_matr->rows; ++i)
	{
		flag_minus = -1;
		flag_plus = -1;
		begin = 0;
		p = old_matr->rows_of_matr[i].elem;
		size_of_delete = 0;
		for (size_t j = 0;j < old_matr->rows_of_matr[i].size_of_collums; ++j) 
		{
			if (*p > 0){ flag_plus = 1; }
			if (*p < 0){ flag_minus = 1; }
			if (((flag_plus == 1) ||(flag_minus == 1)) && (begin == 0))
			{
				new_matr->rows_of_matr[count_of_rows].size_of_collums = old_matr->rows_of_matr[i].size_of_collums + 1;
				new_matr->rows_of_matr[count_of_rows].elem = (int*)malloc(sizeof(int) * (new_matr->rows_of_matr[count_of_rows].size_of_collums + 1));

				p_new = new_matr->rows_of_matr[count_of_rows].elem;
				++p_new;

				begin++;

			}
			if ((flag_plus == -1) && (flag_minus == -1) && (j == old_matr->rows_of_matr[i].size_of_collums - 1))
			{
				size_of_delete = old_matr->rows_of_matr[i].size_of_collums;
				new_matr->rows_of_matr[count_of_rows].size_of_collums = 1;
				new_matr->rows_of_matr[count_of_rows].elem = (int*)malloc(sizeof(int) * (new_matr->rows_of_matr[count_of_rows].size_of_collums));
				new_matr->rows_of_matr[count_of_rows].elem[0] = size_of_delete;
				printf("J == %d\n", j);
				printf("Size of deleteeeeee 1 %d\n", size_of_delete);
			}
			if ((flag_plus == -1) && (flag_minus == -1) && (j != old_matr->rows_of_matr[i].size_of_collums - 1))
			{
				++size_of_delete;

				printf("Size of deleteeeeee 2 %d\n", size_of_delete);
			}
			if (flag_plus == 1)
			{	
				if (flag_minus == 1)
				{
					*p_new = *p;
					size_of_delete = size_of_delete + old_matr->rows_of_matr[i].size_of_collums - j - 1;
					new_matr->rows_of_matr[count_of_rows].elem[0] = size_of_delete;
					break;
				}
				*p_new = *p;
				++p_new;
				
			}
			if (flag_minus == 1)
			{
				if (flag_plus == 1)
				{
					*p_new = *p; //old_matr->rows_of_matr[i].elem[j]
					size_of_delete = size_of_delete + old_matr->rows_of_matr[i].size_of_collums - j - 1;
					new_matr->rows_of_matr[count_of_rows].elem[0] = size_of_delete;
					break;
				}
				*p_new = *p;
				++p_new;
			}
			if (j == old_matr->rows_of_matr[i].size_of_collums - 1)
			{
				new_matr->rows_of_matr[count_of_rows].elem[0] = size_of_delete;
			}
			++p;
			
		}
		count_of_rows++;
	}

}


int main()
{
	struct matrix matr;
	struct matrix new_matr;
	create_matrix(&matr);
	printf("Old matrix:\n");
	print_matrix(&matr);
	task_function(&matr, &new_matr);
	printf("New matrix:\n");
	print_matrix(&new_matr);
	clear_memory(&matr);
	clear_memory(&new_matr);
	return 0;
}

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
