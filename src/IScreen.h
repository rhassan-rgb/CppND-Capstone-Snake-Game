#ifndef ISCRREN_H
#define ISCRREN_H
#include <functional>
#include <initializer_list>
#include <mutex>
#include <string>
#include <vector>

#include "util.h"
template <typename T>
class IScreen {
   public:
    IScreen(std::string title)
        : _title(title), _isActive(false), _currentItem(0) {}
    IScreen() {}
    ~IScreen() = default;
    virtual bool Update() = 0;
    virtual void Control(const KeyStroke& key) = 0;
    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
    virtual int GetSelection() = 0;
    virtual std::string GetTitle() = 0;
    virtual const std::vector<T>& GetScreenContext() const = 0;
    std::function<void(KeyStroke)> controlCallback;

   protected:
    std::string _title;
    bool _isActive;
    int _currentItem;
    std::vector<T> _menuItems;
    std::mutex _itemsMutex;
    std::mutex _activeMutex;
};

#endif /*ISCRREN_H*/