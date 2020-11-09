#include <iostream>

#include "graph__square_mat.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
	int init_rows{4}, init_cols{8}, final_rows{3}, final_cols{15};
	Graph_Matrix<int> mat(init_rows,init_cols);
	mat.displayMat();
	mat.resize(final_rows, final_cols);
	mat.displayMat();

	return 0;
}
