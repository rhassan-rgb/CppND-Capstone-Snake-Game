
#include "controller.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "SDL.h"
#include "snake.h"

std::atomic<bool> stopFlag(false);

Controller::Controller() : _keyStrokes(new MessageQueue<KeyStroke>) {}

void Controller::InputHandler(std::function<void(KeyStroke)> callback) const {
    while (true && !stopFlag.load()) {
        KeyStroke key = _keyStrokes->receive();
        callback(key);
        if (key == KeyStroke::KEY_EXT)  // exit signal, needs to be forwarded
        {
            _keyStrokes->send(KeyStroke::KEY_EXT);
            return;
        }
    }
}

// void Controller::HandleInput(std::shared_ptr<bool> running,
// std::shared_ptr<Snake> snake) const {

//   while (true)
//   {
//     KeyStroke key = _keyStrokes->receive();

//   }
// }

void Controller::InputListener(std::shared_ptr<bool> running) const {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                *running = false;
                _keyStrokes->send(KeyStroke::KEY_EXT);
                return;
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
    }
}
