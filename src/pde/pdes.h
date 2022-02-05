#pragma once

#ifndef PDES_H
#define PDES_H

#include "pde.h"

struct TaylorVortexPDE2 : PDE<2>{
    TaylorVortexPDE2():PDE(PDETypes::Poisson){};

    virtual double laplacianOP(arrayd<2> &x, double t) override;
    virtual double truth(arrayd<2> &x, double t) override;
};

#endif