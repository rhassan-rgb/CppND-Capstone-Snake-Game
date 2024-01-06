#ifndef SCREEN_SM_H
#define SCREEN_SM_H
#include <iostream>
#include <memory>

#include "GameScreen.h"
#include "IScreen.h"
#include "Leaderboard.h"
#include "WelcomeScreen.h"
#include "controller.h"
#include "renderer.h"
#include "util.h"

enum class Screens {
    SCREEN_WELCOME = 0,
    SCREEN_PLAYER_NAME,
    SCREEN_LEADER_BOARD,
    SCREEN_PAUSE_GAME,
    SCREEN_RESUME_GAME,
    SCREEN_GAME_START,
    SCREEN_NEW_GAME,
    SCREEN_EXIT,
    TOTAL_ITEMS
};

class ScreenManager {
   public:
    ScreenManager() = delete;
    ScreenManager(std::size_t grid_width, std::size_t grid_height);
    ~ScreenManager();
    bool Update(Renderer &renderer);
    void Start(Controller &controller, Renderer &renderer,
               std::size_t target_frame_duration);

   private:
    WelcomeScreen _welcomeScreen;
    GameScreen _gameScreen;
    LeaderBoard _leaderBoard;
    Screens _currentScreen;
    Screens _previousScreen;
};

#endif /*SCREEN_SM_H*/