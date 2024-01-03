#include "WelcomeScreen.h"

#include <iostream>

WelcomeScreen::WelcomeScreen() : IScreen("Snake Game"), _currentItem(1) {
    _menuItems.emplace_back(MenuItem("Welcome!"));
    _menuItems.emplace_back(MenuItem("New Game", true, true));
    _menuItems.emplace_back(MenuItem("Leader board", true));
    _menuItems.emplace_back(MenuItem("Exit", true));
    controlCallback =
        std::bind(&WelcomeScreen::Control, this, std::placeholders::_1);
}
WelcomeScreen::~WelcomeScreen() {}
void WelcomeScreen::Render() {}
void WelcomeScreen::Update() {
    std::lock_guard<std::mutex> lck(_itemsMutex);
    _menuItems_cpy = std::vector<MenuItem>(_menuItems);
}
std::function<void(KeyStroke)> controlCallback;
void WelcomeScreen::Activate() {
    std::unique_lock<std::mutex> uLock(_activeMutex);
    _isActive = true;
}
void WelcomeScreen::Deactivate() {
    std::unique_lock<std::mutex> uLock(_activeMutex);
    _isActive = false;
}

const std::vector<MenuItem>& WelcomeScreen::GetScreenContext() const {
    return _menuItems_cpy;
}
void WelcomeScreen::Control(const KeyStroke& key) {
    std::cout << "WelcomeScreen::Control lock Active" << std::endl;
    std::unique_lock<std::mutex> uLock(_activeMutex);
    if (!_isActive) {
        return;
    }
    std::cout << "WelcomeScreen::Control unlock Active" << std::endl;
    uLock.unlock();
    std::cout << "WelcomeScreen::Control lock Items" << std::endl;
    std::lock_guard<std::mutex> lck(_itemsMutex);
    std::cout << "WelcomeScreen::Control locked Items" << std::endl;
    switch (key) {
        case KeyStroke::KEY_UP:
            _menuItems.at(_currentItem).Unselect();
            _currentItem =
                (_currentItem + _menuItems.size() - 1) % _menuItems.size();
            _currentItem =
                (_currentItem == 0) ? _menuItems.size() - 1 : _currentItem;
            _menuItems.at(_currentItem).Select();
            break;
        case KeyStroke::KEY_DOWN:
            _menuItems.at(_currentItem).Unselect();
            _currentItem =
                (_currentItem + _menuItems.size() + 1) % _menuItems.size();
            _currentItem = (_currentItem == 0) ? 1 : _currentItem;
            _menuItems.at(_currentItem).Select();
            break;

        case KeyStroke::KEY_ENTER:

            break;

        default:
            return;
    }
}