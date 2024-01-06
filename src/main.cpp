#include <iostream>

#include "GameScreen.h"
#include "ScreenManager.h"
#include "controller.h"
int main() {
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};

    Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    Controller controller;
    ScreenManager screenManager(kGridWidth, kGridHeight);
    screenManager.Start(controller, renderer, kMsPerFrame);
    std::cout << "Game has terminated successfully!\n";
    return 0;
}