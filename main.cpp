#include <iostream>
#include "class.h"
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

/*!
\file
\brief cpp file with algotithm call
*/


int main()
{
	ifstream f("Data.txt");
	if (f.is_open())
	{
		char* tmp = new char[400];
		char* begin = tmp;
		while (!f.eof())
		{
			*tmp = f.get();
			tmp++;
		}
		tmp--;
		*tmp = '\0';
		f.close();
		tmp = begin;
		int count_vertex = 0;
		for (int i = 0; i < strlen(begin); i++)
		{
			if ((tmp[i] >= '0') && (tmp[i] <= '9'))
				count_vertex++;
			if (tmp[i] == '\n')
				break;
		}

		int** a = new int* [count_vertex];
		for (int i = 0; i < count_vertex; i++)
			a[i] = new int[count_vertex];

		for (int i = 0; i < count_vertex; i++)
		{
			for (int j = 0; j < count_vertex; j++)
				a[i][j] = 0;
		}

		int row = 0, col = 0, length = 0;
		while (*begin != '\0')
		{
			if ((*begin >= '0') && (*begin <= '9'))
				length++;
			if (*begin == 'I')
				a[row][col] = INT_MAX;
			if ((*begin == ' ') || (*begin == '\n'))
			{
				for (int i = length; i > 0; i--)
					a[row][col] += ((*(begin - i) - '0') * pow(10, i - 1));
				col++;
				length = 0;
			}
			if (*begin == '\n')
			{
				row++;
				col = 0;
				length = 0;
			}
			if ((row == count_vertex) && (col == count_vertex))
				break;
			begin++;
		}
		vector<pair<int, int>> v1;
		vector<pair<int, int>> v2;
		clock_t start = clock();
		find_way(&v1, a, count_vertex);
		clock_t finish = clock();
		cout << "Time for the first " << (double)(finish - start) / CLK_TCK * 1000 << "mls" << endl;
		start = clock();
		greedy_method(&v2, a, count_vertex);
		finish = clock();
		cout << "Time for the second " << (double)(finish - start) / CLK_TCK * 1000 << "mls" << endl;
		create_graph(v1, v2, a, count_vertex);
		delete[] tmp;
		delete[] a;
	}
	return 0;
}














