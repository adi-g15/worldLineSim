#include <iostream>

#include "graph__square_mat.hpp"


using namespace std;

// LEARNT - make_unsigned_t<> is just make_unsigned<>::type... that was why it doesn't define any operators, make_unsigned is always a struct, while make_unsigned_t is not
typedef std::make_unsigned<int32_t>::type udimen;

int main(int argc, char const *argv[])
{
    udimen a = 34;
    if( a == -2 ){
        cout<<"Error"<<endl;
    }else cout<<"Fine"<<endl;


    // int init_order{15}, final_order{0};
    // Square_Matrix<int> mat(init_order);
    // mat.displayMat();
    // mat.resize(final_order);
    // mat.displayMat();

    int init_rows{4}, init_cols{8}, final_rows{3}, final_cols{15};
    Graph_Matrix<int> mat(init_rows,init_cols);
    mat.displayMat();
    mat.resize(final_rows, final_cols);
    mat.displayMat();


    return 0;
}
