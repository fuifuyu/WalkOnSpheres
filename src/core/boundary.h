#pragma once

#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <functional>
#include <mytypes.h>

using namespace std;
using namespace MyTypes;

template <size_t N>
struct Boundary{
    Boundary(
        function<double(arrayd<N> const&)> cond,
        function<bool(arrayd<N> &)> meet
    ) : cond(cond), meet(meet){};
    
    function<double(arrayd<N> &)> cond;
    function<bool(arrayd<N> &)> meet;
};

template<size_t N>
struct WosBoundary {
    WosBoundary(
        Boundary<N> boundary,
        function<double(arrayd<N> &)> distFn
    ) : boundary(boundary), minDistFromBoundary(distFn){};
    WosBoundary(
        function<double(arrayd<N> &)> cond,
        function<bool(arrayd<N> &)> meet,
        function<double(arrayd<N> &)> distFn
    ) : boundary(Boundary<N>(cond,meet)), minDistFromBoundary(distFn){};

    operator Boundary<N>() const {return boundary;}
    Boundary<N> boundary;
    function<double(arrayd<N> &)> minDistFromBoundary;

    double cond(arrayd<N> &x){ return boundary.cond(x);}
    bool meet(arrayd<N> &x){ return boundary.meet(x);}
};

#endif