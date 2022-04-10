#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include < stdio.h >
#include<io.h>
#include <fstream>
#include <stdarg.h>

// SO LOG DEFINITION 
#define N 20
#define N2 200


// MAX FIle Size 
#define MAX_FILE_NAME_SIZE 255
#define FILE_EXTENTION_TXT ".txt"
#define FILE_EXTENTION_BIN ".bin"


#define TEST 1

using namespace std;


struct strings {
	void* f;// Pointer on valid file structure

	int len;
	int pos;

	int operator > (strings& right)
	{
		if (len > right.len) return 1;
		return 0;
	}
	int operator < (strings& right)
	{
		if (len < right.len) return 1;
		return 0;
	}
	strings& operator = (strings& c)
	{
		len = c.len;
		//???
		//strcpy(f->name, c.f->name);

		return *this;
	}
};


// Struct that story all information about file and work wit file
struct input_file {

	char name[MAX_FILE_NAME_SIZE];
	FILE* f; // Pointer on file

	long current_pos;

	int open(const char* name) {

		// Check input len of string here
		// if strlen(name) 

		strcpy(this->name, name);
		// error
		f = fopen(name, "r");
		if (f == NULL)
			// File is not exists
			return -1;
		// Other errors can be checked here 
		return 0;
	}
	int set_position_to_begin_file()
	{
		this->current_pos = 0;
		fseek(f, 0, SEEK_SET);
		return 0;
	}

	//	
	int read_line(strings* strs, int max_num)
	{

		if (strstr(this->name, FILE_EXTENTION_TXT))
		{
			read_line(strs, max_num, (long*)&this->current_pos);
		}
		else if (strstr(this->name, FILE_EXTENTION_BIN))
		{
			read_line(strs, max_num, (int*)&this->current_pos);
		}
		else
			// ERROR We do not support such kind file
			return -1;

		return 0;
	}

	//
	int read_line(strings* strs, int max_num, long* pos)
	{
		char ps[N2];
		int len = 0;

		if (fgets(ps, sizeof(ps), this->f) == NULL)
			return -2; //EOF

		len = strlen(ps);
		{
			strs->len = len;
			strs->pos = *pos;
			strs->f = (void*)this;
			*pos += len;
		}

		return 0;
	}

	int read_line(strings* strs, int max_num, int* pos)
	{
		//char ps[N2];
		int d = 0;

		if (feof(this->f))
			return -2; //EOF

		int len = fread(&d, sizeof(d), 1, f);
		if (len <= 0)
			return -2; //EOF

		strs->len = d; // converting liitle/big ending
		strs->pos = *pos;
		strs->f = (void*)this;

		*pos += (d + sizeof(d));
		fseek(f, d * sizeof(char), SEEK_CUR);

		return 0;
	}


	void close()
	{
		if (f != NULL)
			fclose(f);
		else
			; // Exception here , someting wrong

	}
};





/*char Fill_str(strings* b, int& n, char f_name[N], char p)
{
	FILE* f;
	char* ps;
	ps = (char*)malloc(N2 * sizeof(char));
	int pos = 0;
	if ((f = fopen(f_name, "r")) == NULL) puts("Ошибка загрузки"); // Открываем файл для чтения
	else
	{
		while (!feof(f))
		{
			fgets(ps, sizeof(ps), f);
			b[n].len = strlen(ps);
			b[n].pos = pos;
			pos++;
			n++;
		}
	}
		fclose(f); // Закрываем файл
		return p;
}

int Fill_str(strings * b, int& n, char f_name[N], int p)
	{
	//char f_name[N];
	FILE* f;
	int pos = 0;
	if ((f = fopen(f_name, "rb")) == NULL) puts("Ошибка загрузки"); // Открываем файл для чтения
	else
	{
		int d;
		int pos = 0;
		while (!feof(f))
		{
			fread(&d, sizeof(d), 1, f);
			b[n].len = d;
			b[n].pos = pos;
			fseek(f, d * sizeof(char), 1);
			n++;
			pos++;
		}
		fclose(f);
	}
	return 0;
}*/

// Input strucure 
int Init_strs(strings* strs, int* strs_num, int n_args, ...)
{
	char c = NULL;
	int a = 0;
	int n = 0;

	va_list ap;
	va_start(ap, n_args);


	for (int i = 1; i <= n_args; i++) {
		// Why is used int here ???? 
		input_file* in_file = (input_file*)va_arg(ap, int);
		if (in_file)
		{
			in_file->set_position_to_begin_file();
			int res = 0;
			do {
				res = in_file->read_line(strs + n, N);
				if (res >= 0)
					n++;
			} while (res == 0);
		}
	}

	if (strs_num)
		*strs_num = n;

	va_end(ap);

	return 0;

}

void Print_strs(strings* strs, int strs_num)
{
	printf("Strs count : %i \n", strs_num);
	for (int i = 0; i < strs_num; i++)
		printf("Strs[%d] len:%d pos:%d file:%x\n", i, strs[i].len, strs[i].pos, (unsigned int)strs[i].f);
}

// File  
int Add_New_file(char* filename)
{
	printf("Введите название файла: ");
	scanf("%s", filename);


	// does file exist and have the correct actirbutes    
	// https://docs.microsoft.com/ru-ru/cpp/c-runtime-library/reference/access-waccess?view=msvc-170
	//if (_access(filename, 0) != 0)
	//	return -1;

	// TODO
	//if !FILE_EXTENTION_TXT and !FILE_EXTENTION_BIN
	//	return -2;

	return 0;
}

void Menu()
{
	strings* str = (strings*)malloc(N2 * sizeof(strings));
	memset(str, 0, N2 * sizeof(strings));
	int str_counter = 0;
	char c;
	const char* ss[] = { "0 - добавить новый файл", "1 - Запустить сортировку ", "2 - ВЫХОД" };
	int i, k = sizeof(ss) / sizeof(ss[0]);
	input_file* files = (input_file*)malloc(sizeof(input_file));
	int n_files = 0;
	// Menu


#ifdef TEST	
	n_files = 2;
	files = (input_file*)realloc(files, (n_files + 1) * sizeof(input_file));
	files[1].open("1t_t.txt");
	files[0].open("1t.bin");
#endif


	for (;;)
	{
		for (i = 0; i < k; i++) puts(ss[i]);
		c = _getch(); // выбор действия
		if (c == '0') {
			// 
			char file_name[MAX_FILE_NAME_SIZE];
			if (Add_New_file(file_name) >= 0)
			{
				if (n_files >= 1)
				{
					files = (input_file*)realloc(files, (n_files + 1) * sizeof(input_file));
					if (!files) printf("Allocation error.");
				}

				files[n_files].open(file_name);
				n_files++;
			}
		}
		if (c == '1')
		{
			// n_files
			// I do not know  how reuse ...
			// Ask an user how many files he wants to handle 			
			if (n_files == 3)
				Init_strs(str, &str_counter, n_files, files, files + 1, files + 2);
			if (n_files == 2)
				Init_strs(str, &str_counter, n_files, files, files + 1);
			// Check the result
			Print_strs(str, str_counter);

			//Final_Sort();
		}

		if (c == '2') return;
	}



	// Free memory and close all files
	for (int i = 0; i < n_files; i++)
		files[i].close();
	free(files);
	//
}


int main()
{
	//setlocale(LC_ALL, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("color F3");
	Menu();
}