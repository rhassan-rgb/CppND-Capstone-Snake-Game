#include "ScreenManager.h"

#include <thread>
ScreenManager::ScreenManager(std::size_t grid_width, std::size_t grid_height)
    : _currentScreen(Screens::SCREEN_WELCOME),
      _previousScreen(Screens::SCREEN_WELCOME),
      _welcomeScreen(),
      _gameScreen(grid_width, grid_height),
      _leaderBoard() {}

ScreenManager::~ScreenManager() {}

bool ScreenManager::Update(Renderer &renderer) {
    int selection = 0;
    bool retVal = true;
    switch (_currentScreen) {
        case Screens::SCREEN_WELCOME:
            if (_previousScreen != Screens::SCREEN_WELCOME) {
                _welcomeScreen.Activate();
                renderer.Render(_welcomeScreen.GetScreenContext());
                _previousScreen = Screens::SCREEN_WELCOME;
            }
            if (!_welcomeScreen.Update()) break;
            renderer.Render(_welcomeScreen.GetScreenContext());
            selection = _welcomeScreen.GetSelection();
            switch (static_cast<WelcomeItems>(selection)) {
                case WelcomeItems::ITEM_NEW_GAME:
                    _welcomeScreen.Deactivate();
                    _welcomeScreen.GameStarted();
                    _currentScreen = Screens::SCREEN_GAME_START;
                    _gameScreen.Activate();
                    break;
                case WelcomeItems::ITEM_LEADER_BOARD:
                    _welcomeScreen.Deactivate();
                    _previousScreen = _currentScreen;
                    _currentScreen = Screens::SCREEN_LEADER_BOARD;
                    _leaderBoard.Activate();
                    break;
                case WelcomeItems::ITEM_EXIT:
                    _welcomeScreen.Deactivate();
                    retVal = false;
                    break;
                default:
                    break;
            }
            break;
        case Screens::SCREEN_GAME_START:
            if (!_gameScreen.Update()) break;
            renderer.Render(_gameScreen.GetScreenContext());
            selection = _gameScreen.GetSelection();
            switch (static_cast<GameItems>(selection)) {
                case GameItems::ITEM_PAUSE_GAME:
                    _gameScreen.Deactivate();
                    _previousScreen = _currentScreen;
                    _currentScreen = Screens::SCREEN_WELCOME;

                    break;
                case GameItems::ITEM_GAME_OVER:
                    _gameScreen.Deactivate();
                    _leaderBoard.WriteScore(_gameScreen.GetScore());
                    _welcomeScreen.GameOver(_gameScreen.GetScore());
                    _previousScreen = _currentScreen;
                    _currentScreen = Screens::SCREEN_WELCOME;
                    break;
                default:
                    break;
            }
            break;
        case Screens::SCREEN_LEADER_BOARD:
            if (!_leaderBoard.Update()) break;
            renderer.Render(_leaderBoard.GetScreenContext());
            selection = _leaderBoard.GetSelection();
            switch (static_cast<LeaderBoardItems>(selection)) {
                case LeaderBoardItems::ITEM_EXIT:
                    std::swap<Screens>(_previousScreen, _currentScreen);
                    _leaderBoard.Deactivate();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return retVal;
}

void ScreenManager::Start(Controller &controller, Renderer &renderer,
                          std::size_t target_frame_duration) {
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    _welcomeScreen.Activate();
    controller.RegisterHandlerCallBack(_welcomeScreen.controlCallback);
    controller.RegisterHandlerCallBack(_gameScreen.controlCallback);
    controller.RegisterHandlerCallBack(_leaderBoard.controlCallback);

    renderer.Render(_welcomeScreen.GetScreenContext());
    while (controller.isRunning()) {
        frame_start = SDL_GetTicks();
        if (!Update(renderer)) {
            controller.Stop();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render
        // cycle takes.
        frame_count++;
        frame_duration = frame_end - frame_start;
        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.
        if (frame_duration < target_frame_duration) {
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }
}