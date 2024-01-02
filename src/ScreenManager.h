#ifndef SCREENMGR_H
#define SCREENMGR_H
#include "renderer.h"
#include <string>

enum class Screens
{
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

enum class WelcomeItems
{
    ITEM_NEW_GAME = 0,
    ITEM_LEADER_BOARD,
    ITEM_EXIT,
    TOTAL_ITEMS
};

enum class PauseItems
{
    ITEM_RESUME = 0,
    ITEM_NEW_GAME,
    ITEM_EXIT,
    TOTAL_ITEMS
};

class ScreenManager : public Renderer
{
public:
    ScreenManager() = delete;
    ScreenManager(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
    void Render() override;
    void render(Screens currentScreen, int selected);
private:
    TTF_Font* _gFont; // Use the default font provided by SDL_ttf
    SDL_Color _normalColor = {255, 255, 255};
    SDL_Color _selectedColor = {255, 0, 0}; // Red for selected text
    std::size_t _maxNameLength = 20;
    std::string _playerName;

    void drawText(const std::string& text, int x, int y, bool selected);    
    void drawTextbox(const std::string& text, int x, int y);

    void handleTextInput(const SDL_TextInputEvent& e, std::string& playerName);
    void renderWelcomeScreen(WelcomeItems selected);
    void RenderPlayerNameScreen();
};

class SelectionMenu
{
    public:
    SelectionMenu() = delete;
    SelectionMenu(int totalItems):_size(totalItems), _current(0)
    {}
    void nextItem()
    {
        _current = (_current + _size + 1) %_size;
    }
    int getSelected()
    {
        return _current;
    }
    void previousItem()
    {
        _current = (_current + _size - 1) %_size;
    }
    private:
    int _size;
    int _current;

};
#endif /*SCREENMGR_H*/