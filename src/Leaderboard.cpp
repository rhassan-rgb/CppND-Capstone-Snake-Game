#include "Leaderboard.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

LeaderBoard::LeaderBoard()
    : IScreen("LeaderBoard"),
      _pressedKey(KeyStroke::KEY_NONE),
      _selectAction(false),
      _screenUpdated(true) {
    _currentItem = 0;
    _menuItems.emplace_back(TextScreenItem({200, 100}, "Leader Board"));
    _menuItems.emplace_back(TextScreenItem({200, 150}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 200}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 250}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 300}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 350}, "0"));
    _menuItems.emplace_back(TextScreenItem({200, 600}, "Press Esc to exit"));

    controlCallback =
        std::bind(&LeaderBoard::Control, this, std::placeholders::_1);
}

std::string LeaderBoard::GetTitle() { return _title; }

LeaderBoard::~LeaderBoard() {}
bool LeaderBoard::Update() {
    std::lock_guard<std::mutex> lck(_pressedKeyLock);
    if (_screenUpdated || _pressedKey == KeyStroke::KEY_ESC) {
        _screenUpdated = false;
        ReadLeaderBoard();
        return true;
    }
    return false;
}

int LeaderBoard::GetSelection() {
    std::cout << "LeaderBoard::GetSelection lock Items" << std::endl;
    std::lock_guard<std::mutex> lck(_pressedKeyLock);
    std::cout << "LeaderBoard::GetSelection locked Items" << std::endl;
    if (_pressedKey == KeyStroke::KEY_ESC) {
        std::cout << "LeaderBoard::GetSelection Return Exit" << std::endl;
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
    std::fstream leaderBoardFile(fileName, std::ios::in);

    // Check if the file is open
    if (!leaderBoardFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        _menuItems.emplace_back(
            TextScreenItem({200, 400}, "Couldn't Load Leader Board"));
        return;
    }

    // Read numbers from the file into a vector
    std::vector<int> numbers;
    int num;
    int counter = 0;
    while (leaderBoardFile >> num && counter < 5) {
        std::cout << "Reading Score: " << num << std::endl;
        numbers.push_back(num);
        ++counter;
    }

    // Sort the numbers
    std::sort(numbers.begin(), numbers.end(), std::greater<int>());

    // Clear the file content
    // file.clear();
    // file.seekp(0, std::ios::beg);

    // // Write the sorted numbers back to the file
    // for (int sortedNum : numbers) {
    //     file << sortedNum << std::endl;
    // }

    // Close the file
    leaderBoardFile.close();
    counter = 1;
    for (auto& number : numbers) {
        std::cout << "list Score: " << number << std::endl;
        _menuItems.at(counter).UpdateContent(std::to_string(number));
        if (++counter > 6) break;
    }

    std::cout << "Numbers read from " << fileName << "." << std::endl;
}

void LeaderBoard::Control(const KeyStroke& key) {
    std::cout << "LeaderBoard::Control lock Active" << std::endl;
    std::unique_lock<std::mutex> uLock(_activeMutex);
    if (!_isActive) {
        std::cout << "LeaderBoard::Not Active -> returning" << std::endl;
        return;
    }
    std::cout << "LeaderBoard::Control unlock Active" << std::endl;
    uLock.unlock();
    std::cout << "LeaderBoard::Control lock Items" << std::endl;
    std::lock_guard<std::mutex> lck(_pressedKeyLock);
    std::cout << "LeaderBoard::Control locked Items" << std::endl;
    switch (key) {
        case KeyStroke::KEY_ESC:
            _pressedKey = KeyStroke::KEY_ESC;
            break;

        default:
            _pressedKey = KeyStroke::KEY_NONE;
            return;
    }
}

void LeaderBoard::WriteScore(int Score) {
    const char* fileName = "LeaderBoard.txt";

    // Open the file for both reading and writing
    std::fstream leaderBoardFile(fileName, std::ios::in | std::ios::out);

    // Check if the file is open
    if (!leaderBoardFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        _menuItems.emplace_back(
            TextScreenItem({200, 400}, "Couldn't Load Leader Board"));
        return;
    }

    // Read numbers from the file into a vector
    std::vector<int> numbers(Score);
    int num;

    while (leaderBoardFile) {
        numbers.push_back(num);
    }

    // Sort the numbers
    std::sort(numbers.begin(), numbers.end(), std::greater<int>());

    // Clear the file content file.clear();
    leaderBoardFile.seekp(0, std::ios::beg);

    // Write the sorted numbers back to the file
    int counter = 0;
    for (int sortedNum : numbers) {
        leaderBoardFile << sortedNum << std::endl;
        if (++counter < 5) break;
    }

    // Close the file
    leaderBoardFile.close();
    std::cout << "Numbers written to " << fileName << "." << std::endl;
}