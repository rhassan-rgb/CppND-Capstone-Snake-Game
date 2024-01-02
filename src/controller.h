#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include <memory>
#include <future>
#include <vector>
#include "MessageQueue.h"
#include "util.h"


class Controller {
public:
    Controller();
    void HandleInput(std::shared_ptr<bool> running, std::shared_ptr<Snake> snake) const;
    void InputHandler(std::function<void(KeyStroke)> callback) const; 
    void InputListener(std::shared_ptr<bool> running) const;
private:
    std::vector<std::future<void>> _handlers;
    std::shared_ptr<MessageQueue<KeyStroke>> _keyStrokes;
};

#endif