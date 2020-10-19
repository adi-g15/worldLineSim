#include <iostream>

#include "graph_mat.hpp"


using namespace std;

int main(int argc, char const *argv[])
{
    Graph_Matrix<int> mat(6,6);
    // Graph_Matrix<int> mat;

    mat.displayMat();    

    return 0;
}
