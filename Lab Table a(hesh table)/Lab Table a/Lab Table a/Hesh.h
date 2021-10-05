#define size_key  3
#define SIZE 3

int h2(int k) 
{
	int a[10] = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 23};
	return a[k % SIZE];
}

int h1(int k) 
{
	return (k % SIZE);
}
int h(int k, int i, int sizeks2) 
{
	return (h1(k) + i*h2(k)) % sizeks2;
}
