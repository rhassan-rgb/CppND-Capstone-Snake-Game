#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <future>
#include <memory>
#include <vector>

#include "MessageBox.h"
#include "snake.h"
#include "util.h"

class Controller {
   public:
    Controller();
    ~Controller();

    void InputListener();
    void RegisterHandlerCallBack(std::function<void(KeyStroke)> &&callback);
    bool isRunning() const;

   private:
    std::vector<std::future<void>> _handlers;
    std::shared_ptr<MessageBox<KeyStroke>> _keyStrokes;
    std::mutex _runningGuard;
    bool _isRunning;

    bool ListenToKeys();
    void InputHandler(std::function<void(KeyStroke)> callback);
};

#endif