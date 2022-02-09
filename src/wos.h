#pragma once
#define _USE_MATH_DEFINES

#ifndef WOS_H
#define WOS_H

#include "pde.h"
#include "mytypes.h"
#include <array>
#include <random>
#include <math.h>

using namespace std;
using namespace MyTypes;

template<size_t N>
class WOS {
public:
	WOS(PDE<N> *pde, WosBoundary<N> *boundary, mt19937 generator)
	: pde(pde), boundary(boundary), generator(generator)
	{};
	double eval(arrayd<N> &x, double t) const
	{
		if (boundary->meet(x)) return boundary->cond(x);
		double res = 0;
		double R = boundary->minDistFromBoundary(x);

		arrayd<N> randOnSphere = randPointOnSphere(x, R);
		arrayd<N> randInSphere = randPointInSphere(x, R);

		double u_1 = eval(randOnSphere, t);
		double harmonic = 0;
		if (pde->type == PDETypes::Poisson) {
			double f_1 = pde->laplacianOP(randInSphere, t);
			harmonic = f_1 * G(x, randInSphere, R) * calcSphereArea(R);
		}
		return u_1 + harmonic;
	}
	arrayd<N> evalGrad(arrayd<N> &x, double t) const{
		if(boundary->meet(x)) return pde->gradTruth(x,t);
		double R = boundary->minDistFromBoundary(x);
		arrayd<N> randOnSphere = randPointOnSphere(x,R);
		arrayd<N> sol = times(subtract(randOnSphere,x),N*eval(randOnSphere,t)/(R*R));
		if(pde->type==PDETypes::Poisson){
			arrayd<N> randInSphere = randPointInSphere(x,R);
			arrayd<N> source = times(gradG(x,randInSphere,R),M_PI*R*R*pde->laplacianOP(randInSphere,t));
			return add(sol, source);
		}
		return sol;
	}
	double truth(arrayd<N> &x, double t) const{
		return pde->truth(x,t);
	}
	arrayd<N> gradTruth(arrayd<N> &x, double t) const{
		return pde->gradTruth(x,t);
	}
protected:
	PDE<N> *pde;
	WosBoundary<N> *boundary;
	mutable std::mt19937 generator;

	virtual double calcSphereArea(double radius) const = 0;
	virtual arrayd<N> randPointOnSphere(arrayd<N> &x, double radius) const = 0;
	virtual arrayd<N> randPointInSphere(arrayd<N> &x, double radius) const = 0;
	virtual double G(arrayd<N> &x, arrayd<N> &y, double sphereR) const = 0;
	virtual arrayd<N> gradG(arrayd<N> &x, arrayd<N> &y, double sphereR) const = 0;
};

class WOS2d : public WOS<2> {
public:
	using WOS::WOS;
	using WOS::eval;

	double eval(double x, double y, double time) const;
protected:
	double calcSphereArea(double radius) const override;
	arrayd<2> randPointOnSphere(arrayd<2> &x, double radius) const override;
	arrayd<2> randPointInSphere(arrayd<2> &x, double radius) const override;
	double G(arrayd<2> &x, arrayd<2> &y, double sphereR) const override;
	arrayd<2> gradG(arrayd<2> &x, arrayd<2> &y, double sphereR) const override;
};

class WOS3d : public WOS<3> {
public:
	using WOS::WOS;

protected:
	double G(arrayd<3> &x, arrayd<3> &y, double sphereR) const override;
};

#endif