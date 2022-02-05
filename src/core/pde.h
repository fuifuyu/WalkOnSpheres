#pragma once

#ifndef PDE_H
#define PDE_H

#include <array>
#include <functional>
#include <exception>
#include <mytypes.h>
#include <boundary.h>

using namespace std;
using namespace MyTypes;

enum class PDETypes : int {
    Laplace, Poisson
};

template<size_t N>
struct PDE{
    PDETypes type;
    PDE(PDETypes type) : type(type){};
    virtual double laplacianOP(arrayd<N> & x, double t) = 0;
    virtual double truth(arrayd<N> & x, double t){throw std::exception("No analytical solution exist for this PDE.");}
};

#endif