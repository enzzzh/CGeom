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
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    bool isRunning = true;
    SDL_Event event;
    Uint64 last_time = SDL_GetPerformanceCounter();
    double fps = 0.0;

    while (isRunning) {
        Uint64 current_time = SDL_GetPerformanceCounter();
        double delta_time = (double)((current_time - last_time) / (double)SDL_GetPerformanceFrequency());
        last_time = current_time;
        if (delta_time > 0.0) {
            fps = 1.0 / delta_time;
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        SDL_FillRect(surface, NULL, COLOR_BLACK); // Clear screen 
        SDL_Rect rect = {200, 200, 200, 200};
        SDL_FillRect(surface, &rect, COLOR_WHITE);
        char fps_text[32];
        snprintf(fps_text, sizeof(fps_text), "FPS: %.2f", fps);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, fps_text, textColor);
        if (textSurface) {
            SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h };
            SDL_BlitSurface(textSurface, NULL, surface, &textRect);
            SDL_FreeSurface(textSurface);
        }
        SDL_UpdateWindowSurface(window);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
