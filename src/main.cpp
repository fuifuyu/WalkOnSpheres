#include <chrono>
#include <random>
#include <iostream>
#include <string>

#include "window.h"
#include "boundary/boxboundary.h"
#include "wos.h"
#include "pde/pdes.h"
#include "mytypes.h"

#undef main

using namespace MyTypes;

double minBound = -3 * M_PI / 2;
double maxBound = 3 * M_PI / 2;

SDL_Window *window;
SDL_Renderer *renderer;

// Local is the pixel space
double localToDomain(int a, int numRows) {
	return a * maxBound * 2.0/ numRows + minBound;
}

int main() {
	Window window(50,50,15);
	double *streamAcc = new double[window.numRows * window.numCols]();
	double *streamVal = new double[window.numRows * window.numCols]();
	size_t numSamples = 1;
	std::random_device rd;
	std::mt19937 generator(rd());
	arrayd<2> bb {{minBound,maxBound}};
	WosBoxBoundary2D taylorVortex([](arrayd<2> const&x){return 0;},bb,0.005);
	WOS2d taylorVortexWOS(
		&TaylorVortexPDE2(),
		&taylorVortex,
		generator
	);
	while(numSamples++){
		for(auto it : window){
			streamAcc[it->i()] += taylorVortexWOS.eval(
				localToDomain(it->x(),window.numRows),
				localToDomain(it->y(),window.numRows), 0);
			streamVal [it->i()] = streamAcc[it->i()]/numSamples;
		}
		window.setTitle(std::to_string(numSamples));
		window.clear();
		window.plotWeights(streamVal);
		SDL_PumpEvents();
		SDL_RenderPresent(window.getRenderer());
	}
	while (true);
	return 0;
}