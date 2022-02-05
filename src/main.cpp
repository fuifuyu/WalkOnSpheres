#include <chrono>
#include <random>
#include <iostream>
#include <string>

#include <SDL.h>
#include "boundary/boxboundary.h"
#include "wos.h"
#include "pde/pdes.h"
#include "mytypes.h"

#undef main

using namespace MyTypes;

#define IX(i,j) (i+(N+1)*(j))

double minBound = -3 * M_PI / 2;
double maxBound = 3 * M_PI / 2;
const size_t N = 50;
int h = 15;

SDL_Window *window;
SDL_Renderer *renderer;

double vorticityAt(arrayd<2> const &x, size_t iteration) {
	return -2 * cos(x[0]) * cos(x[1]);
}

double localToDomain(double a) {
	return a * maxBound * 2/ N + minBound;
}

void plotWeight(std::function<double(double,double)> func, double *acc, int numSamples) {
	for (int i = 0; i <= N; i++) {
		int x = i * h;
		for (int j = 0; j <= N; j++) {
			int y = j * h;
			SDL_Rect fillRect = { x,y,h,h };
			acc[IX(i, j)] += func(localToDomain(i), localToDomain(j));
			double color = acc[IX(i, j)] / numSamples * 255;
			if (color > 255) color = 255;
			else if (color < -255) color = -255;
			if (color > 0) {
				SDL_SetRenderDrawColor(renderer, color, 0, 0, 255);
				SDL_RenderFillRect(renderer, &fillRect);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 0, -color, 255);
				SDL_RenderFillRect(renderer, &fillRect);
			}
		}
	}
}

int screenToLocal(int a) {
	return a / h;
}
double taylorGreenStream(double x, double y) {
	return -cos(x) * cos(y);
}

int main() {
	SDL_CreateWindowAndRenderer((N+1) * h, (N+1) * h, 0, &window, &renderer);
	double *streamVal = new double[(N + 1) * (N + 1)]();
	size_t samples = 1;
	std::random_device rd;
	std::mt19937 generator(rd());
	arrayd<2> bb {{minBound,maxBound}};
	WosBoxBoundary2D taylorVortex([](arrayd<2> const&x){return 0;},bb,0.005);
	WOS2d taylorVortexWOS(
		&TaylorVortexPDE2(),
		&taylorVortex,
		generator
	);
	while(true){
		SDL_SetWindowTitle(window, std::to_string(samples).c_str());
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		plotWeight([&taylorVortexWOS](double x, double y) {return taylorVortexWOS.eval(x, y, 0);}, streamVal, samples);
		samples++;
		SDL_RenderPresent(renderer);
		int mouseX, mouseY;
		SDL_PumpEvents();
		SDL_GetMouseState(&mouseX, &mouseY);
	}
	while (true);
	return 0;
}