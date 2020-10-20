#include "world_plot.hpp"

bool WorldPlot::isPathClear( const Graph_Box<_box>* origin, const directionalPath& path ) const{
    const Graph_Box<_box> *temp{ origin };

    for ( uint32_t i = 0; i<path.size(); ++i ){
        temp = temp->get_adj_box(path[i]);
        if( ! temp->getData().entities.empty() )    return false;
    }
    
    return true;
}

WorldPlot::WorldPlot(){
    
}
