#include "renderer.h"

#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
    _normalColor = {255, 255, 255};
    _selectedColor = {255, 0, 0};  // Red for selected text

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    // initialize TTF
    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize.\n";
        std::cerr << "TTF_Error: " << SDL_GetError() << "\n";
    }

    // Create Window
    sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen_width,
                                  screen_height, SDL_WINDOW_SHOWN);

    if (nullptr == sdl_window) {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create renderer
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == sdl_renderer) {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
    _font = TTF_OpenFont("../src/RubikLines-Regular.ttf", 28);
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    TTF_Quit();
    SDL_Quit();
}

void Renderer::renderContent(std::vector<TextScreenItem> const &screenContent) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);
    for (auto &item : screenContent) {
        drawText(item.ToString(), item.GetCoordinates(), item.GetColor());
    }
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::renderContent(std::vector<ScreenItem> const &screenContent) {
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;
    Colors tempClr = screenContent.at(0).GetColor();
    Coordinates tempCor = screenContent.at(0).GetCoordinates();

    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);

    SDL_SetRenderDrawColor(sdl_renderer, tempClr.r, tempClr.g, tempClr.b,
                           tempClr.a);
    for (auto &item : screenContent) {
        if (tempClr != item.GetColor()) {
            tempClr = item.GetColor();
            SDL_SetRenderDrawColor(sdl_renderer, tempClr.r, tempClr.g,
                                   tempClr.b, tempClr.a);
        }
        tempCor = item.GetCoordinates();
        block.x = tempCor.x * block.w;
        block.y = tempCor.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Update Screen
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::drawText(const std::string &text, const Coordinates &cord,
                        const Colors &clrs) {
    SDL_Color textColor = {.r = clrs.r, .g = clrs.g, .b = clrs.b, .a = clrs.a};
    SDL_Surface *surface = TTF_RenderText_Solid(_font, text.c_str(), textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;

    SDL_Rect destRect = {cord.x, cord.y, textWidth, textHeight};
    SDL_RenderCopy(sdl_renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
