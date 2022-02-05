#include "pde/pdes.h"

double TaylorVortexPDE2::laplacianOP(arrayd<2> & x, double t){
    return -2 * cos(x[0]) * cos(x[1]);
}