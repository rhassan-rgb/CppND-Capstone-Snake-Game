
#include "controller.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "SDL.h"
#include "snake.h"

constexpr int KEYBOARD_SAMPLING_RATE = 20;  // ms

std::atomic<bool> stopFlag(false);

Controller::Controller() : _keyStrokes(new MessageBox<KeyStroke>) {
    _isRunning = true;
    _handlers.emplace_back(std::async(&Controller::InputListener, this));
}

void Controller::InputHandler(std::function<void(KeyStroke)> callback) {
    while (true) {
        KeyStroke key = _keyStrokes->receive();
        callback(key);
        if (key == KeyStroke::KEY_EXT)  // exit signal, needs to be forwarded
        {
            _keyStrokes->send(KeyStroke::KEY_EXT);
            return;
        }
    }
}

bool Controller::ListenToKeys() {
    bool retVal = true;
    SDL_Event e;
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
            stopFlag.store(true);
            return;
        }
    }
}

void Controller::RegisterHandlerCallBack(
    std::function<void(KeyStroke)> &&callback)

{
    _handlers.emplace_back(std::async(std::launch::async, [this, &callback]() {
        this->InputHandler(callback);
    }));
}

bool Controller::isRunning() const { return !stopFlag.load(); }

Controller::~Controller() {
    for (auto &handler : _handlers) {
        std::cout << "exiting ..";
        handler.wait();
    }
}