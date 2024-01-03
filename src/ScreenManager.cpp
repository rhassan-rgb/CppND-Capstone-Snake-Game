#include "ScreenManager.h"

ScreenManager::ScreenManager(const std::size_t screen_width,
                             const std::size_t screen_height,
                             const std::size_t grid_width,
                             const std::size_t grid_height)
    : Renderer(screen_width, screen_height, grid_width, grid_height) {
    _gFont = TTF_OpenFont("arial.ttf", 28);
    _normalColor = {255, 255, 255};
    _selectedColor = {255, 0, 0};  // Red for selected text
    _maxNameLength = 20;
}
void ScreenManager::Render() {}

void ScreenManager::drawText(const std::string& text, int x, int y,
                             bool selected) {
    SDL_Color textColor = selected ? _selectedColor : _normalColor;
    SDL_Surface* surface =
        TTF_RenderText_Solid(_gFont, text.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;

    SDL_Rect destRect = {x, y, textWidth, textHeight};
    SDL_RenderCopy(sdl_renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void ScreenManager::render(Screens currentScreen, int selected) {
    // Clear the screen
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl_renderer);

    switch (currentScreen) {
        case Screens::SCREEN_WELCOME:
            renderWelcomeScreen(static_cast<WelcomeItems>(selected));
            break;

        case Screens::SCREEN_PLAYER_NAME:
            RenderPlayerNameScreen();
            break;

        case Screens::SCREEN_LEADER_BOARD:

            break;

        case Screens::SCREEN_PAUSE_GAME:

            break;
    }

    // Update the screen
    SDL_RenderPresent(sdl_renderer);
}

void ScreenManager::handleTextInput(const SDL_TextInputEvent& e,
                                    std::string& playerName) {
    // Handle text input for player name
    if (playerName.length() < _maxNameLength) {
        playerName += e.text;
    }
}

void ScreenManager::renderWelcomeScreen(WelcomeItems selected) {
    // Draw the welcome text
    drawText("Welcome to the Game", 150, 100, false);

    // Draw "Start a new game" button with highlight if selected
    drawText("Start a new game", 150, 200,
             WelcomeItems::ITEM_NEW_GAME == selected);

    drawText("Leader board", 150, 260,
             WelcomeItems::ITEM_LEADER_BOARD == selected);

    // Draw "Exit" button with highlight if selected
    drawText("Exit", 150, 320, WelcomeItems::ITEM_EXIT == selected);
}
void ScreenManager::drawTextbox(const std::string& text, int x, int y) {
    SDL_Color textColor = _normalColor;
    SDL_Surface* surface =
        TTF_RenderText_Solid(_gFont, text.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;

    SDL_Rect destRect = {x, y, textWidth, textHeight};
    SDL_RenderCopy(sdl_renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void ScreenManager::RenderPlayerNameScreen() {
    drawText("Welcome to the Game", 150, 100, false);
    drawTextbox("Player Name: " + _playerName, 150, 200);
}

Screens handleScreenAction(Screens currentScreen, int selectedItem) {
    Screens retVal = Screens::TOTAL_ITEMS;
    switch (currentScreen) {
        case Screens::SCREEN_WELCOME:
            switch (static_cast<WelcomeItems>(selectedItem)) {
                case WelcomeItems::ITEM_NEW_GAME:
                    retVal = Screens::SCREEN_PLAYER_NAME;
                    break;
                case WelcomeItems::ITEM_LEADER_BOARD:
                    retVal = Screens::SCREEN_LEADER_BOARD;
                    break;
                case WelcomeItems::ITEM_EXIT:
                    // returning retVal = Screens::TOTAL_ITEMS will cause the
                    // program to terminate;
                default:
                    break;
            }
            break;

        case Screens::SCREEN_PAUSE_GAME:
            switch (static_cast<PauseItems>(selectedItem)) {
                case PauseItems::ITEM_RESUME:
                    retVal = Screens::SCREEN_RESUME_GAME;
                    break;
                case PauseItems::ITEM_NEW_GAME:
                    retVal = Screens::SCREEN_RESUME_GAME;
                    break;
                case PauseItems::ITEM_EXIT:
                    // returning retVal = Screens::TOTAL_ITEMS will cause the
                    // program to terminate;
                default:
                    break;
            }
            break;
        case Screens::SCREEN_PLAYER_NAME:
            retVal = Screens::SCREEN_GAME_START;
    }

    return retVal;
}