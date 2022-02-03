#include "wos.h"

using namespace std;

uniform_real_distribution<double> uniformDis(0.0, 1.0);

const double GCONST_2D = 1 / (2 * M_PI);
const double GCONST_3D = (1 / (4 * M_PI));

double WOS2d::eval(double x, double y, double time) const {
	arrayd<2> p = arrayd<2>{{x,y}};
	return WOS<2>::eval(p,time);
}

double WOS2d::calcSphereArea(double r) const {
	return M_PI * r * r;
}

double WOS2d::G(arrayd<2> &x, arrayd<2> &y, double sphereR) const {
	double r = (y[0] - x[0]) * (y[0] - x[0]) + (y[1] - x[1]) * (y[1] - x[1]);
	r = sqrt(r)+0.01;
	return GCONST_2D * log(sphereR / r);
}

arrayd<2> WOS2d::randPointOnSphere(arrayd<2> &x, double radius) const {
	double theta = uniformDis(generator) * 2 * M_PI;
	return arrayd<2>{ {x[0] + radius * cos(theta), x[1] + radius * sin(theta)}};
}

arrayd<2> WOS2d::randPointInSphere(arrayd<2> &x, double radius) const {
	double r = radius * sqrt(uniformDis(generator));
	double theta = uniformDis(generator) * 2 * M_PI;
	return arrayd<2>{ {x[0] + r * cos(theta), x[1] + r * sin(theta)}};
}

double WOS3d::G(arrayd<3> &x, arrayd<3> &y, double sphereR) const {
	double r = 0;
	for (size_t i = 0; i < 3; i++) {
		r += (y[i] - x[i]);
	}
	r = sqrt(r);
	return GCONST_3D * (sphereR - r) / (r * sphereR);
}