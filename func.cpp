#include <iostream>
#include "class.h"
#include <cstring>
#include <vector>
#include <fstream>
#include <ctime>


using namespace std;
int s;


/*!
\file
\brief cpp file with function definition
*/


/*! writing a matrix to the current element
\param int**m - the matrix is written
\param int n - size of this matrix
\return void
*/

void node::input_matrix(int** m, int n)
{
	size = n;
	matrix = new int* [n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[n];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matrix[i][j] = m[i][j];
		}
	}
}

/*! finding minimums for zero elements
\param int line
\param int col - current line or column of zero element, 
\param char* x - indicates a search in line or column
\return int minimum
*/

int node::find_min(const int line, const int col, const char* x)
{
	int min = INT_MAX;
	for (int i = 1; i < size; i++)
	{
		if (strcmp(x, "line") == 0)
		{
			if ((i != col) && (matrix[line][i] < min))
			{
				min = matrix[line][i];
			}
		}
		else
		{
			if ((i != line) && (matrix[i][col] < min))
			{
				min = matrix[i][col];
			}
		}
	}
	return min;
}

/*! reduction of matrix
\param char* x - indicates a search in line or column
\return int sum of minimums by line or column
*/

int node::matrix_reduction(const char* x) 
{
	int sum_of_min = 0;
	vector<int> v(size - 1);
	for (int i = 1; i < size; i++)
	{
		v[i - 1] = INT_MAX;
		for (int j = 1; j < size; j++)
		{
			if (strcmp(x, "line") == 0)
			{
				if (matrix[i][j] < v[i - 1])
					v[i - 1] = matrix[i][j];

			}
			else
			{
				if (matrix[j][i] < v[i - 1])
					v[i - 1] = matrix[j][i];
			}
		}
	}
	for (int i = 1; i < size; i++)
	{
		for (int j = 1; j < size; j++)
		{
			if (strcmp(x, "line") == 0)
			{
				if (matrix[i][j] != INT_MAX)
					matrix[i][j] -= v[i - 1];
			}
			else
			{
				if (matrix[j][i] != INT_MAX)
					matrix[j][i] -= v[i - 1];
			}
		}
	}
	for (int i = 0; i < size - 1; i++)
		sum_of_min += v[i];
	v.clear();
	return sum_of_min;
}


/*! cost estimate of zero elements
\return pair<pai<int line of maximum element, int column of maximum element>, int cost>
*/

pair<pair<int, int>, int> node::cost_estimate()
{
	pair<pair<int, int>, int> p;
	p.second = INT_MIN;
	int cost;
	for (int i = 1; i < size; i++)
	{
		for (int j = 1; j < size; j++)
		{
			cost = 0;
			if (matrix[i][j] == 0)
			{
				cost = find_min(i, j, "line");
				int col_cost = find_min(i, j, "col");
				if ((cost != INT_MAX) && (col_cost != INT_MAX))
				{
					cost += col_cost;
				}
				if (cost >= p.second)
				{
					p.second = cost;
					p.first.first = i;
					p.first.second = j;
				}
			}
		}
	}
	return p;
}

/*! matrix plotting
\param pair<int,int> e -  edge to cross out
\param int x - line to cross out
\param int y - columt to cross out
\return void
*/
void node::change_matrix(pair<int, int> e, int x, int y)
{
	for (int j = e.second; j < size - 1; ++j) {
		for (int i = 0; i < size; ++i)
			matrix[i][j] = matrix[i][j + 1];
	}

	for (int i = e.first; i < size - 1; ++i) {
		for (int j = 0; j < size - 1; ++j)
			matrix[i][j] = matrix[i + 1][j];
	}

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - 1; j++)
		{
			if ((matrix[0][j] == x) && (matrix[i][0] == y) && (size - 1 != 2))
				matrix[i][j] = INT_MAX;
		}
	}
}

/*! creation of new on and off nodes
\param pair<pait<int,int>,int> e - edge to create nodes with "off-cost"
\return pair<node on, node off>*
*/
pair<node, node>* node::creation(pair<pair<int, int>, int> e)
{
	node new_elem_on;
	new_elem_on.input_matrix(matrix, s);
	new_elem_on.change_matrix(e.first, matrix[e.first.first][0], matrix[0][e.first.second]);
	new_elem_on.size = size - 1;

	node new_elem_off;
	new_elem_off.input_matrix(matrix, s);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if ((new_elem_off.matrix[0][j] == matrix[0][e.first.second]) && (new_elem_off.matrix[i][0] == matrix[e.first.first][0]))
				new_elem_off.matrix[i][j] = INT_MAX;
		}
	}
	new_elem_off.size = size;
	new_elem_off.edge[0] = -1 * matrix[e.first.first][0];
	new_elem_off.edge[1] = -1 * matrix[0][e.first.second];
	if (e.second != INT_MAX)
		new_elem_off.bottom_line = bottom_line + e.second;
	else
		new_elem_off.bottom_line = INT_MAX;

	new_elem_on.bottom_line += new_elem_on.matrix_reduction("line");
	new_elem_on.bottom_line += new_elem_on.matrix_reduction("col");
	new_elem_on.bottom_line += bottom_line;
	new_elem_on.edge[0] = matrix[e.first.first][0];
	new_elem_on.edge[1] = matrix[0][e.first.second];
	pair<node, node>* new_elem = new pair<node, node>;
	new_elem->first = new_elem_on;
	new_elem->second = new_elem_off;
	return new_elem;
}

/*! clear tree
\param node* cur - current node point
\return void
*/
void tree_list::clear(node* cur)
{
	if ((cur->left == NULL) && (cur->right == NULL))
	{
		for (int i = 0; i < cur->size; i++)
			delete[] cur->matrix[i];
		delete[] cur->matrix;
	}
	else
	{
		clear(cur->left);
		clear(cur->right);
	}
}

/*! comparing two nodes and finding minimal one
\param node* x - first node point
\param node* y - second node point
\return node* the smallest node point
*/
node* tree_list::min_bottom_line(node* x, node* y)
{
	if (x->bottom_line <= y->bottom_line)
		return x;
	return y;
}

/*! finding the smallest node in the tree
\param node* cur - current node point
\return node* the smallest node in the tree point
*/
node* tree_list::find_min(node* cur)
{
	if ((cur->left == NULL) && (cur->right == NULL))
		return cur;
	else
		return min_bottom_line(find_min(cur->left), find_min(cur->right));
}

/*! pushing node to the tree
\param node* n - add node point
\param node* cur - current node point
\return void
*/
void tree_list::push(node* n, node* cur)
{
	if (n->edge[0] < 0)
		cur->right = n;

	else
		cur->left = n;
	n->prev = cur;
}

/*! pushing root to the tree
\param node* n - root point
\return void
*/
void tree_list::push_root(node* n)
{
	root = n;
}

/*! getting way using the branch and bound algorithm
\param node* cur - current node point
\param vector<pair<int,int>>* way - final way of edges point
\return void
*/
void tree_list::get_way(node* cur, vector<pair<int, int>>* way)
{
	pair <int, int> p;
	if (cur->prev != NULL)
	{
		if (cur->get_edge(0) >= 0)
		{
			p.first = cur->get_edge(0);
			p.second = cur->get_edge(1);
			way->push_back(p);
		}
		cur = cur->prev;
		get_way(cur, way);
	}
}
/*! finding and deleting a cycle
\param node*cur - current node
\return pair<vertex 1, vertex 2> for deleting
*/

void tree_list::delete_cycle(node* cur)
{
	vector<pair<int, int>> w;
	node* n = cur;
	get_way(cur->prev, &w);
	vector<pair<int, int>>::iterator it = w.begin();
	it = w.begin();
	for (; it < w.end(); ++it)
	{
		if (cur->edge[1] == it->first)
		{
			for (int i = 0; i < cur->size; i++)			
			{
				for (int j = 0; j < cur->size; j++)
					if ((cur->matrix[i][0] == it->second) && (cur->matrix[0][j] == cur->edge[0]))
						cur->matrix[i][j] = INT_MAX;
			}
			w.clear();
			return;
		}
		if (cur->edge[0] == it->second)
		{
			for (int i = 0; i < cur->size; i++)
			{
				for (int j = 0; j < cur->size; j++)
					if ((cur->matrix[i][0] == cur->edge[1]) && (cur->matrix[0][j] == it->first))
						cur->matrix[i][j] = INT_MAX;
			}
			w.clear();
			return;
		}
	}
	return;
}

/*! branch and bound algorithm
\param vector<pair<int,int>>* way - final way of edges point
\param int** m - input cost matrix, 
\param int size - size of matrix
\return void
*/
void find_way(vector<pair<int, int>>* way, int** m, int size)
{
	s = size;
	tree_list l;
	node a;
	a.input_matrix(m, size);
	node* cur = &a;
	cur->set(cur->matrix_reduction("line"));
	cur->set(cur->matrix_reduction("col"));
	l.push_root(cur);
	cur = l.find_min(l.get_root());
	while (cur->get_size() != 2)
	{
		if (cur->get_edge(0) < 0)
		{
			cur->matrix_reduction("line");
			cur->matrix_reduction("col");
		}
		pair<node, node>* elem = cur->creation(cur->cost_estimate());
		l.push(&elem->first, cur);
		l.delete_cycle(&elem->first);
		l.push(&elem->second, cur);
		cur = l.find_min(l.get_root());
	}
	l.get_way(cur, way);
	way->push_back(make_pair(cur->matrix[1][0], cur->matrix[0][1]));
	cout << "BRUNCH AND BOUND" << endl;
	cout << cur->bottom_line << endl;
	vector<pair<int, int>>::iterator it = way->begin();
	for (; it < way->end(); ++it)
		cout << it->first << " " << it->second << endl;
}

/*! greedy algorithm
\param vector<pair<int, int>>* v - final way of edges point
\param int** m - input cost matrix
\param int size - size of matrix
\param int pos - current position
\param int cost - current minimum of cost
\param int* visites - visited vertex point
\return void
*/
void find_greedy_way(vector<pair<int, int>>* v, int** m, int size, int pos, int cost, int* visited)
{
	if (v->size() == size - 2)
	{
		cost += m[pos][1];
		v->push_back(make_pair(pos, 1));
		cout << "GREEDY" << endl;
		cout << cost << endl;
		vector<pair<int, int>>::iterator it = v->begin();
		for (; it < v->end(); ++it)
			cout << it->first << " " << it->second << endl;

	}
	else
	{
		visited[pos - 1] = 1;
		int min = INT_MAX;
		int min_index = 0;
		for (int i = 1; i < size; i++)
		{
			if ((m[pos][i] < min) && (visited[i - 1] == 0))
			{
				min = m[pos][i];
				min_index = i;
			}
		}
		cost += min;
		v->push_back(make_pair(pos, min_index));
		find_greedy_way(v, m, size, min_index, cost, visited);
	}
}
/*! greedy algorithm call
\param vector<pair<int, int>>* v - path vector
\param int** m - input cost matrix
\param int n - size of matrix
\return void
*/
void greedy_method(vector<pair<int, int>>* v, int** m, int n)
{
	int* visited = new int[n - 1];
	for (int i = 0; i < n; i++)
		visited[i] = 0;
	find_greedy_way(v, m, n, 1, 0, visited);
}

/*! identifier of the occurrence of an edge in one of the algorithm
\param vector<pair<int, int>> v1 - finel way of algorithm
\param int i - first vertex of edge
\param int j - second vertex of edge
\return bool identifier
*/
bool find_result(vector<pair<int, int>> v1, int i, int j)
{
	for (int k = 0; k < v1.size(); k++)
	{
		if ((v1[k].first == i) && (v1[k].second == j))
			return 1;
	}
	return 0;
}
/*! graph creation (green - branch and bound algorithm, blue - greedy algorithm)
\param vector<pair<int, int>> v1 - branch and bound algorithm, 
\param vector<pair<int, int>> v2 - greedy algorithm
\param int** a - cost matrix
\param int size - matrix size
\return void
*/

void create_graph(vector<pair<int, int>> v1, vector<pair<int, int>> v2, int** a, int size)
{
	ofstream f("Result.dot");
	if (f.is_open())
	{
		f << "digraph Way {" << endl;
		for (int i = 1; i < size; i++)
		{
			for (int j = 1; j < size; j++)
			{
				if (a[i][j] != INT_MAX)
				{
					if (find_result(v1, i, j))
						f << i << "->" << j << "[label = " << a[i][j] << ", color = \"green\"];" << endl;
					if (find_result(v2, i, j))
						f << i << "->" << j << "[label = " << a[i][j] << ", color = \"blue\"];" << endl;
					if ((!find_result(v1, i, j)) && (!find_result(v2, i, j)))
						f << i << "->" << j << "[label = " << a[i][j] << "];" << endl;

				}
			}
		}
		f << "}";
	}
	f.close();
}



