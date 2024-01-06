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
    Controller(const Controller& other) = delete;
    Controller(Controller&& other) = delete;

    Controller& operator=(const Controller& other) = delete;
    Controller& operator=(Controller&& other) = delete;

    ~Controller();

    void InputListener();
    void RegisterHandlerCallBack(std::function<void(KeyStroke)>& callback);
    bool isRunning() const;

   private:
    std::future<void> _listener;
    std::future<void> _handler;
    std::vector<std::function<void(KeyStroke)>> _callbacks;
    std::unique_ptr<MessageBox<KeyStroke>> _keyStrokes;
    std::mutex _runningGuard;
    std::mutex _handlersGuard;
    bool _isValid;
    bool _isRunning;

    bool ListenToKeys();
    void InputHandler();
};

#endif