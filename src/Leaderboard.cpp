#include "Leaderboard.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

LeaderBoard::LeaderBoard()
    : IScreen("LeaderBoard"),
      _pressedKey(KeyStroke::KEY_NONE),
      _selectAction(false),
      _screenUpdated(true),
      _leaderBoard(5) {
    _currentItem = 0;
    _menuItems.emplace_back(TextScreenItem({200, 100}, "Leader Board"));
    _menuItems.emplace_back(TextScreenItem({200, 150}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 200}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 250}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 300}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 350}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 600}, "Press Esc to exit"));
    ReadLeaderBoard();
    controlCallback =
        std::bind(&LeaderBoard::Control, this, std::placeholders::_1);
}

std::string LeaderBoard::GetTitle() { return _title; }

LeaderBoard::~LeaderBoard() {}
bool LeaderBoard::Update() {
    std::lock_guard<std::mutex> lck(_pressedKeyLock);
    if (_screenUpdated || _pressedKey == KeyStroke::KEY_ESC) {
        _screenUpdated = false;
        LoadLeaderBoard();
        return true;
    }
    return false;
}

int LeaderBoard::GetSelection() {
    std::lock_guard<std::mutex> lck(_pressedKeyLock);
    if (_pressedKey == KeyStroke::KEY_ESC) {
        _pressedKey = KeyStroke::KEY_NONE;
        return static_cast<int>(LeaderBoardItems::ITEM_EXIT);
    }
    return static_cast<int>(LeaderBoardItems::ITEM_LEADER_BOARD);
}

void LeaderBoard::Activate() {
    std::unique_lock<std::mutex> uLock(_activeMutex);
    _isActive = true;
    _screenUpdated = true;
}
void LeaderBoard::Deactivate() {
    std::unique_lock<std::mutex> uLock(_activeMutex);
    _isActive = false;
    _screenUpdated = false;
}

const std::vector<TextScreenItem>& LeaderBoard::GetScreenContext() const {
    return _menuItems;
}

void LeaderBoard::ReadLeaderBoard() {
    const char* fileName = "../src/LeaderBoard.txt";

    // Open the file for both reading and writing
    std::fstream leaderBoardFile(fileName, std::ios::in | std::ios::out);

    // Check if the file is open
    if (!leaderBoardFile.is_open()) {
        std::cerr << "Error opening file. Creating a new one" << std::endl;
        leaderBoardFile.open(fileName, std::ios::out);
        leaderBoardFile.close();
        leaderBoardFile.open(fileName, std::ios::in);
        if (!leaderBoardFile.is_open()) {
            std::cerr << "Error opening file. Can't load leader board"
                      << std::endl;
            return;
        }
    }

    int num;
    int index = 0;
    while (leaderBoardFile >> num && index < _leaderBoard.size()) {
        _leaderBoard.at(index) = num;
        ++index;
    }

    // Sort the numbers
    std::sort(_leaderBoard.begin(), _leaderBoard.end(), std::greater<int>());

    leaderBoardFile.close();
}
void LeaderBoard::LoadLeaderBoard() {
    int counter = 1;
    for (auto score : _leaderBoard) {
        _menuItems.at(counter).UpdateContent(std::to_string(score));
        counter++;
    }
}
void LeaderBoard::Control(const KeyStroke& key) {
    std::unique_lock<std::mutex> uLock(_activeMutex);
    if (!_isActive) {
        return;
    }
    uLock.unlock();
    std::lock_guard<std::mutex> lck(_pressedKeyLock);
    switch (key) {
        case KeyStroke::KEY_ESC:
            _pressedKey = KeyStroke::KEY_ESC;
            break;

        default:
            _pressedKey = KeyStroke::KEY_NONE;
            return;
    }
}

void LeaderBoard::WriteScore(int score) {
    // decide whether to include the score
    bool newHighScore(false);
    for (auto& lbScore : _leaderBoard) {
        if (score > lbScore) {
            std::swap(score, lbScore);
        }
    }

    const char* fileName = "../src/LeaderBoard.txt";

    // Open the file for both reading and writing
    std::fstream leaderBoardFile(fileName, std::ios::out);

    // Check if the file is open
    if (!leaderBoardFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        leaderBoardFile.open(fileName, std::ios::out);
        if (!leaderBoardFile.is_open()) {
            std::cerr << "Error opening file. Can't load leader board"
                      << std::endl;
            return;
        }
    }
    // Write the sorted numbers back to the file
    for (auto score : _leaderBoard) {
        leaderBoardFile << score << std::endl;
    }
    // Close the file
    leaderBoardFile.close();
}