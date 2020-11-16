#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
#include <thread>
#include <chrono>
#include <stack>

#define startCHAR 'S'
#define endCHAR 'E'
#define blockCHAR '#'

using namespace std;

struct box{
    bool isEnd{ false };
    bool isBlocked{ false };
    bool isVisited{ false };
};

struct dungeon{
    typedef uint32_t _type;

    vector<vector<box>> board;
    _type R, C;
    pair<_type, _type> start;

    public:
    void reset_visited(){
        this->x_finalPath.clear();
        this->y_finalPath.clear();
        for( auto i = 0; i < R; i++ )
            for( auto j = 0; j < C; ++j )
                this->board[i][j].isVisited = false;
    }

    void printDungeon(){
        return;
        cout << '\n';
        for( auto i = 0; i < R; i++ ){
            for( auto j = 0; j < C; ++j ){
                if( this->board[i][j].isBlocked ) cout << '#';
                else if( this->board[i][j].isEnd ) cout << 'E';
                else if( this->start.first == i && this->start.second == j ) cout << 'S';
                else if( this->board[i][j].isVisited ) cout << '+';
                else cout << ' ';
                cout << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }

    dungeon(_type R, _type C, istream& stream) : board{ R, vector<box>(C) }{
        char c;
        this->R = R;
        this->C = C;

        for( auto i = 0; i < R; i++ ){
            for( auto j = 0; j < C; ++j ){
                cin >> c;
                if( c == blockCHAR )    this->board[i][j].isBlocked = true;
                else if( c == endCHAR ) this->board[i][j].isEnd = true;
                else if( c == startCHAR )   this->start = { i,j };
            }
        }
    }

    bool canVisit(_type x, _type y){
        return !(x < 0 || y < 0 || x >= R || y >= C) && !(this->board[x][y].isVisited || this->board[x][y].isBlocked);
    }

    short dir_x[4]{ 1,-1,0,0 };
    short dir_y[4]{ 0,0,1,-1 };

    vector<_type> x_finalPath;
    vector<_type> y_finalPath;

    bool rec(_type x, _type y){
        this->board[x][y].isVisited = true;
 
        // this_thread::sleep_for(std::chrono::milliseconds(100));
        this->printDungeon();
        if( this->board[x][y].isEnd ) return true;

        for( short i = 0; i < 4; ++i ){
            if( canVisit(x + dir_x[i], y + dir_y[i])
                && rec(x + dir_x[i], y + dir_y[i]) ){
                this->x_finalPath.push_back(x + dir_x[i]);
                this->x_finalPath.push_back(y + dir_y[i]);
                return true;
            }
        }

        return false;
    }

    void printPath(){
        return;
        cout << '\n';
        for( auto i = 0; i < R; i++ ){
            for( auto j = 0; j < C; ++j ){
                if( this->board[i][j].isBlocked ) cout << '#';
                else if( this->board[i][j].isEnd ) cout << 'E';
                else if( this->start.first == i && this->start.second == j ) cout << 'S';
                else if( find(x_finalPath.begin(), x_finalPath.end(), i) != x_finalPath.end() && 
                        find(y_finalPath.begin(), y_finalPath.end(), j) != y_finalPath.end()
                 ) cout << '+';
                else cout << ' ';
                cout << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }

    auto dfs(){
        stack<_type> x_stack;
        stack<_type> y_stack;

        x_stack.push(start.first);
        y_stack.push(start.second);

        int count = 0;

        while ( ! x_stack.empty() )
        {
            for(int i=0; i<x_stack.size(); ++i){
                
            }
        }
        


    }

    auto bfs(){
        queue<_type> x_queue;
        queue<_type> y_queue;

        x_queue.push(start.first);
        y_queue.push(start.second);

        long last_layer_length = 1;
        long prev_layer_length = 1;
        long curr_layer_processod = 0;

        _type curr_x, curr_y;
        long count = 0;
        bool isFinished{ false };
        while ( ! x_queue.empty() )
        {
            printDungeon();

            last_layer_length = 0;
            for(int n=0; n<prev_layer_length; ++n){
                curr_x = x_queue.front();
                curr_y = y_queue.front();
                board[curr_x][curr_y].isVisited = true;

                if( board[curr_x][curr_y].isEnd ){
                    isFinished = true;
                    break;
                }

                for(int i=0; i < 4; ++i){
                    if( canVisit(curr_x + dir_x[i], curr_y + dir_y[i]) ){
                        ++last_layer_length;

                        x_queue.push(curr_x + dir_x[i]);
                        y_queue.push(curr_y + dir_y[i]);
                    }
                }

                x_queue.pop();
                y_queue.pop();
            }
            if(isFinished) break;
            ++count;
            prev_layer_length = last_layer_length;

            // ++curr_layer_processod;
            // if(curr_layer_processod == last_layer_length){
                // ++count;
            //     curr_layer_processod = 0;
            //     last_layer_length = 0;
            // }
        //     x_queue.pop();
        //     y_queue.pop();
        }

        // cout << "Took " << count << "steps\n";

    }

    _type shortestPath(_type x1, _type y1, _type x2, _type y2){
        _type moved = 0;

        this->reset_visited();

        queue<_type> x_queue;
        queue<_type> y_queue;

        x_queue.push(this->start.first);
        y_queue.push(this->start.second);

        rec(start.first, start.second);
        printDungeon();

        // if( std::any_of(this->board.begin(), this->board.end(), [](const vector<box>& v){
        //     return std::any_of(v.begin(), v.end(), [](const box& b){
        //         return !b.isVisited || b.isBlocked || b.isEnd;
        //     });
        // }) ){
        //     cout << "Not all elements were visited";
        // }

        return 0;
    }
    ~dungeon(){}
};

int main(){
    uint32_t R, C;
    cin >> R >> C;

    auto beg = std::chrono::high_resolution_clock::now();
    dungeon b(R, C, cin);
    // b.shortestPath(0, 0, 0, 0);
    b.bfs();
    // b.printPath();

    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end-beg;
    cout<<"time: "<< diff.count()<<endl;

    // delete b;

}