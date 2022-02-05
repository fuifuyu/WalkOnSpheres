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
	double eval(arrayd<N> &x, double iteration) const
	{
		if (boundary->meet(x)) return boundary->cond(x);
		double res = 0;
		double R = boundary->minDistFromBoundary(x);

		arrayd<N> randOnSphere = randPointOnSphere(x, R);
		arrayd<N> randInSphere = randPointInSphere(x, R);

		double u_1 = eval(randOnSphere, iteration);
		double harmonic = 0;
		if (pde->type == PDETypes::Poisson) {
			double f_1 = pde->laplacianOP(randInSphere, iteration);
			harmonic = f_1 * G(x, randInSphere, R) * calcSphereArea(R);
		}
		return u_1 + harmonic;
	}

protected:
	PDE<N> *pde;
	WosBoundary<N> *boundary;
	mutable std::mt19937 generator;

	virtual double calcSphereArea(double radius) const = 0;
	virtual arrayd<N> randPointOnSphere(arrayd<N> &x, double radius) const = 0;
	virtual arrayd<N> randPointInSphere(arrayd<N> &x, double radius) const = 0;
	virtual double G(arrayd<N> &x, arrayd<N> &y, double sphereR) const = 0;

};

class WOS2d : public WOS<2> {
public:
	using WOS::WOS;

	double eval(double x, double y, double time) const;
protected:
	double calcSphereArea(double radius) const override;
	arrayd<2> randPointOnSphere(arrayd<2> &x, double radius) const override;
	arrayd<2> randPointInSphere(arrayd<2> &x, double radius) const override;
	double G(arrayd<2> &x, arrayd<2> &y, double sphereR) const override;
};

class WOS3d : public WOS<3> {
public:
	using WOS::WOS;

protected:
	double G(arrayd<3> &x, arrayd<3> &y, double sphereR) const override;
};

#endif