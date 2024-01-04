#ifndef SNAKE_H
#define SNAKE_H

#include <functional>
#include <mutex>
#include <vector>

#include "SDL.h"
#include "util.h"

class Snake {
   public:
    enum class Direction { kUp, kDown, kLeft, kRight };
    Snake(int grid_width, int grid_height);

    void Update();

    void GrowBody();
    bool SnakeCell(int x, int y);
    void Control(const KeyStroke &key);

    Direction direction = Direction::kUp;
    float speed{0.1f};
    int size{1};
    bool alive{true};
    float head_x;
    float head_y;
    std::vector<SDL_Point> body;

   private:
    void UpdateHead();
    void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
    void ChangeDirection(Direction input, Direction opposite);
    bool growing{false};
    int grid_width;
    int grid_height;
};

#endif