
#include "controller.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "SDL.h"
#include "snake.h"

constexpr int KEYBOARD_SAMPLING_RATE = 100;  // ms

Controller::Controller()
    : _keyStrokes(new MessageBox<KeyStroke>), _stopFlag(false) {
    _isRunning = true;
    _listener = std::async(&Controller::InputListener, this);
    _handler = std::thread(&Controller::InputHandler, this);
}

void Controller::InputHandler() {
    while (true) {
        KeyStroke key = _keyStrokes->receive();
        std::lock_guard<std::mutex> lock(_handlersGuard);
        for (auto &&callback : _callbacks) {
            std::cout << _callbacks.size() << "handler : " << &callback
                      << std::endl;
            callback(key);
            std::cout << "handler : " << &callback << " EXIT" << std::endl;
        }
        if (KeyStroke::KEY_EXT == key) {
            return;
        }
    }
}

bool Controller::ListenToKeys() {
    bool retVal = true;
    SDL_Event e;
    if (_stopFlag.load()) {
        _keyStrokes->send(KeyStroke::KEY_EXT);
        return false;
    }
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            _keyStrokes->send(KeyStroke::KEY_EXT);
            retVal = false;
            break;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    _keyStrokes->send(KeyStroke::KEY_UP);
                    break;

                case SDLK_DOWN:
                    _keyStrokes->send(KeyStroke::KEY_DOWN);
                    break;

                case SDLK_LEFT:
                    _keyStrokes->send(KeyStroke::KEY_LEFT);
                    break;

                case SDLK_RIGHT:
                    _keyStrokes->send(KeyStroke::KEY_RIGHT);
                    break;
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    _keyStrokes->send(KeyStroke::KEY_ENTER);
                    break;
                case SDLK_ESCAPE:
                    _keyStrokes->send(KeyStroke::KEY_ESC);
                    break;
                default:
                    break;
            }
        }
    }
    return retVal;
}
void Controller::InputListener() {
    while (true) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(KEYBOARD_SAMPLING_RATE));
        if (!ListenToKeys()) {
            std::lock_guard<std::mutex> lck(_runningGuard);
            _stopFlag.store(true);
            return;
        }
    }
}

void Controller::RegisterHandlerCallBack(
    std::function<void(KeyStroke)> &callback) {
    std::lock_guard<std::mutex> lock(_handlersGuard);
    _callbacks.emplace_back(callback);
}

bool Controller::isRunning() const { return !_stopFlag.load(); }

Controller::~Controller() {
    _listener.wait();
    std::cout << "Listener Exit" << std::endl;

    _handler.join();
    std::cout << "Handler Exit" << std::endl;
}

void Controller::Stop() { _stopFlag.store(true); }