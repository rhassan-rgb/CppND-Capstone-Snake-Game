#ifndef ISCRREN_H
#define ISCRREN_H
#include <functional>
#include <initializer_list>
#include <mutex>
#include <string>
#include <vector>

#include "util.h"

class IScreen {
   public:
    IScreen(std::string title) : _title(title), _isActive(false) {}
    IScreen() {}
    ~IScreen() = default;
    virtual void Render() = 0;
    virtual void Update() = 0;
    virtual void Control(const KeyStroke& key) = 0;
    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
    virtual const std::vector<MenuItem>& GetScreenContext() const = 0;
    std::function<void(KeyStroke)> controlCallback;

   protected:
    std::vector<MenuItem> _menuItems;
    bool _isActive;
    std::mutex _itemsMutex;
    std::mutex _activeMutex;
    std::string _title;
};

#endif /*ISCRREN_H*/