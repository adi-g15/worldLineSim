#pragma once

#include "graph_mat_decl.hpp"
#include <queue>

template< typename node_dtype, typename dimen_t = int32_t >
class Square_Matrix: public Graph_Matrix<node_dtype, dimen_t>{
	public:
		typedef coord coord_type;
		typedef Graph_Box<node_dtype> graph_node;
		dimen_t getOrder() const{  //order of the matrix
			return this->_total_y_abs;
		}
		void resize(dimen_t new_order){
			Graph_Matrix<node_dtype, dimen_t>::resize(new_order, new_order);
		}

		dimen_t getShortestDistance( graph_node* start, graph_node* end ){
			if( start == end )	return 0;

			dimen_t last_layer_depth{1};
			dimen_t curr_layer_depth{0};

			dimen_t diff{0};

			std::queue< graph_node* > queue;
			queue.push(start);

			while ( ! queue.empty() )
			{
				curr_layer_depth = 0;

				while ( last_layer_depth-- )
				{
					if(queue.front() == end)	return diff;

					if( queue.front()->up ){
						queue.push( queue.front()->up );
						++curr_layer_depth;
					}
					if( queue.front()->left ){
						queue.push( queue.front()->left );
						++curr_layer_depth;
					}
					if( queue.front()->right ){
						queue.push( queue.front()->right );
						++curr_layer_depth;
					}
					if( queue.front()->down ){
						queue.push( queue.front()->down );
						++curr_layer_depth;
					}

					queue.pop();
				}

				++diff;	// entering the next layer, so moved 1 unit
				last_layer_depth = curr_layer_depth;
			}

		}

		dimen_t getShortestPath( coord_type& start, coord_type& end ){
			
		}

	Square_Matrix(): Square_Matrix(1){}
	Square_Matrix(dimen_t order): Graph_Matrix<node_dtype, dimen_t>(order, order){

	}

};
