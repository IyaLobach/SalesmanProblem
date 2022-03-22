#include <iostream>
#include <vector>
using namespace std;

/*!
\file
\brief Header file with class description
*/

/*! \brief Tree node class
*/

class node
{
private:
	node* left; //< left neighbor
	node* right; ///< right neighbor
	node* prev; ///< parent point
	int** matrix; ///< current matrix
	size_t size; ///< matrix size
	int bottom_line; ///< bottom line
	int edge[2]; ///< edge
public:
	/*! node constructor
	*/
	node()
	{
		left = NULL;
		right = NULL;
		prev = NULL;
		size = 0;
		matrix = NULL;
		bottom_line = 0;
		edge[0] = 0;
		edge[1] = 0;
	}
	pair<pair<int, int>, int> cost_estimate();
	void set(int x) { bottom_line += x; }
	void change_matrix(pair<int, int> e, int x, int y);
	int matrix_reduction(const char* x);
	int find_min(const int line, const int col, const char* x);
	void input_matrix(int** m, int n);
	pair<node, node>* creation(pair<pair<int, int>, int>);
	/*! getting matrix size
	\return int size
	*/
	int get_size() { return  size; }
	/*! getting edge
	\return int index
	*/
	int get_edge(int x) { return edge[x]; }
	friend void find_way(vector<pair<int, int>>* v, int** m, int size);
	friend class tree_list;
};

/*! \brief Tree class
*/
class tree_list
{
private:
	node* root; ///< tree root address
public:
	/*! tree constructor
	*/
	tree_list()
	{
		root = NULL;
	}
	/*! tree destructor
	*/
	~tree_list()
	{
		clear(get_root());
		root = NULL;
	}
	void clear(node* cur);
	node* get_root() { return root; }
	node* find_min(node* cur);
	node* min_bottom_line(node* x, node* y);
	void push(node* n, node* cur);
	void push_root(node* n);
	void get_way(node* cur, vector<pair<int, int>>* way);
	void delete_cycle(node* cur);
};

void find_way(vector<pair<int, int>>* v, int** m, int n);
void greedy_method(vector < pair<int, int>>* v, int** m, int n);
void create_graph(vector<pair<int, int>> v1, vector<pair<int, int>> v2, int** a, int size);