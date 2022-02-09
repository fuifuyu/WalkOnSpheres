#include "pde/pdes.h"

double TaylorVortexPDE2::laplacianOP(arrayd<2> & x, double t){
    return -2 * cos(x[0]) * cos(x[1]);
}

double TaylorVortexPDE2::truth(arrayd<2> & x, double t){
    return -cos(x[0]) * cos(x[1]);
}

arrayd<2> TaylorVortexPDE2::gradTruth(arrayd<2> & x, double t){
    return {{sin(x[0])*cos(x[1]),cos(x[0])*sin(x[1])}};
}