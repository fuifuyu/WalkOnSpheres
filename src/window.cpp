#include "window.h"
#include <iostream>
using namespace::MyTypes;

void Window::plotWeights(double *weights){
    for(int xi = 0; xi < numCols; xi++){
        int x = xi * pixelWidth;
        for(int yi = 0; yi < numRows; yi++){
            int y = yi * pixelWidth;
            SDL_Rect fillRect = {x,y,pixelWidth,pixelWidth};
            double color = weights[IX(xi,yi)]*255;
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

void Window::plotVector2(arrayd<2> *vec){
    for(int xi = 0; xi < numCols; xi++){
        int x = xi * pixelWidth;
        for(int yi = 0; yi < numRows; yi++){
            int y = yi * pixelWidth;
            double v0 = vec[IX(xi,yi)][0];
            double v1 = vec[IX(xi,yi)][1];
            double dist = sqrt(pow(v0,2) + pow(v1,2));
            if(dist == 0) continue;
			if (v0 != 0 || v1 != 0) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderDrawLine(renderer, x, y, x + v0 * pixelWidth, y + v1 * pixelWidth);
			}
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawPoint(renderer, x + v0 * pixelWidth, y + v1 * pixelWidth);
        }
    }
}

bool Window::saveScreenshotBMP(std::string filepath) {
	SDL_Surface *saveSurface = NULL;
	SDL_Surface *infoSurface = NULL;
	infoSurface = SDL_GetWindowSurface(window);
	if (infoSurface == NULL) {
		std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
	}
	else {
		unsigned char *pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
		if (pixels == 0) {
			std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
			return false;
		}
		else {
			if (SDL_RenderReadPixels(renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
				std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
				delete[] pixels;
				return false;
			}
			else {
				saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
				if (saveSurface == NULL) {
					std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
					delete[] pixels;
					return false;
				}
				SDL_SaveBMP(saveSurface, filepath.c_str());
				SDL_FreeSurface(saveSurface);
				saveSurface = NULL;
			}
			delete[] pixels;
		}
		SDL_FreeSurface(infoSurface);
		infoSurface = NULL;
	}
	return true;
}