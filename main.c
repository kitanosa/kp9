#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Key
{
	float number;
} Key;

typedef struct _Row
{
	Key _key;
	char _str[81];
} Row;


int check_sort(const Key *k1, const Key *k2)
{
	return k1->number <= k2->number;
}

int isEqualKeys(const Key *k1, const Key *k2)
{
	return k1->number == k2->number;
}
void swapRows(Row *r1, Row *r2)
{
	Row tmp;

	tmp = *r1;
	*r1 = *r2;
	*r2 = tmp;
}


void printTable(const Row *a, const int size)
{
	int i;

	printf("+---------+------------------------------------------------+\n");
	printf("|   Ключ  |                    Значение                    |\n");
	printf("+---------+------------------------------------------------+\n");

	for (i = 0; i < size; i++)
		printf("|    %4f|%48s|\n",a[i]._key.number, a[i]._str);

	printf("+---------+------------------------------------------------+\n");
}

int binSearch(const Row *arr, const int size, const Key *key)
{
	int start = 0, end = size - 1, mid;

	if (size <= 0)
		return -1;

	while (start < end)
	{
		mid = start + (end - start) / 2;

		if (isEqualKeys(&arr[mid]._key, key))
			return mid;
		else if (check_sort(&arr[mid]._key, key))
			start = mid + 1;
		else
			end = mid;
	}

	if (isEqualKeys(&arr[end]._key, key))
		return end;

	return -1;
}

void sort(Row *arr, const int size)
{
	for (int i = 1; i < size; i++)
    {
        int low = 0;
        int high = i - 1;
        int temp = arr[i]._key.number;
        //Find
        while (low <= high)
        {
            int mid = (low + high) / 2;
            if (temp < arr[mid]._key.number)
                high = mid - 1;
            else
                low = mid + 1;
        }
        //backward shift
        for (int j = i - 1; j >= low; j--){
            arr[j + 1]._key.number = arr[j]._key.number;
            swapRows(&arr[j], &arr[j + 1]);
        }
        arr[low]._key.number = temp;
    }
}


void getRow(FILE *stream, char *str, const int size)
{
	int rows = 0, ch;

	while ((ch = getc(stream)) != '\n' && rows < size - 1)
		str[rows++] = ch;

	str[rows] = '\0';
}


int isSorted(const Row *a, const int size)
{
	int i;

	for (i = 0; i < size - 1; i++)
		if (!check_sort(&a[i]._key, &a[i + 1]._key))
			return 0;

	return 1;
}

int main(void)
{
	const int N = 50;
	int i, rows, action;
	char ch;
	Row arr[N];// массив из строк
	Key key;
	FILE *file = fopen("text.txt", "r");

	if (file == NULL)
	{
		printf("Ошибка при открытии файла\n");

		return 0;
	}

	i = 0;

	while (i < N && fscanf(file, "%f",&arr[i]._key.number) == 1)
	{
		fscanf(file, "%c", &ch);
		getRow(file, arr[i]._str, sizeof(arr[i]._str));

		i++;
	}// i это количество строк

	fclose(file);

	rows = i;

	do
	{
		printf("Меню\n");
		printf("1) Печать\n");
		printf("2) Двоичный поиск\n");
		printf("3) Сортировка\n");
		printf("4) Выход\n");
		printf("Выберите действие\n");
		scanf("%d", &action);

		switch (action)
		{
			case 1:
			{
				printTable(arr, rows);
			}
			break;

			case 2:
			{
				if (!isSorted(arr, rows))
					printf("Ошибка. Таблица не отсортирована\n");
				else
				{
					printf("Введите ключ: ");
					scanf("%f",&key.number);

					i = binSearch(arr, rows, &key);

					if (i > -1)
						printf("Найдена строка: %s\n", arr[i]._str);
					else
						printf("Строка с таким ключом не найдена\n");
				}
			}
			break;

			case 3:
			{
				sort(arr, rows);
			}
			break;



			case 4: break;

			default:
			{
				printf("Ошибка. Такого пункта меню не существует\n");
			}
			break;
		}
	}
	while (action != 4);

	return 0;
}

