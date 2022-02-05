#pragma once 

#ifndef BOX_BOUNDARY_H
#define BOX_BOUNDARY_H

#include <mytypes.h>
#include "boundary.h"

using namespace MyTypes;


struct BoxBoundary2D : Boundary<2>{
    BoxBoundary2D(function<double(arrayd<2> const&)> cond, arrayd<2> & bounds, double error)
    :Boundary(
        cond,
        [bounds, error](arrayd<2> const &x){ //meet funciton
            if (x[0] < bounds[0] + error || x[0] > bounds[1] - error) return true;
            if (x[1] < bounds[0] + error || x[1] > bounds[1] - error) return true;
            return false;
        }
    ){};
};

struct WosBoxBoundary2D : WosBoundary<2>{
    WosBoxBoundary2D(function<double(arrayd<2> const&)> cond, arrayd<2> & bounds, double error)
    : WosBoundary(
        BoxBoundary2D(cond,bounds,error),
        [bounds](arrayd<2> const &x){
            double x_dist = x[0] - bounds[0] < bounds[1] - x[0] ? x[0] - bounds[0] : bounds[1] - x[0];
            double y_dist = x[1] - bounds[0] < bounds[1] - x[1] ? x[1] - bounds[0] : bounds[1] - x[1];
            return x_dist < y_dist ? x_dist : y_dist;
        }
    ){};
};

#endif