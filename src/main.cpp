#include <iostream>

#include "ScreenManager.h"
#include "ScreenSM.h"
#include "controller.h"
#include "game.h"
int main() {
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};

    ScreenManager renderer(kScreenWidth, kScreenHeight, kGridWidth,
                           kGridHeight);
    Controller controller;
    ScreenSM screenStateMachine(kGridWidth, kGridHeight);
    screenStateMachine.Start(controller, renderer, kMsPerFrame);
    // Game game(kGridWidth, kGridHeight);
    // game.Run(controller, renderer, kMsPerFrame);
    std::cout << "Game has terminated successfully!\n";
    // std::cout << "Score: " << game.GetScore() << "\n";
    // std::cout << "Size: " << game.GetSize() << "\n";
    return 0;
}