#ifndef GAME_H
#define GAME_H

#include <memory>
#include <random>

#include "IScreen.h"
#include "SDL.h"
#include "WelcomeScreen.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class GameScreen : public IScreen<ScreenItem> {
   public:
    GameScreen(std::size_t grid_width, std::size_t grid_height);
    void Run(Controller& controller, Renderer& renderer,
             std::size_t target_frame_duration);
    int GetScore() const;
    int GetSize() const;
    bool Update() override;
    void Control(const KeyStroke& key) override;
    void Activate() override;
    void Deactivate() override;
    int GetSelection() override;
    virtual std::string GetTitle() override;
    const std::vector<ScreenItem>& GetScreenContext() const override;
    std::function<void(KeyStroke)> controlCallback;

   private:
    WelcomeScreen welcomeScreen;
    Snake snake;
    SDL_Point food;

    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;

    int score{0};

    void PlaceFood();
    void generateScreenContext(Snake const& snake, SDL_Point const& food);
};

#endif