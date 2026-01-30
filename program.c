#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x000000
#define cam_x 0.0
#define cam_y 0.0
#define cam_zoom 1.0f

typedef struct{
	float x,y;
	float zoom;
	int w, h;
} Camera;

Camera cam = {cam_x, cam_y, cam_zoom, WIDTH, HEIGHT};


void DrawRect(SDL_Renderer* renderer,Camera* cam, SDL_FRect* worldRect){
	SDL_FRect screenRect;
    	screenRect.x = (worldRect->x - cam->x) * cam->zoom + (cam->w / 2.0f);
   	screenRect.y = (worldRect->y - cam->y) * cam->zoom + (cam->h / 2.0f);
    
	screenRect.w = worldRect->w * cam->zoom;
    	screenRect.h = worldRect->h * cam->zoom;

    	SDL_RenderFillRectF(renderer, &screenRect);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/TTF/IosevkaTermSlabNerdFontPropo-BoldItalic.ttf", 24);
    if (!font) {
        fprintf(stderr, "Error: Font not found. Please ensure DejaVuSans.ttf is installed.\n");
        return 1;
    }
    SDL_Color textColor = { 255, 255, 255, 255 }; // White
    SDL_Window *window = SDL_CreateWindow("Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0); 
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
    bool isRunning = true;
    SDL_Event event;
    Uint64 last_time = SDL_GetPerformanceCounter();
    double fps = 0.0;

while (isRunning) {
        Uint64 current_time = SDL_GetPerformanceCounter();
        double delta_time = (double)(current_time - last_time) / SDL_GetPerformanceFrequency();
        last_time = current_time;
        if (delta_time > 0.0) fps = 1.0 / delta_time;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) isRunning = false;

            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) cam.zoom *= 1.1f;
                else if (event.wheel.y < 0) cam.zoom /= 1.1f;
            }
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);
        float speed = 400.0f * (float)delta_time; // 400 pixels per second
        
        if (state[SDL_SCANCODE_LEFT])  cam.x -= speed / cam.zoom;
        if (state[SDL_SCANCODE_RIGHT]) cam.x += speed / cam.zoom;
        if (state[SDL_SCANCODE_UP])    cam.y -= speed / cam.zoom;
        if (state[SDL_SCANCODE_DOWN])  cam.y += speed / cam.zoom;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(renderer);

        SDL_FRect worldBox = {200, 200, 200, 200};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
        DrawRect(renderer, &cam, &worldBox);

        char fps_text[32];
        snprintf(fps_text, sizeof(fps_text), "FPS: %.2f", fps);
        SDL_Surface* textSurf = TTF_RenderText_Solid(font, fps_text, textColor);
        if (textSurf) {
            SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, textSurf);
            SDL_Rect textRect = {10, 10, textSurf->w, textSurf->h};
            SDL_RenderCopy(renderer, textTex, NULL, &textRect);
            SDL_FreeSurface(textSurf);
            SDL_DestroyTexture(textTex);
        }

        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
