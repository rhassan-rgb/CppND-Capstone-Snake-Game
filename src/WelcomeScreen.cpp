#include "WelcomeScreen.h"

#include <iostream>

WelcomeScreen::WelcomeScreen()
    : IScreen("Snake Game"),
      _pressedKey(KeyStroke::KEY_NONE),
      _selectAction(false),
      _screenUpdated(true),
      _gameStarted(false) {
    _currentItem = 0;
    _menuItems.emplace_back(TextScreenItem({200, 100}, "Welcome!"));
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
    std::cout << "WelcomeScreen::Control lock Active" << std::endl;
    std::unique_lock<std::mutex> uLock(_activeMutex);
    if (!_isActive) {
        std::cout << "WelcomeScreen::Not Active -> returning" << std::endl;
        return;
    }
    std::cout << "WelcomeScreen::Control unlock Active" << std::endl;
    uLock.unlock();
    std::cout << "WelcomeScreen::Control lock Items" << std::endl;
    std::lock_guard<std::mutex> lck(_pressedKeyLock);
    std::cout << "WelcomeScreen::Control locked Items" << std::endl;
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