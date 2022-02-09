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
double maxBound =  3 * M_PI / 2;

SDL_Window *window;
SDL_Renderer *renderer;

// Local is the pixel space
double localToDomain(int a, int numRows) {
	return a * (maxBound - minBound)/ numRows + minBound;
}	

int mouseX, mouseY;
double errorMax, errorMaxX, errorMaxY;
bool interactiveError = false;
bool quit = false;

void input(Window &window){
	SDL_Event e;
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT) {
		quit = true;
	}
	if (keystates[SDL_SCANCODE_S]) {
		std::string fileName;
		std::cout << "Please enter a file name: (enter q to cancel)\n";
		std::cin >> fileName;
		if(fileName != "q") {
			window.saveScreenshotBMP("C:/Personal/Project/graphics/wos/"+fileName+".bmp");
			std::cout << "Saved!";
		}
		else std::cout << "Operation canceled\n";
	}
	if (keystates[SDL_SCANCODE_E]) {
		interactiveError = true;
	}
	if (keystates[SDL_SCANCODE_F]) {
		interactiveError = false;
	}
}
void debugStream(Window window, WOS2d wos, double *streamVal){
	if (interactiveError) {
		SDL_GetMouseState(&mouseX, &mouseY);
		int localX = window.screenToLocal(mouseX);
		int localY = window.screenToLocal(mouseY);
		arrayd<2> x = {{localToDomain(localX,window.numCols), localToDomain(localY,window.numRows)}};
		std::cout << wos.truth(x,0) << " - " << streamVal[window.IX(localX,localY)] << " = ";
		std::cout << wos.truth(x,0)- streamVal[window.IX(localX,localY)] << '\n';
	}
	else {
		std::cout << "Max error: " << errorMax << ", " << errorMaxX << ", " << errorMaxY << "]\n";
	}
}

int main() {
	Window window(50,50,15);
	double *streamAcc = new double[window.numRows * window.numCols]();
	double *streamVal = new double[window.numRows * window.numCols]();
	arrayd<2> *vecAcc = new arrayd<2>[window.numRows * window.numCols]();
	arrayd<2> *vecVal = new arrayd<2>[window.numRows * window.numCols]();
	size_t numSamples = 1;
	std::random_device rd;
	std::mt19937 generator(rd());
	arrayd<2> bb {{minBound,maxBound}};
	WosBoxBoundary2D taylorVortexBoundary([](arrayd<2> const&x){return 0;},bb,0.005);
	WOS2d taylorVortexWOS(
		&TaylorVortexPDE2(),
		&taylorVortexBoundary,
		generator
	);
	while(!quit && numSamples++){
		input(window);
		errorMax = errorMaxX = errorMaxY = 0;
		double invN = 1.0/numSamples;
		for(auto it : window){
			arrayd<2> x = {{localToDomain(it->x(),window.numRows),localToDomain(it->y(),window.numRows)}};
			streamAcc[it->i()] += taylorVortexWOS.eval(x, 0);
			streamVal [it->i()] = streamAcc[it->i()]/numSamples;
			double error = abs(taylorVortexWOS.truth(x,0)-streamVal[it->i()]);
			if(error > errorMax) errorMax = error;
			auto tmp = taylorVortexWOS.evalGrad(x,0);
			vecAcc[it->i()] = add(vecAcc[it->i()], {{tmp[1],-tmp[0]}});
			vecVal[it->i()] = times(vecAcc[it->i()],invN);

			error = abs(taylorVortexWOS.gradTruth(x,0)[1]-vecVal[it->i()][0]);
			if(error > errorMaxX) errorMaxX = error;
			error = abs(-taylorVortexWOS.gradTruth(x,0)[0]-vecVal[it->i()][1]);
			if(error > errorMaxY) errorMaxY = error;
		}
		debugStream(window, taylorVortexWOS, streamVal);
		window.setTitle(std::to_string(numSamples));
		window.clear();
		// window.plotWeights(streamVal);
		window.plotVector2(vecVal);
		SDL_RenderPresent(window.getRenderer());
	}
	return 0;
}