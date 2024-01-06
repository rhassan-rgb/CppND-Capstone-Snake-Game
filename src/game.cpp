#include "game.h"

#include <functional>
#include <future>
#include <iostream>
#include <thread>

#include "SDL.h"

GameScreen::GameScreen(std::size_t grid_width, std::size_t grid_height)
    : IScreen("Snake Score: 0 FPS: 0"),
      snake(grid_width, grid_height),
      welcomeScreen(),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
    controlCallback =
        std::bind(&GameScreen::Control, this, std::placeholders::_1);
    PlaceFood();
}

void GameScreen::Run(Controller &controller, Renderer &renderer,
                     std::size_t target_frame_duration) {}

void GameScreen::PlaceFood() {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);
        // Check that the location is not occupied by a snake item before
        // placing food.
        if (!snake.SnakeCell(x, y)) {
            food.x = x;
            food.y = y;
            return;
        }
    }
}

int GameScreen::GetScore() const { return score; }
int GameScreen::GetSize() const { return snake.size; }

void GameScreen::Control(const KeyStroke &key) {
    std::cout << "GameScreen::Control lock Active" << std::endl;
    std::unique_lock<std::mutex> uLock(_activeMutex);
    if (!_isActive) {
        std::cout << "GameScreen::Control InActive returning" << std::endl;

        return;
    }
    std::cout << "GameScreen::Control unlock Active" << std::endl;
    uLock.unlock();
    std::cout << "GameScreen::Control unlocked Active" << std::endl;
    std::cout << "GameScreen::Control lock Items" << std::endl;
    std::lock_guard<std::mutex> uLock_items(_itemsMutex);
    std::cout << "GameScreen::Control locked Items" << std::endl;
    if (KeyStroke::KEY_ESC == key) {
        _currentItem = static_cast<int>(GameItems::ITEM_PAUSE_GAME);
        return;
    }
    snake.Control(key);
}
void GameScreen::Activate() {
    _currentItem = static_cast<int>(GameItems::ITEM_GAME);
    std::unique_lock<std::mutex> uLock(_activeMutex);
    _isActive = true;
}
void GameScreen::Deactivate() {
    std::unique_lock<std::mutex> uLock(_activeMutex);
    _isActive = false;
}
int GameScreen::GetSelection() { return _currentItem; }

void GameScreen::generateScreenContext(Snake const &snake,
                                       SDL_Point const &food) {
    constexpr Colors FOOD_COLOR = {.r = 0xFF, .g = 0xCC, .b = 0x00, .a = 0xFF};
    constexpr Colors SNAKE_COLOR = {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};
    constexpr Colors ALIVE_COLOR = {.r = 0x00, .g = 0x7A, .b = 0xCC, .a = 0xFF};
    constexpr Colors DEAD_COLOR = {.r = 0xFF, .g = 0x00, .b = 0x00, .a = 0xFF};

    std::vector<ScreenItem> items;
    ScreenItem temp = ScreenItem({.x = food.x, .y = food.y});
    temp.SetColors(FOOD_COLOR);
    items.push_back(temp);
    for (auto const &point : snake.body) {
        temp = ScreenItem({.x = point.x, .y = point.y});
        temp.SetColors(SNAKE_COLOR);
        items.push_back(temp);
    }

    temp = ScreenItem({.x = static_cast<int>(snake.head_x),
                       .y = static_cast<int>(snake.head_y)});
    if (snake.alive)
        temp.SetColors(ALIVE_COLOR);
    else
        temp.SetColors(DEAD_COLOR);
    items.push_back(temp);
    _menuItems = std::vector<ScreenItem>(items);
}
bool GameScreen::Update() {
    std::unique_lock<std::mutex> uLock_active(_activeMutex);
    if (!_isActive) {
        return false;
    }
    uLock_active.unlock();
    if (!snake.alive) {
        if (static_cast<int>(GameItems::ITEM_GAME_OVER) != _currentItem) {
            _currentItem = static_cast<int>(GameItems::ITEM_GAME_OVER);
            return true;
        }
        return false;
    }

    std::unique_lock<std::mutex> uLock(_itemsMutex);
    snake.Update();  // snake has a shred resource "direction" that's being
                     // accessed in this function in the main thread as well as
                     // the control thread
    uLock.unlock();

    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);

    // Check if there's food over here
    if (food.x == new_x && food.y == new_y) {
        score++;
        PlaceFood();
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += 0.02;
    }
    generateScreenContext(snake, food);
    // renderer.Render(snake, food);

    // frame_end = SDL_GetTicks();

    // // Keep track of how long each loop through the input/update/render
    // // cycle takes.
    // frame_count++;
    // frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    // if (frame_end - title_timestamp >= 1000) {
    //     renderer.UpdateWindowTitle(score, frame_count);
    //     frame_count = 0;
    //     title_timestamp = frame_end;
    // }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    // if (frame_duration < target_frame_duration) {
    //     SDL_Delay(target_frame_duration - frame_duration);
    // }
    return true;
}

std::string GameScreen::GetTitle() { return std::string(); }
const std::vector<ScreenItem> &GameScreen::GetScreenContext() const {
    std::vector<ScreenItem> ret;
    return _menuItems;
}