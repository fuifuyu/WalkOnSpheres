#pragma once
#define _USE_MATH_DEFINES

#ifndef WOS_H
#define WOS_H

#include <functional>
#include <array>
#include <random>
#include <math.h>
#include <assert.h>

using namespace std;
template<size_t N>
using arrayd = array<double, N>;

template<size_t N>
class WOS {
public:
	WOS(function<double(arrayd<N> &, size_t iteration)> f,
		function<double(arrayd<N> &)> dirichletCondition,
		function<bool(arrayd<N> &)> meetBoundary,
		function<double(arrayd<N> &)> radiusFromBoundary,
		mt19937 generator) :
		f(f), dirichletCondition(dirichletCondition), meetBoundary(meetBoundary), radiusFromBoundary(radiusFromBoundary), generator(generator)
	{};
	double eval(arrayd<N> &x, size_t iteration) const
	{
		if (meetBoundary(x)) return dirichletCondition(x);
		assert(iteration >= 0);
		double res = 0;
		double R = radiusFromBoundary(x);

		arrayd<N> randOnSphere = randPointOnSphere(x, R);
		arrayd<N> randInSphere = randPointInSphere(x, R);

		double u_1 = eval(randOnSphere, iteration);
		double harmonic = 0;
		if (f) {
			double f_1 = f(randInSphere, iteration);
			harmonic = f_1 * G(x, randInSphere, R) * calcSphereArea(R);
		}
		return u_1 + harmonic;
	}

protected:
	function<double(arrayd<N> &, size_t iteration)> f;
	function<double(arrayd<N> &)> radiusFromBoundary;
	function<double(arrayd<N> &)> dirichletCondition;
	function<bool(arrayd<N> &)> meetBoundary;
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