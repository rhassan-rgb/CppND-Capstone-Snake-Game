#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"
#include "WelcomeScreen.h"
#include "snake.h"

class Renderer {
   public:
    Renderer() = delete;
    Renderer(const std::size_t screen_width, const std::size_t screen_height,
             const std::size_t grid_width, const std::size_t grid_height);
    ~Renderer();

    template <typename T>
    void Render(std::vector<T> const &screenContent) {
        ScreenItemType type = screenContent.at(0).GetType();
        renderContent(screenContent);
    }

   protected:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    SDL_Color _normalColor;
    SDL_Color _selectedColor;
    TTF_Font *_font;
    const std::size_t screen_width;
    const std::size_t screen_height;
    const std::size_t grid_width;
    const std::size_t grid_height;

    void drawText(const std::string &text, const Coordinates &cord,
                  const Colors &clrs);
    void renderContent(std::vector<TextScreenItem> const &screenContent);
    void renderContent(std::vector<ScreenItem> const &screenContent);
};

#endif