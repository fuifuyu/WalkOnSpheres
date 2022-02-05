#include "window.h"

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