#include "WelcomeScreen.h"

#include <iostream>

WelcomeScreen::WelcomeScreen()
    : IScreen("Snake Game Score: 0 FPS: 0"),
      _pressedKey(KeyStroke::KEY_NONE),
      _selectAction(false),
      _screenUpdated(true),
      _gameStarted(false),
      _gameOver(false) {
    _currentItem = 0;
    _menuItems.emplace_back(TextScreenItem({250, 100}, "Snake!"));
    _menuItems.emplace_back(TextScreenItem({100, 200}, "New Game", true));
    _menuItems.emplace_back(TextScreenItem({100, 300}, "Leader board", true));
    _menuItems.emplace_back(TextScreenItem({100, 400}, "Exit", true));
    ;
    controlCallback =
        std::bind(&WelcomeScreen::Control, this, std::placeholders::_1);
}

std::string WelcomeScreen::GetTitle() { return _title; }

WelcomeScreen::~WelcomeScreen() {}
bool WelcomeScreen::Update() {
    (void)handlePressedKey();
    if (_screenUpdated) {
        _screenUpdated = false;
        return true;
    }
    return false;
}

int WelcomeScreen::GetSelection() {
    if (_gameOver && _currentItem == 1) {
        return static_cast<int>(WelcomeItems::ITEM_WELCOME);
    }
    if (_selectAction) {
        _selectAction = false;
        return _currentItem;
    }

    return static_cast<int>(WelcomeItems::ITEM_WELCOME);
}

void WelcomeScreen::Activate() {
    std::unique_lock<std::mutex> uLock(_activeMutex);
    _isActive = true;
}
void WelcomeScreen::Deactivate() {
    std::unique_lock<std::mutex> uLock(_activeMutex);
    _isActive = false;
}

const std::vector<TextScreenItem>& WelcomeScreen::GetScreenContext() const {
    return _menuItems;
}
void WelcomeScreen::changeSelection(Direction dir) {
    int index = dir == Direction::NEXT ? -1 : 1;
    index += static_cast<int>(WelcomeItems::TOTAL_ITEMS);

    _menuItems.at(_currentItem).SetColors(UNSELECTED_COLOR);
    _currentItem =
        (_currentItem + index) % static_cast<int>(WelcomeItems::TOTAL_ITEMS);
    _currentItem = (_currentItem == 0)
                       ? (index % static_cast<int>(WelcomeItems::TOTAL_ITEMS))
                       : _currentItem;
    _menuItems.at(_currentItem).SetColors(SELECTED_COLOR);
}

bool WelcomeScreen::handlePressedKey() {
    bool retVal = false;
    _screenUpdated = true;
    std::lock_guard<std::mutex> lck(_pressedKeyLock);
    _selectAction = false;
    switch (_pressedKey) {
        case KeyStroke::KEY_UP:
            changeSelection(Direction::NEXT);
            break;
        case KeyStroke::KEY_DOWN:
            changeSelection(Direction::PREVIOUS);
            break;

        case KeyStroke::KEY_ENTER:
            _selectAction = true;
            retVal = true;
            break;

        default:
            _screenUpdated = false;
            break;
    }
    _pressedKey = KeyStroke::KEY_NONE;
    return retVal;
}

void WelcomeScreen::Control(const KeyStroke& key) {
    std::unique_lock<std::mutex> uLock(_activeMutex);
    if (!_isActive) {
        return;
    }
    uLock.unlock();
    std::lock_guard<std::mutex> lck(_pressedKeyLock);
    switch (key) {
        case KeyStroke::KEY_UP:
            _pressedKey = KeyStroke::KEY_UP;
            break;

        case KeyStroke::KEY_DOWN:
            _pressedKey = KeyStroke::KEY_DOWN;
            break;

        case KeyStroke::KEY_ENTER:
            _pressedKey = KeyStroke::KEY_ENTER;
            break;

        default:
            return;
    }
}

void WelcomeScreen::GameStarted() {
    if (!_gameStarted) {
        _gameStarted = true;
        _menuItems.at(0).UpdateContent("Game Paused!");
        _menuItems.at(1).UpdateContent("Resume Game");
    }
}

void WelcomeScreen::GameOver(int Score) {
    if (!_gameOver) {
        _gameOver = true;
        _menuItems.at(0).UpdateContent("Game Over!");
        _menuItems.at(1).UpdateContent(std::string("Your Score ") +
                                       std::to_string(Score));
    }
}