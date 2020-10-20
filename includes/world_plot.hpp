#pragma once

#include "graphMat/graph__square_mat_decl.hpp"
#include "includes/declarations.hpp"

class WorldPlot: public Square_Matrix<_box>{
    public:
        bool isPathClear( const Graph_Box<_box>* origin, const directionalPath& path ) const;

    WorldPlot();
    WorldPlot(const WorldPlot&) = delete;
    WorldPlot(const WorldPlot&&) = delete;

};
