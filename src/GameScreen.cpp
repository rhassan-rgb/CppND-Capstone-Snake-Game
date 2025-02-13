#include "GameScreen.h"

#include <functional>
#include <future>
#include <iostream>
#include <thread>

#include "SDL.h"

GameScreen::GameScreen(std::size_t grid_width, std::size_t grid_height)
    : IScreen("Snake Game Score: 0 FPS: 0"),
      snake(grid_width, grid_height),
      welcomeScreen(),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      bonusFoodActive(false),
      bonusFoodCounter(0) {
    controlCallback =
        std::bind(&GameScreen::Control, this, std::placeholders::_1);
    PlaceFood();
    PlaceBonusFood();
}

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
    std::unique_lock<std::mutex> uLock(_activeMutex);
    if (!_isActive) {
        return;
    }
    uLock.unlock();
    std::lock_guard<std::mutex> uLock_items(_itemsMutex);
    if (KeyStroke::KEY_ESC == key) {
        _currentItem = static_cast<int>(GameItems::ITEM_PAUSE_GAME);
        return;
    } else if (KeyStroke::KEY_EXT == key) {
        std::lock_guard<std::mutex> lock(bonusFoodMutex);
        bonusFoodActive = false;
        foodIsEaten.notify_one();
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
    constexpr Colors BONUS_COLOR = {.r = 0x00, .g = 0xF0, .b = 0x00, .a = 0xFF};

    _menuItems.clear();
    ScreenItem temp = ScreenItem({.x = food.x, .y = food.y});
    temp.SetColors(FOOD_COLOR);
    _menuItems.push_back(temp);

    std::unique_lock<std::mutex> lock(bonusFoodMutex);
    if (bonusFoodActive) {
        temp = ScreenItem({.x = bonusFood.x, .y = bonusFood.y});
        temp.SetColors(BONUS_COLOR);
        _menuItems.push_back(temp);
    }
    lock.unlock();

    for (auto const &point : snake.body) {
        temp = ScreenItem({.x = point.x, .y = point.y});
        temp.SetColors(SNAKE_COLOR);
        _menuItems.push_back(temp);
    }

    temp = ScreenItem({.x = static_cast<int>(snake.head_x),
                       .y = static_cast<int>(snake.head_y)});
    if (snake.alive)
        temp.SetColors(ALIVE_COLOR);
    else
        temp.SetColors(DEAD_COLOR);
    _menuItems.push_back(temp);
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
        bonusFoodCounter++;
        PlaceFood();
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += 0.02;
    }
    std::unique_lock<std::mutex> lock(bonusFoodMutex);
    if (bonusFoodCounter == 5) {
        bonusFoodCounter = 0;
        if (!bonusFoodActive) {
            bonusFoodActive = true;
            std::thread bonusCounter =
                std::thread(&GameScreen::BonusFoodTimer, this);
            bonusCounter.detach();
            PlaceBonusFood();
        }
    }
    // Check if there's Bonus food over here
    if (bonusFoodActive) {
        if (bonusFood.x == new_x && bonusFood.y == new_y) {
            score += 5;
            bonusFoodActive = false;
            foodIsEaten.notify_one();
        }
    }
    lock.unlock();
    generateScreenContext(snake, food);
    return true;
}

std::string GameScreen::GetTitle() { return _title; }
const std::vector<ScreenItem> &GameScreen::GetScreenContext() const {
    std::vector<ScreenItem> ret;
    return _menuItems;
}

void GameScreen::PlaceBonusFood() {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);
        // Check that the location is not occupied by a snake item and food
        // before placing bonus food.
        if (!snake.SnakeCell(x, y) && x != food.x && y != food.y) {
            bonusFood.x = x;
            bonusFood.y = y;
            return;
        }
    }
}

// if (!is_bonus_food_active) {  // Check if bonus food is already active
//     PlaceBonusFood();
//     is_bonus_food_active = true;
//     bonusFoodThread = std::thread(&Game::BonusFoodTimer, this);
//     bonusFoodThread.detach();
//     already_appeared = true;
// }

void GameScreen::BonusFoodTimer() {
    const int bonusSeconds = 10;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto pause_Seconds =
        std::chrono::duration_cast<std::chrono::seconds>(startTime - startTime)
            .count();
    std::unique_lock<std::mutex> lock(bonusFoodMutex);
    while (bonusFoodActive) {
        lock.unlock();

        std::unique_lock<std::mutex> activeLock(_activeMutex);
        if (!_isActive) {
            auto current_Time = std::chrono::high_resolution_clock::now();
            pause_Seconds = std::chrono::duration_cast<std::chrono::seconds>(
                                current_Time - startTime)
                                .count();
        }
        activeLock.unlock();
        auto current_Time = std::chrono::high_resolution_clock::now();
        auto elapsed_Seconds = std::chrono::duration_cast<std::chrono::seconds>(
                                   current_Time - startTime)
                                   .count();
        if (elapsed_Seconds - pause_Seconds >= bonusSeconds) {
            // Bonus food time is up
            lock.lock();
            bonusFoodActive = false;
            break;
        }
        lock.lock();
        // Wait for a short interval or until the condition_variable is notified
        foodIsEaten.wait_for(lock, std::chrono::milliseconds(500));
    }
}