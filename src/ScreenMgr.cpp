#include "ScreenSM.h"

ScreenSM::ScreenSM(std::size_t grid_width, std::size_t grid_height)
    : _currentScreen(Screens::SCREEN_WELCOME),
      _nextScreen(Screens::SCREEN_WELCOME),
      _welcomeScreen(),
      _gameScreen(grid_width, grid_height) {}

ScreenSM::~ScreenSM() {}

void ScreenSM::Update(Renderer &renderer) {
    int selection = 0;
    switch (_currentScreen) {
        case Screens::SCREEN_WELCOME:
            if (!_welcomeScreen.Update()) break;
            renderer.Render(_welcomeScreen.GetScreenContext());
            selection = _welcomeScreen.GetSelection();
            switch (static_cast<WelcomeItems>(selection)) {
                case WelcomeItems::ITEM_NEW_GAME:
                    std::cout << "New Game Is Selected" << std::endl;
                    _welcomeScreen.Deactivate();
                    _currentScreen = Screens::SCREEN_GAME_START;
                    _gameScreen.Activate();
                    break;
                case WelcomeItems::ITEM_LEADER_BOARD:
                    std::cout << "Leader board Is Selected" << std::endl;
                    break;
                case WelcomeItems::ITEM_EXIT:
                    std::cout << "Exit Is Selected" << std::endl;
                    _welcomeScreen.Deactivate();
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
                    std::cout << "Game Is Paused" << std::endl;
                    // _gameScreen.Deactivate();
                    _gameScreen.Activate();
                    break;
                case GameItems::ITEM_GAME_OVER:
                    std::cout << "Game Over Selected" << std::endl;
                    // renderer.Render(_gameScreen.GetScreenContext());
                    // _welcomeScreen.Deactivate();
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

void ScreenSM::Start(Controller &controller, Renderer &renderer,
                     std::size_t target_frame_duration) {
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    _welcomeScreen.Activate();
    controller.RegisterHandlerCallBack(_welcomeScreen.controlCallback);
    controller.RegisterHandlerCallBack(_gameScreen.controlCallback);
    renderer.Render(_welcomeScreen.GetScreenContext());
    while (controller.isRunning()) {
        Update(renderer);
        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.
        if (frame_duration < target_frame_duration) {
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }
}